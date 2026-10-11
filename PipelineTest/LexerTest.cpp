#include "LexerTest.h"
#include "CompilationTest.h"
#include "Lexer.h"

void LexerTest::TestDeclaration()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Declaration.chessy") == true, "Declaration.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    // func main() {
    CompilationTest::Assert(tokens[0].globalType == GlobalTokenType::_KEYWORD    && tokens[0].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[1].globalType == GlobalTokenType::_IDENTIFIER && tokens[1].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[2].globalType == GlobalTokenType::_DELIMITOR  && tokens[2].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[3].globalType == GlobalTokenType::_DELIMITOR  && tokens[3].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[4].globalType == GlobalTokenType::_DELIMITOR  && tokens[4].type == TokenType::_L_BRACE);

    // bool test_bool = true;
    CompilationTest::Assert(tokens[5].globalType == GlobalTokenType::_IDENTIFIER && tokens[5].type == TokenType::_BOOL);
    CompilationTest::Assert(tokens[6].globalType == GlobalTokenType::_IDENTIFIER && tokens[6].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[7].globalType == GlobalTokenType::_OPERAND    && tokens[7].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[8].globalType == GlobalTokenType::_KEYWORD    && tokens[8].type == TokenType::_TRUE);
    CompilationTest::Assert(tokens[9].globalType == GlobalTokenType::_DELIMITOR  && tokens[9].type == TokenType::_SEMI_COLON);

    // i8 test_i8 = 127;
    CompilationTest::Assert(tokens[10].globalType == GlobalTokenType::_IDENTIFIER && tokens[10].type == TokenType::_I8);
    CompilationTest::Assert(tokens[11].globalType == GlobalTokenType::_IDENTIFIER && tokens[11].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[12].globalType == GlobalTokenType::_OPERAND    && tokens[12].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[13].globalType == GlobalTokenType::_LITTERAL   && tokens[13].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[14].globalType == GlobalTokenType::_DELIMITOR  && tokens[14].type == TokenType::_SEMI_COLON);

    // i16 test_i16 = 32767;
    CompilationTest::Assert(tokens[15].globalType == GlobalTokenType::_IDENTIFIER && tokens[15].type == TokenType::_I16);
    CompilationTest::Assert(tokens[16].globalType == GlobalTokenType::_IDENTIFIER && tokens[16].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[17].globalType == GlobalTokenType::_OPERAND    && tokens[17].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[18].globalType == GlobalTokenType::_LITTERAL   && tokens[18].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[19].globalType == GlobalTokenType::_DELIMITOR  && tokens[19].type == TokenType::_SEMI_COLON);

    // i32 test_i32 = 2147483647;
    CompilationTest::Assert(tokens[20].globalType == GlobalTokenType::_IDENTIFIER && tokens[20].type == TokenType::_I32);
    CompilationTest::Assert(tokens[21].globalType == GlobalTokenType::_IDENTIFIER && tokens[21].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[22].globalType == GlobalTokenType::_OPERAND    && tokens[22].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[23].globalType == GlobalTokenType::_LITTERAL   && tokens[23].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[24].globalType == GlobalTokenType::_DELIMITOR  && tokens[24].type == TokenType::_SEMI_COLON);

    // i64 test_i64 = 9223372036854775808;
    CompilationTest::Assert(tokens[25].globalType == GlobalTokenType::_IDENTIFIER && tokens[25].type == TokenType::_I64);
    CompilationTest::Assert(tokens[26].globalType == GlobalTokenType::_IDENTIFIER && tokens[26].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[27].globalType == GlobalTokenType::_OPERAND    && tokens[27].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[28].globalType == GlobalTokenType::_LITTERAL   && tokens[28].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[29].globalType == GlobalTokenType::_DELIMITOR  && tokens[29].type == TokenType::_SEMI_COLON);

    // u8 test_u8 = 255;
    CompilationTest::Assert(tokens[30].globalType == GlobalTokenType::_IDENTIFIER && tokens[30].type == TokenType::_U8);
    CompilationTest::Assert(tokens[31].globalType == GlobalTokenType::_IDENTIFIER && tokens[31].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[32].globalType == GlobalTokenType::_OPERAND    && tokens[32].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[33].globalType == GlobalTokenType::_LITTERAL   && tokens[33].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[34].globalType == GlobalTokenType::_DELIMITOR  && tokens[34].type == TokenType::_SEMI_COLON);

    // u16 test_u16 = 65535;
    CompilationTest::Assert(tokens[35].globalType == GlobalTokenType::_IDENTIFIER && tokens[35].type == TokenType::_U16);
    CompilationTest::Assert(tokens[36].globalType == GlobalTokenType::_IDENTIFIER && tokens[36].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[37].globalType == GlobalTokenType::_OPERAND    && tokens[37].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[38].globalType == GlobalTokenType::_LITTERAL   && tokens[38].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[39].globalType == GlobalTokenType::_DELIMITOR  && tokens[39].type == TokenType::_SEMI_COLON);

    // u32 test_u32 = 4294967295;
    CompilationTest::Assert(tokens[40].globalType == GlobalTokenType::_IDENTIFIER && tokens[40].type == TokenType::_U32);
    CompilationTest::Assert(tokens[41].globalType == GlobalTokenType::_IDENTIFIER && tokens[41].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[42].globalType == GlobalTokenType::_OPERAND    && tokens[42].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[43].globalType == GlobalTokenType::_LITTERAL   && tokens[43].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[44].globalType == GlobalTokenType::_DELIMITOR  && tokens[44].type == TokenType::_SEMI_COLON);

    // u64 test_u64 = 18446744073709551615;
    CompilationTest::Assert(tokens[45].globalType == GlobalTokenType::_IDENTIFIER && tokens[45].type == TokenType::_U64);
    CompilationTest::Assert(tokens[46].globalType == GlobalTokenType::_IDENTIFIER && tokens[46].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[47].globalType == GlobalTokenType::_OPERAND    && tokens[47].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[48].globalType == GlobalTokenType::_LITTERAL   && tokens[48].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[49].globalType == GlobalTokenType::_DELIMITOR  && tokens[49].type == TokenType::_SEMI_COLON);

    // string test_string = "Hello World !";
    CompilationTest::Assert(tokens[50].globalType == GlobalTokenType::_IDENTIFIER && tokens[50].type == TokenType::_STRING_IDENTIFIER);
    CompilationTest::Assert(tokens[51].globalType == GlobalTokenType::_IDENTIFIER && tokens[51].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[52].globalType == GlobalTokenType::_OPERAND    && tokens[52].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[53].globalType == GlobalTokenType::_LITTERAL   && tokens[53].type == TokenType::_STRING_LITTERAL);
    CompilationTest::Assert(tokens[54].globalType == GlobalTokenType::_DELIMITOR  && tokens[54].type == TokenType::_SEMI_COLON);

    // bitboard test_bitboard;
    CompilationTest::Assert(tokens[55].globalType == GlobalTokenType::_IDENTIFIER && tokens[55].type == TokenType::_BITBOARD);
    CompilationTest::Assert(tokens[56].globalType == GlobalTokenType::_IDENTIFIER && tokens[56].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[57].globalType == GlobalTokenType::_DELIMITOR  && tokens[57].type == TokenType::_SEMI_COLON);

    // position test_position;
    CompilationTest::Assert(tokens[58].globalType == GlobalTokenType::_IDENTIFIER && tokens[58].type == TokenType::_SQUARE);
    CompilationTest::Assert(tokens[59].globalType == GlobalTokenType::_IDENTIFIER && tokens[59].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[60].globalType == GlobalTokenType::_DELIMITOR  && tokens[60].type == TokenType::_SEMI_COLON);

    // piece test_piece;
    CompilationTest::Assert(tokens[61].globalType == GlobalTokenType::_IDENTIFIER && tokens[61].type == TokenType::_PIECE);
    CompilationTest::Assert(tokens[62].globalType == GlobalTokenType::_IDENTIFIER && tokens[62].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[63].globalType == GlobalTokenType::_DELIMITOR  && tokens[63].type == TokenType::_SEMI_COLON);

    // move test_move;
    CompilationTest::Assert(tokens[64].globalType == GlobalTokenType::_IDENTIFIER && tokens[64].type == TokenType::_MOVE);
    CompilationTest::Assert(tokens[65].globalType == GlobalTokenType::_IDENTIFIER && tokens[65].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[66].globalType == GlobalTokenType::_DELIMITOR  && tokens[66].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[67].globalType == GlobalTokenType::_DELIMITOR && tokens[67].type == TokenType::_R_BRACE);
}

