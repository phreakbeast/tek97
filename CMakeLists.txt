cmake_minimum_required(VERSION 3.00)

project(tek97)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -pedantic")
add_definitions(-DTEK_DEBUG)
add_definitions(-DTEK_64)

if (WIN32)

    set(PLATFORM_FILES
            "src/platform/win32/*.cpp"
            )

    set(PLATFORM_INCLUDES
            "src/platform/win32"
            )
    add_definitions(-DTEK_PLATFORM_WINDOWS)
endif (WIN32)

if (UNIX)
    find_package(X11 REQUIRED)

    message(STATUS "X11_FOUND = ${X11_FOUND}")
    message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
    message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

    find_package(OpenGL REQUIRED GLX)

    message(STATUS "OpenGL_FOUND = ${OpenGL_FOUND}")
    message(STATUS "OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
    message(STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")

    set(PLATFORM_FILES
            "src/platform/linux/*.cpp"
            )

    set(PLATFORM_INCLUDES
            "src/platform/linux/"
            )

    add_definitions(-DTEK_PLATFORM_LINUX)
endif (UNIX)

include_directories(
        ${X11_INCLUDE_DIR}
        ${PLATFORM_INCLUDES}
        "src/extern"
        "src/extern/include"
)

link_directories(
        ${X11_LIBRARIES}
        "/usr/local/lib/"
)

file(GLOB SOURCES
        ${PLATFORM_FILES}
        "src/*.cpp"
        "src/core/*.cpp"
        "src/drawing/*.cpp"
        "src/math/*.cpp"
        "src/platform/*.cpp"
        "src/game/*.cpp"
        )

add_executable(tek97 ${SOURCES})

target_link_libraries(tek97
        ${X11_LIBRARIES}
        ${OPENGL_LIBRARIES}
        m
)
