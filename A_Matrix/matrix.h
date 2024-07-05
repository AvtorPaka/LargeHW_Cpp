#define MATRIX_SQUARE_MATRIX_IMPLEMENTED
#ifndef LARGETASKS_MATRIX_H
#define LARGETASKS_MATRIX_H

#include <cstdint>
#include <exception>
#include <istream>
#include <ostream>
#include <algorithm>

class MatrixOutOfRange {};
class MatrixInvalidDimensions : std::exception {};
class MatrixIsDegenerateError {};

template <class T, size_t N, size_t M>
class Matrix {
 public:
  Matrix<T, N - 1, M - 1> TrimMatrix(size_t i_d, size_t j_d) const {
    Matrix<T, N - 1, M - 1> trimmed_matrix;
    int32_t i_offset = 0;
    for (size_t i = 0u; i < N; ++i) {
      int32_t j_offset = 0;
      if (i == i_d) {
        i_offset = -1;
        continue;
      }
      for (size_t j = 0u; j < M; ++j) {
        if (j == j_d) {
          j_offset = -1;
          continue;
        }
        trimmed_matrix.inner_matrix_[i + i_offset][j + j_offset] = this->inner_matrix_[i][j];
      }
    }

    return trimmed_matrix;
  }

  T inner_matrix_[N][M];

  [[nodiscard]] size_t RowsNumber() const {
    return N;
  }

  [[nodiscard]] size_t ColumnsNumber() const {
    return M;
  }

  T& operator()(size_t n, size_t m) {
    return inner_matrix_[n][m];
  }

  const T& operator()(size_t n, size_t m) const {
    return inner_matrix_[n][m];
  }

  T& At(size_t n, size_t m) {
    if (n >= N || m >= M) {
      throw MatrixOutOfRange();
    }
    return inner_matrix_[n][m];
  }

  const T& At(size_t n, size_t m) const {
    if (n >= N || m >= M) {
      throw MatrixOutOfRange();
    }
    return inner_matrix_[n][m];
  }

  Matrix<T, N, M> operator+(const Matrix<T, N, M>& other) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0u; i < N; ++i) {
      for (size_t j = 0u; j < M; ++j) {
        new_matrix(i, j) = inner_matrix_[i][j] + other(i, j);
      }
    }
    return new_matrix;
  }

  Matrix<T, N, M>& operator+=(const Matrix<T, N, M>& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        this->inner_matrix_[i][j] += other.inner_matrix_[i][j];
      }
    }
    return *this;
  }

  Matrix<T, N, M> operator-(const Matrix<T, N, M>& other) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0u; i < N; ++i) {
      for (size_t j = 0u; j < M; ++j) {
        new_matrix(i, j) = inner_matrix_[i][j] - other(i, j);
      }
    }
    return new_matrix;
  }

  Matrix<T, N, M>& operator-=(const Matrix<T, N, M>& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        this->inner_matrix_[i][j] -= other.inner_matrix_[i][j];
      }
    }
    return *this;
  }

  template <size_t M1, size_t W>
  Matrix<T, N, W> operator*(const Matrix<T, M1, W>& other) const{
    if (M != M1) {
      throw MatrixInvalidDimensions{};
    }
    Matrix<T, N, W> new_matrix;
    for (size_t i = 0u; i < N; ++i) {
      for (size_t w = 0u; w < W; ++w) {
        T nw_temp_value = T();
        for (size_t j = 0u; j < M; ++j) {
          nw_temp_value += this->inner_matrix_[i][j] * other.inner_matrix_[j][w];
        }
        new_matrix.inner_matrix_[i][w] = nw_temp_value;
      }
    }

    return new_matrix;
  }

  template <size_t M1, size_t W>
  Matrix<T, N, W>& operator*=(const Matrix<T, M1, W>& other) {
    if (M != M1) {
      throw MatrixInvalidDimensions{};
    }
    Matrix<T, N, W> almost_new_matrix;
    for (size_t w = 0u; w < W; ++w) {
      for (size_t i = 0u; i < N; ++i) {
        T nw_temp_value = T();
        for (size_t j = 0u; j < M; ++j) {
          nw_temp_value += this->inner_matrix_[i][j] * other.inner_matrix_[j][w];
        }
        almost_new_matrix.inner_matrix_[i][w] = nw_temp_value;
      }
    }

    *this = almost_new_matrix;
    return *this;
  }

  bool operator==(const Matrix<T, N, M>& other) const {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        if (this->inner_matrix_[i][j] != other.inner_matrix_[i][j]) {
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const Matrix<T, N, M> other) const {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        if (this->inner_matrix_[i][j] != other.inner_matrix_[i][j]) {
          return true;
        }
      }
    }
    return false;
  }

  Matrix<T, N, M> operator*(const T& num) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_matrix.inner_matrix_[i][j] = this->inner_matrix_[i][j] * num;
      }
    }
    return new_matrix;
  }

  friend Matrix<T, N, M> operator*(const T& num, Matrix<T, N, M> matrix) {
    Matrix<T, N, M> new_matrix = matrix * num;
    return new_matrix;
  }

  Matrix<T, N, M>& operator*=(const T& num) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        this->inner_matrix_[i][j] *= num;
      }
    }

    return *this;
  }

  template <class W>
  Matrix<T, N, M> operator/(const W& num) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_matrix.inner_matrix_[i][j] = this->inner_matrix_[i][j] / num;
      }
    }
    return new_matrix;
  }

  template <class W>
  Matrix<T, N, M>& operator/=(const W& num) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        this->inner_matrix_[i][j] /= num;
      }
    }

    return *this;
  }

  friend std::istream& operator>>(std::istream& is, Matrix<T, N, M>& matrix) {
    for (size_t i = 0u; i < N; ++i) {
      for (size_t j = 0u; j < M; ++j) {
        is >> matrix.inner_matrix_[i][j];
      }
    }

    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix<T, N, M>& matrix) {
    for (size_t i = 0u; i < N; ++i) {
      for (size_t j = 0u; j < M; ++j) {
        os << matrix.inner_matrix_[i][j] << (j == M - 1 ? "\n" : " ");
      }
    }
    return os;
  }

  ~Matrix() = default;
};

