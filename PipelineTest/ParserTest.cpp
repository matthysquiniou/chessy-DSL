#include "ParserTest.h"
#include "CompilationTest.h"

void ParserTest::AssertIdentifier(Expression* e, const char* name, const char* msg)
{
    auto* id = ExpressionCast<ExpressionType::_IDENTIFIER>(e);
    CompilationTest::Assert(id != nullptr, msg);
    if (!id) return;
    CompilationTest::Assert(id->token.type == TokenType::_SYMBOL, msg);
    CompilationTest::Assert(id->token.view == name, msg);
}

void ParserTest::AssertType(Expression* e, TokenType type, const char* msg)
{
    auto* t = ExpressionCast<ExpressionType::_TYPE>(e);
    CompilationTest::Assert(t != nullptr, msg);
    if (!t) return;
    CompilationTest::Assert(t->token.type == type, msg);
}

void ParserTest::AssertLiteral(Expression* e, TokenType type, const char* msg)
{
    auto* lit = ExpressionCast<ExpressionType::_LITERAL>(e);
    CompilationTest::Assert(lit != nullptr, msg);
    if (!lit) return;
    CompilationTest::Assert(lit->token.type == type, msg);
}

BinaryExpression* ParserTest::AssertBinary(Expression* e, TokenType op, const char* msg)
{
    auto* bin = ExpressionCast<ExpressionType::_BINARY>(e);
    CompilationTest::Assert(bin != nullptr, msg);
    if (!bin) return nullptr;
    CompilationTest::Assert(bin->token.type == op, msg);
    return bin;
}

VariableDeclarationExpression* ParserTest::AssertVarDecl(Expression* e, TokenType type, const char* name, const char* msg)
{
    auto* var = ExpressionCast<ExpressionType::_VARIABLE_DECLARATION>(e);
    CompilationTest::Assert(var != nullptr, msg);
    if (!var) return nullptr;
    AssertType(var->varType, type, msg);
    AssertIdentifier(var->name, name, msg);
    return var;
}

VariableAssignmentExpression* ParserTest::AssertVarAssign(Expression* e, const char* name, const char* msg)
{
    auto* var = ExpressionCast<ExpressionType::_VARIABLE_ASSIGNMENT>(e);
    CompilationTest::Assert(var != nullptr, msg);
    if (!var) return nullptr;
    AssertIdentifier(var->name, name, msg);
    return var;
}

FunctionDeclarationExpression* ParserTest::GetSingleMainFunction(ProgramExpression* program, const char* file)
{
    CompilationTest::Assert(program != nullptr, file);
    if (!program) return nullptr;

    CompilationTest::Assert(program->content.size() == 1, "program should contain exactly one function");
    if (program->content.size() != 1) return nullptr;

    auto* func = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(program->content[0]);
    CompilationTest::Assert(func != nullptr, "first program expression should be a function");
    if (!func) return nullptr;

    AssertIdentifier(func->functionName, "main", "function name should be main");
    CompilationTest::Assert(func->parameters.empty(), "main should have no parameters");
    CompilationTest::Assert(func->returnType == nullptr, "main should have no return type");
    CompilationTest::Assert(func->content != nullptr, "function should have a block");

    return func;
}

