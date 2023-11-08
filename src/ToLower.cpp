#include <ToLower.h>

#include <boost/algorithm/string/case_conv.hpp>

#include <iterator>

namespace stringAlgos
{

void toLower(std::ostream &output, std::string_view input)
{
    boost::algorithm::to_lower_copy(std::ostream_iterator<char>(output), input);
}

} // namespace stringAlgos
