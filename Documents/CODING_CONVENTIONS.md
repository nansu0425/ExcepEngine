# ExcepEngine 코딩 컨벤션

## 1. 네이밍 컨벤션

### 클래스 및 구조체
- **PascalCase** 사용
- 예: `D3D11Renderer`, `Vector3`, `Vector4`

### 함수 및 메서드
- **PascalCase** 사용
- 동사로 시작 (Create, Initialize, Shutdown, Render 등)
- 예: `Initialize()`, `CreateDeviceAndSwapChain()`, `ReadShaderFile()`

### 변수
- **멤버 변수**: `m_` prefix + camelCase
  - 예: `m_device`, `m_width`, `m_renderTargetView`
- **정적 멤버 변수**: `s_` prefix + camelCase
  - 예: `s_instanceCount`, `s_defaultTexture`
- **전역 변수**: `g_` prefix + camelCase
  - 예: `g_renderer`, `g_hwnd`
- **로컬 변수**: camelCase
  - 예: `width`, `height`, `vertexBuffer`

### 상수
- **UPPER_CASE** with underscores
- 예: `MAX_LOADSTRING`, `WIN32_LEAN_AND_MEAN`

### 네임스페이스
- **Root 네임스페이스**: `Excep`
- **Sub 네임스페이스**: `Excep::Math`, `Excep::Graphics`
- 네임스페이스 닫을 때 주석 추가:
  ```cpp
  } // namespace Graphics
  } // namespace Excep
  ```

## 2. 코드 스타일

### 중괄호 배치
- **Allman 스타일** (여는 중괄호 새 줄)
```cpp
bool8 Initialize()
{
    if (condition)
    {
        // code
    }
}
```

### 들여쓰기
- **스페이스 4칸** 사용 (탭 사용 금지)

### Include Guard
- **`#pragma once`** 사용 (전통적인 `#ifndef` 가드 사용 금지)
- 모든 헤더 파일 첫 줄에 위치

### 파일 인코딩
- **UTF-8 with BOM**
- 한국어 주석 사용 가능

### 포인터 및 참조 정렬
- 타입에 붙여서 작성: `Type&`, `Type*`
```cpp
const Vector3& v      // ✓ 올바름
const Vector3 &v      // ✗ 잘못됨
```

## 3. 타입 시스템

### 커스텀 타입 사용
`Core/Types.h`에 정의된 명시적 크기 타입을 사용:

```cpp
// 정수 타입
int8, int16, int32, int64
uint8, uint16, uint32, uint64

// 부동소수점 타입
float32, float64

// 기타 타입
bool8, char8, char16
```

### 타입 사용 규칙
- **항상 사용**: 플랫폼 독립적 코드 작성을 위해 커스텀 타입 사용
- **예외**: Windows API 타입 (HWND, HRESULT, DWORD 등)은 그대로 사용
- **엔진 타입 우선**: 컨테이너는 `Excep::Container`, 포인터는 `Excep::Memory` 사용
- **STL 사용 제한**: 서드파티 라이브러리 인터페이스 등 필요한 경우에만 STL 직접 사용

### 타입 크기 검증
- 새로운 타입 정의 시 `static_assert`로 크기 검증:
```cpp
static_assert(sizeof(int32) == 4, "int32 must be 4 bytes");
```

### 컨테이너 사용
`Excep::Container`에 구현된 엔진 컨테이너를 사용합니다.

**사용 가능한 컨테이너:**
```cpp
// 동적 배열
DynamicArray<T>         // std::vector 래퍼

// 고정 크기 배열
StaticArray<T, N>       // std::array 래퍼

// 맵 (Key-Value)
TreeMap<K, V>           // std::map 래퍼 (정렬됨, O(log n))
HashMap<K, V>           // std::unordered_map 래퍼 (정렬 안 됨, O(1))

// 집합 (Set)
TreeSet<T>              // std::set 래퍼 (정렬됨, O(log n))
HashSet<T>              // std::unordered_set 래퍼 (정렬 안 됨, O(1))

// 문자열
String8                 // UTF-8 문자열 (char8 기반)
String16                // UTF-16 문자열 (char16 기반, Windows API 호환)
```

**사용 예시:**
```cpp
using namespace Excep::Container;

// 동적 배열
DynamicArray<int32> numbers;
numbers.Add(10);
numbers.Add(20);
int32 first = numbers.GetAt(0);

// 맵
HashMap<String8, int32> scores;
scores.Insert(String8("player1"), 100);
if (scores.Contains(String8("player1")))
{
    int32 score = scores.GetAt(String8("player1"));
}

// 문자열 (Windows API와 함께 사용)
String16 filePath = L"C:\\path\\to\\file.txt";
CreateFileW(filePath.GetCString(), ...);
```

**규칙:**
- STL 컨테이너(`std::vector`, `std::map` 등) 대신 엔진 컨테이너 사용
- `std::string`, `std::wstring` 대신 `String8`, `String16` 사용
- 예외: 서드파티 라이브러리와의 인터페이스에서는 STL 사용 가능

### 스마트 포인터 사용
Raw 포인터 사용을 지양하고 `Excep::Memory`의 스마트 포인터를 사용합니다.

**사용 가능한 스마트 포인터:**
```cpp
// 독점 소유권 포인터
UniquePtr<T>            // std::unique_ptr 래퍼 (이동 전용)

// 공유 소유권 포인터
SharedPtr<T>            // std::shared_ptr 래퍼 (복사 가능, 참조 카운팅)

// 약한 참조 포인터
WeakPtr<T>              // std::weak_ptr 래퍼 (순환 참조 방지)
```

