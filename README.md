# BerkEngine

Modular, multi-backend 3D game engine built with C++17, featuring abstracted render backend support for OpenGL and Vulkan (planned), entity-component-system (ECS) architecture, and modern rendering patterns.

## Overview

BerkEngine is a lightweight game engine designed with extensibility and multi-API support in mind. The architecture separates rendering concerns through a pluggable backend abstraction layer, allowing seamless switching between rendering APIs without core engine changes.

### Key Features

- **Multi-Backend Rendering**: Abstracted renderer interface supporting OpenGL (production-ready) and Vulkan (skeleton)
- **Entity-Component-System (ECS)**: Scene graph built on [EnTT](https://github.com/skypjack/entt) for efficient entity management
- **Factory Pattern**: Polymorphic resource creation (Shaders, Meshes) that adapts to the active backend
- **Modern C++ Patterns**: Smart pointers, move semantics, and const-correctness throughout
- **Platform Abstraction**: Windowing and input via GLFW, math via GLM
- **Logging System**: Structured logging with spdlog

## Architecture

### Rendering Backend Abstraction

The core rendering system is built around a `RenderBackend` abstract interface:

```
RenderBackend (abstract interface)
├── OpenGLBackend (fully implemented)
│   ├── OpenGLShader
│   └── OpenGLMesh
└── VulkanBackend (skeleton - TODO)
    ├── VulkanShader (stub)
    └── VulkanMesh (stub)
```

**Backend Selection:**
- Backend type specified via `Renderer::Init(RenderBackendType type, width, height)`
- Currently hardcoded to `RenderBackendType::OpenGL` in `Application`
- Resource factories (`Shader::Create`, `Mesh::Create`) currently bound to OpenGL implementations

### Entity-Component-System

Scene management leverages EnTT for efficient ECS:

- **Entity**: Lightweight ID wrapper representing game objects
- **Component**: Data containers (e.g., `TransformComponent`, `MeshComponent`, `TagComponent`)
- **System**: Logic operating on entities with specific components (e.g., render loop in `Scene::OnUpdate`)

Example:
```cpp
auto cube = scene->CreateEntity("3D Cube");
cube.AddComponent<TransformComponent>();
cube.AddComponent<MeshComponent>(meshPtr);
```

Rendering is performed via view iteration:
```cpp
auto view = m_Registry.view<TransformComponent, MeshComponent>();
view.each([&](auto entity, auto& transform, auto& mesh) {
    Renderer::Submit(*mesh.MeshPtr, transform.Transform, *m_Shader, *m_Camera);
});
```

## Project Structure

```
BerkEngine/
├── src/
│   ├── bepch.h                     # Precompiled header
│   ├── BerkEngine.h                # Public engine API
│   ├── Core/
│   │   ├── Application.h/cpp       # Main application loop & initialization
│   │   └── Log.h/cpp               # Logging system (spdlog wrapper)
│   ├── Renderer/
│   │   ├── Renderer.h/cpp          # Static renderer interface & dispatch
│   │   ├── Camera.h/cpp            # Perspective camera
│   │   ├── Shader.h/cpp            # Shader factory
│   │   ├── Mesh.h/cpp              # Mesh factory & cube generation
│   │   ├── RenderTypes.h           # Shared types (Vertex, RenderBackendType)
│   │   └── Backend/
│   │       ├── RenderBackend.h     # Abstract backend interface
│   │       ├── OpenGL/
│   │       │   ├── OpenGLBackend.h/cpp
│   │       │   ├── OpenGLShader.h/cpp
│   │       │   └── OpenGLMesh.h/cpp
│   │       └── Vulkan/
│   │           ├── VulkanBackend.h/cpp
│   │           ├── VulkanShader.h  (stub)
│   │           └── VulkanMesh.h    (stub)
│   └── Scene/
│       ├── Scene.h/cpp             # ECS scene manager
│       ├── Entity.h/cpp            # Entity wrapper
│       └── Components.h            # Core components (Transform, Mesh, Tag)
├── vendor/
│   ├── GLFW/                       # Windowing & input
│   ├── Glad/                       # OpenGL loader
│   ├── GLM/                        # Math library
│   ├── spdlog/                     # Logging
│   └── entt/                       # ECS framework
└── Sandbox/
    ├── src/SandboxApp.cpp          # Test application
    └── assets/shaders/             # Shader files (GLSL)
```

## Current Status

### ✅ Implemented

- **OpenGL Backend**: Fully functional vertex/fragment shader compilation, mesh VAO/VBO/EBO setup, MVP transforms
- **ECS Scene**: Entity creation, component attachment, render loop with EnTT views
- **Application Layer**: GLFW window initialization, GLAD loader, render loop
- **Mesh Primitives**: Cube mesh with proper normals and indices
- **Camera**: Perspective camera with view-projection matrix
- **Shader System**: File-based GLSL loading, uniform setting (matrices)
- **Build System**: Visual Studio solution with proper project includes (`.vcxproj` manually updated)

### ⚠️ In Progress / Planned

- **Vulkan Backend**: Architecture skeleton exists; implementation pending:
  - Instance/device creation
  - Surface & swapchain management
  - Command buffers & render passes
  - Pipeline & descriptor set layout
  - Memory management (VMA)
  - Shader compilation (glslc → SPIR-V)
- **Dynamic Backend Selection**: Make `Shader::Create` and `Mesh::Create` dispatch to correct backend
- **Asset Pipeline**: Offline shader compilation (GLSL → SPIR-V), reflection metadata
- **Rendering Features**:
  - Texture support
  - Lighting & normal mapping
  - Shadow mapping
  - Post-processing
- **Input System**: Keyboard/mouse input handling

## Building & Running

### Prerequisites

- Visual Studio 2022 (v143 platform toolset)
- CMake 3.20+ (optional, for future Vulkan build integration)
- Windows 10/11

### Build

```bash
# Open solution in Visual Studio
start BerkEngine.slnx

# Or build via MSBuild
msbuild BerkEngine.slnx /p:Configuration=Debug /p:Platform=x64 /m:1
```

### Run

```bash
# Navigate to output directory
cd bin/Debug-windows-x86_64/Sandbox/

# Execute Sandbox
Sandbox.exe
```

### Expected Output

- Rotating wireframe cube in OpenGL window
- Console logs showing shader compilation, scene updates, render calls
- Log file: `BerkEngine/berkengine.log`

## Code Examples

### Creating a Custom Component

```cpp
struct VelocityComponent {
    glm::vec3 Velocity;
    VelocityComponent(glm::vec3 v = {0,0,0}) : Velocity(v) {}
};

auto entity = scene->CreateEntity("MovingCube");
entity.AddComponent<VelocityComponent>(glm::vec3(1.0f, 0.0f, 0.0f));
```

### Rendering Pipeline

```cpp
// In Scene::OnUpdate
Renderer::BeginFrame();
auto view = m_Registry.view<TransformComponent, MeshComponent>();
view.each([&](auto entity, auto& transform, auto& mesh) {
    if (mesh.MeshPtr && m_Shader && m_Camera) {
        Renderer::Submit(*mesh.MeshPtr, transform.Transform, *m_Shader, *m_Camera);
    }
});
Renderer::EndFrame();
```

### Switching to Vulkan (Future)

```cpp
// In Application::Application
// Currently: Renderer::Init(RenderBackendType::OpenGL, 1280, 720);
// To enable Vulkan:
Renderer::Init(RenderBackendType::Vulkan, 1280, 720);
// (Requires VulkanBackend, VulkanShader, VulkanMesh implementations)
```

## Compiler Compatibility

- **MSVC 2022 v143**: Full support
- **Clang 15+**: Untested (no pragma warnings in non-MSVC)
- **GCC 11+**: Untested (Unix/Linux support pending)

## Preprocessor Definitions

Defined in `bepch.h` for Windows platform:

```cpp
BE_PLATFORM_WINDOWS
WIN32_LEAN_AND_MEAN       // Reduce Windows header size
NOMINMAX                  // Prevent min/max macro pollution
_CRT_SECURE_NO_WARNINGS   // Legacy CRT API warnings
```

## Dependencies & Licenses

| Library | Purpose | License |
|---------|---------|---------|
| GLFW | Windowing & input | Zlib |
| GLAD | OpenGL loader | MIT |
| GLM | Linear algebra | MIT |
| spdlog | Logging | MIT |
| EnTT | ECS framework | MIT |

## Contributing & Future Work

### Roadmap

1. **Q2 2026**: Vulkan backend implementation
2. **Q3 2026**: Unified shader asset pipeline (glslc, SPIRV-Cross)
3. **Q4 2026**: Texture & material system
4. **2027**: Advanced rendering (deferred shading, shadows)

### Known Limitations

- Single-threaded renderer
- No resource pooling/object reuse
- Hardcoded OpenGL in factories (TODO: dynamic dispatch)
- No validation layers or debug callbacks (Vulkan)

## License

MIT (to be specified)

## Contact

Built as a learning project to explore modern graphics API abstraction and ECS patterns in game engines.
