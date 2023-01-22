cmake_minimum_required(VERSION 3.0)

project(lab2)

# set(CMAKE_CXX_COMPILER "gfilt")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Locate GTest
find_package(GTest REQUIRED)

include_directories(include tests ${GTEST_INCLUDE_DIRS})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fPIE -fno-omit-frame-pointer -fsanitize-memory-track-origins")

add_executable(tests
        src/tests.cpp
        include/dictionary/iDictionary.hpp
        include/icollection.hpp
        include/dictionary/indexing.hpp
        tests/test_indexing.cpp
        tests/test_nullable.cpp
        tests/test_binTree.cpp
        tests/test_tree.cpp
        tests/test_set.cpp
        tests/TestHelper.hpp
        include/nullable.hpp
        include/virtualFS.hpp
        include/tree.hpp
        include/dynamicArray.hpp
        include/linkedList.hpp
        include/sequence/linkedListSequence.hpp
        include/sequence/sequence.hpp
        include/sequence/dynamicArraySequence.hpp
        include/sparseVector.hpp
        include/dictionary/dictionary.hpp
        include/dictionary/binTreeDictEntry.hpp
        tests/test_dictonary.cpp
        tests/test_sparseVector.cpp
        tests/sequence/dynamicArraySequenceTest.cpp
        tests/sequence/linkedListSequenceTest.cpp include/TestIndexing.hpp include/graph.hpp tests/test_graph.cpp)


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

add_executable(speedTest
        include/speedTest.hpp
        src/virtualFS.cpp
        include/dictionary/indexing.hpp
        include/virtualFS.hpp
        include/tree.hpp
        include/dynamicArray.hpp
        include/linkedList.hpp
        include/sequence/linkedListSequence.hpp
        include/sequence/sequence.hpp
        include/sequence/dynamicArraySequence.hpp include/speedTest.hpp include/sparseVector.hpp include/dictionary/dictionary.hpp src/speedTestMain.cpp include/TestIndexing.hpp  include/graph.hpp)

target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
target_compile_features(tests PRIVATE cxx_std_17)
install(TARGETS tests RUNTIME DESTINATION bin)

# SET(CMAKE_CXX_FLAGS_DEBUG "-O2")
# SET(CMAKE_C_FLAGS_DEBUG "-O0")

install(TARGETS tests RUNTIME DESTINATION bin)