template <class T, size_t N>
constexpr T Determinant(const Matrix<T, N, N>& matrix) {
  T matrix_determinant = T();
  for (size_t j = 0u; j < N; ++j) {
    int32_t sign = (j % 2 == 0 ? 1 : -1);
    Matrix<T, N - 1, N - 1> matrix_part = matrix.TrimMatrix(0, j);
    matrix_determinant += matrix.inner_matrix_[0][j] * sign * Determinant(matrix_part);
  }

  return matrix_determinant;
}

template <class T>
constexpr T Determinant(const Matrix<T, 1, 1>& matrix) {
  return matrix.inner_matrix_[0][0];
}

template <class T>
constexpr T Determinant(const Matrix<T, 2, 2>& matrix) {
  return matrix.inner_matrix_[0][0] * matrix.inner_matrix_[1][1] -
         matrix.inner_matrix_[0][1] * matrix.inner_matrix_[1][0];
}

template <class T, size_t N>
T Trace(const Matrix<T, N, N>& matrix) {
  T matrix_trace = T();
  for (size_t i = 0u; i < N; ++i) {
    matrix_trace += matrix.inner_matrix_[i][i];
  }
  return matrix_trace;
}

template <class T, size_t N, size_t M>
Matrix<T, M, N> GetTransposed(const Matrix<T, N, M>& matrix) {
  Matrix<T, M, N> transposed_matrix;
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < M; ++j) {
      transposed_matrix.inner_matrix_[j][i] = matrix.inner_matrix_[i][j];
    }
  }

  return transposed_matrix;
}
template <class T, size_t N>
void Transpose(Matrix<T, N, N>& matrix) {
  Matrix<T, N, N> matrix_transposed =  GetTransposed(matrix);
  matrix = matrix_transposed;
}


template <class T, size_t N>
Matrix<T, N, N> GetInversed(const Matrix<T, N, N>& matrix) {
  T matrix_det = Determinant(matrix);
  if (matrix_det == T()) {
    throw MatrixIsDegenerateError{};
  }

  Matrix<T, N, N> invesed_matrix;
  for (size_t i = 0u; i < N; ++i) {
    for (size_t j = 0u; j < N; ++j) {
      int32_t sign = ((i + j) % 2 == 0 ? 1 : -1);
      T cur_det = Determinant(matrix.TrimMatrix(i, j));
      invesed_matrix.inner_matrix_[i][j] = sign * cur_det;
    }
  }

  Transpose(invesed_matrix);
  return invesed_matrix / matrix_det;
}

template <class T>
Matrix<T, 1, 1> GetInversed(const Matrix<T, 1, 1>& matrix) {
  if (matrix.inner_matrix_[0][0] == 0) {
    throw MatrixIsDegenerateError{};
  }
  Matrix<T, 1, 1> inversed_matrix;
  inversed_matrix.inner_matrix_[0][0] = 1 / matrix.inner_matrix_[0][0];
  return inversed_matrix;
}

template <class T, size_t N>
void Inverse(Matrix<T, N, N>& matrix) {
  Matrix<T, N, N> inversed_matrix = GetInversed(matrix);
  matrix = inversed_matrix;
}

#endif
