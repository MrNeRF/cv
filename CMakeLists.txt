cmake_minimum_required(VERSION 3.18)
project(cv LANGUAGES CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR “In - source build detected !”)
  return()
endif()

set(CMAKE_MODULE_PATH $ { CMAKE_MODULE_PATH }
                      "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS)
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

find_program(CCACHE ccache)
if(CCACHE)
  message("using ccache")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
  message("ccache not found! Not usable")
endif()

# glfw
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# glew
find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")

set(MY_EXECUTABLE_SOURCES
    src/Camera.cpp
    src/File.cpp
    src/Logger.cpp
    src/main.cpp
    src/Importer.cpp
    src/Model.cpp
    src/ObjFileParser.cpp
    src/OpenGL3DDataObject.cpp
    src/PhongShader.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/stb_image.cpp
    src/Texture.cpp
    src/Viewer.cpp
    src/Window.cpp
    )

add_subdirectory(extern)
add_executable(cv ${MY_EXECUTABLE_SOURCES})
target_include_directories(cv PRIVATE src ${GLFW3_INCLUDE_DIR})
target_compile_options(
  cv
  PRIVATE -ggdb3
          -O0
          -Wall
          -Wextra
          -Werror
          -Wpedantic
          -Wno-write-strings
          -Wno-unused-parameter
          -D_GLIBCXX_DEBUG)
target_link_libraries(
  cv
  PRIVATE -pthread
          spdlog
          eigen
          fmt
          ${GLEW_LIBRARY}
          -lGLU
          -lGL
          ${GLFW3_LIBRARY})

add_library(libavcodec STATIC IMPORTED)
set_target_properties(libavcodec PROPERTIES IMPORTED_LOCATION /home/paja/projects/cv/lib/libavcodec.a)
set_target_properties(libavcodec PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /home/paja/projects/cv/include)

add_library(libavutil STATIC IMPORTED)
set_target_properties(libavutil PROPERTIES IMPORTED_LOCATION /home/paja/projects/cv/lib/libavutil.a)
set_target_properties(libavutil PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /home/paja/projects/cv/include)
add_library(libavformat STATIC IMPORTED)
set_target_properties(libavformat PROPERTIES IMPORTED_LOCATION /home/paja/projects/cv/lib/libavformat.a)
set_target_properties(libavformat PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /home/paja/projects/cv/include)
#add_library(libswscale STATIC IMPORTED)
#set_target_properties(libswscale PROPERTIES IMPORTED_LOCATION /home/paja/projects/cv/lib/libswscale.a)
#set_target_properties(libswscale PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /home/paja/projects/cv/include)
add_library(libswresample STATIC IMPORTED)
set_target_properties(libswresample PROPERTIES IMPORTED_LOCATION /home/paja/projects/cv/lib/libswscale.a)
set_target_properties(libswresample PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /home/paja/projects/cv/include)

add_executable(video src/video.cpp)
target_include_directories(video PRIVATE include)
target_link_libraries(video PRIVATE -lavcodec -llzma -lz -lswresample -lavutil -lavformat -lm -latomic -lz)
