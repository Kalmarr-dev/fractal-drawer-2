workspace "InfiniteStamp"
  configurations { "Debug", "Release", "EmscriptenRelease" }
  toolset "gcc"

project "InfiniteStamp"
  targetname "infinite-stamp"
  targetdir "./"
  kind "WindowedApp"
  language "C++"
  location "build"
  files { "**.h", "**.cpp" }
  links { "glfw", "GL", "X11", "pthread", "Xrandr", "dl", "OpenGL", "tbb" }
  includedirs { "../include" }
  includedirs { "../Core/src" }
  includedirs { "../Recursive Core/src" }
  includedirs { "src" }
  files { "../dependencies/**.h", "../dependencies/**.cpp", "../dependencies/**.c" }

  filter { "configurations:Debug" }
    defines { "_DEBUG" }
    symbols "On"
    libdirs { "../Core/build/bin/Debug" }
    libdirs { "../Recursive Core/build/bin/Debug" }
    links { "Debug/recursivecore" }
    links { "Debug/core" }
    removefiles { "../dependencies/glad_es/**.c" }

  filter { "configurations:Release" }
    optimize "Speed"
    libdirs { "../Core/build/bin/Release" }
    libdirs { "../Recursive Core/build/bin/Release" }
    links { "Release/core" }
    links { "Release/recursivecore" }
    removefiles { "../dependencies/glad_es/**.c" }

  filter { "configurations:EmscriptenRelease" }
    defines { "NO_OMP", "NO_STD_PARALLEL", "__EMSCRIPTEN__" }
    optimize "Speed"
    libdirs { "../Core/build/bin/EmscriptenRelease" }
    libdirs { "../Recursive Core/build/bin/EmscriptenRelease" }
    links { "Emscriptenrelease/core" }
    links { "Emscriptenrelease/recursivecore" }
    removefiles { "../dependencies/glad/**.c" }

  filter { "system:linux", "action:gmake" }
    buildoptions { "-Wall", "-Wformat", "-std=c++17", "-fopenmp" }
    linkoptions { "-fopenmp" }
