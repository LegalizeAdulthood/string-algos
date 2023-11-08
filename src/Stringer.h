#pragma once

#include <functional>
#include <iosfwd>
#include <string>

namespace stringer
{

using LineTransformer = std::function<void(std::ostream &output, const std::string &line)>;

void transformLines(LineTransformer transformer, std::istream &input, std::ostream &output);

int main(int argc, char *argv[], std::ostream &err, std::ostream &out);

} // namespace stringer
