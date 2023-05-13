#include "hw1.h"


static void check_square(const Matrix& matrix) {
    if (matrix.size() != matrix[0].size()) {
        throw std::logic_error("The matrix is not square!");
    }
}

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
    if (min > max) {
        throw std::logic_error("min cannot be greater than max");
    }
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
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            if (j != 0) std::cout << " ";
            std::cout << std::setw(3) << std::setfill(' ') << std::setprecision(2)
                    << static_cast<double>(matrix[i][j]);
        }
    std::cout << "\n";
}
}

Matrix multiply(const Matrix& matrix, double c) {
    if (matrix.size() == 0) {
        return matrix;
    }
    Matrix mat{zeros(matrix.size(), matrix[0].size())};
    for (int i = 0; i < matrix.size(); i++) 
    {
        for (int j = 0; j < mat[0].size(); j++) 
        {
            mat[i][j] = matrix[i][j] * c;
        } 
    }
    return mat;
}

Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
    if (matrix1.size() == 0) 
    {
        return matrix1;
    }
    if (matrix2.size() == 0) 
    {
        return matrix2;
    }
    int row1, col1, row2, col2;
    row1 = matrix1.size(), col1 = matrix1[0].size(), row2 = matrix2.size(), col2 = matrix2[0].size();
    if (col1 != row2) {
        throw std::logic_error("Wrong dimensions cannot be multiplied");
    }
    Matrix result{zeros(row1, col2)};
    for (int i = 0; i < result.size(); i++)
    {
        for (int j = 0; j < result[0].size(); j++)
        {
            double temp = 0;
            for (int k = 0; k < col1; k++)
            {
                temp += matrix1[i][k] * matrix2[k][j];
            }
            result[i][j] = temp;
        }
    }
    return result;
}

Matrix sum(const Matrix& matrix, double c) {
    if (matrix.size() == 0) 
    {
        return matrix;
    }
    Matrix mat{zeros(matrix.size(), matrix[0].size())};
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            mat[i][j] = matrix[i][j] + c;
        }
    }
    return mat;
}

Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
    if (matrix1.size() == 0 && matrix2.size() == 0) 
    {
        return matrix1;
    }
    if (matrix1.size() == 0 || matrix2.size() == 0)
    {
        throw std::logic_error("Wrong dimensions cannot be sum!");
    }
    int row1, col1, row2, col2;
    row1 = matrix1.size(), col1 = matrix1[0].size(), row2 = matrix2.size(), col2 = matrix2[0].size();
    Matrix mat{zeros(row1, col1)};
    if (row1 != row2 || col1 != col2) 
    {
        throw std::logic_error("Wrong dimensions cannot be sum!");
    }
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col1; j++)
        {
            mat[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return mat;
}

Matrix transpose(const Matrix& matrix) {
    if (matrix.size() == 0)
    {
        return matrix;
    }
    int row, col;
    row = matrix.size();
    col = matrix[0].size();
    Matrix mat{zeros(col, row)};
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            mat[j][i] = matrix[i][j];
        }
    }
    return mat;
}

Matrix minor(const Matrix& matrix, size_t n, size_t m) {
    int row, col;
    row = matrix.size(), col = matrix[0].size();
    check_dim(row, col);
    check_square(matrix);
    if (n >= row || m>= col) {
        throw std::logic_error("Index out of bound!");
    }
    Matrix mat{zeros(row-1, col-1)};
    for (int i = 0; i < row-1; i++) 
    {
        for (int j = 0; j < col-1; j++)
        {
            int ii = (i < n) ? i : i + 1;
            int jj = (j < m) ? j : j + 1;
            mat[i][j] = matrix[ii][jj];
        }
    }
    return mat;
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




