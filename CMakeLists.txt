cmake_minimum_required(VERSION 4.1)
project(set)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_executable(set
        include/AbstractClasses/ListAbstract.h
        include/AbstractClasses/TableAbstract.h
        include/BaseClasses/Container.h
        include/BaseClasses/GroupContainer.h
        include/BaseClasses/GroupList.h
        include/Containers/List.h
        include/Containers/Table.h
        include/MM/Mem.h
        include/MM/MemoryManager.h
        src/containers/List.cpp
        src/containers/Table.cpp
        src/main/TableTestMain.cpp
        include/Containers/UsedBuck.h
        src/containers/Table.cpp
)

target_include_directories(set PRIVATE include)
