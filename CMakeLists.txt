cmake_minimum_required(VERSION 3.1.0)

# project global setting

project(qtcbor)

set(QTCBOR_MAJOR_VERSION 1)
set(QTCBOR_MINOR_VERSION 0)
set(QTCBOR_PATCH_VERSION 0)
set(QTCBOR_VERSION
 ${QTCBOR_MAJOR_VERSION}.${QTCBOR_MINOR_VERSION}.${QTCBOR_PATCH_VERSION})
set(CMAKE_BUILD_TYPE Debug)
set(WORKDIR "build-debug")


# compiler setting

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
if(DEFINED ENV{QTPM_INCLUDE_PATH})
    include_directories("$ENV{QTPM_INCLUDE_PATH}")
endif()
if(DEFINED ENV{QTPM_LIBRARY_PATH})
    link_directories("$ENV{QTPM_LIBRARY_PATH}")
endif()
find_package(Qt5Core REQUIRED)


# include extra settings

include("CMakeExtra.txt")

# build setting

include_directories(src)
set(sources src/private/decoder.cpp src/private/encoder.cpp src/private/input.cpp src/private/listener_debug.cpp src/private/output_dynamic.cpp src/private/output_static.cpp src/qtcbor.cpp)
add_library(qtcbor SHARED ${sources})
SET_TARGET_PROPERTIES(qtcbor PROPERTIES VERSION ${QTCBOR_VERSION} SOVERSION ${QTCBOR_MAJOR_VERSION})
qt5_use_modules(qtcbor Core)
# build examples

file(GLOB examples RELATIVE examples *.cpp)
foreach(path IN LISTS examples)
  get_filename_component(target ${path} NAME_WE)
  add_executable(${target} ${path})
  qt5_use_modules(${target} Core)
  target_link_libraries(${target} qtcbor)
endforeach()

# enable tests

enable_testing()
find_package(Qt5Test REQUIRED)

set(tests qtcbor_test)
foreach(file IN LISTS tests)
  add_executable(${file} test/${file}.cpp ${sources} ${extra_test_sources})
  add_test(NAME ${file} COMMAND ${file})
  qt5_use_modules(${file} Test Core)
endforeach()


# install setting

install(TARGETS qtcbor LIBRARY DESTINATION lib)

set(public_headers src/qtcbor.h src/qtcbor_global.h)
install(FILES ${public_headers} DESTINATION include/qtcbor/)
