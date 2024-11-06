__kernel void matrix_multiply(
    __global const float* A,
    __global const float* B,
    __global float* C,
    const int M,  // A rows
    const int N,  // A cols / B rows
    const int P   // B cols
) {
    int row = get_global_id(0);
    int col = get_global_id(1);
    
    if (row < M && col < P) {
        float sum = 0.0f;
        for (int k = 0; k < N; k++) {
            sum += A[row * N + k] * B[k * P + col];
        }
        C[row * P + col] = sum;
    }
}