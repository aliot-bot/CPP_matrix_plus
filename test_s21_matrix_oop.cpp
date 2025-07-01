#include <gtest/gtest.h>

#include <cmath>

#include "s21_matrix_oop.h"

#define EPSILON 1e-7

class S21MatrixTest : public ::testing::Test {
 protected:
  void SetUp() override {
    m_2x2 = S21Matrix(2, 2);
    m_2x2(0, 0) = 1.0;
    m_2x2(0, 1) = 2.0;
    m_2x2(1, 0) = 3.0;
    m_2x2(1, 1) = 4.0;
  }

  S21Matrix m_2x2;
};

TEST_F(S21MatrixTest, DefaultConstructor) {
  S21Matrix m;
  EXPECT_EQ(m.get_rows(), 1);
  EXPECT_EQ(m.get_cols(), 1);
  EXPECT_DOUBLE_EQ(m(0, 0), 0.0);
}
TEST_F(S21MatrixTest, ParameterizedConstructor) {
  S21Matrix m(3, 2);
  EXPECT_EQ(m.get_rows(), 3);
  EXPECT_EQ(m.get_cols(), 2);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      EXPECT_DOUBLE_EQ(m(i, j), 0.0);
    }
  }
  EXPECT_THROW(S21Matrix(0, 2), std::invalid_argument);
  EXPECT_THROW(S21Matrix(3, -1), std::invalid_argument);
}

TEST_F(S21MatrixTest, CopyConstructor) {
  S21Matrix m_copy(m_2x2);
  EXPECT_EQ(m_copy.get_rows(), 2);
  EXPECT_EQ(m_copy.get_cols(), 2);
  EXPECT_DOUBLE_EQ(m_copy(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m_copy(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m_copy(1, 0), 3.0);
  EXPECT_DOUBLE_EQ(m_copy(1, 1), 4.0);
  m_copy(0, 0) = 10.0;
  EXPECT_DOUBLE_EQ(m_2x2(0, 0), 1.0);
}

TEST_F(S21MatrixTest, MoveConstructor) {
  S21Matrix m_temp(m_2x2);
  S21Matrix m_move = std::move(m_temp);
  EXPECT_EQ(m_move.get_rows(), 2);
  EXPECT_EQ(m_move.get_cols(), 2);
  EXPECT_DOUBLE_EQ(m_move(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m_move(0, 1), 2.0);
  EXPECT_EQ(m_temp.get_rows(), 0);
  EXPECT_EQ(m_temp.get_cols(), 0);
  EXPECT_THROW(m_temp(0, 0), std::out_of_range);
}

TEST_F(S21MatrixTest, Destructor) {
  S21Matrix* m = new S21Matrix(3, 3);
  delete m;
}

TEST_F(S21MatrixTest, Getters) {
  EXPECT_EQ(m_2x2.get_rows(), 2);
  EXPECT_EQ(m_2x2.get_cols(), 2);
  S21Matrix m(4, 5);
  EXPECT_EQ(m.get_rows(), 4);
  EXPECT_EQ(m.get_cols(), 5);
}

TEST_F(S21MatrixTest, SetRows) {
  S21Matrix m(m_2x2);
  m.set_rows(3);
  EXPECT_EQ(m.get_rows(), 3);
  EXPECT_EQ(m.get_cols(), 2);
  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 4.0);
  EXPECT_DOUBLE_EQ(m(2, 0), 0.0);
  EXPECT_DOUBLE_EQ(m(2, 1), 0.0);
  m.set_rows(1);
  EXPECT_EQ(m.get_rows(), 1);
  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_THROW(m.set_rows(0), std::invalid_argument);
}

TEST_F(S21MatrixTest, SetCols) {
  S21Matrix m(m_2x2);
  m.set_cols(3);
  EXPECT_EQ(m.get_rows(), 2);
  EXPECT_EQ(m.get_cols(), 3);
  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(0, 2), 0.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 4.0);
  EXPECT_DOUBLE_EQ(m(1, 2), 0.0);
  m.set_cols(1);
  EXPECT_EQ(m.get_cols(), 1);
  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
  EXPECT_THROW(m.set_cols(-1), std::invalid_argument);
}

TEST_F(S21MatrixTest, AllocateMatrix) {
  S21Matrix m(3, 3);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      EXPECT_DOUBLE_EQ(m(i, j), 0.0);
    }
  }
}

TEST_F(S21MatrixTest, DeallocateMatrix) {
  S21Matrix m(3, 3);
  m.set_rows(1);
  EXPECT_EQ(m.get_rows(), 1);
  EXPECT_EQ(m.get_cols(), 3);
}

