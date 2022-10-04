cmake_minimum_required(VERSION 3.0)

project(lab1)

# Locate GTest
find_package(GTest REQUIRED)

include_directories(include tests ${GTEST_INCLUDE_DIRS})

add_executable(tests
        src/tests.cpp
        include/dynamicArray.hpp
        include/linkedList.hpp
        include/sequence/linkedListSequence.hpp
        include/sequence/sequence.hpp
        include/sequence/dynamicArraySequence.hpp
        include/sort.hpp
        tests/sequence/linkedListSequenceTest.hpp
        tests/sequence/dynamicArraySequenceTest.hpp
        tests/sequence/sequenceTest.hpp
        tests/sortTest.hpp
        tests/TestHelper.hpp)

add_executable(lab1
        src/main.cpp
        include/dynamicArray.hpp
        include/linkedList.hpp
        include/sequence/linkedListSequence.hpp
        include/sequence/sequence.hpp
        include/sequence/dynamicArraySequence.hpp
        include/sort.hpp)


target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
target_compile_features(tests PRIVATE cxx_std_17)
install(TARGETS tests RUNTIME DESTINATION bin)

install(TARGETS lab1 RUNTIME DESTINATION bin)
