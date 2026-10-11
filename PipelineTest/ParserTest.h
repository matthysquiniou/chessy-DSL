#pragma once
#include "Defines.h"
#include "ExpressionTrait.h"
#include "Lexer.h"
#include "Parser.h"

class ParserTest
{
public:
    static void TestDeclaration();
    static void TestMathOperation();
    static void TestBoolOperation();
    static void TestBinaOperation();
    static void TestFunction();
    static void TestLoop();

    static void AssertIdentifier(Expression* e, const char* name, const char* msg);
    static void AssertType(Expression* e, TokenType type, const char* msg);
    static void AssertLiteral(Expression* e, TokenType type, const char* msg);
    static BinaryExpression* AssertBinary(Expression* e, TokenType op, const char* msg);
    static VariableDeclarationExpression* AssertVarDecl(Expression* e, TokenType type, const char* name, const char* msg);
    static VariableAssignmentExpression* AssertVarAssign(Expression* e, const char* name, const char* msg);
    static FunctionDeclarationExpression* GetSingleMainFunction(ProgramExpression* program, const char* file);
};