void ParserTest::TestDeclaration()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Declaration.chessy") == true, "Declaration.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    ProgramExpression* program = parser.GetProgram();
    auto* func = GetSingleMainFunction(program, "Declaration.chessy: program is null");
    if (!func) return;

    auto& stmts = func->content->content;
    CompilationTest::Assert(stmts.size() == 14, "Declaration: main should have 14 statements");
    if (stmts.size() != 14) return;
    auto* v0 = AssertVarDecl(stmts[0], TokenType::_BOOL, "test_bool", "stmt0 bool");
    if (v0) AssertLiteral(v0->value, TokenType::_TRUE, "stmt0 value true");

    auto* v1 = AssertVarDecl(stmts[1], TokenType::_I8, "test_i8", "stmt1 i8");
    if (v1) AssertLiteral(v1->value, TokenType::_VALUE, "stmt1 value");

    auto* v2 = AssertVarDecl(stmts[2], TokenType::_I16, "test_i16", "stmt2 i16");
    if (v2) AssertLiteral(v2->value, TokenType::_VALUE, "stmt2 value");

    auto* v3 = AssertVarDecl(stmts[3], TokenType::_I32, "test_i32", "stmt3 i32");
    if (v3) AssertLiteral(v3->value, TokenType::_VALUE, "stmt3 value");

    auto* v4 = AssertVarDecl(stmts[4], TokenType::_I64, "test_i64", "stmt4 i64");
    if (v4) AssertLiteral(v4->value, TokenType::_VALUE, "stmt4 value");

    auto* v5 = AssertVarDecl(stmts[5], TokenType::_U8, "test_u8", "stmt5 u8");
    if (v5) AssertLiteral(v5->value, TokenType::_VALUE, "stmt5 value");

    auto* v6 = AssertVarDecl(stmts[6], TokenType::_U16, "test_u16", "stmt6 u16");
    if (v6) AssertLiteral(v6->value, TokenType::_VALUE, "stmt6 value");

    auto* v7 = AssertVarDecl(stmts[7], TokenType::_U32, "test_u32", "stmt7 u32");
    if (v7) AssertLiteral(v7->value, TokenType::_VALUE, "stmt7 value");

    auto* v8 = AssertVarDecl(stmts[8], TokenType::_U64, "test_u64", "stmt8 u64");
    if (v8) AssertLiteral(v8->value, TokenType::_VALUE, "stmt8 value");

    auto* v9 = AssertVarDecl(stmts[9], TokenType::_STRING_IDENTIFIER, "test_string", "stmt9 string");
    if (v9) AssertLiteral(v9->value, TokenType::_STRING_LITTERAL, "stmt9 value");

    auto* v10 = AssertVarDecl(stmts[10], TokenType::_BITBOARD, "test_bitboard", "stmt10 bitboard");
    if (v10) CompilationTest::Assert(v10->value == nullptr, "stmt10 no value");

    auto* v11 = AssertVarDecl(stmts[11], TokenType::_SQUARE, "test_square", "stmt11 position");
    if (v11) CompilationTest::Assert(v11->value == nullptr, "stmt11 no value");

    auto* v12 = AssertVarDecl(stmts[12], TokenType::_PIECE, "test_piece", "stmt12 piece");
    if (v12) CompilationTest::Assert(v12->value == nullptr, "stmt12 no value");

    auto* v13 = AssertVarDecl(stmts[13], TokenType::_MOVE, "test_move", "stmt13 move");
    if (v13) CompilationTest::Assert(v13->value == nullptr, "stmt13 no value");
}