void LexerTest::TestMathOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Math_Operation.chessy") == true, "Math_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    // func main() {
    CompilationTest::Assert(tokens[0].globalType == GlobalTokenType::_KEYWORD    && tokens[0].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[1].globalType == GlobalTokenType::_IDENTIFIER && tokens[1].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[2].globalType == GlobalTokenType::_DELIMITOR  && tokens[2].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[3].globalType == GlobalTokenType::_DELIMITOR  && tokens[3].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[4].globalType == GlobalTokenType::_DELIMITOR  && tokens[4].type == TokenType::_L_BRACE);

    // i32 tester1 = 5;
    CompilationTest::Assert(tokens[5].globalType  == GlobalTokenType::_IDENTIFIER && tokens[5].type  == TokenType::_I32);
    CompilationTest::Assert(tokens[6].globalType  == GlobalTokenType::_IDENTIFIER && tokens[6].type  == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[7].globalType  == GlobalTokenType::_OPERAND    && tokens[7].type  == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[8].globalType  == GlobalTokenType::_LITTERAL   && tokens[8].type  == TokenType::_VALUE);
    CompilationTest::Assert(tokens[9].globalType  == GlobalTokenType::_DELIMITOR  && tokens[9].type  == TokenType::_SEMI_COLON);

    // i32 tester2 = 2;
    CompilationTest::Assert(tokens[10].globalType == GlobalTokenType::_IDENTIFIER && tokens[10].type == TokenType::_I32);
    CompilationTest::Assert(tokens[11].globalType == GlobalTokenType::_IDENTIFIER && tokens[11].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[12].globalType == GlobalTokenType::_OPERAND    && tokens[12].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[13].globalType == GlobalTokenType::_LITTERAL   && tokens[13].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[14].globalType == GlobalTokenType::_DELIMITOR  && tokens[14].type == TokenType::_SEMI_COLON);

    // i32 result = tester1+tester2;
    CompilationTest::Assert(tokens[15].globalType == GlobalTokenType::_IDENTIFIER && tokens[15].type == TokenType::_I32);
    CompilationTest::Assert(tokens[16].globalType == GlobalTokenType::_IDENTIFIER && tokens[16].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[17].globalType == GlobalTokenType::_OPERAND    && tokens[17].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[18].globalType == GlobalTokenType::_IDENTIFIER && tokens[18].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[19].globalType == GlobalTokenType::_OPERAND    && tokens[19].type == TokenType::_ADD);
    CompilationTest::Assert(tokens[20].globalType == GlobalTokenType::_IDENTIFIER && tokens[20].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[21].globalType == GlobalTokenType::_DELIMITOR  && tokens[21].type == TokenType::_SEMI_COLON);

    // result = tester1 - tester2;
    CompilationTest::Assert(tokens[22].globalType == GlobalTokenType::_IDENTIFIER && tokens[22].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[23].globalType == GlobalTokenType::_OPERAND    && tokens[23].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[24].globalType == GlobalTokenType::_IDENTIFIER && tokens[24].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[25].globalType == GlobalTokenType::_OPERAND    && tokens[25].type == TokenType::_SUB);
    CompilationTest::Assert(tokens[26].globalType == GlobalTokenType::_IDENTIFIER && tokens[26].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[27].globalType == GlobalTokenType::_DELIMITOR  && tokens[27].type == TokenType::_SEMI_COLON);

    // result = tester1 * tester2;
    CompilationTest::Assert(tokens[28].globalType == GlobalTokenType::_IDENTIFIER && tokens[28].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[29].globalType == GlobalTokenType::_OPERAND    && tokens[29].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[30].globalType == GlobalTokenType::_IDENTIFIER && tokens[30].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[31].globalType == GlobalTokenType::_OPERAND    && tokens[31].type == TokenType::_MUL);
    CompilationTest::Assert(tokens[32].globalType == GlobalTokenType::_IDENTIFIER && tokens[32].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[33].globalType == GlobalTokenType::_DELIMITOR  && tokens[33].type == TokenType::_SEMI_COLON);

    // result = tester1 ** tester2;
    CompilationTest::Assert(tokens[34].globalType == GlobalTokenType::_IDENTIFIER && tokens[34].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[35].globalType == GlobalTokenType::_OPERAND    && tokens[35].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[36].globalType == GlobalTokenType::_IDENTIFIER && tokens[36].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[37].globalType == GlobalTokenType::_OPERAND    && tokens[37].type == TokenType::_POW);
    CompilationTest::Assert(tokens[38].globalType == GlobalTokenType::_IDENTIFIER && tokens[38].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[39].globalType == GlobalTokenType::_DELIMITOR  && tokens[39].type == TokenType::_SEMI_COLON);

    // result = result / tester2;
    CompilationTest::Assert(tokens[40].globalType == GlobalTokenType::_IDENTIFIER && tokens[40].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[41].globalType == GlobalTokenType::_OPERAND    && tokens[41].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[42].globalType == GlobalTokenType::_IDENTIFIER && tokens[42].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[43].globalType == GlobalTokenType::_OPERAND    && tokens[43].type == TokenType::_DIV);
    CompilationTest::Assert(tokens[44].globalType == GlobalTokenType::_IDENTIFIER && tokens[44].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[45].globalType == GlobalTokenType::_DELIMITOR  && tokens[45].type == TokenType::_SEMI_COLON);

    // result = tester1 * tester2 % tester2;
    CompilationTest::Assert(tokens[46].globalType == GlobalTokenType::_IDENTIFIER && tokens[46].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[47].globalType == GlobalTokenType::_OPERAND    && tokens[47].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[48].globalType == GlobalTokenType::_IDENTIFIER && tokens[48].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[49].globalType == GlobalTokenType::_OPERAND    && tokens[49].type == TokenType::_MUL);
    CompilationTest::Assert(tokens[50].globalType == GlobalTokenType::_IDENTIFIER && tokens[50].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[51].globalType == GlobalTokenType::_OPERAND    && tokens[51].type == TokenType::_MOD);
    CompilationTest::Assert(tokens[52].globalType == GlobalTokenType::_IDENTIFIER && tokens[52].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[53].globalType == GlobalTokenType::_DELIMITOR  && tokens[53].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[54].globalType == GlobalTokenType::_DELIMITOR  && tokens[54].type == TokenType::_R_BRACE);
}

