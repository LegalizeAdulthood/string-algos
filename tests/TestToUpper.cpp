#include <ToUpper.h>

#include <gtest/gtest.h>

#include <sstream>

class TestToUpper : public testing::Test
{
protected:
    std::ostringstream output;
    std::string_view   input;
};

TEST_F(TestToUpper, empty)
{
    stringAlgos::toUpper(output, input);

    EXPECT_TRUE(output.str().empty());
}

TEST_F(TestToUpper, upperCaseInputUnchanged)
{
    input = "ALREADY UPPER CASE";

    stringAlgos::toUpper(output, input);

    EXPECT_EQ(output.str(), input);
}

TEST_F(TestToUpper, lowerCaseConvertedToUpperCase)
{
    stringAlgos::toUpper(output, "This is a mixed case String for the Algorithm");

    EXPECT_EQ(output.str(), "THIS IS A MIXED CASE STRING FOR THE ALGORITHM");
}