TEST_F(S21MatrixTest, CopyMatrix) {
  S21Matrix m(m_2x2);
  S21Matrix m_copy(1, 1);
  m_copy = m;
  EXPECT_EQ(m_copy.get_rows(), 2);
  EXPECT_EQ(m_copy.get_cols(), 2);
  EXPECT_DOUBLE_EQ(m_copy(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m_copy(0, 1), 2.0);
}

TEST_F(S21MatrixTest, GetMinor) {
  S21Matrix minor(1, 1);
  m_2x2.GetMinor(0, 0, minor);
  EXPECT_DOUBLE_EQ(minor(0, 0), 4.0);
  m_2x2.GetMinor(0, 1, minor);
  EXPECT_DOUBLE_EQ(minor(0, 0), 3.0);
}

TEST_F(S21MatrixTest, EqMatrix) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  EXPECT_TRUE(m1.EqMatrix(m2));
  m2(0, 0) = 1.0 + EPSILON / 2;
  EXPECT_TRUE(m1.EqMatrix(m2));
  m2(0, 0) = 2.0;
  EXPECT_FALSE(m1.EqMatrix(m2));
  S21Matrix m3(3, 2);
  EXPECT_FALSE(m1.EqMatrix(m3));
}

TEST_F(S21MatrixTest, SumMatrix) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  m1.SumMatrix(m2);
  EXPECT_DOUBLE_EQ(m1(0, 0), 2.0);
  EXPECT_DOUBLE_EQ(m1(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(m1(1, 0), 6.0);
  EXPECT_DOUBLE_EQ(m1(1, 1), 8.0);
  S21Matrix m3(3, 2);
  EXPECT_THROW(m1.SumMatrix(m3), std::logic_error);
}

TEST_F(S21MatrixTest, SubMatrix) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  m1.SubMatrix(m2);
  EXPECT_DOUBLE_EQ(m1(0, 0), 0.0);
  EXPECT_DOUBLE_EQ(m1(0, 1), 0.0);
  EXPECT_DOUBLE_EQ(m1(1, 0), 0.0);
  EXPECT_DOUBLE_EQ(m1(1, 1), 0.0);
  S21Matrix m3(2, 3);
  EXPECT_THROW(m1.SubMatrix(m3), std::logic_error);
}

TEST_F(S21MatrixTest, MulNumber) {
  S21Matrix m(m_2x2);
  m.MulNumber(2.0);
  EXPECT_DOUBLE_EQ(m(0, 0), 2.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 6.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 8.0);
}

TEST_F(S21MatrixTest, MulMatrix) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  m1.MulMatrix(m2);
  EXPECT_DOUBLE_EQ(m1(0, 0), 7.0);
  EXPECT_DOUBLE_EQ(m1(0, 1), 10.0);
  EXPECT_DOUBLE_EQ(m1(1, 0), 15.0);
  EXPECT_DOUBLE_EQ(m1(1, 1), 22.0);
  S21Matrix m3(3, 2);
  EXPECT_THROW(m1.MulMatrix(m3), std::logic_error);
}

TEST_F(S21MatrixTest, Transpose) {
  S21Matrix m = m_2x2.Transpose();
  EXPECT_EQ(m.get_rows(), 2);
  EXPECT_EQ(m.get_cols(), 2);
  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 3.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 2.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 4.0);
}

TEST_F(S21MatrixTest, CalcComplements) {
  S21Matrix m = m_2x2.CalcComplements();
  EXPECT_DOUBLE_EQ(m(0, 0), 4.0);
  EXPECT_DOUBLE_EQ(m(0, 1), -3.0);
  EXPECT_DOUBLE_EQ(m(1, 0), -2.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 1.0);
  S21Matrix m1x1(1, 1);
  EXPECT_THROW(m1x1.CalcComplements(), std::logic_error);
  S21Matrix m3x2(3, 2);
  EXPECT_THROW(m3x2.CalcComplements(), std::logic_error);
}

TEST_F(S21MatrixTest, Determinant) {
  S21Matrix m(m_2x2);
  double det = m.Determinant();
  EXPECT_DOUBLE_EQ(det, -2.0);
  S21Matrix m1x1(1, 1);
  m1x1(0, 0) = 5.0;
  EXPECT_DOUBLE_EQ(m1x1.Determinant(), 5.0);
  S21Matrix m3x2(3, 2);
  EXPECT_THROW(m3x2.Determinant(), std::logic_error);
}

TEST_F(S21MatrixTest, InverseMatrix) {
  S21Matrix m(m_2x2);
  S21Matrix inv = m.InverseMatrix();
  EXPECT_DOUBLE_EQ(inv(0, 0), -2.0);
  EXPECT_DOUBLE_EQ(inv(0, 1), 1.0);
  EXPECT_DOUBLE_EQ(inv(1, 0), 1.5);
  EXPECT_DOUBLE_EQ(inv(1, 1), -0.5);
  S21Matrix m_singular(2, 2);
  m_singular(0, 0) = 1.0;
  m_singular(0, 1) = 1.0;
  m_singular(1, 0) = 1.0;
  m_singular(1, 1) = 1.0;
  EXPECT_THROW(m_singular.InverseMatrix(), std::logic_error);
  S21Matrix m3x2(3, 2);
  EXPECT_THROW(m3x2.InverseMatrix(), std::logic_error);
}