void LexerTest::TestBoolOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Bool_Operation.chessy") == true, "Bool_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    // func main() {
    CompilationTest::Assert(tokens[0].globalType == GlobalTokenType::_KEYWORD    && tokens[0].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[1].globalType == GlobalTokenType::_IDENTIFIER && tokens[1].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[2].globalType == GlobalTokenType::_DELIMITOR  && tokens[2].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[3].globalType == GlobalTokenType::_DELIMITOR  && tokens[3].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[4].globalType == GlobalTokenType::_DELIMITOR  && tokens[4].type == TokenType::_L_BRACE);

    // i32 tester1 = 5;
    CompilationTest::Assert(tokens[5].globalType  == GlobalTokenType::_IDENTIFIER && tokens[5].type  == TokenType::_I32);
    CompilationTest::Assert(tokens[6].globalType  == GlobalTokenType::_IDENTIFIER && tokens[6].type  == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[7].globalType  == GlobalTokenType::_OPERAND    && tokens[7].type  == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[8].globalType  == GlobalTokenType::_LITTERAL   && tokens[8].type  == TokenType::_VALUE);
    CompilationTest::Assert(tokens[9].globalType  == GlobalTokenType::_DELIMITOR  && tokens[9].type  == TokenType::_SEMI_COLON);

    // i32 tester2 = 2;
    CompilationTest::Assert(tokens[10].globalType == GlobalTokenType::_IDENTIFIER && tokens[10].type == TokenType::_I32);
    CompilationTest::Assert(tokens[11].globalType == GlobalTokenType::_IDENTIFIER && tokens[11].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[12].globalType == GlobalTokenType::_OPERAND    && tokens[12].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[13].globalType == GlobalTokenType::_LITTERAL   && tokens[13].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[14].globalType == GlobalTokenType::_DELIMITOR  && tokens[14].type == TokenType::_SEMI_COLON);

    // bool booleans = tester1 == tester2;
    CompilationTest::Assert(tokens[15].globalType == GlobalTokenType::_IDENTIFIER && tokens[15].type == TokenType::_BOOL);
    CompilationTest::Assert(tokens[16].globalType == GlobalTokenType::_IDENTIFIER && tokens[16].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[17].globalType == GlobalTokenType::_OPERAND    && tokens[17].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[18].globalType == GlobalTokenType::_IDENTIFIER && tokens[18].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[19].globalType == GlobalTokenType::_OPERAND    && tokens[19].type == TokenType::_EQUAL_OP);
    CompilationTest::Assert(tokens[20].globalType == GlobalTokenType::_IDENTIFIER && tokens[20].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[21].globalType == GlobalTokenType::_DELIMITOR  && tokens[21].type == TokenType::_SEMI_COLON);

    // booleans = tester1 != tester2;
    CompilationTest::Assert(tokens[22].globalType == GlobalTokenType::_IDENTIFIER && tokens[22].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[23].globalType == GlobalTokenType::_OPERAND    && tokens[23].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[24].globalType == GlobalTokenType::_IDENTIFIER && tokens[24].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[25].globalType == GlobalTokenType::_OPERAND    && tokens[25].type == TokenType::_NOT_EQUAL_OP);
    CompilationTest::Assert(tokens[26].globalType == GlobalTokenType::_IDENTIFIER && tokens[26].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[27].globalType == GlobalTokenType::_DELIMITOR  && tokens[27].type == TokenType::_SEMI_COLON);

    // booleans = tester1 < tester2;
    CompilationTest::Assert(tokens[28].globalType == GlobalTokenType::_IDENTIFIER && tokens[28].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[29].globalType == GlobalTokenType::_OPERAND    && tokens[29].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[30].globalType == GlobalTokenType::_IDENTIFIER && tokens[30].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[31].globalType == GlobalTokenType::_OPERAND    && tokens[31].type == TokenType::_LOWER_THAN);
    CompilationTest::Assert(tokens[32].globalType == GlobalTokenType::_IDENTIFIER && tokens[32].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[33].globalType == GlobalTokenType::_DELIMITOR  && tokens[33].type == TokenType::_SEMI_COLON);

    // booleans = tester1 <= tester2;
    CompilationTest::Assert(tokens[34].globalType == GlobalTokenType::_IDENTIFIER && tokens[34].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[35].globalType == GlobalTokenType::_OPERAND    && tokens[35].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[36].globalType == GlobalTokenType::_IDENTIFIER && tokens[36].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[37].globalType == GlobalTokenType::_OPERAND    && tokens[37].type == TokenType::_LOWER_EQ_THAN);
    CompilationTest::Assert(tokens[38].globalType == GlobalTokenType::_IDENTIFIER && tokens[38].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[39].globalType == GlobalTokenType::_DELIMITOR  && tokens[39].type == TokenType::_SEMI_COLON);

    // booleans = tester1 > tester2;
    CompilationTest::Assert(tokens[40].globalType == GlobalTokenType::_IDENTIFIER && tokens[40].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[41].globalType == GlobalTokenType::_OPERAND    && tokens[41].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[42].globalType == GlobalTokenType::_IDENTIFIER && tokens[42].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[43].globalType == GlobalTokenType::_OPERAND    && tokens[43].type == TokenType::_GREATER_THAN);
    CompilationTest::Assert(tokens[44].globalType == GlobalTokenType::_IDENTIFIER && tokens[44].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[45].globalType == GlobalTokenType::_DELIMITOR  && tokens[45].type == TokenType::_SEMI_COLON);

    // booleans = tester1 >= tester2;
    CompilationTest::Assert(tokens[46].globalType == GlobalTokenType::_IDENTIFIER && tokens[46].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[47].globalType == GlobalTokenType::_OPERAND    && tokens[47].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[48].globalType == GlobalTokenType::_IDENTIFIER && tokens[48].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[49].globalType == GlobalTokenType::_OPERAND    && tokens[49].type == TokenType::_GREATER_EQ_THAN);
    CompilationTest::Assert(tokens[50].globalType == GlobalTokenType::_IDENTIFIER && tokens[50].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[51].globalType == GlobalTokenType::_DELIMITOR  && tokens[51].type == TokenType::_SEMI_COLON);

    // booleans = true && true;
    CompilationTest::Assert(tokens[52].globalType == GlobalTokenType::_IDENTIFIER && tokens[52].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[53].globalType == GlobalTokenType::_OPERAND    && tokens[53].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[54].globalType == GlobalTokenType::_KEYWORD    && tokens[54].type == TokenType::_TRUE);
    CompilationTest::Assert(tokens[55].globalType == GlobalTokenType::_OPERAND    && tokens[55].type == TokenType::_AND_OP);
    CompilationTest::Assert(tokens[56].globalType == GlobalTokenType::_KEYWORD    && tokens[56].type == TokenType::_TRUE);
    CompilationTest::Assert(tokens[57].globalType == GlobalTokenType::_DELIMITOR  && tokens[57].type == TokenType::_SEMI_COLON);

    // booleans = true && false;
    CompilationTest::Assert(tokens[58].globalType == GlobalTokenType::_IDENTIFIER && tokens[58].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[59].globalType == GlobalTokenType::_OPERAND    && tokens[59].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[60].globalType == GlobalTokenType::_KEYWORD    && tokens[60].type == TokenType::_TRUE);
    CompilationTest::Assert(tokens[61].globalType == GlobalTokenType::_OPERAND    && tokens[61].type == TokenType::_AND_OP);
    CompilationTest::Assert(tokens[62].globalType == GlobalTokenType::_KEYWORD    && tokens[62].type == TokenType::_FALSE);
    CompilationTest::Assert(tokens[63].globalType == GlobalTokenType::_DELIMITOR  && tokens[63].type == TokenType::_SEMI_COLON);

    // booleans = false && false;
    CompilationTest::Assert(tokens[64].globalType == GlobalTokenType::_IDENTIFIER && tokens[64].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[65].globalType == GlobalTokenType::_OPERAND    && tokens[65].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[66].globalType == GlobalTokenType::_KEYWORD    && tokens[66].type == TokenType::_FALSE);
    CompilationTest::Assert(tokens[67].globalType == GlobalTokenType::_OPERAND    && tokens[67].type == TokenType::_AND_OP);
    CompilationTest::Assert(tokens[68].globalType == GlobalTokenType::_KEYWORD    && tokens[68].type == TokenType::_FALSE);
    CompilationTest::Assert(tokens[69].globalType == GlobalTokenType::_DELIMITOR  && tokens[69].type == TokenType::_SEMI_COLON);

    // booleans = true || true;
    CompilationTest::Assert(tokens[70].globalType == GlobalTokenType::_IDENTIFIER && tokens[70].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[71].globalType == GlobalTokenType::_OPERAND    && tokens[71].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[72].globalType == GlobalTokenType::_KEYWORD    && tokens[72].type == TokenType::_TRUE);
    CompilationTest::Assert(tokens[73].globalType == GlobalTokenType::_OPERAND    && tokens[73].type == TokenType::_OR_OP);
    CompilationTest::Assert(tokens[74].globalType == GlobalTokenType::_KEYWORD    && tokens[74].type == TokenType::_TRUE);
    CompilationTest::Assert(tokens[75].globalType == GlobalTokenType::_DELIMITOR  && tokens[75].type == TokenType::_SEMI_COLON);

    // booleans = true || false;
    CompilationTest::Assert(tokens[76].globalType == GlobalTokenType::_IDENTIFIER && tokens[76].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[77].globalType == GlobalTokenType::_OPERAND    && tokens[77].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[78].globalType == GlobalTokenType::_KEYWORD    && tokens[78].type == TokenType::_TRUE);
    CompilationTest::Assert(tokens[79].globalType == GlobalTokenType::_OPERAND    && tokens[79].type == TokenType::_OR_OP);
    CompilationTest::Assert(tokens[80].globalType == GlobalTokenType::_KEYWORD    && tokens[80].type == TokenType::_FALSE);
    CompilationTest::Assert(tokens[81].globalType == GlobalTokenType::_DELIMITOR  && tokens[81].type == TokenType::_SEMI_COLON);

    // booleans = false || false;
    CompilationTest::Assert(tokens[82].globalType == GlobalTokenType::_IDENTIFIER && tokens[82].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[83].globalType == GlobalTokenType::_OPERAND    && tokens[83].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[84].globalType == GlobalTokenType::_KEYWORD    && tokens[84].type == TokenType::_FALSE);
    CompilationTest::Assert(tokens[85].globalType == GlobalTokenType::_OPERAND    && tokens[85].type == TokenType::_OR_OP);
    CompilationTest::Assert(tokens[86].globalType == GlobalTokenType::_KEYWORD    && tokens[86].type == TokenType::_FALSE);
    CompilationTest::Assert(tokens[87].globalType == GlobalTokenType::_DELIMITOR  && tokens[87].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[88].globalType == GlobalTokenType::_DELIMITOR  && tokens[88].type == TokenType::_R_BRACE);
}

