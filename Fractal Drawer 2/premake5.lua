workspace "FractalDrawer2"
  configurations { "Debug", "Release", "EmscriptenRelease" }
  toolset "gcc"

project "FractalDrawer2"
  targetname "fractal-drawer-2"
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

  filter { "system:linux", "action:gmake" }
    buildoptions { "-Wall", "-Wformat", "-std=c++17", "-fopenmp" }
    linkoptions { "-fopenmp" }



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
    links { "EmscriptenRelease/core" }
    links { "EmscriptenRelease/recursivecore" }
    removefiles { "../dependencies/glad/**.c" }
    removelinks { "Xrandr", "OpenGL", "tbb" }
    removelinkoptions { "-fopenmp" }
    removebuildoptions { "-fopenmp" }
    
    local web_dir = "web"
    targetdir (web_dir)
    toolset "emcc"
    architecture "wasm64"
    system "emscripten"
    local EMS = { "--use-port=contrib.glfw3" }
    buildoptions (EMS)
    linkoptions { "-s FULL_ES3=1", "-s MIN_WEBGL_VERSION=2", "-s MAX_WEBGL_VERSION=2" }
    linkoptions { "-s WASM=1", "-s ALLOW_MEMORY_GROWTH=1", "-s ASSERTIONS=1" }
    linkoptions { "--no-heap-copy", "--preload-file ../res" }
    linkoptions { "--shell-file ../my_shell.html" }
    linkoptions (EMS)
    linkoptions { "-o ../" .. web_dir .. "/index.html" }
    
