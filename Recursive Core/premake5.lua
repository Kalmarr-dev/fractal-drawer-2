workspace "RecursiveCore"
  configurations { "Debug", "Release", "EmscriptenRelease" }
  toolset "gcc"
  libdirs { "../Core/build/bin" }

project "RecursiveCore"
  targetname "recursivecore"
  kind "StaticLib"
  language "C++"
  location "build"
  files { "**.h", "**.cpp" }
  links { "glfw", "GL", "X11", "pthread", "Xrandr", "dl", "OpenGL", "tbb" }
  includedirs { "../include" }
  includedirs { "../Core/src" }
  includedirs { "src" }
  files { "../dependencies/**.h", "../dependencies/**.cpp", "../dependencies/**.c" }

  filter { "system:linux", "action:gmake" }
    buildoptions { "-Wall", "-Wformat", "-std=c++17", "-fopenmp" }



  filter { "configurations:Debug" }
    defines { "_DEBUG" }
    symbols "On"
    links { "debug/libcore" }
    removefiles { "../dependencies/glad_es/**.c" }

  filter { "configurations:Release" }
    optimize "Speed"
    links { "release/libcore" }
    removefiles { "../dependencies/glad_es/**.c" }

  filter { "configurations:EmscriptenRelease" }
    defines { "NO_OMP", "NO_STD_PARALLEL", "__EMSCRIPTEN__" }
    optimize "Speed"
    links { "emscriptenrelease/libcore" }
    removefiles { "../dependencies/glad/**.c" }
    toolset "emcc"
    architecture "wasm64"
    system "emscripten"
