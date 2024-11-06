#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define CHECK_ERROR(err) \
    if (err != CL_SUCCESS) { \
        printf("Error: %d\n", err); \
        return err; \
    }

int matrix_multiply(float* A, float* B, float* C, int M, int N, int P) {
    // Validate matrix dimensions
    if (M <= 0 || N <= 0 || P <= 0) {
        printf("Error: Invalid matrix dimensions\n");
        return -1;
    }

    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    
    // Get platform and device
    err = clGetPlatformIDs(1, &platform, NULL);
    CHECK_ERROR(err);
    
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    CHECK_ERROR(err);
    
    // Create context and command queue
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    CHECK_ERROR(err);
    
    queue = clCreateCommandQueue(context, device, 0, &err);
    CHECK_ERROR(err);
    
    // Create buffers
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, M * N * sizeof(float), NULL, &err);
    CHECK_ERROR(err);
    
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, N * P * sizeof(float), NULL, &err);
    CHECK_ERROR(err);
    
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, M * P * sizeof(float), NULL, &err);
    CHECK_ERROR(err);
    
    // Write input data to buffers
    err = clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, M * N * sizeof(float), A, 0, NULL, NULL);
    CHECK_ERROR(err);
    
    err = clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, N * P * sizeof(float), B, 0, NULL, NULL);
    CHECK_ERROR(err);
    
    // Create and build program
    const char* source = // Load kernel source code here
    program = clCreateProgramWithSource(context, 1, &source, NULL, &err);
    CHECK_ERROR(err);
    
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* log = (char*)malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Build Error:\n%s\n", log);
        free(log);
        return err;
    }
    
    // Create kernel and set arguments
    kernel = clCreateKernel(program, "matrix_multiply", &err);
    CHECK_ERROR(err);
    
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 3, sizeof(int), &M);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 4, sizeof(int), &N);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 5, sizeof(int), &P);
    CHECK_ERROR(err);
    
    // Execute kernel
    size_t global_work_size[2] = {M, P};
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
    CHECK_ERROR(err);
    
    // Read result
    err = clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, M * P * sizeof(float), C, 0, NULL, NULL);
    CHECK_ERROR(err);
    
    // Cleanup
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    
    return CL_SUCCESS;
}