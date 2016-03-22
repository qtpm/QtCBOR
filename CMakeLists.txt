cmake_minimum_required(VERSION 3.1.0)

# project global setting

project(qtcbor)

set(QTCBOR_MAJOR_VERSION 1)
set(QTCBOR_MINOR_VERSION 0)
set(QTCBOR_PATCH_VERSION 0)
set(QTCBOR_VERSION 1.0.0)

if(CMAKE_BUILD_TYPE MATCHES Release)
    message("-- Release Build")
    include_directories(build-release/include)
    link_directories(build-release/lib)
else()
    message("-- Debug Build")
    include_directories(build-debug/include)
    link_directories(build-debug/lib)
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
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()
find_package(Qt5Core REQUIRED)

# include extra settings



# build setting

include_directories(src)
set(sources src/private/cbor.h src/private/decoder.cpp src/private/decoder.h src/private/encoder.cpp src/private/encoder.h src/private/endian.h src/private/input.cpp src/private/input.h src/private/listener.h src/private/listener_debug.cpp src/private/listener_debug.h src/private/log.h src/private/output.h src/private/output_dynamic.cpp src/private/output_dynamic.h src/private/output_static.cpp src/private/output_static.h src/qtcbor.cpp)
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

set(public_headers src/qtcbor.h src/qtcbor_global.h)
install(FILES ${public_headers} DESTINATION include/qtcbor/)
