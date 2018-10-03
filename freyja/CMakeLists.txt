cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

include(cmake_modules/artifactory.cmake)

file(GLOB srcFiles src/*.cpp)
file(GLOB implSrcFiles src/impl/*.cpp)

set(_target freyja)

declare_target(SHARED_LIBRARY TARGET ${_target} VERSION 1.0.0.0 SOVERSION 1.0 SOURCES ${srcFiles} ${implSrcFiles})

#target_link_libraries(${_target} <<libs>>)

deploy_to_local_repo(${_fqTarget} ${_target})
add_publish_step(${_fqTarget} ${_target})