void ParserTest::TestMathOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Math_Operation.chessy") == true, "Math_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    ProgramExpression* program = parser.GetProgram();
    auto* func = GetSingleMainFunction(program, "Math_Operation.chessy: program is null");
    if (!func) return;

    auto& stmts = func->content->content;
    CompilationTest::Assert(stmts.size() == 8, "Math: main should have 8 statements");
    if (stmts.size() != 8) return;

    auto* v0 = AssertVarDecl(stmts[0], TokenType::_I32, "tester1", "stmt0 tester1");
    if (v0) AssertLiteral(v0->value, TokenType::_VALUE, "stmt0 value");

    auto* v1 = AssertVarDecl(stmts[1], TokenType::_I32, "tester2", "stmt1 tester2");
    if (v1) AssertLiteral(v1->value, TokenType::_VALUE, "stmt1 value");

    auto* v2 = AssertVarDecl(stmts[2], TokenType::_I32, "result", "stmt2 result");
    if (v2)
    {
        auto* add = AssertBinary(v2->value, TokenType::_ADD, "stmt2 add");
        if (add)
        {
            AssertIdentifier(add->left, "tester1", "stmt2 left");
            AssertIdentifier(add->right, "tester2", "stmt2 right");
        }
    }

    auto* assign3 = AssertVarAssign(stmts[3], "result", "stmt3 assign");
    if (assign3)
    {
        auto* sub = AssertBinary(assign3->value, TokenType::_SUB, "stmt3 sub");
        if (sub)
        {
            AssertIdentifier(sub->left, "tester1", "stmt3 sub left");
            AssertIdentifier(sub->right, "tester2", "stmt3 sub right");
        }
    }

    auto* assign4 = AssertVarAssign(stmts[4], "result", "stmt4 assign");
    if (assign4)
    {
        auto* mul = AssertBinary(assign4->value, TokenType::_MUL, "stmt4 mul");
        if (mul)
        {
            AssertIdentifier(mul->left, "tester1", "stmt4 mul left");
            AssertIdentifier(mul->right, "tester2", "stmt4 mul right");
        }
    }

    auto* assign5 = AssertVarAssign(stmts[5], "result", "stmt5 assign");
    if (assign5)
    {
        auto* pow = AssertBinary(assign5->value, TokenType::_POW, "stmt5 pow");
        if (pow)
        {
            AssertIdentifier(pow->left, "tester1", "stmt5 pow left");
            AssertIdentifier(pow->right, "tester2", "stmt5 pow right");
        }
    }

    auto* assign6 = AssertVarAssign(stmts[6], "result", "stmt6 assign");
    if (assign6)
    {
        auto* div = AssertBinary(assign6->value, TokenType::_DIV, "stmt6 div");
        if (div)
        {
            AssertIdentifier(div->left, "result", "stmt6 div left");
            AssertIdentifier(div->right, "tester2", "stmt6 div right");
        }
    }

    auto* assign7 = AssertVarAssign(stmts[7], "result", "stmt7 assign");
    if (assign7)
    {
        auto* mod = AssertBinary(assign7->value, TokenType::_MOD, "stmt7 mod");
        if (mod)
        {
            auto* mul = AssertBinary(mod->left, TokenType::_MUL, "stmt7 mul");
            if (mul)
            {
                AssertIdentifier(mul->left, "tester1", "stmt7 mul left");
                AssertIdentifier(mul->right, "tester2", "stmt7 mul right");
            }
            AssertIdentifier(mod->right, "tester2", "stmt7 mod right");
        }
    }
}

