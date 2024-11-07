#include "../include/matrix.hpp"
#include <iostream>

int main(){
	float testdata1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	Matrix testmatrix1 = Matrix(testdata1, 3, 4);
	float testdata2[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
	Matrix testmatrix2 = Matrix(testdata2, 4, 5);

	Matrix resultmatrix = multiplyMatrices(testmatrix1, testmatrix2);
	float* returndata = resultmatrix.getData();

    int rows = resultmatrix.getRows();
    int cols = resultmatrix.getCols();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << returndata[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }


	return 0;

}