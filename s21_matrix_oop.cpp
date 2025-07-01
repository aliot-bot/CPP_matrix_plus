#include "s21_matrix_oop.h"

#include <cmath>
#include <cstring>
#include <stdexcept>

#define EPSILON 1e-7

void S21Matrix::AllocateMatrix() {
  if (rows_ <= 0 || cols_ <= 0) {
    throw std::invalid_argument("Matrix dimensions must be positive");
  }
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_]();
  }
}

void S21Matrix::DeallocateMatrix() {
  if (matrix_) {
    for (int i = 0; i < rows_; ++i) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
  rows_ = 0;
  cols_ = 0;
}

void S21Matrix::CopyMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    DeallocateMatrix();
    rows_ = other.rows_;
    cols_ = other.cols_;
    AllocateMatrix();
  }
  for (int i = 0; i < rows_; ++i) {
    std::memcpy(matrix_[i], other.matrix_[i], cols_ * sizeof(double));
  }
}

void S21Matrix::GetMinor(int row, int col, S21Matrix& minor) const {
  int minor_row = 0;
  for (int i = 0; i < rows_; ++i) {
    if (i != row) {
      int minor_col = 0;
      for (int j = 0; j < cols_; ++j) {
        if (j != col) {
          minor.matrix_[minor_row][minor_col] = matrix_[i][j];
          ++minor_col;
        }
      }
      ++minor_row;
    }
  }
}

S21Matrix::S21Matrix() : rows_(1), cols_(1) {
  AllocateMatrix();
  matrix_[0][0] = 0.0;
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  AllocateMatrix();
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  AllocateMatrix();
  CopyMatrix(other);
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

S21Matrix::~S21Matrix() { DeallocateMatrix(); }

void S21Matrix::set_rows(int rows) {
  if (rows <= 0) {
    throw std::invalid_argument("Rows must be positive");
  }
  S21Matrix temp(rows, cols_);
  int min_rows = rows < rows_ ? rows : rows_;
  for (int i = 0; i < min_rows; ++i) {
    std::memcpy(temp.matrix_[i], matrix_[i], cols_ * sizeof(double));
  }
  *this = std::move(temp);
}

void S21Matrix::set_cols(int cols) {
  if (cols <= 0) {
    throw std::invalid_argument("Cols must be positive");
  }
  S21Matrix temp(rows_, cols);
  int min_cols = cols < cols_ ? cols : cols_;
  for (int i = 0; i < rows_; ++i) {
    std::memcpy(temp.matrix_[i], matrix_[i], min_cols * sizeof(double));
  }
  *this = std::move(temp);
}

bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (std::fabs(matrix_[i][j] - other.matrix_[i][j]) > EPSILON) {
        return false;
      }
    }
  }
  return true;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::logic_error("Matrix dimensions must match");
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::logic_error("Matrix dimensions must match");
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::logic_error("Cols of first must equal rows of second");
  }
  S21Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      result.matrix_[i][j] = 0;
      for (int k = 0; k < cols_; ++k) {
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = std::move(result);
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix result(cols_, rows_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      result.matrix_[j][i] = matrix_[i][j];
    }
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square");
  }
  if (rows_ == 1) {
    throw std::logic_error("1x1 matrix has no complements");
  }
  S21Matrix result(rows_, cols_);
  S21Matrix minor(rows_ - 1, cols_ - 1);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      GetMinor(i, j, minor);
      double det = minor.Determinant();
      result.matrix_[i][j] = det * std::pow(-1.0, i + j);
    }
  }
  return result;
}

double S21Matrix::Determinant() const {
  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square");
  }
  if (rows_ == 1) {
    return matrix_[0][0];
  }
  if (rows_ == 2) {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  }
  double result = 0.0;
  S21Matrix minor(rows_ - 1, cols_ - 1);
  for (int j = 0; j < cols_; ++j) {
    GetMinor(0, j, minor);
    result += matrix_[0][j] * std::pow(-1.0, j) * minor.Determinant();
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() const {
  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square");
  }
  double det = Determinant();
  if (std::fabs(det) < EPSILON) {
    throw std::logic_error("Determinant is zero, no inverse");
  }
  if (rows_ == 1) {
    S21Matrix result(1, 1);
    result.matrix_[0][0] = 1.0 / matrix_[0][0];
    return result;
  }
  S21Matrix complements = CalcComplements();
  S21Matrix transposed = complements.Transpose();
  S21Matrix result(rows_, cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      result.matrix_[i][j] = transposed.matrix_[i][j] / det;
    }
  }
  return result;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(double num) const {
  S21Matrix result(*this);
  result.MulNumber(num);
  return result;
}

S21Matrix operator*(double num, const S21Matrix& matrix) {
  return matrix * num;
}

bool S21Matrix::operator==(const S21Matrix& other) const {
  return EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    CopyMatrix(other);
  }
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  if (this != &other) {
    DeallocateMatrix();
    rows_ = other.rows_;
    cols_ = other.cols_;
    matrix_ = other.matrix_;
    other.matrix_ = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
  }
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(double num) {
  MulNumber(num);
  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Index out of range");
  }
  return matrix_[i][j];
}

const double& S21Matrix::operator()(int i, int j) const {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Index out of range");
  }
  return matrix_[i][j];
}