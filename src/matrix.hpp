#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <concepts>

typedef uint16_t u16;

template <typename E>
concept MatrixElement = std::is_arithmetic_v<E>;

template <MatrixElement E>
class Matrix{
    u16 row_;
    u16 col_;
    std::vector<E> matrix_;

public:
    Matrix(u16 row, u16 col) : row_(row), col_(col), matrix_(row * col) {}

    E& at(u16 row, u16 col) {
        if (row >= row_ || col >= col_)
            throw std::out_of_range("Matrix index out of range");
        return matrix_[row * col_ + col];
    }
    const E& at(u16 row, u16 col) const {
        if (row >= row_ || col >= col_)
            throw std::out_of_range("Matrix index out of range");
        return matrix_[row * col_ + col];
    }

    E& operator[](u16 row, u16 col) { return matrix_[row * col_ + col]; }
    const E& operator[](u16 row, u16 col) const { return matrix_[row * col_ + col]; }

    auto begin()       { return matrix_.begin(); }
    auto end()         { return matrix_.end(); }
    auto begin() const { return matrix_.cbegin(); }
    auto end()   const { return matrix_.cend(); }

    u16 size() const { return row_ * col_; }
    u16 rows() const { return row_; }
    u16 cols() const { return col_; }

    u16 count(const E& value) const {
        return static_cast<u16>(std::count(matrix_.begin(), matrix_.end(), value));
    }


};

// We need a special case for boolean vector
template <>
class Matrix<bool> {
    u16 row_;
    u16 col_;
    std::vector<bool> matrix_;

public:
    Matrix(u16 row, u16 col) : row_(row), col_(col), matrix_(row * col) {}

    std::vector<bool>::reference at(u16 row, u16 col) {
        if (row >= row_ || col >= col_)
            throw std::out_of_range("Matrix index out of range");
        return matrix_[row * col_ + col];
    }

    bool at(u16 row, u16 col) const {
        if (row >= row_ || col >= col_)
            throw std::out_of_range("Matrix index out of range");
        return matrix_[row * col_ + col];
    }

    std::vector<bool>::reference operator[](u16 row, u16 col) {
        return matrix_[row * col_ + col];
    }

    bool operator[](u16 row, u16 col) const {
        return matrix_[row * col_ + col];
    }

    u16 rows() const { return row_; }
    u16 cols() const { return col_; }
    u16 size() const { return row_ * col_; }

    auto begin()       { return matrix_.begin(); }
    auto end()         { return matrix_.end(); }
    auto begin() const { return matrix_.cbegin(); }
    auto end()   const { return matrix_.cend(); }

    u16 count(bool value) const {
        return static_cast<u16>(std::count(matrix_.begin(), matrix_.end(), value));
    }
};