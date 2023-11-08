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

} // namespace

void breakLine(std::ostream &output, std::string_view input)
{
    std::string line{boost::algorithm::trim_copy(input)};
    squishInteriorWhiteSpace(line);

    static auto isWord = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; };
    static auto isWordFinder = token_finder(isWord, boost::algorithm::token_compress_on);
    while (line.length() > LINE_LENGTH)
    {
        auto pos = boost::algorithm::find(line, isWordFinder);
        if (!pos.empty() && pos.size() <= LINE_LENGTH)
        {
            const std::string_view word{boost::begin(pos), boost::end(pos)};
            output << word << "\n    ";
            line = line.substr(word.length());
        }
        else
        {
            output << line.substr(0, LINE_LENGTH) << "\n    ";
            line = line.substr(LINE_LENGTH);
        }
    }
    output << line;
}

} // namespace stringAlgos