void ParserTest::TestBoolOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Bool_Operation.chessy") == true, "Bool_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    ProgramExpression* program = parser.GetProgram();
    auto* func = GetSingleMainFunction(program, "Bool_Operation.chessy: program is null");
    if (!func) return;

    auto& stmts = func->content->content;
    CompilationTest::Assert(stmts.size() == 14, "Bool: main should have 14 statements");
    if (stmts.size() != 14) return;

    AssertVarDecl(stmts[0], TokenType::_I32, "tester1", "stmt0 tester1");
    AssertVarDecl(stmts[1], TokenType::_I32, "tester2", "stmt1 tester2");

    auto* v2 = AssertVarDecl(stmts[2], TokenType::_BOOL, "booleans", "stmt2 booleans");
    if (v2)
    {
        auto* eq = AssertBinary(v2->value, TokenType::_EQUAL_OP, "stmt2 equal");
        if (eq)
        {
            AssertIdentifier(eq->left, "tester1", "stmt2 left");
            AssertIdentifier(eq->right, "tester2", "stmt2 right");
        }
    }

    auto* assign3 = AssertVarAssign(stmts[3], "booleans", "stmt3 assign");
    if (assign3)
    {
        auto* neq = AssertBinary(assign3->value, TokenType::_NOT_EQUAL_OP, "stmt3 not equal");
        if (neq)
        {
            AssertIdentifier(neq->left, "tester1", "stmt3 left");
            AssertIdentifier(neq->right, "tester2", "stmt3 right");
        }
    }

    auto* assign4 = AssertVarAssign(stmts[4], "booleans", "stmt4 assign");
    if (assign4)
    {
        auto* lt = AssertBinary(assign4->value, TokenType::_LOWER_THAN, "stmt4 lower");
        if (lt)
        {
            AssertIdentifier(lt->left, "tester1", "stmt4 left");
            AssertIdentifier(lt->right, "tester2", "stmt4 right");
        }
    }

    auto* assign5 = AssertVarAssign(stmts[5], "booleans", "stmt5 assign");
    if (assign5)
    {
        auto* le = AssertBinary(assign5->value, TokenType::_LOWER_EQ_THAN, "stmt5 lower eq");
        if (le)
        {
            AssertIdentifier(le->left, "tester1", "stmt5 left");
            AssertIdentifier(le->right, "tester2", "stmt5 right");
        }
    }

    auto* assign6 = AssertVarAssign(stmts[6], "booleans", "stmt6 assign");
    if (assign6)
    {
        auto* gt = AssertBinary(assign6->value, TokenType::_GREATER_THAN, "stmt6 greater");
        if (gt)
        {
            AssertIdentifier(gt->left, "tester1", "stmt6 left");
            AssertIdentifier(gt->right, "tester2", "stmt6 right");
        }
    }

    auto* assign7 = AssertVarAssign(stmts[7], "booleans", "stmt7 assign");
    if (assign7)
    {
        auto* ge = AssertBinary(assign7->value, TokenType::_GREATER_EQ_THAN, "stmt7 greater eq");
        if (ge)
        {
            AssertIdentifier(ge->left, "tester1", "stmt7 left");
            AssertIdentifier(ge->right, "tester2", "stmt7 right");
        }
    }

    auto* assign8 = AssertVarAssign(stmts[8], "booleans", "stmt8 assign");
    if (assign8)
    {
        auto* andOp = AssertBinary(assign8->value, TokenType::_AND_OP, "stmt8 and");
        if (andOp)
        {
            AssertLiteral(andOp->left, TokenType::_TRUE, "stmt8 left true");
            AssertLiteral(andOp->right, TokenType::_TRUE, "stmt8 right true");
        }
    }

    auto* assign9 = AssertVarAssign(stmts[9], "booleans", "stmt9 assign");
    if (assign9)
    {
        auto* andOp = AssertBinary(assign9->value, TokenType::_AND_OP, "stmt9 and");
        if (andOp)
        {
            AssertLiteral(andOp->left, TokenType::_TRUE, "stmt9 left true");
            AssertLiteral(andOp->right, TokenType::_FALSE, "stmt9 right false");
        }
    }

    auto* assign10 = AssertVarAssign(stmts[10], "booleans", "stmt10 assign");
    if (assign10)
    {
        auto* andOp = AssertBinary(assign10->value, TokenType::_AND_OP, "stmt10 and");
        if (andOp)
        {
            AssertLiteral(andOp->left, TokenType::_FALSE, "stmt10 left false");
            AssertLiteral(andOp->right, TokenType::_FALSE, "stmt10 right false");
        }
    }

    auto* assign11 = AssertVarAssign(stmts[11], "booleans", "stmt11 assign");
    if (assign11)
    {
        auto* orOp = AssertBinary(assign11->value, TokenType::_OR_OP, "stmt11 or");
        if (orOp)
        {
            AssertLiteral(orOp->left, TokenType::_TRUE, "stmt11 left true");
            AssertLiteral(orOp->right, TokenType::_TRUE, "stmt11 right true");
        }
    }

    auto* assign12 = AssertVarAssign(stmts[12], "booleans", "stmt12 assign");
    if (assign12)
    {
        auto* orOp = AssertBinary(assign12->value, TokenType::_OR_OP, "stmt12 or");
        if (orOp)
        {
            AssertLiteral(orOp->left, TokenType::_TRUE, "stmt12 left true");
            AssertLiteral(orOp->right, TokenType::_FALSE, "stmt12 right false");
        }
    }

    auto* assign13 = AssertVarAssign(stmts[13], "booleans", "stmt13 assign");
    if (assign13)
    {
        auto* orOp = AssertBinary(assign13->value, TokenType::_OR_OP, "stmt13 or");
        if (orOp)
        {
            AssertLiteral(orOp->left, TokenType::_FALSE, "stmt13 left false");
            AssertLiteral(orOp->right, TokenType::_FALSE, "stmt13 right false");
        }
    }
}

