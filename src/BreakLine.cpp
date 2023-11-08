#include <BreakLine.h>

#include <boost/algorithm/string/trim.hpp>

#include <string>

namespace stringAlgos
{

constexpr size_t LINE_LENGTH = 80;

void breakLine(std::ostream &output, std::string_view input)
{
    std::string line{boost::algorithm::trim_copy(input)};
    while (line.length() > LINE_LENGTH)
    {
        output << line.substr(0, LINE_LENGTH) << "\n    ";
        line = line.substr(LINE_LENGTH);
    }
    output << line;
}

} // namespace stringAlgos
