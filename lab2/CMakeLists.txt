cmake_minimum_required(VERSION 3.0)

project(lab2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Locate GTest
find_package(GTest REQUIRED)

include_directories(include tests ${GTEST_INCLUDE_DIRS})

add_executable(tests
        src/tests.cpp
        include/dictionary/iDictionary.hpp
        include/icollection.hpp
        include/dictionary/indexing.hpp
        tests/test_indexing.hpp
        tests/test_nullable.hpp
        tests/test_binTree.hpp
        tests/test_tree.hpp
        tests/test_set.hpp
        tests/TestHelper.hpp
        include/nullable.hpp
        include/virtualFS.hpp
        include/tree.hpp
        include/dynamicArray.hpp
        include/linkedList.hpp
        include/sequence/linkedListSequence.hpp
        include/sequence/sequence.hpp
        include/sequence/dynamicArraySequence.hpp include/sparseVector.hpp include/dictionary/dictionary.hpp include/dictionary/binTreeDictEntry.hpp tests/test_dictonary.hpp)


add_executable(virtualFS
        src/vfsmain.cpp
        src/virtualFS.cpp
        include/dictionary/indexing.hpp
        include/virtualFS.hpp
        include/tree.hpp
        include/dynamicArray.hpp
        include/linkedList.hpp
        include/sequence/linkedListSequence.hpp
        include/sequence/sequence.hpp
        include/sequence/dynamicArraySequence.hpp)

add_executable(TestSpeed
        src/virtualFS.cpp
        include/dictionary/indexing.hpp
        include/virtualFS.hpp
        include/tree.hpp
        include/dynamicArray.hpp
        include/linkedList.hpp
        include/sequence/linkedListSequence.hpp
        include/sequence/sequence.hpp
        include/sequence/dynamicArraySequence.hpp include/testSpeed.hpp include/sparseVector.hpp include/dictionary/dictionary.hpp)



target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
target_compile_features(tests PRIVATE cxx_std_17)
install(TARGETS tests RUNTIME DESTINATION bin)

# SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
# SET(CMAKE_C_FLAGS_DEBUG "-O0")

install(TARGETS tests RUNTIME DESTINATION bin)
