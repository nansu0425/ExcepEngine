#pragma once

// 엔진 기본 타입
#include "Core/Types.h"

// 엔진 수학 라이브러리
#include "Math/Vector3.h"
#include "Math/Vector4.h"

// 엔진 컨테이너
#include "Container/DynamicArray.h"

// C++ 표준 라이브러리
#include <memory>
#include <string>
#include <algorithm>

#ifdef EXCEP_EXPORTS
#define EXCEP_API __declspec(dllexport)
#else
#define EXCEP_API __declspec(dllimport)
#endif