void LexerTest::TestBinaOperation()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Bina_Operation.chessy") == true, "Bina_Operation.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    // func main() {
    CompilationTest::Assert(tokens[0].globalType == GlobalTokenType::_KEYWORD    && tokens[0].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[1].globalType == GlobalTokenType::_IDENTIFIER && tokens[1].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[2].globalType == GlobalTokenType::_DELIMITOR  && tokens[2].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[3].globalType == GlobalTokenType::_DELIMITOR  && tokens[3].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[4].globalType == GlobalTokenType::_DELIMITOR  && tokens[4].type == TokenType::_L_BRACE);

    // i32 tester1 = 5;
    CompilationTest::Assert(tokens[5].globalType  == GlobalTokenType::_IDENTIFIER && tokens[5].type  == TokenType::_I32);
    CompilationTest::Assert(tokens[6].globalType  == GlobalTokenType::_IDENTIFIER && tokens[6].type  == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[7].globalType  == GlobalTokenType::_OPERAND    && tokens[7].type  == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[8].globalType  == GlobalTokenType::_LITTERAL   && tokens[8].type  == TokenType::_VALUE);
    CompilationTest::Assert(tokens[9].globalType  == GlobalTokenType::_DELIMITOR  && tokens[9].type  == TokenType::_SEMI_COLON);

    // i32 tester2 = 2;
    CompilationTest::Assert(tokens[10].globalType == GlobalTokenType::_IDENTIFIER && tokens[10].type == TokenType::_I32);
    CompilationTest::Assert(tokens[11].globalType == GlobalTokenType::_IDENTIFIER && tokens[11].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[12].globalType == GlobalTokenType::_OPERAND    && tokens[12].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[13].globalType == GlobalTokenType::_LITTERAL   && tokens[13].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[14].globalType == GlobalTokenType::_DELIMITOR  && tokens[14].type == TokenType::_SEMI_COLON);

    // i32 result = tester1 | tester2;
    CompilationTest::Assert(tokens[15].globalType == GlobalTokenType::_IDENTIFIER && tokens[15].type == TokenType::_I32);
    CompilationTest::Assert(tokens[16].globalType == GlobalTokenType::_IDENTIFIER && tokens[16].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[17].globalType == GlobalTokenType::_OPERAND    && tokens[17].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[18].globalType == GlobalTokenType::_IDENTIFIER && tokens[18].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[19].globalType == GlobalTokenType::_OPERAND    && tokens[19].type == TokenType::_OR_BINARY);
    CompilationTest::Assert(tokens[20].globalType == GlobalTokenType::_IDENTIFIER && tokens[20].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[21].globalType == GlobalTokenType::_DELIMITOR  && tokens[21].type == TokenType::_SEMI_COLON);

    // result = tester1 ^ tester2;
    CompilationTest::Assert(tokens[22].globalType == GlobalTokenType::_IDENTIFIER && tokens[22].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[23].globalType == GlobalTokenType::_OPERAND    && tokens[23].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[24].globalType == GlobalTokenType::_IDENTIFIER && tokens[24].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[25].globalType == GlobalTokenType::_OPERAND    && tokens[25].type == TokenType::_XOR_BINARY);
    CompilationTest::Assert(tokens[26].globalType == GlobalTokenType::_IDENTIFIER && tokens[26].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[27].globalType == GlobalTokenType::_DELIMITOR  && tokens[27].type == TokenType::_SEMI_COLON);

    // result = tester1 & tester2;
    CompilationTest::Assert(tokens[28].globalType == GlobalTokenType::_IDENTIFIER && tokens[28].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[29].globalType == GlobalTokenType::_OPERAND    && tokens[29].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[30].globalType == GlobalTokenType::_IDENTIFIER && tokens[30].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[31].globalType == GlobalTokenType::_OPERAND    && tokens[31].type == TokenType::_AND_BINARY);
    CompilationTest::Assert(tokens[32].globalType == GlobalTokenType::_IDENTIFIER && tokens[32].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[33].globalType == GlobalTokenType::_DELIMITOR  && tokens[33].type == TokenType::_SEMI_COLON);

    // result = !result;
    CompilationTest::Assert(tokens[34].globalType == GlobalTokenType::_IDENTIFIER && tokens[34].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[35].globalType == GlobalTokenType::_OPERAND    && tokens[35].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[36].globalType == GlobalTokenType::_OPERAND    && tokens[36].type == TokenType::_NOT_OP);
    CompilationTest::Assert(tokens[37].globalType == GlobalTokenType::_IDENTIFIER && tokens[37].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[38].globalType == GlobalTokenType::_DELIMITOR  && tokens[38].type == TokenType::_SEMI_COLON);

    // result = tester1 << tester2;
    CompilationTest::Assert(tokens[39].globalType == GlobalTokenType::_IDENTIFIER && tokens[39].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[40].globalType == GlobalTokenType::_OPERAND    && tokens[40].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[41].globalType == GlobalTokenType::_IDENTIFIER && tokens[41].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[42].globalType == GlobalTokenType::_OPERAND    && tokens[42].type == TokenType::_L_SHIFT);
    CompilationTest::Assert(tokens[43].globalType == GlobalTokenType::_IDENTIFIER && tokens[43].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[44].globalType == GlobalTokenType::_DELIMITOR  && tokens[44].type == TokenType::_SEMI_COLON);

    // result = tester1 >> tester2;
    CompilationTest::Assert(tokens[45].globalType == GlobalTokenType::_IDENTIFIER && tokens[45].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[46].globalType == GlobalTokenType::_OPERAND    && tokens[46].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[47].globalType == GlobalTokenType::_IDENTIFIER && tokens[47].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[48].globalType == GlobalTokenType::_OPERAND    && tokens[48].type == TokenType::_R_SHIFT);
    CompilationTest::Assert(tokens[49].globalType == GlobalTokenType::_IDENTIFIER && tokens[49].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[50].globalType == GlobalTokenType::_DELIMITOR  && tokens[50].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[51].globalType == GlobalTokenType::_DELIMITOR  && tokens[51].type == TokenType::_R_BRACE);
}

