#include "ShaderProc.h"
#include <gtest/gtest.h>

TEST(ShaderParserSimpleTests, RemovesSingleLineComment) {
    Twisted::ShaderParser parser;
    std::string input = "int x = 5; // comment";
    EXPECT_EQ(parser.RemoveComments(input), "int x = 5; ");
}