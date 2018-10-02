set(_target InterfaceLibrary_gtest)

include_directories(${GTEST_DIR}/include ${GMOCK_DIR}/include)

add_definitions(-DGTEST)

find_library(gmockMainLib gmock_main ${GMOCK_DIR}/lib)
find_library(gmockLib gmock ${GMOCK_DIR}/lib)
find_library(gtestMainLib gtest_main ${GTEST_DIR}/lib)
find_library(gtestLib gtest ${GTEST_DIR}/lib)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${_target} TestInterfaceLibrary.cpp)

set(BOOST_ROOT "/opt/boost_1.64.0")

find_package(Boost 1.64.0 REQUIRED system thread filesystem chrono)

target_link_libraries(${_target} ${gmockLib} ${gmockMainLib} ${gtestLib} ${gtestMainLib} pthread dl InterfaceLibrary)
set_property(TARGET ${_target} PROPERTY CXX_STANDARD 11)

add_test(${_target} ${_target})

add_custom_command(TARGET ${_target}
                   POST_BUILD
                   COMMAND ${_target}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Running ${_target}" VERBATIM)
