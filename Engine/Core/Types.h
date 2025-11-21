#pragma once
#include <cstdint>

// 부호 있는 정수 타입
using int8   = int8_t;
using int16  = int16_t;
using int32  = int32_t;
using int64  = int64_t;

// 부호 없는 정수 타입
using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

// 부동소수점 타입
using float32 = float;
using float64 = double;

// 문자 및 불리언 타입
using bool8  = bool;
using char8  = char;
using char16 = wchar_t;

// 타입 크기 검증
static_assert(sizeof(int8)    == 1, "int8 must be 1 byte");
static_assert(sizeof(int16)   == 2, "int16 must be 2 bytes");
static_assert(sizeof(int32)   == 4, "int32 must be 4 bytes");
static_assert(sizeof(int64)   == 8, "int64 must be 8 bytes");
static_assert(sizeof(uint8)   == 1, "uint8 must be 1 byte");
static_assert(sizeof(uint16)  == 2, "uint16 must be 2 bytes");
static_assert(sizeof(uint32)  == 4, "uint32 must be 4 bytes");
static_assert(sizeof(uint64)  == 8, "uint64 must be 8 bytes");
static_assert(sizeof(float32) == 4, "float32 must be 4 bytes");
static_assert(sizeof(float64) == 8, "float64 must be 8 bytes");
static_assert(sizeof(bool8)   == 1, "bool8 must be 1 byte");
static_assert(sizeof(char8)   == 1, "char8 must be 1 byte");
static_assert(sizeof(char16)  == 2, "char16 must be 2 bytes");
