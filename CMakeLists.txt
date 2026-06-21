cmake_minimum_required(VERSION 4.1)
project(Table_Thread)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_executable(Table_Thread
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
)

target_include_directories(Table_Thread PRIVATE include)
