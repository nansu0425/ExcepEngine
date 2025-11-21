#pragma once

// 엔진 기본 타입
#include "Core/Types.h"

// 엔진 수학 라이브러리
#include "Math/Vector3.h"
#include "Math/Vector4.h"

// 엔진 컨테이너
#include "Container/DynamicArray.h"
#include "Container/StaticArray.h"
#include "Container/TreeMap.h"
#include "Container/HashMap.h"
#include "Container/TreeSet.h"
#include "Container/HashSet.h"
#include "Container/String8.h"
#include "Container/String16.h"

// 엔진 메모리 관리
#include "Memory/Memory.h"

// C++ 표준 라이브러리
#include <algorithm>

#ifdef EXCEP_EXPORTS
#define EXCEP_API __declspec(dllexport)
#else
#define EXCEP_API __declspec(dllimport)
#endif
