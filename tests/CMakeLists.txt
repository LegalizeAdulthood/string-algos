find_package(gtest CONFIG REQUIRED)
include(GoogleTest)

add_executable(TestStringAlgos
    TestToUpper.cpp
    TestToLower.cpp
    TestBreakLine.cpp
)
set_target_properties(TestStringAlgos PROPERTIES FOLDER Tests)
target_link_libraries(TestStringAlgos PUBLIC GTest::gtest_main StringAlgos)
gtest_discover_tests(TestStringAlgos)