TEST_F(S21MatrixTest, OperatorPlus) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  S21Matrix result = m1 + m2;
  EXPECT_DOUBLE_EQ(result(0, 0), 2.0);
  EXPECT_DOUBLE_EQ(result(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(result(1, 0), 6.0);
  EXPECT_DOUBLE_EQ(result(1, 1), 8.0);
}

TEST_F(S21MatrixTest, OperatorMinus) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  S21Matrix result = m1 - m2;
  EXPECT_DOUBLE_EQ(result(0, 0), 0.0);
  EXPECT_DOUBLE_EQ(result(0, 1), 0.0);
  EXPECT_DOUBLE_EQ(result(1, 0), 0.0);
  EXPECT_DOUBLE_EQ(result(1, 1), 0.0);
}

TEST_F(S21MatrixTest, OperatorMulMatrix) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  S21Matrix result = m1 * m2;
  EXPECT_DOUBLE_EQ(result(0, 0), 7.0);
  EXPECT_DOUBLE_EQ(result(0, 1), 10.0);
  EXPECT_DOUBLE_EQ(result(1, 0), 15.0);
  EXPECT_DOUBLE_EQ(result(1, 1), 22.0);
}

TEST_F(S21MatrixTest, OperatorMulNumber) {
  S21Matrix m(m_2x2);
  S21Matrix result = m * 2.0;
  EXPECT_DOUBLE_EQ(result(0, 0), 2.0);
  EXPECT_DOUBLE_EQ(result(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(result(1, 0), 6.0);
  EXPECT_DOUBLE_EQ(result(1, 1), 8.0);
  result = 2.0 * m;
  EXPECT_DOUBLE_EQ(result(0, 0), 2.0);
  EXPECT_DOUBLE_EQ(result(0, 1), 4.0);
}

TEST_F(S21MatrixTest, OperatorEqual) {
  S21Matrix m1(m_2x2);
  S21Matrix m2(m_2x2);
  EXPECT_TRUE(m1 == m2);
  m2(0, 0) = 2.0;
  EXPECT_FALSE(m1 == m2);
}

TEST_F(S21MatrixTest, OperatorAssignCopy) {
  S21Matrix m1(1, 1);
  m1 = m_2x2;
  EXPECT_EQ(m1.get_rows(), 2);
  EXPECT_EQ(m1.get_cols(), 2);
  EXPECT_DOUBLE_EQ(m1(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m1(0, 1), 2.0);
}

TEST_F(S21MatrixTest, OperatorAssignMove) {
  S21Matrix m1(1, 1);
  S21Matrix m_temp(m_2x2);
  m1 = std::move(m_temp);
  EXPECT_EQ(m1.get_rows(), 2);
  EXPECT_EQ(m1.get_cols(), 2);
  EXPECT_DOUBLE_EQ(m1(0, 0), 1.0);
  EXPECT_EQ(m_temp.get_rows(), 0);
  EXPECT_EQ(m_temp.get_cols(), 0);
}

TEST_F(S21MatrixTest, OperatorPlusAssign) {
  S21Matrix m(m_2x2);
  m += m_2x2;
  EXPECT_DOUBLE_EQ(m(0, 0), 2.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 6.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 8.0);
}

TEST_F(S21MatrixTest, OperatorMinusAssign) {
  S21Matrix m(m_2x2);
  m -= m_2x2;
  EXPECT_DOUBLE_EQ(m(0, 0), 0.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 0.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 0.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 0.0);
}

TEST_F(S21MatrixTest, OperatorMulAssignMatrix) {
  S21Matrix m(m_2x2);
  m *= m_2x2;
  EXPECT_DOUBLE_EQ(m(0, 0), 7.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 10.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 15.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 22.0);
}

TEST_F(S21MatrixTest, OperatorMulAssignNumber) {
  S21Matrix m(m_2x2);
  m *= 2.0;
  EXPECT_DOUBLE_EQ(m(0, 0), 2.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 6.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 8.0);
}

TEST_F(S21MatrixTest, OperatorParentheses) {
  S21Matrix m(m_2x2);
  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  m(0, 0) = 10.0;
  EXPECT_DOUBLE_EQ(m(0, 0), 10.0);
  EXPECT_THROW(m(2, 0), std::out_of_range);
  EXPECT_THROW(m(0, 2), std::out_of_range);
  EXPECT_THROW(m(-1, 0), std::out_of_range);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}