#include "AnalyserTest.h"
#include "Lexer.h"
#include "Parser.h"
#include "Analyser.h"
#include "CompilationTest.h"

void AnalyserTest::TestDeclaration()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Declaration.chessy") == true, "Declaration.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    Analyser analyser = Analyser(parser.GetProgram());
    analyser.Analyse();
    CompilationTest::Assert(!analyser.HasErrors(), "AnalyserTest::TestDeclaration");
}

void AnalyserTest::TestMathOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Math_Operation.chessy") == true, "Math_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    Analyser analyser = Analyser(parser.GetProgram());
    analyser.Analyse();
    CompilationTest::Assert(!analyser.HasErrors(), "AnalyserTest::TestMathOperation");
}

void AnalyserTest::TestBoolOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Bool_Operation.chessy") == true, "Bool_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    Analyser analyser = Analyser(parser.GetProgram());
    analyser.Analyse();
    CompilationTest::Assert(!analyser.HasErrors(), "AnalyserTest::TestBoolOperation");
}

void AnalyserTest::TestBinaOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Bina_Operation.chessy") == true, "Bina_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    Analyser analyser = Analyser(parser.GetProgram());
    analyser.Analyse();
    CompilationTest::Assert(!analyser.HasErrors(), "AnalyserTest::TestBinaOperation");
}

void AnalyserTest::TestFunction()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Function.chessy") == true, "Function.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    Analyser analyser = Analyser(parser.GetProgram());
    analyser.Analyse();
    CompilationTest::Assert(!analyser.HasErrors(), "AnalyserTest::TestFunction");
}

void AnalyserTest::TestLoop()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Loop.chessy") == true, "Loop.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    Analyser analyser = Analyser(parser.GetProgram());
    analyser.Analyse();
    CompilationTest::Assert(!analyser.HasErrors(), "AnalyserTest::TestLoop");
}
