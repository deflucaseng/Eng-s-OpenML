__kernel void matrix_multiply(__global float* A, __global float* B, __global float* C,
                               const unsigned int N, const unsigned int M, const unsigned int K) {
    // Get the row and column index of the element to process
    unsigned int row = get_global_id(1);
    unsigned int col = get_global_id(0);

    if (row < N && col < K) {
        float sum = 0.0f;
        for (unsigned int i = 0; i < M; i++) {
            sum += A[row * M + i] * B[i * K + col];
        }
        C[row * K + col] = sum;
    }
}
