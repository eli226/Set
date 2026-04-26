cmake_minimum_required(VERSION 4.1)
project(set)

set(CMAKE_CXX_STANDARD 20)

add_executable(set src/main/main.cpp
        include/Containers/Set.h
        src/containers/Set.cpp
        include/Containers/List2.h
        src/containers/List2.cpp
        include/Containers/Table.h
        src/containers/Table.cpp
        src/containers/Container.cpp
        include/Containers/UsedBuck.h
)

target_include_directories(set PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)