void LexerTest::TestFunction()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Function.chessy") == true, "Function.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    // func Test() {
    CompilationTest::Assert(tokens[0].globalType  == GlobalTokenType::_KEYWORD    && tokens[0].type  == TokenType::_FUNC);
    CompilationTest::Assert(tokens[1].globalType  == GlobalTokenType::_IDENTIFIER && tokens[1].type  == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[2].globalType  == GlobalTokenType::_DELIMITOR  && tokens[2].type  == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[3].globalType  == GlobalTokenType::_DELIMITOR  && tokens[3].type  == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[4].globalType  == GlobalTokenType::_DELIMITOR  && tokens[4].type  == TokenType::_L_BRACE);

    // return;
    CompilationTest::Assert(tokens[5].globalType  == GlobalTokenType::_KEYWORD    && tokens[5].type  == TokenType::_RETURN);
    CompilationTest::Assert(tokens[6].globalType  == GlobalTokenType::_DELIMITOR  && tokens[6].type  == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[7].globalType  == GlobalTokenType::_DELIMITOR  && tokens[7].type  == TokenType::_R_BRACE);

    // func Test_Parameters(i32 a) {
    CompilationTest::Assert(tokens[8].globalType  == GlobalTokenType::_KEYWORD    && tokens[8].type  == TokenType::_FUNC);
    CompilationTest::Assert(tokens[9].globalType  == GlobalTokenType::_IDENTIFIER && tokens[9].type  == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[10].globalType == GlobalTokenType::_DELIMITOR  && tokens[10].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[11].globalType == GlobalTokenType::_IDENTIFIER && tokens[11].type == TokenType::_I32);
    CompilationTest::Assert(tokens[12].globalType == GlobalTokenType::_IDENTIFIER && tokens[12].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[13].globalType == GlobalTokenType::_DELIMITOR  && tokens[13].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[14].globalType == GlobalTokenType::_DELIMITOR  && tokens[14].type == TokenType::_L_BRACE);

    // return;
    CompilationTest::Assert(tokens[15].globalType == GlobalTokenType::_KEYWORD    && tokens[15].type == TokenType::_RETURN);
    CompilationTest::Assert(tokens[16].globalType == GlobalTokenType::_DELIMITOR  && tokens[16].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[17].globalType == GlobalTokenType::_DELIMITOR  && tokens[17].type == TokenType::_R_BRACE);

    // func Test_Return(): string {
    CompilationTest::Assert(tokens[18].globalType == GlobalTokenType::_KEYWORD    && tokens[18].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[19].globalType == GlobalTokenType::_IDENTIFIER && tokens[19].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[20].globalType == GlobalTokenType::_DELIMITOR  && tokens[20].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[21].globalType == GlobalTokenType::_DELIMITOR  && tokens[21].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[22].globalType == GlobalTokenType::_DELIMITOR  && tokens[22].type == TokenType::_COLON);
    CompilationTest::Assert(tokens[23].globalType == GlobalTokenType::_IDENTIFIER && tokens[23].type == TokenType::_STRING_IDENTIFIER);
    CompilationTest::Assert(tokens[24].globalType == GlobalTokenType::_DELIMITOR  && tokens[24].type == TokenType::_L_BRACE);

    // return "Work";
    CompilationTest::Assert(tokens[25].globalType == GlobalTokenType::_KEYWORD    && tokens[25].type == TokenType::_RETURN);
    CompilationTest::Assert(tokens[26].globalType == GlobalTokenType::_LITTERAL   && tokens[26].type == TokenType::_STRING_LITTERAL);
    CompilationTest::Assert(tokens[27].globalType == GlobalTokenType::_DELIMITOR  && tokens[27].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[28].globalType == GlobalTokenType::_DELIMITOR  && tokens[28].type == TokenType::_R_BRACE);

    // func Test_Parameters_Return(i32 a) : i32 {
    CompilationTest::Assert(tokens[29].globalType == GlobalTokenType::_KEYWORD    && tokens[29].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[30].globalType == GlobalTokenType::_IDENTIFIER && tokens[30].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[31].globalType == GlobalTokenType::_DELIMITOR  && tokens[31].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[32].globalType == GlobalTokenType::_IDENTIFIER && tokens[32].type == TokenType::_I32);
    CompilationTest::Assert(tokens[33].globalType == GlobalTokenType::_IDENTIFIER && tokens[33].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[34].globalType == GlobalTokenType::_DELIMITOR  && tokens[34].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[35].globalType == GlobalTokenType::_DELIMITOR  && tokens[35].type == TokenType::_COLON);
    CompilationTest::Assert(tokens[36].globalType == GlobalTokenType::_IDENTIFIER && tokens[36].type == TokenType::_I32);
    CompilationTest::Assert(tokens[37].globalType == GlobalTokenType::_DELIMITOR  && tokens[37].type == TokenType::_L_BRACE);

    // return a;
    CompilationTest::Assert(tokens[38].globalType == GlobalTokenType::_KEYWORD    && tokens[38].type == TokenType::_RETURN);
    CompilationTest::Assert(tokens[39].globalType == GlobalTokenType::_IDENTIFIER && tokens[39].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[40].globalType == GlobalTokenType::_DELIMITOR  && tokens[40].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[41].globalType == GlobalTokenType::_DELIMITOR  && tokens[41].type == TokenType::_R_BRACE);

    // func main() {
    CompilationTest::Assert(tokens[42].globalType == GlobalTokenType::_KEYWORD    && tokens[42].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[43].globalType == GlobalTokenType::_IDENTIFIER && tokens[43].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[44].globalType == GlobalTokenType::_DELIMITOR  && tokens[44].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[45].globalType == GlobalTokenType::_DELIMITOR  && tokens[45].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[46].globalType == GlobalTokenType::_DELIMITOR  && tokens[46].type == TokenType::_L_BRACE);

    // Test();
    CompilationTest::Assert(tokens[47].globalType == GlobalTokenType::_IDENTIFIER && tokens[47].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[48].globalType == GlobalTokenType::_DELIMITOR  && tokens[48].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[49].globalType == GlobalTokenType::_DELIMITOR  && tokens[49].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[50].globalType == GlobalTokenType::_DELIMITOR  && tokens[50].type == TokenType::_SEMI_COLON);

    // i32 value = 5;
    CompilationTest::Assert(tokens[51].globalType == GlobalTokenType::_IDENTIFIER && tokens[51].type == TokenType::_I32);
    CompilationTest::Assert(tokens[52].globalType == GlobalTokenType::_IDENTIFIER && tokens[52].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[53].globalType == GlobalTokenType::_OPERAND    && tokens[53].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[54].globalType == GlobalTokenType::_LITTERAL   && tokens[54].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[55].globalType == GlobalTokenType::_DELIMITOR  && tokens[55].type == TokenType::_SEMI_COLON);

    // Test_Parameters(value);
    CompilationTest::Assert(tokens[56].globalType == GlobalTokenType::_IDENTIFIER && tokens[56].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[57].globalType == GlobalTokenType::_DELIMITOR  && tokens[57].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[58].globalType == GlobalTokenType::_IDENTIFIER && tokens[58].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[59].globalType == GlobalTokenType::_DELIMITOR  && tokens[59].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[60].globalType == GlobalTokenType::_DELIMITOR  && tokens[60].type == TokenType::_SEMI_COLON);

    // value = Test_Parameters_Return(value);
    CompilationTest::Assert(tokens[61].globalType == GlobalTokenType::_IDENTIFIER && tokens[61].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[62].globalType == GlobalTokenType::_OPERAND    && tokens[62].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[63].globalType == GlobalTokenType::_IDENTIFIER && tokens[63].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[64].globalType == GlobalTokenType::_DELIMITOR  && tokens[64].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[65].globalType == GlobalTokenType::_IDENTIFIER && tokens[65].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[66].globalType == GlobalTokenType::_DELIMITOR  && tokens[66].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[67].globalType == GlobalTokenType::_DELIMITOR  && tokens[67].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[68].globalType == GlobalTokenType::_DELIMITOR  && tokens[68].type == TokenType::_R_BRACE);
}

