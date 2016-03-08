cmake_minimum_required(VERSION 3.1.0)

# project global setting

project(qtcbor)

set(QTCBOR_MAJOR_VERSION 1)
set(QTCBOR_MINOR_VERSION 0)
set(QTCBOR_PATCH_VERSION 0)
set(QTCBOR_VERSION
 ${QTCBOR_MAJOR_VERSION}.${QTCBOR_MINOR_VERSION}.${QTCBOR_PATCH_VERSION})

# compiler setting

set(CMAKE_CXX_STANDARD 11)

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    endif()
endif()

# enable Qt

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()
find_package(Qt5Core REQUIRED)


# include extra settings

include("CMakeExtra.txt")

# build setting

include_directories("${PROJECT_SOURCE_DIR}/src")

add_library(qtcbor SHARED  ${PROJECT_SOURCE_DIR}/src/private/decoder.cpp ${PROJECT_SOURCE_DIR}/src/private/encoder.cpp ${PROJECT_SOURCE_DIR}/src/private/input.cpp ${PROJECT_SOURCE_DIR}/src/private/listener_debug.cpp ${PROJECT_SOURCE_DIR}/src/private/output_dynamic.cpp ${PROJECT_SOURCE_DIR}/src/private/output_static.cpp ${PROJECT_SOURCE_DIR}/src/qtcbor.cpp)
SET_TARGET_PROPERTIES(qtcbor PROPERTIES VERSION ${QTCBOR_VERSION} SOVERSION ${QTCBOR_MAJOR_VERSION})
qt5_use_modules(qtcbor Core)


# enable tests

enable_testing()
find_package(Qt5Test REQUIRED)

add_executable(qtcbor_test ${PROJECT_SOURCE_DIR}/test/qtcbor_test.cpp ${PROJECT_SOURCE_DIR}/src/private/decoder.cpp ${PROJECT_SOURCE_DIR}/src/private/encoder.cpp ${PROJECT_SOURCE_DIR}/src/private/input.cpp ${PROJECT_SOURCE_DIR}/src/private/listener_debug.cpp ${PROJECT_SOURCE_DIR}/src/private/output_dynamic.cpp ${PROJECT_SOURCE_DIR}/src/private/output_static.cpp ${PROJECT_SOURCE_DIR}/src/qtcbor.cpp)
add_test(NAME qtcbor_test COMMAND qtcbor_test WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
qt5_use_modules(qtcbor_test Test Core)


# install setting

INSTALL(TARGETS qtcbor LIBRARY DESTINATION lib)
INSTALL(FILES ${PROJECT_SOURCE_DIR}/src/qtcbor.h ${PROJECT_SOURCE_DIR}/src/qtcbor_global.h DESTINATION include/qtcbor)