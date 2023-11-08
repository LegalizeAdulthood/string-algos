#include <BreakLine.h>

#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/formatter.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <cctype>
#include <string>

namespace stringAlgos
{

constexpr size_t LINE_LENGTH = 80;

namespace
{

void squishInteriorWhiteSpace(std::string &line)
{
    static auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; };
    static auto finder = token_finder(isSpace, boost::algorithm::token_compress_on);
    static auto formatter = boost::algorithm::const_formatter(" ");
    find_format(line, finder, formatter);
}

enum class Pivot
{
    Keep = 1,
    Drop = 2,
};

using StringRange = boost::iterator_range<std::string::iterator>;

bool breakRange(std::string &line, const StringRange &range, std::ostream &output, Pivot dropPivot)
{
    if (!range.empty() && range.size() <= LINE_LENGTH)
    {
        const std::string_view text{boost::begin(range), boost::end(range)};
        output << text << "\n    ";
        line = line.substr(text.length() + (dropPivot == Pivot::Drop ? 1 : 0));
        return true;
    }

    return false;
}

} // namespace

void breakLine(std::ostream &output, std::string_view input)
{
    std::string line{boost::algorithm::trim_copy(input)};
    squishInteriorWhiteSpace(line);

    static auto isWord = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; };
    static auto isWordFinder = token_finder(isWord, boost::algorithm::token_compress_on);
    static auto isNonSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)) == 0; };
    static auto isNonSpaceFinder = token_finder(isNonSpace, boost::algorithm::token_compress_on);
    while (line.length() > LINE_LENGTH)
    {
        const StringRange nonSpaceText = boost::algorithm::find(line, isNonSpaceFinder);
        if (breakRange(line, nonSpaceText, output, Pivot::Drop))
        {
            continue;
        }

        const StringRange wordText = boost::algorithm::find(line, isWordFinder);
        if (breakRange(line, wordText, output, Pivot::Keep))
        {
            continue;
        }

        output << line.substr(0, LINE_LENGTH) << "\n    ";
        line = line.substr(LINE_LENGTH);
    }
    output << line;
}

} // namespace stringAlgos
