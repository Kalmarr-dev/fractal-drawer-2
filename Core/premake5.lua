workspace "Core"
  configurations { "Debug", "Release", "EmscriptenRelease" }
  toolset "gcc"

project "Core"
  targetname "core"
  kind "StaticLib"
  language "C++"
  location "build"
  files { "**.h", "**.cpp" }
  links { "glfw", "GL", "X11", "pthread", "Xrandr", "dl", "OpenGL", "tbb" }
  includedirs { "../include" }
  files { "../dependencies/**.h", "../dependencies/**.cpp", "../dependencies/**.c" }

  filter { "system:linux", "action:gmake" }
    buildoptions { "-Wall", "-Wformat", "-std=c++17", "-fopenmp" }



  filter { "configurations:Debug" }
    defines { "_DEBUG" }
    symbols "On"
    removefiles { "../dependencies/glad_es/**.c" }

  filter { "configurations:Release" }
    optimize "Speed"
    removefiles { "../dependencies/glad_es/**.c" }

  filter { "configurations:EmscriptenRelease" }
    defines { "NO_OMP", "NO_STD_PARALLEL", "__EMSCRIPTEN__" }
    optimize "Speed"
    removefiles { "../dependencies/glad/**.c" }
    toolset "emcc"
    architecture "wasm64"
    system "emscripten"
