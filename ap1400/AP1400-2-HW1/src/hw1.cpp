#include "hw1.h"


static void check_dim(size_t n, size_t m) {
    if (n==0 || m==0) {
        throw std::invalid_argument("The size of the matrix can't be zero!");
    }
}
namespace algebra {

Matrix zeros(size_t n, size_t m) {
    check_dim(n, m);
    return Matrix(n, std::vector<double>(m, 0));
}

Matrix ones(size_t n, size_t m) {
    check_dim(n, m);
    return Matrix(n, std::vector<double>(m, 1));
}

Matrix random(size_t n, size_t m, double min, double max) {
    check_dim(n, m);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(min, max);
    Matrix matrix = zeros(n, m);
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            matrix[i][j] = dist(mt);
        }
    }
    return matrix;
    
}

void show(const Matrix& matrix) {

}

Matrix multiply(const Matrix& matrix, double c) {
    return zeros(1, 1);
}

Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
    return zeros(1, 1);
}

Matrix sum(const Matrix& matrix, double c) {
    return zeros(1, 1);
}

Matrix transpose(const Matrix& matrix) {
    return zeros(1, 1);
}

Matrix minor(const Matrix& matrix, size_t n, size_t m) {
    return zeros(1, 1);
}

double determinant(const Matrix& matrix) {
    return 1;
}

Matrix inverse(const Matrix& matrix) {
    return zeros(1, 1);
}

Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0) {
    return zeros(1, 1);
}

Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
    return zeros(1, 1);
}

Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
    return zeros(1, 1);
}

Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
    return zeros(1, 1);
}

Matrix upper_triangular(const Matrix& matrix) {
    return zeros(1, 1);
}

}




