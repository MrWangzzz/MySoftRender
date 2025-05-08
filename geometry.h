#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <ostream>
#include <cassert>
#include <vector>

template <size_t DIM, typename T> struct vec
{
    vec() { for (size_t i = DIM; i--; data_[i] = T()); }
    T& operator[](const size_t i) { assert(i < DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i < DIM); return data_[i]; }

private:
    T data_[DIM];
};

template <typename T> struct vec<2, T>
{
    T x, y;

    vec() :x(T()), y(T()) {}
    vec(T x, T y) :x(x), y(y) {}

    T& operator[](const size_t i) { assert(i < 2); return i <= 0 ? x : y; }
    const T& operator[](const size_t i) const { assert(i < 2); return i <= 0 ? x : y; }
};


template <typename T> struct vec<3, T>
{
    T x, y, z;
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    float norm() { return std::sqrt(x * x + y * y + z * z); }
    vec<3, T>& normalize(T l = 1) { *this = (*this) * (l / norm()); return *this; }


    T& operator[](const size_t i) { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }
    const T& operator[](const size_t i) const { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }
};


template<size_t DIM, typename T>vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
{
    for (size_t i = DIM; i--; lhs[i] -= rhs[i]);
    return lhs;

}
template<size_t DIM, typename T>vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
{
    for (size_t i = DIM; i--; lhs[i] += rhs[i]);
    return lhs;
}





template<size_t DIM, typename T, typename U> vec<DIM, T> operator*(vec<DIM, T> lhs, const U& rhs) {
    for (size_t i = DIM; i--; lhs[i] *= rhs);
    return lhs;
}

template<size_t DIM, typename T, typename U> vec<DIM, T> operator/(vec<DIM, T> lhs, const U& rhs) {
    for (size_t i = DIM; i--; lhs[i] /= rhs);
    return lhs;
}

template<size_t DIM, typename T> T dot(const vec<DIM, T>& a, const vec<DIM, T>& b) {
    T result = T();
    for (size_t i = DIM; i--; result += a[i] * b[i]);
    return result;
}
template<typename T>vec<3, T> cross(const vec<3, T>& a, const vec<3, T>& b) {
    return vec<3, T>(
        a.y * b.z - a.z * b.y,    // x轴
        a.z * b.x - a.x * b.z,    // y轴
        a.x * b.y - a.y * b.x     // z轴
        );
}

typedef vec<2, float> Vec2f;
typedef vec<2, int>   Vec2i;
typedef vec<3, float> Vec3f;
typedef vec<3, int>   Vec3i;
typedef vec<4, float> Vec4f;

const int DEFAULT_ALLOC = 4;
class Matrix
{
    std::vector<std::vector<float> > m;
    int rows, cols;

public:
    Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC) : rows(r), cols(c) {
        m.resize(r);
        for (int i = 0; i < r; i++) {
            m[i].resize(c, 0.0f);
        }
    }

    std::vector<float>& operator[](const int i) {
        return m[i];
    }
    inline int RowCount();
    inline int ColCount();

    static Matrix orthographic(float left, float right, float bottom, float top, float fnear, float ffar) 
    {
        Matrix result(4, 4);
        // 缩放
        result[0][0] = 2.0f / (right - left);    // x轴缩放
        result[1][1] = 2.0f / (top - bottom);    // y轴缩放
        result[2][2] = -2.0f / (ffar - fnear);   // z轴缩放，注意负号

        // 平移
        result[0][3] = -(right + left) / (right - left);  // x轴平移
        result[1][3] = -(top + bottom) / (top - bottom);  // y轴平移
        result[2][3] = -(ffar + fnear) / (ffar - fnear);  // z轴平移

        result[3][3] = 1.0f;
        return result;
    }

private:

};



#endif