cmake_minimum_required(VERSION 3.1.0)

# project global setting

project(qtcbor)

set(QTCBOR_MAJOR_VERSION 1)
set(QTCBOR_MINOR_VERSION 0)
set(QTCBOR_PATCH_VERSION 0)
set(QTCBOR_VERSION 1.0.0)

if(NOT DEFINED QTPM_BUILD_DIR)
    set(QTPM_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(QTPM_READ_SUBDIRECTORIES, TRUE)
else()
    set(QTPM_READ_SUBDIRECTORIES, FALSE)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../../vendor)
    set(VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
else()
    set(VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
endif()

# compiler setting

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_STANDARD 11)
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

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
set(CMAKE_AUTORCC ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()
find_package(Qt5Core REQUIRED)

# extra setting

include("CMakeExtra.txt" OPTIONAL)

# build setting

include_directories(src)
set(sources src/qtcbor/private/cbor.h src/qtcbor/private/decoder.cpp src/qtcbor/private/decoder.h src/qtcbor/private/encoder.cpp src/qtcbor/private/encoder.h src/qtcbor/private/endian.h src/qtcbor/private/input.cpp src/qtcbor/private/input.h src/qtcbor/private/listener.h src/qtcbor/private/listener_debug.cpp src/qtcbor/private/listener_debug.h src/qtcbor/private/log.h src/qtcbor/private/output.h src/qtcbor/private/output_dynamic.cpp src/qtcbor/private/output_dynamic.h src/qtcbor/private/output_static.cpp src/qtcbor/private/output_static.h src/qtcbor/qtcbor.cpp src/qtcbor/qtcbor.h)
add_library(qtcbor STATIC ${sources})
qt5_use_modules(qtcbor Core)
# enable tests

enable_testing()
find_package(Qt5Test REQUIRED)

set(tests test/qtcbor_test.cpp)
foreach(file IN LISTS tests)
  get_filename_component(execname ${file} NAME_WE)
  add_executable(${execname} ${file} ${sources} ${extra_test_sources})
  add_test(NAME ${execname} COMMAND ${execname})
  qt5_use_modules(${execname} Test Core)
endforeach()

# install setting

install(TARGETS qtcbor ARCHIVE DESTINATION lib)


install(FILES ${public_headers} DESTINATION include/qtcbor/)
