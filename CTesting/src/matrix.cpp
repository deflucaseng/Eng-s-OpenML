#include "../include/matrix.hpp"
#include <stdexcept>
#include <iostream>

// Constructor
Matrix::Matrix(float* values, int numRows, int numCols) {
    rows = numRows;
    cols = numCols;
    data = new float[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
        data[i] = values[i];
    }
}

// Copy constructor
Matrix::Matrix(const Matrix& other) {
    rows = other.rows;
    cols = other.cols;
    data = new float[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
        data[i] = other.data[i];
    }
}

// Destructor
Matrix::~Matrix() {
    delete[] data;
}

// Assignment operator
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        delete[] data;
        rows = other.rows;
        cols = other.cols;
        data = new float[rows * cols];
        for (int i = 0; i < rows * cols; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

// Getters
int Matrix::getRows() const { return rows; }
int Matrix::getCols() const { return cols; }
float* Matrix::getData() const { return data; }

// Setters
void Matrix::setElement(int row, int col, float value) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Index out of bounds");
    }
    data[row * cols + col] = value;
}

float Matrix::getElement(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[row * cols + col];
}

// Display method
void Matrix::display() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << data[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

// Friend multiplication function implementation
Matrix multiplyMatrices(const Matrix& a, const Matrix& b) {
    if (a.getCols() != b.getRows()) {
        throw std::invalid_argument("Matrices cannot be multiplied");
    }
    
    int rows = a.getRows();
    int cols = b.getCols();
    int commonDim = a.getCols();
    float testdata[rows * cols];  // Allocate a float array
    // Create result matrix directly

    Matrix result(testdata, rows, cols);  // Uses default constructor
    
    // Perform multiplication
    //std::cout << "rows: " << rows << " columns: " << cols << " common dimension: " << commonDim; 

	//float testdata1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};    
    //float testdata2[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    //float testdata3[9];
    //matrix_multiply(testdata1, testdata2, testdata3, 3, 3, 3);
    //for (size_t i = 0; i < 9; i++){
    //    std::cout <<testdata3[i] << " ";
    //}
    //std::cout << "\n";
    matrix_multiply(a.getData(), b.getData(), result.getData(), rows, commonDim, cols);


    return result;
}


// Operator overload implementation
Matrix Matrix::operator*(const Matrix& other) const {
    return multiplyMatrices(*this, other);
}