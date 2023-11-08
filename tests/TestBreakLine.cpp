#include <BreakLine.h>

#include <gtest/gtest.h>

#include <iterator>
#include <sstream>

#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/formatter.hpp>

#include <cctype>

namespace
{

template <size_t N>
std::string repeat(const char (&repeaters)[N])
{
    std::string line;
    for (char c : repeaters)
    {
        if (c > 0)
        {
            line += std::string(10, c);
        }
    }
    return line;
}

} // namespace

TEST(TestRepeat, count)
{
    const std::string line{repeat("0123")};

    EXPECT_EQ(40U, line.size());
}

class TestBreakLine : public testing::Test
{
protected:
    std::ostringstream m_output;
};

TEST_F(TestBreakLine, emptyLineUnchanged)
{
    stringAlgos::breakLine(m_output, "");

    EXPECT_TRUE(m_output.str().empty());
}

TEST_F(TestBreakLine, leadingHorizontalWhiteSpaceDropped)
{
    stringAlgos::breakLine(m_output, " \tLeading horizontal white space.");

    EXPECT_EQ(m_output.str(), "Leading horizontal white space.");
}

TEST_F(TestBreakLine, trailingHorizontalWhiteSpaceDropped)
{
    stringAlgos::breakLine(m_output, "Trailing horizontal white space.\t ");

    EXPECT_EQ(m_output.str(), "Trailing horizontal white space.");
}

static const char *const s_separator{"\n    "};

TEST_F(TestBreakLine, longLinesWithoutWhiteSpaceAreHardFolded)
{
    const std::string line{repeat("012345678")};

    stringAlgos::breakLine(m_output, line);

    EXPECT_EQ(repeat("01234567") + s_separator + repeat("8"), m_output.str());
}

TEST_F(TestBreakLine, lineWithoutWhiteSpaceExactlyLineLengthNotFolded)
{
    const std::string line{repeat("01234567")};

    stringAlgos::breakLine(m_output, line);

    EXPECT_EQ(line, m_output.str());
}

TEST_F(TestBreakLine, multipleInteriorWhiteSpaceSquished)
{
    const std::string line{repeat("x \ty")};

    stringAlgos::breakLine(m_output, line);

    EXPECT_EQ(repeat("x") + ' ' + repeat("y"), m_output.str());
}

TEST_F(TestBreakLine, preferFoldAtNonAlphaNumeric)
{
    const std::string first{repeat("01234") + "_________"};
    const std::string second{"(" + repeat("567")};

    stringAlgos::breakLine(m_output, first + second);

    EXPECT_EQ(first + s_separator + second, m_output.str());
}

TEST_F(TestBreakLine, hardFoldWhenNonAlphaNumericPastLineLength)
{
    const std::string first{repeat("01234567")};
    const std::string second{repeat("890") + "_________"};

    stringAlgos::breakLine(m_output, first + second);

    EXPECT_EQ(first + s_separator + second, m_output.str());
}

TEST_F(TestBreakLine, preferFoldAtWhiteSpace)
{
    const std::string first{repeat("012345")};
    const std::string second{"xx__" + repeat("67")};

    stringAlgos::breakLine(m_output, first + ' ' + second);

    EXPECT_EQ(first + s_separator + second, m_output.str());
}
