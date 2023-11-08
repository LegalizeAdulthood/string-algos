#include <BreakLine.h>

#include <gtest/gtest.h>

#include <sstream>

class TestBreakLine : public testing::Test
{
protected:
    std::ostringstream output;
};

TEST_F(TestBreakLine, emptyLineUnchanged)
{
    stringAlgos::breakLine(output, "");

    EXPECT_TRUE(output.str().empty());
}

TEST_F(TestBreakLine, leadingHorizontalWhiteSpaceDropped)
{
    stringAlgos::breakLine(output, " \tLeading horizontal white space.");

    EXPECT_EQ(output.str(), "Leading horizontal white space.");
}
