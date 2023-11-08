#include <BreakLine.h>

#include <boost/algorithm/string/trim.hpp>

namespace stringAlgos
{

void breakLine(std::ostream &output, std::string_view input)
{
    output << boost::algorithm::trim_copy(input);
}

} // namespace stringAlgos
