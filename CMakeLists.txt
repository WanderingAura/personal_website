cmake_minimum_required(VERSION 3.31)
project(personal_gallery)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FindRaylib)

# Web Configurations
if (DEFINED PLATFORM AND ${PLATFORM} STREQUAL "Web")
    set(CMAKE_SYSROOT "$ENV{EMSDK}/upstream/emscripten/cache/sysroot")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

add_subdirectory(src)