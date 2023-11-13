
local rootPath = path.getabsolute(".")

workspace "D7045E"
    architecture "x64"
    startproject "D7045E"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["GLFW"] = "D7045E/vendor/GLFW/include"
    IncludeDir["Glad"] = "D7045E/vendor/Glad/include"
    IncludeDir["ImGui"] = "D7045E/vendor/imgui"
    IncludeDir["glm"] = "D7045E/vendor/glm"

group "Dependencies"
    include "D7045E/vendor/GLFW"
    include "D7045E/vendor/Glad"
    include "D7045E/vendor/imgui"
group ""

project "D7045E"
    location "D7045E"
    kind "ConsoleApp"
    staticruntime "off"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "D7045E/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    removefiles
    {
        "%{prj.name}/src/Platform/**"
    }

    libdirs
    {
        "%{prj.name}/vendor/GLFW/lib-vc2022",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "ROOT_PATH=\"" .. rootPath .. "/" .. "%{prj.name}\""
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{prj.name}/src",
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "PLATFORM_WINDOWS",
            "BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
        
        links 
        {
            "opengl32.lib"
        }

        files
        {
            "%{prj.name}/src/Platform/Windows/**.cpp",
            "%{prj.name}/src/Platform/Windows/**.h",
        }
    filter "system:linux"
        systemversion "latest"
        defines
        {
            "PLATFORM_LINUX",
            "BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        links
        {
            "GL"
        }
        files
        {
            "%{prj.name}/src/Platform/Linux/**.cpp",
            "%{prj.name}/src/Platform/Linux/**.h",
        }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DIST"
        runtime "Release"
        optimize "on"
