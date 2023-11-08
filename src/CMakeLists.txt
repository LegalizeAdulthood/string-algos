find_package(Boost REQUIRED)

add_library(StringAlgos
    ToLower.h
    ToLower.cpp
    ToUpper.h
    ToUpper.cpp
    BreakLine.h
    BreakLine.cpp
)
set_target_properties(StringAlgos PROPERTIES FOLDER Libraries)
target_include_directories(StringAlgos PUBLIC .)
target_link_libraries(StringAlgos PRIVATE Boost::boost)

add_executable(Stringer Stringer.cpp)
set_target_properties(Stringer PROPERTIES FOLDER Tools)
target_link_libraries(Stringer PUBLIC StringAlgos)
