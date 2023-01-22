cmake_minimum_required(VERSION 3.0)

project(lab3)

# set(CMAKE_CXX_COMPILER "gfilt")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Locate GTest
find_package(GTest REQUIRED)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fPIE -fno-omit-frame-pointer -fsanitize-memory-track-origins")

add_executable(tests graph.hpp test_graph.cpp)


target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
target_compile_features(tests PRIVATE cxx_std_17)
install(TARGETS tests RUNTIME DESTINATION bin)

# SET(CMAKE_CXX_FLAGS_DEBUG "-O2")
# SET(CMAKE_C_FLAGS_DEBUG "-O0")

install(TARGETS tests RUNTIME DESTINATION bin)
