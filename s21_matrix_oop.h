#ifndef S21_MATRIX_OPP_H
#define S21_MATRIX_OPP_H

#include <cmath>
#include <stdexcept>

#define EPSILON 1e-7

class S21Matrix {
 private:
  int rows_, cols_;
  double** matrix_;

  // вспомогательные функции, выделние и освобождение памяти, и копирования
  // матрицы  (для опреатора присваивания)
  void AllocateMatrix();
  void DeallocateMatrix();
  void CopyMatrix(const S21Matrix& other);

 public:
  // консктруторы и деструктор
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  // геттеры и сеттеры
  int get_rows() const { return rows_; }
  int get_cols() const { return cols_; }
  void set_rows(int rows);
  void set_cols(int cols);

  // операции над матрицами
  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  void GetMinor(int row, int col, S21Matrix& minor) const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  // перегрузка операторов
  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix operator*(double num) const;
  friend S21Matrix operator*(double num, const S21Matrix& matrix);
  bool operator==(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(double num);
  double& operator()(int i, int j);
  const double& operator()(int i, int j) const;
};

#endif