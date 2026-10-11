#pragma once
#include "Defines.h"

class CompilationTest
{
public:
    static void RunTest();

    static void TestLexer();
    static void TestParser();
    static void TestSEMA();

    static void Assert(const bool condition, const str msg = "", const i32 errorCode = -1, const std::source_location loc = std::source_location::current());
};