**사용 예시:**
```cpp
using namespace Excep::Memory;

// UniquePtr - 독점 소유권
auto renderer = MakeUnique<D3D11Renderer>();
renderer->Initialize(hwnd, width, height);

// SharedPtr - 공유 소유권
auto texture = MakeShared<Texture>();
auto copy = texture;  // 복사 가능
uint64 refCount = texture.GetUseCount();  // 참조 카운트: 2

// WeakPtr - 순환 참조 방지
WeakPtr<Texture> weakTexture = texture;
if (!weakTexture.IsExpired())
{
    auto lockedTexture = weakTexture.Lock();
    lockedTexture->Use();
}
```

**포인터 선택 가이드:**
- **UniquePtr**: 기본 선택. 명확한 소유권이 있는 경우
- **SharedPtr**: 여러 곳에서 소유권을 공유해야 하는 경우
- **WeakPtr**: 순환 참조를 방지해야 하는 경우 (캐싱, 옵저버 패턴 등)
- **Raw 포인터**: 소유권이 없는 참조만 필요한 경우 (매개변수 전달 등)

**규칙:**
- `new`/`delete` 직접 사용 금지 (스마트 포인터 또는 스택 할당 사용)
- 소유권이 명확한 경우 `UniquePtr` 우선 사용
- 팩토리 함수는 `MakeUnique`, `MakeShared` 사용
- Raw 포인터는 non-owning reference로만 사용

## 4. API 디자인

### DLL Export
- 공개 API 클래스에 `EXCEP_API` 매크로 사용:
```cpp
class EXCEP_API D3D11Renderer
{
    // ...
};
```

### 성공/실패 반환 타입
- 초기화 및 생성 함수: `bool8` 반환
  - `true` = 성공
  - `false` = 실패
- 실패할 수 없는 함수: `void` 반환

```cpp
bool8 Initialize();    // 초기화 함수
void Shutdown();       // 정리 함수
```

### 에러 처리 패턴
- Early return 사용:
```cpp
if (!CreateDevice())
{
    return false;
}
if (!CreateSwapChain())
{
    return false;
}
```

## 5. 주석 작성

### 일반 주석
- 단일 라인: `//` 사용
- 블록 주석: `/* */` 최소화
- 언어: 한국어/영어 혼용 가능

```cpp
// 백 버퍼 지우기
m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
```

### 공개 API 주석 (Doxygen 스타일)
공개 API (EXCEP_API로 export되는 클래스, 함수)는 Doxygen 스타일 주석 작성:

```cpp
/// @brief 렌더러를 초기화합니다
/// @param hwnd 렌더링 대상 윈도우 핸들
/// @param width 렌더링 해상도 너비
/// @param height 렌더링 해상도 높이
/// @return 초기화 성공 시 true, 실패 시 false
bool8 Initialize(HWND hwnd, int32 width, int32 height);

/// @brief 한 프레임을 렌더링합니다
void Render();
```

**Doxygen 태그:**
- `@brief` - 간단한 설명
- `@param` - 매개변수 설명
- `@return` - 반환값 설명
- `@note` - 주의사항
- `@warning` - 경고사항
- `@see` - 관련 항목 참조

## 6. 파일 조직

### 파일 명명
- 헤더 파일: `.h` 확장자
- 소스 파일: `.cpp` 확장자
- 헤더와 소스 파일은 동일한 이름 사용

### 폴더 구조
```
Source/
  Engine/
    Core/          - 핵심 타입, API 정의
    Math/          - 수학 라이브러리
    Container/     - 컨테이너 (배열, 맵, 집합, 문자열)
    Memory/        - 메모리 관리 (스마트 포인터)
    Graphics/      - 렌더링 시스템
      D3D11/       - DirectX 11 구현
  Editor/
    Main/          - 에디터 진입점
```

### Include 순서
1. 해당 소스의 헤더 (`.cpp` 파일의 경우)
2. 같은 모듈의 헤더
3. 다른 모듈의 엔진 헤더
4. 서드파티 라이브러리 헤더
5. 표준 라이브러리 헤더
6. 플랫폼 헤더 (Windows.h 등)

```cpp
#include "Core/Pch.h"                    // PCH
#include "Graphics/D3D11/D3D11Renderer.h" // 모듈 헤더
#include <d3dcompiler.h>                 // 서드파티
#include <fstream>                       // 표준 라이브러리
#include <sstream>
```

### 헤더 구조
```cpp
#pragma once

// Include 영역
#include "Core/Types.h"

// Using 선언 (필요한 경우)
using Microsoft::WRL::ComPtr;

// 네임스페이스 열기
namespace Excep
{
namespace Graphics
{

// 전방 선언
struct Vertex;

// 클래스/구조체 정의
class EXCEP_API D3D11Renderer
{
    // ...
};

} // namespace Graphics
} // namespace Excep
```

## 7. 기타 규칙

### Pragma Warning
- 불가피한 경우 pragma로 경고 제어:
```cpp
#pragma warning(push)
#pragma warning(disable: 4251)
// COM 인터페이스 등
#pragma warning(pop)
```

### 인라인 함수
- 수학 연산자 오버로딩 등 성능이 중요한 경우 `inline` 사용
- 복잡한 로직은 `.cpp` 파일에 구현

### Using 선언
- 네임스페이스 단축을 위해 사용:
```cpp
using Excep::Graphics::D3D11Renderer;
using Excep::Math::Vector3;
```
- 헤더 파일에서는 최소화, 소스 파일에서 자유롭게 사용
