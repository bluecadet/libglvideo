cmake_minimum_required( VERSION 3.0 FATAL_ERROR )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(glvideo)

find_package(libglvideo REQUIRED PATHS "${PROJECT_SOURCE_DIR}/cmake" NO_DEFAULT_PATH)

include_directories("${LIBGLVIDEO_INCLUDE_DIRS}")
list( APPEND LIBS libglvideo )

IF (MSVC)
    add_definitions(-DGLEW_STATIC)
    add_definitions(-DOPENCTM_STATIC)
    add_definitions(/wd4996)
    set(PLATFORM_LIBS opengl32)
    set(CONSOLE_SYSTEM WIN32)
ELSEIF (APPLE)
    # Create a nibless project per http://blog.hyperjeff.net/code?id=292

    find_library(COCOA_LIBRARY Cocoa)

    set(PLATFORM_LIBS
            ${COCOA_LIBRARY}
            /System/Library/Frameworks/OpenGL.framework)
ELSEIF (UNIX)
    set(PLATFORM_LIBS X11 GL)
ENDIF ()
