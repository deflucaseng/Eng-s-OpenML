#ifndef MATRIX_MUL_H
#define MATRIX_MUL_H
#ifdef __cplusplus
extern "C"{

#endif
int matrix_multiply(float* A, float* B, float* C, unsigned int N, unsigned int M, unsigned int K);
//int matrix_multiply();

#ifdef __cplusplus

}
#endif
#endif