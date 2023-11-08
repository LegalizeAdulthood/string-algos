#include <BreakLine.h>

#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/formatter.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/range/adaptors.hpp>

#include <cctype>
#include <string>

namespace stringAlgos
{

constexpr size_t LINE_LENGTH = 80;

namespace
{

bool g_lastLineEmpty{false};

void squishInteriorWhiteSpace(std::string &line)
{
    static auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; };
    static auto finder = token_finder(isSpace, boost::algorithm::token_compress_on);
    static auto formatter = boost::algorithm::const_formatter(" ");
    find_format_all(line, finder, formatter);
}

enum class Pivot
{
    Keep = 1,
    Drop = 2,
};

using StringRange = boost::iterator_range<std::string::iterator>;

bool breakRange(std::string &line, size_t pad, const StringRange &range, std::ostream &output, Pivot dropPivot)
{
    if (!range.empty() && range.size() <= LINE_LENGTH - pad)
    {
        const std::string_view text{range.begin(), range.end()};
        output << text << "\n    ";
        line = line.substr(text.length() + (dropPivot == Pivot::Drop ? 1 : 0));
        return true;
    }

    return false;
}

StringRange getSpaceFold(std::string &line, size_t pad)
{
    static auto isNonSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)) == 0; };
    static auto isNonSpaceFinder = token_finder(isNonSpace, boost::algorithm::token_compress_on);
    StringRange spaceFold = boost::algorithm::find(line, isNonSpaceFinder);
    if (!spaceFold.empty() && spaceFold.size() <= LINE_LENGTH - pad)
    {
        StringRange search{spaceFold.end(), line.end()};
        StringRange next = boost::algorithm::find(search, isNonSpaceFinder);
        while (!next.empty() && spaceFold.size() + next.size() < LINE_LENGTH - pad)
        {
            spaceFold.advance_end(next.size() + 1);
            search.advance_begin(next.size() + 1);
            next = boost::algorithm::find(search, isNonSpaceFinder);
        }
    }
    return spaceFold;
}

StringRange getWordFold(std::string &line, size_t pad)
{
    static auto isNotWord = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) == 0 && c != '_'; };
    static auto isNotWordFinder = token_finder(isNotWord, boost::algorithm::token_compress_on);

    StringRange subrange{line.begin(), line.begin() + LINE_LENGTH - pad};

    StringRange wordFold = find(subrange, isNotWordFinder);
    if (!wordFold.empty())
    {
        return StringRange{line.begin(), wordFold.begin()};
    }
    return StringRange{};
}

} // namespace

bool breakLine(std::ostream &output, std::string_view input)
{
    std::string line{boost::algorithm::trim_copy(input)};
    const bool  empty = line.empty();
    if (empty)
    {
        if (g_lastLineEmpty)
        {
            return false;
        }
    }
    g_lastLineEmpty = empty;
    squishInteriorWhiteSpace(line);

    size_t pad{};
    while (line.length() > LINE_LENGTH - pad)
    {
        const StringRange spaceFold{getSpaceFold(line, pad)};
        if (breakRange(line, pad, spaceFold, output, Pivot::Drop))
        {
            pad = 4;
            continue;
        }

        const StringRange wordText{getWordFold(line, pad)};
        if (breakRange(line, pad, wordText, output, Pivot::Keep))
        {
            pad = 4;
            continue;
        }

        output << line.substr(0, LINE_LENGTH - pad) << "\n    ";
        line = line.substr(LINE_LENGTH - pad);
        pad = 4;
    }
    output << line;
    return true;
}

} // namespace stringAlgos
