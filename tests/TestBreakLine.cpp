#include <BreakLine.h>

#include <gtest/gtest.h>

#include <sstream>

TEST(TestBreakLine, emptyLineUnchanged)
{
    std::ostringstream     output;
    const std::string_view input;

    stringAlgos::breakLine(output, input);

    EXPECT_EQ(output.str(), input);
}
