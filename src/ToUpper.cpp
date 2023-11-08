#include <ToUpper.h>

#include <boost/algorithm/string/case_conv.hpp>

#include <iterator>

namespace stringAlgos
{

bool toUpper(std::ostream &output, std::string_view input)
{
    boost::algorithm::to_upper_copy(std::ostream_iterator<char>(output), input);
    return true;
}

} // namespace stringAlgos
