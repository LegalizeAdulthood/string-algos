#include <BreakLine.h>

#include <Stringer.h>

#include <gtest/gtest.h>

#include <iterator>
#include <sstream>

#include <boost/algorithm/string/join.hpp>

#include <cctype>

namespace
{

template <size_t N>
std::string repeatCount(const char (&repeaters)[N], int count)
{
    std::string line;
    for (char c : repeaters)
    {
        if (c > 0)
        {
            line += std::string(count, c);
        }
    }
    return line;
}

template <size_t N>
std::string repeat6(const char (&repeaters)[N])
{
    return repeatCount(repeaters, 6);
}

template <size_t N>
std::string repeat10(const char (&repeaters)[N])
{
    return repeatCount(repeaters, 10);
}

} // namespace

TEST(TestRepeat, count)
{
    const std::string line{repeatCount("0123", 4)};

    EXPECT_EQ(4 * 4U, line.size());
}

TEST(TestRepeat, count6)
{
    const std::string line{repeat6("0123")};

    EXPECT_EQ(4 * 6U, line.size());
}

TEST(TestRepeat, count10)
{
    const std::string line{repeat10("0123")};

    EXPECT_EQ(4 * 10U, line.size());
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
    const std::string line{repeat10("012345678")};

    stringAlgos::breakLine(m_output, line);

    EXPECT_EQ(repeat10("01234567") + s_separator + repeat10("8"), m_output.str());
}

TEST_F(TestBreakLine, lineWithoutWhiteSpaceExactlyLineLengthNotFolded)
{
    const std::string line{repeat10("01234567")};

    stringAlgos::breakLine(m_output, line);

    EXPECT_EQ(line, m_output.str());
}

TEST_F(TestBreakLine, multipleInteriorWhiteSpaceSquished)
{
    const std::string line{repeat10("x \ty\t\t\t   z")};

    stringAlgos::breakLine(m_output, line);

    EXPECT_EQ(boost::algorithm::join(std::vector{repeat10("x"), repeat10("y"), repeat10("z")}, " "), m_output.str());
}

TEST_F(TestBreakLine, preferFoldAtNonAlphaNumeric)
{
    const std::string first{repeat10("01234") + "_________"};
    const std::string second{"(" + repeat10("567")};

    stringAlgos::breakLine(m_output, first + second);

    EXPECT_EQ(first + s_separator + second, m_output.str());
}

TEST_F(TestBreakLine, preferFoldAtLastNonAlphaNumeric)
{
    const std::string first{repeat10("woody")};
    const std::string second{repeat10("(xy)567")};

    stringAlgos::breakLine(m_output, first + second);

    EXPECT_EQ(first + s_separator + second, m_output.str());
}

TEST_F(TestBreakLine, hardFoldWhenNonAlphaNumericPastLineLength)
{
    const std::string first{repeat10("01234567")};
    const std::string second{repeat10("890") + "_________"};

    stringAlgos::breakLine(m_output, first + second);

    EXPECT_EQ(first + s_separator + second, m_output.str());
}

TEST_F(TestBreakLine, preferFoldAtWhiteSpace)
{
    const std::string first{repeat10("012345")};
    const std::string second{"xx__" + repeat10("67")};

    stringAlgos::breakLine(m_output, first + ' ' + second);

    EXPECT_EQ(first + s_separator + second, m_output.str());
}

TEST_F(TestBreakLine, multipleFoldsRequired)
{
    // clang-format off
    std::vector parts{
        repeat10("01234567"),
        repeat10("89abcde") + repeat6("f"), // 6 == 10 - columnWidth(s_separator)
        repeat6("f") + repeat10("defghij"),
        repeat10("klmnopq") + repeat6("r"),
        repeat6("r") + repeat10("stuvwxy"),
        repeat10("z")
    };
    // clang-format on

    stringAlgos::breakLine(m_output, boost::algorithm::join(parts, ""));

    EXPECT_EQ(boost::algorithm::join(parts, s_separator), m_output.str());
}

TEST_F(TestBreakLine, multipleBlankLinesSquished)
{
    std::istringstream input{R"(one



two
)"};

    stringer::transformLines(stringAlgos::breakLine, input, m_output);

    EXPECT_EQ(R"(one

two
)",
              m_output.str());
}

TEST_F(TestBreakLine, preferBreakingAtLastWhiteSpace)
{
    const std::vector<std::string> lines{"This is a really long text line with multiple instances of white space that",
                                         "could all potentially be break points, but only the last white space before",
                                         "the line length should be chosen."};

    stringAlgos::breakLine(m_output, boost::algorithm::join(lines, " "));

    EXPECT_EQ(boost::algorithm::join(lines, s_separator), m_output.str());
}
