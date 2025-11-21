#pragma once
#include "Core/Types.h"

namespace Excep
{
namespace Math
{

struct Vector3
{
    float32 x, y, z;

    // 생성자
    Vector3();
    Vector3(float32 x, float32 y, float32 z);
    explicit Vector3(float32 scalar);

    // 단항 연산자
    Vector3 operator-() const;

    // 이항 연산자 (벡터-벡터)
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;

    // 이항 연산자 (벡터-스칼라)
    Vector3 operator*(float32 scalar) const;
    Vector3 operator/(float32 scalar) const;

    // 복합 대입 연산자 (벡터)
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);

    // 복합 대입 연산자 (스칼라)
    Vector3& operator*=(float32 scalar);
    Vector3& operator/=(float32 scalar);
};

// ========== 생성자 구현 ==========

inline Vector3::Vector3()
    : x(0.0f), y(0.0f), z(0.0f)
{
}

inline Vector3::Vector3(float32 x, float32 y, float32 z)
    : x(x), y(y), z(z)
{
}

inline Vector3::Vector3(float32 scalar)
    : x(scalar), y(scalar), z(scalar)
{
}

// ========== 단항 연산자 구현 ==========

inline Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

// ========== 이항 연산자 구현 (벡터-벡터) ==========

inline Vector3 Vector3::operator+(const Vector3& v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3 Vector3::operator-(const Vector3& v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

// ========== 이항 연산자 구현 (벡터-스칼라) ==========

inline Vector3 Vector3::operator*(float32 scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

inline Vector3 operator*(float32 scalar, const Vector3& v)
{
    return v * scalar;
}

inline Vector3 Vector3::operator/(float32 scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

// ========== 복합 대입 연산자 구현 (벡터) ==========

inline Vector3& Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

// ========== 복합 대입 연산자 구현 (스칼라) ==========

inline Vector3& Vector3::operator*=(float32 scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

inline Vector3& Vector3::operator/=(float32 scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

} // namespace Math
} // namespace Excep
