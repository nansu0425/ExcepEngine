#pragma once
#include "Core/Types.h"

namespace Excep
{

struct Vector4
{
    float32 x, y, z, w;

    // 생성자
    Vector4();
    Vector4(float32 x, float32 y, float32 z, float32 w);
    explicit Vector4(float32 scalar);

    // 단항 연산자
    Vector4 operator-() const;

    // 이항 연산자 (벡터-벡터)
    Vector4 operator+(const Vector4& v) const;
    Vector4 operator-(const Vector4& v) const;

    Vector4 operator*(float32 scalar) const;
    Vector4 operator/(float32 scalar) const;

    // 복합 대입 연산자 (벡터)
    Vector4& operator+=(const Vector4& v);
    Vector4& operator-=(const Vector4& v);

    Vector4& operator*=(float32 scalar);
    Vector4& operator/=(float32 scalar);
};

// ========== 생성자 구현 ==========

inline Vector4::Vector4()
    : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

inline Vector4::Vector4(float32 x, float32 y, float32 z, float32 w)
    : x(x), y(y), z(z), w(w)
{
}

inline Vector4::Vector4(float32 scalar)
    : x(scalar), y(scalar), z(scalar), w(scalar)
{
}

// ========== 단항 연산자 구현 ==========

inline Vector4 Vector4::operator-() const
{
    return Vector4(-x, -y, -z, -w);
}

// ========== 이항 연산자 구현 (벡터-벡터) ==========

inline Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

// ========== 이항 연산자 구현 (벡터-스칼라) ==========

inline Vector4 Vector4::operator*(float32 scalar) const
{
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

inline Vector4 operator*(float32 scalar, const Vector4& v)
{
    return v * scalar;
}

inline Vector4 Vector4::operator/(float32 scalar) const
{
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

// ========== 복합 대입 연산자 구현 (벡터) ==========

inline Vector4& Vector4::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

inline Vector4& Vector4::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

// ========== 복합 대입 연산자 구현 (스칼라) ==========

inline Vector4& Vector4::operator*=(float32 scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

inline Vector4& Vector4::operator/=(float32 scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

} // namespace Excep
