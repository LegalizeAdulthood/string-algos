#include <Stringer.h>

#include <BreakLine.h>
#include <ToLower.h>
#include <ToUpper.h>

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
            output << '\n';
        }
    }
}

static int usageMessage(const char *program, std::ostream &err)
{
    // clang-format off
        err <<
            "Usage:\n" <<
            program << R"( <command> [<infile> [<outfile>]]

Use - for standard input <infile>

Commands:
tolower     Convert all upper case characters in the input to lower case.
toupper     Convert all lower case characters in the input to upper case.
breakline   Fold long lines at 80 columns and squish multiple whitespace.
)";
    // clang-format on
    return 1;
}

int main(int argc, char *argv[], std::ostream &err, std::ostream &out)
{
    auto usage = [&] { return usageMessage(argv[0], err); };
    if (argc < 2 || argc > 4)
    {
        return usage();
    }

    using CommandSet = std::map<std::string, LineTransformer>;
    CommandSet commands{
        {"tolower", stringAlgos::toLower}, {"toupper", stringAlgos::toUpper}, {"breakline", stringAlgos::breakLine}};

    auto it = commands.find(argv[1]);
    if (it == commands.end())
    {
        return usage();
    }

    return 0;
}

} // namespace stringer
