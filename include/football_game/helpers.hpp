#pragma once

#include <cmath>
#include <iomanip>
#include <ios>
#include <ostream>

namespace football::physics2d {

template <typename T>
struct Vector;

template <typename T>
struct Point {
  Point<T>& operator+=(const Vector<T>& vector) {
    x += vector.x;
    y += vector.y;
    return *this;
  }

  T x{0};
  T y{0};
};

template <typename T>
struct Vector {
  T x;
  T y;

  Vector operator+(const Vector<T>& other) const {
    return {x + other.x, y + other.y};
  }
  Vector operator-(const Vector<T>& other) const {
    return {x - other.x, y - other.y};
  }
  Vector operator*(const Vector<T>& other) const {
    return {x * other.x, y * other.y};
  }

  Vector& operator+=(const Vector<T>& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vector& operator*=(T factor) {
    x *= factor;
    y *= factor;
    return *this;
  }

  T GetLength() const { return std::sqrt(x * x + y * y); }

  Point<T> GetPoint() const { return {x, y}; }

  Vector<T> Normalize() const {
    const auto length = GetLength();
    return length == static_cast<T>(0) ? Vector<T>{0, 0}
                                       : Vector<T>{x / length, y / length};
  }
};

template <typename T>
Point<T> operator+(const Point<T>& point, const Vector<T>& vector) {
  return {point.x + vector.x, point.y + vector.y};
}

template <typename T>
Point<T> operator-(const Point<T>& point, const Vector<T>& vector) {
  return {point.x - vector.x, point.y - vector.y};
}

template <typename T>
Vector<T> operator*(const Vector<T>& vector, T factor) {
  return {vector.x * factor, vector.y * factor};
}

template <typename T>
Vector<T> operator-(const Point<T>& first, const Point<T>& second) {
  return {first.x - second.x, first.y - second.y};
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Point<T>& point) {
  os << "(" << std::fixed << std::setprecision(1) << point.x << ", " << point.y
     << ")";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vector) {
  os << "[" << std::fixed << std::setprecision(1) << vector.x << ", "
     << vector.y << "]";
  return os;
}

}  // namespace football::physics2d