void ParserTest::TestBinaOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Bina_Operation.chessy") == true, "Bina_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    ProgramExpression* program = parser.GetProgram();
    auto* func = GetSingleMainFunction(program, "Bina_Operation.chessy: program is null");
    if (!func) return;

    auto& stmts = func->content->content;
    CompilationTest::Assert(stmts.size() == 8, "Bina: main should have 8 statements");
    if (stmts.size() != 8) return;

    AssertVarDecl(stmts[0], TokenType::_I32, "tester1", "stmt0 tester1");
    AssertVarDecl(stmts[1], TokenType::_I32, "tester2", "stmt1 tester2");

    auto* v2 = AssertVarDecl(stmts[2], TokenType::_I32, "result", "stmt2 result");
    if (v2)
    {
        auto* orBin = AssertBinary(v2->value, TokenType::_OR_BINARY, "stmt2 or binary");
        if (orBin)
        {
            AssertIdentifier(orBin->left, "tester1", "stmt2 left");
            AssertIdentifier(orBin->right, "tester2", "stmt2 right");
        }
    }

    auto* assign3 = AssertVarAssign(stmts[3], "result", "stmt3 assign");
    if (assign3)
    {
        auto* xorBin = AssertBinary(assign3->value, TokenType::_XOR_BINARY, "stmt3 xor binary");
        if (xorBin)
        {
            AssertIdentifier(xorBin->left, "tester1", "stmt3 left");
            AssertIdentifier(xorBin->right, "tester2", "stmt3 right");
        }
    }

    auto* assign4 = AssertVarAssign(stmts[4], "result", "stmt4 assign");
    if (assign4)
    {
        auto* andBin = AssertBinary(assign4->value, TokenType::_AND_BINARY, "stmt4 and binary");
        if (andBin)
        {
            AssertIdentifier(andBin->left, "tester1", "stmt4 left");
            AssertIdentifier(andBin->right, "tester2", "stmt4 right");
        }
    }

    auto* assign5 = AssertVarAssign(stmts[5], "result", "stmt5 assign");
    if (assign5)
    {
        auto* unary = ExpressionCast<ExpressionType::_UNARY>(assign5->value);
        CompilationTest::Assert(unary != nullptr, "stmt5 should be unary");
        if (unary)
        {
            CompilationTest::Assert(unary->token.type == TokenType::_NOT_OP, "stmt5 should be not op");
            AssertIdentifier(unary->left, "result", "stmt5 unary operand");
        }
    }

    auto* assign6 = AssertVarAssign(stmts[6], "result", "stmt6 assign");
    if (assign6)
    {
        auto* lshift = AssertBinary(assign6->value, TokenType::_L_SHIFT, "stmt6 lshift");
        if (lshift)
        {
            AssertIdentifier(lshift->left, "tester1", "stmt6 left");
            AssertIdentifier(lshift->right, "tester2", "stmt6 right");
        }
    }

    auto* assign7 = AssertVarAssign(stmts[7], "result", "stmt7 assign");
    if (assign7)
    {
        auto* rshift = AssertBinary(assign7->value, TokenType::_R_SHIFT, "stmt7 rshift");
        if (rshift)
        {
            AssertIdentifier(rshift->left, "tester1", "stmt7 left");
            AssertIdentifier(rshift->right, "tester2", "stmt7 right");
        }
    }
}

