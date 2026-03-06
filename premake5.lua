workspace "BerkEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox" -- VS açıldığında Sandbox seçili gelsin

-- Çıktı klasörlerini (bin ve bin-int) düzenleyelim
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- TÜM PROJELER İÇİN ORTAK AYARLAR
filter "system:windows"
    systemversion "latest"
    buildoptions { "/utf-8" } -- UTF-8 hatasını kökten çözer
    defines { "BE_PLATFORM_WINDOWS" }

project "BerkEngine"
    location "BerkEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "configurations:Debug"
        defines { "BE_DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "BE_RELEASE" }
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- Sandbox'ın hem kendi src'sine hem BerkEngine'e bakması lazım
    includedirs {
        "BerkEngine/src",
        "Sandbox/src",
        "BerkEngine/vendor/spdlog/include"
    }

    links {
        "BerkEngine" -- Sandbox'ı BerkEngine.lib'e bağlar
    }

    filter "configurations:Debug"
        defines { "BE_DEBUG" }
        symbols "on"