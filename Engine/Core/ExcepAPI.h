#pragma once

#ifdef EXCEP_EXPORTS
#define EXCEP_API __declspec(dllexport)
#else
#define EXCEP_API __declspec(dllimport)
#endif
