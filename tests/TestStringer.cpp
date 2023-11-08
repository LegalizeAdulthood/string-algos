#include <Stringer.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <ranges>
#include <sstream>

std::vector<char *> makeArgV(std::vector<std::string> &args)
{
    std::vector<char *> argv;
    argv.reserve(args.size());
    for (std::string &arg : args)
    {
        argv.push_back(arg.data());
    }
    return argv;
}

TEST(TestMakeArgv, pointersToStringData)
{
    std::vector<std::string> args{"one", "two", "three"};

    std::vector<char *> argv = makeArgV(args);

    ASSERT_EQ(args.size(), argv.size());
    ASSERT_EQ(argv.end(), std::ranges::find(argv, nullptr));
    for (size_t i = 0; i < args.size(); ++i)
    {
        ASSERT_EQ(args[i], argv[i]);
    }
}

static const char *const s_usageMessage{R"(Usage:
stringer <command> [<infile> [<outfile>]]

Use - for standard input <infile>

Commands:
tolower     Convert all upper case characters in the input to lower case.
toupper     Convert all lower case characters in the input to upper case.
breakline   Fold long lines at 80 columns and squish multiple whitespace.
)"};

TEST(TestStringer, missingArgumentsEmitsUsage)
{
    std::ostringstream       err;
    std::ostringstream       out;
    std::vector<std::string> args{"stringer"};
    int                      argc = args.size();

    const int result = stringer::main(argc, makeArgV(args).data(), err, out);

    EXPECT_EQ(1, result);
    EXPECT_TRUE(out.str().empty());
    EXPECT_EQ(s_usageMessage, err.str());
}

TEST(TestStringer, badCommandEmitsUsage)
{
    std::ostringstream       err;
    std::ostringstream       out;
    std::vector<std::string> args{"stringer", "foo"};
    int                      argc = args.size();

    const int result = stringer::main(argc, makeArgV(args).data(), err, out);

    EXPECT_EQ(1, result);
    EXPECT_TRUE(out.str().empty());
    EXPECT_EQ(s_usageMessage, err.str());
}

TEST(TestString, tooManyArgumentsEmitsUsage)
{
    std::ostringstream       err;
    std::ostringstream       out;
    std::vector<std::string> args{"stringer", "tolower", "infile", "outfile", "extra"};
    int                      argc = args.size();

    const int result = stringer::main(argc, makeArgV(args).data(), err, out);

    EXPECT_EQ(1, result);
    EXPECT_TRUE(out.str().empty());
    EXPECT_EQ(s_usageMessage, err.str());
}

TEST(TestTransformLines, invokesTransformerPerInputLine)
{
    using namespace testing;
    std::istringstream                               input{"one\ntwo\n"};
    std::ostringstream                               output;
    testing::MockFunction<stringer::LineTransformer> mockTransformer;
    EXPECT_CALL(mockTransformer, Call(_, "one"));
    EXPECT_CALL(mockTransformer, Call(_, "two"));

    stringer::transformLines(mockTransformer.AsStdFunction(), input, output);
}

TEST(TestTransformLines, invokesTransformerForPartialInputLine)
{
    using namespace testing;
    using MockTransformer = StrictMock<MockFunction<stringer::LineTransformer>>;
    std::istringstream input{"one\ntwo"};
    std::ostringstream output;
    MockTransformer    mockTransformer;
    EXPECT_CALL(mockTransformer, Call(_, "one"));
    EXPECT_CALL(mockTransformer, Call(_, "two"));

    stringer::transformLines(mockTransformer.AsStdFunction(), input, output);
}
