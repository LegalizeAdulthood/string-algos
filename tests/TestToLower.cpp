#include <ToLower.h>

#include <gtest/gtest.h>

#include <sstream>

class TestToLower : public testing::Test
{
protected:
    std::ostringstream output;
    std::string_view   input;
};

TEST_F(TestToLower, empty)
{
    stringAlgos::toLower(output, input);

    EXPECT_TRUE(output.str().empty());
}

TEST_F(TestToLower, lowerCaseInputUnchanged)
{
    input = "already lower case";

    stringAlgos::toLower(output, input);

    EXPECT_EQ(output.str(), input);
}

TEST_F(TestToLower, upperCaseConvertedToLowerCase)
{
    stringAlgos::toLower(output, "This is a mixed case String for the Algorithm");

    EXPECT_EQ( output.str(), "this is a mixed case string for the algorithm");
}
