workspace "BerkEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Kütüphane yollarını merkezi bir yerde tutalım
IncludeDir = {}
IncludeDir["GLFW"] = "BerkEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "BerkEngine/vendor/Glad/include"
IncludeDir["spdlog"] = "BerkEngine/vendor/spdlog/include"
IncludeDir["glm"] = "BerkEngine/vendor/glm"
IncludeDir["entt"] = "BerkEngine/vendor/entt/single_include" -- EnTT Buraya Eklendi

-- 1. PROJE: GLFW
project "GLFW"
    location "BerkEngine/vendor/GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    files {
        "BerkEngine/vendor/GLFW/src/vulkan.c",
        "BerkEngine/vendor/GLFW/include/GLFW/*.h",
        "BerkEngine/vendor/GLFW/src/context.c",
        "BerkEngine/vendor/GLFW/src/init.c",
        "BerkEngine/vendor/GLFW/src/input.c",
        "BerkEngine/vendor/GLFW/src/monitor.c",
        "BerkEngine/vendor/GLFW/src/window.c",
        "BerkEngine/vendor/GLFW/src/platform.c",
        "BerkEngine/vendor/GLFW/src/null_init.c",
        "BerkEngine/vendor/GLFW/src/null_monitor.c",
        "BerkEngine/vendor/GLFW/src/null_window.c",
        "BerkEngine/vendor/GLFW/src/null_joystick.c"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" }
        files {
            "BerkEngine/vendor/GLFW/src/win32_init.c",
            "BerkEngine/vendor/GLFW/src/win32_module.c",
            "BerkEngine/vendor/GLFW/src/win32_monitor.c",
            "BerkEngine/vendor/GLFW/src/win32_window.c",
            "BerkEngine/vendor/GLFW/src/win32_joystick.c",
            "BerkEngine/vendor/GLFW/src/win32_thread.c",
            "BerkEngine/vendor/GLFW/src/win32_time.c",
            "BerkEngine/vendor/GLFW/src/wgl_context.c",
            "BerkEngine/vendor/GLFW/src/egl_context.c",
            "BerkEngine/vendor/GLFW/src/osmesa_context.c"
        }

-- 2. PROJE: BerkEngine
project "BerkEngine"
    location "BerkEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "BerkEngine/src/**.h",
        "BerkEngine/src/**.cpp",
        "BerkEngine/vendor/Glad/src/glad.c"
    }

    includedirs {
        "BerkEngine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}", -- EnTT Dahil Edildi
        "$(VULKAN_SDK)/Include" 
    }

    links { "GLFW", "opengl32.lib" }

    filter "system:windows"
        systemversion "latest"

-- 3. PROJE: Sandbox
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files { "Sandbox/src/**.h", "Sandbox/src/**.cpp" }

    includedirs {
        "BerkEngine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}", -- Sandbox'ın da EnTT görmesi gerekebilir
        "$(VULKAN_SDK)/Include" 
    }

    libdirs { "$(VULKAN_SDK)/Lib" }

    links { 
        "BerkEngine", 
        "vulkan-1.lib",
        "dwmapi.lib",
        "gdi32.lib",
        "user32.lib",
        "shell32.lib"
    }

    filter "system:windows"
        systemversion "latest"