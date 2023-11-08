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
    int                      argc = static_cast<int>(args.size());

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
    int                      argc = static_cast<int>(args.size());

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
    int                      argc = static_cast<int>(args.size());

    const int result = stringer::main(argc, makeArgV(args).data(), err, out);

    EXPECT_EQ(1, result);
    EXPECT_TRUE(out.str().empty());
    EXPECT_EQ(s_usageMessage, err.str());
}

using MockTransformer = testing::StrictMock<testing::MockFunction<stringer::LineTransformer>>;

TEST(TestTransformLines, invokesTransformerPerInputLine)
{
    using namespace testing;
    std::istringstream input{"one\ntwo\n"};
    std::ostringstream output;
    MockTransformer    mockTransformer;
    EXPECT_CALL(mockTransformer, Call(_, "one"));
    EXPECT_CALL(mockTransformer, Call(_, "two"));

    stringer::transformLines(mockTransformer.AsStdFunction(), input, output);
}

TEST(TestTransformLines, invokesTransformerForPartialInputLine)
{
    using namespace testing;
    std::istringstream input{"one\ntwo"};
    std::ostringstream output;
    MockTransformer    mockTransformer;
    EXPECT_CALL(mockTransformer, Call(_, "one"));
    EXPECT_CALL(mockTransformer, Call(_, "two"));

    stringer::transformLines(mockTransformer.AsStdFunction(), input, output);
}

TEST(TestTransformLines, writesLinesToOutput)
{
    using namespace testing;
    auto               writer = [](std::ostream &str, const std::string &line) { str << line; };
    std::istringstream input{"one\ntwo"};
    std::ostringstream output;
    MockTransformer    mockTransformer;
    EXPECT_CALL(mockTransformer, Call(_, "one")).WillOnce(DoAll(writer, Return(true)));
    EXPECT_CALL(mockTransformer, Call(_, "two")).WillOnce(DoAll(writer, Return(true)));

    stringer::transformLines(mockTransformer.AsStdFunction(), input, output);

    EXPECT_EQ(input.str() + '\n', output.str());
}

TEST(TestTransformLines, skipsNewLineAccordingToTransformer)
{
    using namespace testing;
    auto               writer = [](std::ostream &str, const std::string &line) { str << line; };
    std::istringstream input{"one\ntwo"};
    std::ostringstream output;
    MockTransformer    mockTransformer;
    EXPECT_CALL(mockTransformer, Call(_, "one")).WillOnce(DoAll(writer, Return(false)));
    EXPECT_CALL(mockTransformer, Call(_, "two")).WillOnce(DoAll(writer, Return(true)));

    stringer::transformLines(mockTransformer.AsStdFunction(), input, output);

    EXPECT_EQ("onetwo\n", output.str());
}
