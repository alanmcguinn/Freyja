cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include(cmake_modules/artifactory.cmake)

#project(InterfaceLibrary)

file(GLOB srcFiles src/*.cpp)
file(GLOB implSrcFiles src/impl/*.cpp)

set(_target InterfaceLibrary)

set(GMOCK_DIR /opt/gmock-1.7.0)
set(GTEST_DIR /opt/gtest-1.7.0)
set(BOOST_ROOT /opt/boost_1.64.0)
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/include)

find_package(Boost 1.64.0 REQUIRED system thread filesystem chrono)

include_directories(${Boost_INCLUDE_DIR})

message("Build type is ${CMAKE_BUILD_TYPE}")

declare_target(SHARED_LIBRARY TARGET ${_target} VERSION 1.0.0.0 SOVERSION 1.0 SOURCES ${srcFiles} ${implSrcFiles})
target_link_libraries(${_target} dl ${Boost_LIBRARIES})

deploy_to_local_repo(${_fqTarget} ${_target})
add_publish_step(${_fqTarget} ${_target})

add_subdirectory(gtest)