void LexerTest::TestLoop()
{
    Lexer lex;
    CompilationTest::Assert(lex.ReadFile("PipelineTest/Loop.chessy") == true, "Loop.chessy can't be read");
    lex.Tokenize();
    vector<Token>& tokens = lex.GetTokens();

    // func main() {
    CompilationTest::Assert(tokens[0].globalType == GlobalTokenType::_KEYWORD    && tokens[0].type == TokenType::_FUNC);
    CompilationTest::Assert(tokens[1].globalType == GlobalTokenType::_IDENTIFIER && tokens[1].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[2].globalType == GlobalTokenType::_DELIMITOR  && tokens[2].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[3].globalType == GlobalTokenType::_DELIMITOR  && tokens[3].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[4].globalType == GlobalTokenType::_DELIMITOR  && tokens[4].type == TokenType::_L_BRACE);

    // i8 test = 1;
    CompilationTest::Assert(tokens[5].globalType  == GlobalTokenType::_IDENTIFIER && tokens[5].type  == TokenType::_I8);
    CompilationTest::Assert(tokens[6].globalType  == GlobalTokenType::_IDENTIFIER && tokens[6].type  == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[7].globalType  == GlobalTokenType::_OPERAND    && tokens[7].type  == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[8].globalType  == GlobalTokenType::_LITTERAL   && tokens[8].type  == TokenType::_VALUE);
    CompilationTest::Assert(tokens[9].globalType  == GlobalTokenType::_DELIMITOR  && tokens[9].type  == TokenType::_SEMI_COLON);

    // do {
    CompilationTest::Assert(tokens[10].globalType == GlobalTokenType::_KEYWORD    && tokens[10].type == TokenType::_DO);
    CompilationTest::Assert(tokens[11].globalType == GlobalTokenType::_DELIMITOR  && tokens[11].type == TokenType::_L_BRACE);

    // test = test + 1;
    CompilationTest::Assert(tokens[12].globalType == GlobalTokenType::_IDENTIFIER && tokens[12].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[13].globalType == GlobalTokenType::_OPERAND    && tokens[13].type == TokenType::_EQUAL);
    CompilationTest::Assert(tokens[14].globalType == GlobalTokenType::_IDENTIFIER && tokens[14].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[15].globalType == GlobalTokenType::_OPERAND    && tokens[15].type == TokenType::_ADD);
    CompilationTest::Assert(tokens[16].globalType == GlobalTokenType::_LITTERAL   && tokens[16].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[17].globalType == GlobalTokenType::_DELIMITOR  && tokens[17].type == TokenType::_SEMI_COLON);

    // } while(test < 5)
    CompilationTest::Assert(tokens[18].globalType == GlobalTokenType::_DELIMITOR  && tokens[18].type == TokenType::_R_BRACE);
    CompilationTest::Assert(tokens[19].globalType == GlobalTokenType::_KEYWORD    && tokens[19].type == TokenType::_WHILE);
    CompilationTest::Assert(tokens[20].globalType == GlobalTokenType::_DELIMITOR  && tokens[20].type == TokenType::_L_PARENTHESE);
    CompilationTest::Assert(tokens[21].globalType == GlobalTokenType::_IDENTIFIER && tokens[21].type == TokenType::_SYMBOL);
    CompilationTest::Assert(tokens[22].globalType == GlobalTokenType::_OPERAND    && tokens[22].type == TokenType::_LOWER_THAN);
    CompilationTest::Assert(tokens[23].globalType == GlobalTokenType::_LITTERAL   && tokens[23].type == TokenType::_VALUE);
    CompilationTest::Assert(tokens[24].globalType == GlobalTokenType::_DELIMITOR  && tokens[24].type == TokenType::_R_PARENTHESE);
    CompilationTest::Assert(tokens[25].globalType == GlobalTokenType::_DELIMITOR  && tokens[25].type == TokenType::_SEMI_COLON);

    // }
    CompilationTest::Assert(tokens[26].globalType == GlobalTokenType::_DELIMITOR  && tokens[26].type == TokenType::_R_BRACE);
}