void ParserTest::TestFunction()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Function.chessy") == true, "Function.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    ProgramExpression* program = parser.GetProgram();
    CompilationTest::Assert(program != nullptr, "Function.chessy: program is null");
    if (!program) return;

    auto& funcs = program->content;
    CompilationTest::Assert(funcs.size() == 5, "Function: program should have 5 functions");
    if (funcs.size() != 5) return;

    // func Test() { return; }
    auto* f0 = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(funcs[0]);
    CompilationTest::Assert(f0 != nullptr, "func0 should be function");
    if (f0)
    {
        AssertIdentifier(f0->functionName, "Test", "func0 name");
        CompilationTest::Assert(f0->parameters.empty(), "func0 no params");
        CompilationTest::Assert(f0->returnType == nullptr, "func0 no return type");
        CompilationTest::Assert(f0->content != nullptr, "func0 block");
        if (f0->content)
        {
            CompilationTest::Assert(f0->content->content.size() == 1, "func0 one stmt");
            if (f0->content->content.size() == 1)
            {
                auto* ret = ExpressionCast<ExpressionType::_RETURN>(f0->content->content[0]);
                CompilationTest::Assert(ret != nullptr, "func0 return");
                if (ret) CompilationTest::Assert(ret->value == nullptr, "func0 return no value");
            }
        }
    }

    // func Test_Parameters(i32 a) { return; }
    auto* f1 = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(funcs[1]);
    CompilationTest::Assert(f1 != nullptr, "func1 should be function");
    if (f1)
    {
        AssertIdentifier(f1->functionName, "Test_Parameters", "func1 name");
        CompilationTest::Assert(f1->parameters.size() == 1, "func1 one param");
        if (f1->parameters.size() == 1)
        {
            auto* p = f1->parameters[0];
            AssertType(p->paramType, TokenType::_I32, "func1 param type");
            AssertIdentifier(p->name, "a", "func1 param name");
        }
        CompilationTest::Assert(f1->returnType == nullptr, "func1 no return type");
    }

    // func Test_Return(): string { return "Work"; }
    auto* f2 = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(funcs[2]);
    CompilationTest::Assert(f2 != nullptr, "func2 should be function");
    if (f2)
    {
        AssertIdentifier(f2->functionName, "Test_Return", "func2 name");
        CompilationTest::Assert(f2->parameters.empty(), "func2 no params");
        AssertType(f2->returnType, TokenType::_STRING_IDENTIFIER, "func2 return type");
        if (f2->content && f2->content->content.size() == 1)
        {
            auto* ret = ExpressionCast<ExpressionType::_RETURN>(f2->content->content[0]);
            CompilationTest::Assert(ret != nullptr, "func2 return");
            if (ret) AssertLiteral(ret->value, TokenType::_STRING_LITTERAL, "func2 return string");
        }
    }

    // func Test_Parameters_Return(i32 a) : i32 { return a; }
    auto* f3 = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(funcs[3]);
    CompilationTest::Assert(f3 != nullptr, "func3 should be function");
    if (f3)
    {
        AssertIdentifier(f3->functionName, "Test_Parameters_Return", "func3 name");
        CompilationTest::Assert(f3->parameters.size() == 1, "func3 one param");
        if (f3->parameters.size() == 1)
        {
            auto* p = f3->parameters[0];
            AssertType(p->paramType, TokenType::_I32, "func3 param type");
            AssertIdentifier(p->name, "a", "func3 param name");
        }
        AssertType(f3->returnType, TokenType::_I32, "func3 return type");
        if (f3->content && f3->content->content.size() == 1)
        {
            auto* ret = ExpressionCast<ExpressionType::_RETURN>(f3->content->content[0]);
            CompilationTest::Assert(ret != nullptr, "func3 return");
            if (ret) AssertIdentifier(ret->value, "a", "func3 return a");
        }
    }

    // func main { ... }
    auto* f4 = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(funcs[4]);
    CompilationTest::Assert(f4 != nullptr, "func4 should be main");
    if (!f4) return;

    AssertIdentifier(f4->functionName, "main", "func4 name");
    CompilationTest::Assert(f4->parameters.empty(), "main no params");
    CompilationTest::Assert(f4->returnType == nullptr, "main no return type");
    CompilationTest::Assert(f4->content != nullptr, "main block");
    if (!f4->content) return;

    auto& stmts = f4->content->content;
    CompilationTest::Assert(stmts.size() == 4, "main should have 4 statements");
    if (stmts.size() != 4) return;

    // Test();
    auto* call0 = ExpressionCast<ExpressionType::_FUNCTION_CALL>(stmts[0]);
    CompilationTest::Assert(call0 != nullptr, "stmt0 should be function call");
    if (call0)
    {
        AssertIdentifier(call0->name, "Test", "stmt0 call name");
        CompilationTest::Assert(call0->arguments.empty(), "stmt0 call no args");
    }

    // i32 value = 5;
    auto* v1 = AssertVarDecl(stmts[1], TokenType::_I32, "value", "stmt1 value");
    if (v1) AssertLiteral(v1->value, TokenType::_VALUE, "stmt1 value literal");

    // Test_Parameters(value);
    auto* call2 = ExpressionCast<ExpressionType::_FUNCTION_CALL>(stmts[2]);
    CompilationTest::Assert(call2 != nullptr, "stmt2 should be function call");
    if (call2)
    {
        AssertIdentifier(call2->name, "Test_Parameters", "stmt2 call name");
        CompilationTest::Assert(call2->arguments.size() == 1, "stmt2 one arg");
        if (call2->arguments.size() == 1)
            AssertIdentifier(call2->arguments[0], "value", "stmt2 arg value");
    }

    // value = Test_Parameters_Return(value);
    auto* assign3 = AssertVarAssign(stmts[3], "value", "stmt3 assign");
    if (assign3)
    {
        auto* call3 = ExpressionCast<ExpressionType::_FUNCTION_CALL>(assign3->value);
        CompilationTest::Assert(call3 != nullptr, "stmt3 should be function call");
        if (call3)
        {
            AssertIdentifier(call3->name, "Test_Parameters_Return", "stmt3 call name");
            CompilationTest::Assert(call3->arguments.size() == 1, "stmt3 one arg");
            if (call3->arguments.size() == 1)
                AssertIdentifier(call3->arguments[0], "value", "stmt3 arg value");
        }
    }
}

