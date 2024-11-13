#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/matrix_mul.h"

#define CHECK_ERROR(err) \
    if (err != CL_SUCCESS) { \
        printf("OpenCL error at line %d: %d\n", __LINE__, err); \
        cleanup_resources(); \
        exit(1); \
    }

// Global variables for cleanup
static cl_context context = NULL;
static cl_command_queue queue = NULL;
static cl_program program = NULL;
static cl_kernel kernel = NULL;
static cl_mem buffer_A = NULL;
static cl_mem buffer_B = NULL;
static cl_mem buffer_C = NULL;
static char* kernel_source = NULL;

void cleanup_resources() {
    if (buffer_A) clReleaseMemObject(buffer_A);
    if (buffer_B) clReleaseMemObject(buffer_B);
    if (buffer_C) clReleaseMemObject(buffer_C);
    if (kernel) clReleaseKernel(kernel);
    if (program) clReleaseProgram(program);
    if (queue) clReleaseCommandQueue(queue);
    if (context) clReleaseContext(context);
    if (kernel_source) free(kernel_source);
}

int matrix_multiply(float* A, float* B, float* C, unsigned int N, unsigned int M, unsigned int K) {
    // Input validation
    if (!A || !B || !C) {
        printf("Error: NULL input matrices\n");
        return -1;
    }

    if (N == 0 || M == 0 || K == 0) {
        printf("Error: Invalid matrix dimensions\n");
        return -1;
    }

    cl_int err;
    cl_platform_id platform = NULL;
    cl_device_id device = NULL;
    cl_uint num_platforms, num_devices;

    // Get platform
    err = clGetPlatformIDs(1, &platform, &num_platforms);
    CHECK_ERROR(err);
    if (num_platforms == 0) {
        printf("No OpenCL platforms found\n");
        return -1;
    }

    // Get device
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
    if (err == CL_DEVICE_NOT_FOUND) {
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, &num_devices);
    }
    CHECK_ERROR(err);


    // Create context
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    CHECK_ERROR(err);

    // Create command queue
    #ifdef CL_VERSION_2_0
    queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
    #else
    queue = clCreateCommandQueue(context, device, 0, &err);
    #endif
    CHECK_ERROR(err);

    // Load kernel source
    FILE* kernel_file = fopen("src/matrix_mul.cl", "rb");
    if (!kernel_file) {
        printf("Failed to open kernel file\n");
        cleanup_resources();
        return -1;
    }

    fseek(kernel_file, 0, SEEK_END);
    size_t kernel_size = ftell(kernel_file);
    rewind(kernel_file);

    kernel_source = (char*)malloc(kernel_size + 1);
    if (!kernel_source) {
        printf("Failed to allocate memory for kernel source\n");
        fclose(kernel_file);
        cleanup_resources();
        return -1;
    }

    size_t read_size = fread(kernel_source, 1, kernel_size, kernel_file);
    fclose(kernel_file);
    if (read_size != kernel_size) {
        printf("Failed to read kernel file\n");
        cleanup_resources();
        return -1;
    }
    kernel_source[kernel_size] = '\0';

    // Create and build program
    program = clCreateProgramWithSource(context, 1, (const char**)&kernel_source, &kernel_size, &err);
    CHECK_ERROR(err);

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* build_log = (char*)malloc(log_size + 1);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
        printf("Kernel build error:\n%s\n", build_log);
        free(build_log);
        cleanup_resources();
        return -1;
    }

    // Create kernel
    kernel = clCreateKernel(program, "matrix_multiply", &err);
    CHECK_ERROR(err);

    // Create buffers
    buffer_A = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * N * M, A, &err);
    CHECK_ERROR(err);

    buffer_B = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * M * K, B, &err);
    CHECK_ERROR(err);

    buffer_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(float) * N * K, NULL, &err);
    CHECK_ERROR(err);

    // Set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_A);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_B);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_C);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 3, sizeof(unsigned int), &N);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 4, sizeof(unsigned int), &M);
    CHECK_ERROR(err);
    err = clSetKernelArg(kernel, 5, sizeof(unsigned int), &K);
    CHECK_ERROR(err);

    // Set work sizes
    size_t global_work_size[2] = {
        ((K + 15) / 16) * 16,  // Round up to multiple of 16
        ((N + 15) / 16) * 16   // Round up to multiple of 16
    };
    size_t local_work_size[2] = {16, 16};  // Use 16x16 work-groups

    // Execute kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size,
                                local_work_size, 0, NULL, NULL);
    CHECK_ERROR(err);

    // Read results
    err = clEnqueueReadBuffer(queue, buffer_C, CL_TRUE, 0,
                            sizeof(float) * N * K, C, 0, NULL, NULL);
    CHECK_ERROR(err);

    // Cleanup and return
    cleanup_resources();
    return 0;
}
