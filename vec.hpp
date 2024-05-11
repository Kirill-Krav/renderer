#pragma once

#include <cmath>

template<typename T>
class Vec
{
public:
    Vec()
    {
        data[0] = data[1] = data[2] = T();
    }
    Vec(T x, T y, T z)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }
    T &operator[](int i)
    {
        return data[i];
    }
    T x() const { return data[0]; }
    T r() const { return data[0]; }
    T y() const { return data[1]; }
    T g() const { return data[1]; }
    T z() const { return data[2]; }
    T b() const { return data[2]; }

    T distance() const
    {
        return sqrtf(x() * x() + y() * y() + z() * z());
    }

    Vec &normalize()
    {
        *this = *this / distance();
        return *this;
    }

    Vec operator*(const Vec &other) const
    {
        return Vec(x() * other.x(), y() * other.y(), z() * other.z());
    }

    template<typename T1>
    Vec operator*(const T1 &val) const
    {
        return Vec(x() * val, y() * val, z() * val);
    }

    Vec operator/(T val) const
    {
        return Vec(x() / val, y() / val, z() / val);
    }

    Vec operator-(const Vec &other) const
    {
        return Vec(x() - other.x(), y() - other.y(), z() - other.z());
    }

    Vec operator+(const Vec &other) const
    {
        return Vec(x() + other.x(), y() + other.y(), z() + other.z());
    }
    static T dot(const Vec lhs, const Vec rhs)
    {
        return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
    }
    static Vec cross(const Vec lhs, const Vec rhs)
    {
        return Vec(1,0,0) * (lhs.y() * rhs.z() - lhs.z() * rhs.y()) + Vec(0,1,0) * (lhs.z() * rhs.x() - lhs.x() * rhs.z()) + Vec(0,0,1) * (lhs.x() * rhs.y() - lhs.y() * rhs.x());
    }

private:
    T data[3];
};