void ParserTest::TestLoop()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Loop.chessy") == true, "Loop.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    Parser parser;
    parser.Parse(tokens);

    ProgramExpression* program = parser.GetProgram();
    auto* func = GetSingleMainFunction(program, "Loop.chessy: program is null");
    if (!func) return;

    auto& stmts = func->content->content;
    CompilationTest::Assert(stmts.size() == 2, "Loop: main should have 2 statements");
    if (stmts.size() != 2) return;

    // i8 test = 1;
    auto* v0 = AssertVarDecl(stmts[0], TokenType::_I8, "test", "stmt0 test");
    if (v0) AssertLiteral(v0->value, TokenType::_VALUE, "stmt0 value");

    // do { test = test + 1; } while (test < 5);
    auto* doWhile = ExpressionCast<ExpressionType::_DO_WHILE>(stmts[1]);
    CompilationTest::Assert(doWhile != nullptr, "stmt1 should be a do/while");
    if (!doWhile) return;

    // --- Block : { test = test + 1; } ---
    CompilationTest::Assert(doWhile->block != nullptr, "do/while block");
    if (doWhile->block)
    {
        auto& blockStmts = doWhile->block->content;
        CompilationTest::Assert(blockStmts.size() == 1, "do/while body should have 1 stmt");
        if (blockStmts.size() == 1)
        {
            auto* assign = AssertVarAssign(blockStmts[0], "test", "body assign");
            if (assign)
            {
                auto* add = AssertBinary(assign->value, TokenType::_ADD, "body add");
                if (add)
                {
                    AssertIdentifier(add->left, "test", "body add left");
                    AssertLiteral(add->right, TokenType::_VALUE, "body add right");
                }
            }
        }
    }

    // --- Condition : test < 5 ---
    auto* cond = AssertBinary(doWhile->condition, TokenType::_LOWER_THAN, "do/while condition");
    if (cond)
    {
        AssertIdentifier(cond->left, "test", "condition left");
        AssertLiteral(cond->right, TokenType::_VALUE, "condition right");
    }
}