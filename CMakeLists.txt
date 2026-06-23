cmake_minimum_required(VERSION 4.1)
project(set)

set(CMAKE_CXX_STANDARD 20)

add_executable(set
        src/main/main.cpp

        include/Containers/Set.h
        src/containers/Set.cpp
        include/Containers/List.h
        src/containers/List.cpp
        include/Containers/Table.h
        src/containers/Table.cpp
        include/BaseClasses/GroupList.h
        src/containers/GroupList.cpp
        include/Containers/UsedBuck.h
        tests/SetTests.h
        tests/test_insert.cpp
        tests/test_find.cpp
        tests/test_remove.cpp
        tests/test_empty.cpp
        tests/test_iterator.cpp
)

target_include_directories(set PRIVATE
        ${CMAKE_SOURCE_DIR}/include)