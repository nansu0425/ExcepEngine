# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ExcepEngine is a custom C++ game engine with DirectX 11 rendering support. The project consists of two main components:
- **Engine** (DLL): Core engine library providing containers, math, memory management, and graphics rendering
- **Editor** (Executable): Windows application that uses the engine to provide a visual editor interface

## Build Commands

### Building the Solution
```bash
# Build entire solution (both Engine and Editor)
msbuild ExcepEngine.sln /p:Configuration=Debug /p:Platform=x64
msbuild ExcepEngine.sln /p:Configuration=Release /p:Platform=x64

# Build specific project
msbuild Source/Engine/Engine.vcxproj /p:Configuration=Debug /p:Platform=x64
msbuild Source/Editor/Editor.vcxproj /p:Configuration=Debug /p:Platform=x64
```

### Running
The Editor executable is the main entry point. After building, run from the output directory:
```bash
# Debug build
Source/Editor/x64/Debug/Editor.exe

# Release build
Source/Editor/x64/Release/Editor.exe
```

**Note:** The Editor requires Engine.dll to be in the same directory (automatically handled by build configuration).

### Post-Build Step
The Engine project automatically copies HLSL shader files to the output directory during build:
```bash
xcopy "$(ProjectDir)Graphics\D3D11\Shaders\*.hlsl" "$(OutDir)Shaders\" /Y /I /D
```

## Architecture

### Project Structure
```
Source/
  Engine/              # Core engine DLL
    Core/              # Base types, PCH, API definitions
      Types.h          # Custom type system (int8, int32, float32, etc.)
      ExcepAPI.h       # Main engine header, EXCEP_API macro
      Pch.h/cpp        # Precompiled header
      DllMain.cpp      # DLL entry point
    Container/         # Custom container wrappers (DynamicArray, HashMap, etc.)
    Memory/            # Smart pointer wrappers (UniquePtr, SharedPtr, WeakPtr)
    Math/              # Math types (Vector3, Vector4)
    Graphics/
      D3D11/           # DirectX 11 renderer implementation
        D3D11Renderer.h/cpp
        Shaders/       # HLSL shader files
  Editor/              # Editor application
    Main/
      WinMain.cpp      # Windows entry point, renderer initialization
```

### Namespace Structure
- **Root**: `Excep`
- **Sub-namespaces**: `Excep::Math`, `Excep::Graphics`, `Excep::Memory`, `Excep::Container`
- Always close namespaces with comments: `} // namespace Graphics` / `} // namespace Excep`

### DLL Export System
The engine uses `EXCEP_API` macro for DLL export/import:
- When building Engine (EXCEP_EXPORTS defined): `__declspec(dllexport)`
- When using Engine from Editor: `__declspec(dllimport)`
- Apply to all public API classes and functions

### Type System
All engine code uses custom types defined in [Core/Types.h](Source/Engine/Core/Types.h):
- Integers: `int8`, `int16`, `int32`, `int64`, `uint8`, `uint16`, `uint32`, `uint64`
- Floats: `float32`, `float64`
- Other: `bool8`, `char8`, `char16`
- **Exception**: Windows API types (HWND, HRESULT, etc.) and STL types use standard names

### Container System
The engine provides STL wrapper containers in the `Excep::Container` namespace:
- `DynamicArray<T>` - wraps std::vector
- `StaticArray<T, N>` - wraps std::array
- `HashMap<K, V>` - wraps std::unordered_map
- `TreeMap<K, V>` - wraps std::map
- `HashSet<T>` - wraps std::unordered_set
- `TreeSet<T>` - wraps std::set
- `String8` - wraps std::string (UTF-8)
- `String16` - wraps std::wstring (UTF-16)

These wrappers provide consistent naming (e.g., `Add()` instead of `push_back()`).

### Memory Management
Custom smart pointer wrappers in `Excep::Memory` namespace:
- `UniquePtr<T>` - wraps std::unique_ptr
- `SharedPtr<T>` - wraps std::shared_ptr
- `WeakPtr<T>` - wraps std::weak_ptr
- Use `MakeUnique<T>()` and `MakeShared<T>()` helper functions

### Precompiled Headers
- The Engine project uses [Core/Pch.h](Source/Engine/Core/Pch.h) as the precompiled header
- [Core/Pch.cpp](Source/Engine/Core/Pch.cpp) is set to "Create" precompiled header
- All other .cpp files are set to "Use" precompiled header
- When adding new .cpp files, ensure PCH settings are configured

### Graphics Rendering
The D3D11Renderer is the main rendering interface:
- Initialize with `Initialize(HWND hwnd, int32 width, int32 height)`
- Render with `Render()` each frame
- Handle window resizing with `OnResize(int32 width, int32 height)`
- Shaders located in [Graphics/D3D11/Shaders/](Source/Engine/Graphics/D3D11/Shaders/)

## Coding Conventions

**See [Documents/CODING_CONVENTIONS.md](Documents/CODING_CONVENTIONS.md) for complete details.** Key points:

### Naming
- **Classes/Structs/Functions**: PascalCase (e.g., `CreateDevice`, `D3D11Renderer`)
- **Member variables**: `m_` prefix + camelCase (e.g., `m_device`, `m_width`)
- **Static members**: `s_` prefix + camelCase
- **Global variables**: `g_` prefix + camelCase (e.g., `g_renderer`, `g_hwnd`)
- **Local variables**: camelCase
- **Constants**: UPPER_CASE with underscores

### Code Style
- **Brace style**: Allman (opening brace on new line)
- **Indentation**: 4 spaces (no tabs)
- **Include guard**: `#pragma once` (not `#ifndef`)
- **Encoding**: UTF-8 with BOM
- **Pointer/reference alignment**: Attach to type (`Type*`, `Type&`, not `Type *`)

### Success/Failure Pattern
- Initialization functions return `bool8`: `true` = success, `false` = failure
- Use early returns for error handling
- Functions that cannot fail return `void`

### Comments
- Single-line comments with `//`
- Korean/English mixed comments are allowed
- Public APIs must have Doxygen-style comments with `@brief`, `@param`, `@return`

### Include Order
1. Precompiled header (if in .cpp file)
2. Corresponding header (if in .cpp file)
3. Same module headers
4. Other engine module headers
5. Third-party library headers
6. Standard library headers
7. Platform headers (Windows.h, etc.)

## Adding New Code

### Adding a New Engine Module
1. Create header file in appropriate subdirectory under `Source/Engine/`
2. Add `#pragma once` as first line
3. Include required headers (at minimum `Core/Types.h` for types)
4. Wrap in appropriate namespace(s)
5. Add `EXCEP_API` macro to classes that should be exported
6. Add Doxygen comments for public APIs
7. Add to [Engine.vcxproj](Source/Engine/Engine.vcxproj) under `<ClInclude>`
8. If adding .cpp file, ensure PCH is set to "Use" in project settings

### Adding New Container or Utility
Follow existing patterns in [Container/](Source/Engine/Container/) or [Memory/](Source/Engine/Memory/):
- Create template wrapper around STL type
- Provide consistent method names (Add, Remove, GetSize, etc.)
- Include in [Core/ExcepAPI.h](Source/Engine/Core/ExcepAPI.h) if it's a core utility
- Add comprehensive Doxygen comments

### Working with DirectX 11
- Use ComPtr for D3D11 resource management (Microsoft::WRL::ComPtr)
- Check HRESULT return values from D3D11 API calls
- Shader files (.hlsl) are automatically copied during build
- Clean up resources in reverse order of creation
