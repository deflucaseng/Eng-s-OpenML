#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "../include/matrix_mul.h"

class Matrix {
private:
    float* data;
    int rows;
    int cols;

public:
    // Constructor, destructor, and assignment operator declarations
    Matrix(float* values, int numRows, int numCols);
    Matrix(const Matrix& other);
    ~Matrix();
    Matrix& operator=(const Matrix& other);

    // Getter and setter declarations
    int getRows() const;
    int getCols() const;
    float* getData() const;
    void setElement(int row, int col, float value);
    float getElement(int row, int col) const;

    // Display method declaration
    void display() const;

    // Friend declaration for multiplication
    friend Matrix multiplyMatrices(const Matrix& a, const Matrix& b);

    // Operator overload declaration
    Matrix operator*(const Matrix& other) const;
};

Matrix multiplyMatrices(const Matrix& a, const Matrix& b);

#endif