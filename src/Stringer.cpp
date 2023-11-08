#include <Stringer.h>
#include <ToLower.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace stringer
{

void transformLines(LineTransformer transformer, std::istream &input, std::ostream &output)
{
    while (input)
    {
        std::string line;
        std::getline(input, line);
        if (input)
        {
            transformer(output, line);
        }
    }
}

int main(int argc, char *argv[], std::ostream &err, std::ostream &out)
{
    if (argc < 2)
    {
        // clang-format off
        err <<
            "Usage:\n" <<
            argv[0] << " <command> [<infile> [<outfile>]]\n"
            "\n"
            "Use - for standard input <infile>\n";
        // clang-format on
        return 1;
    }

    using CommandSet = std::map<std::string, LineTransformer>;
    CommandSet commands{{"tolower", stringAlgos::toLower}};
    return 0;
}

} // namespace stringer
