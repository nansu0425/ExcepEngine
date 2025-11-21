#pragma once

#include "Core/Types.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#ifdef EXCEP_EXPORTS
#define EXCEP_API __declspec(dllexport)
#else
#define EXCEP_API __declspec(dllimport)
#endif
