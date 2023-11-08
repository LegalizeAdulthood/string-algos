cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(Stringer CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)

include(CTest)

add_subdirectory(src)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
