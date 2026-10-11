#pragma once
#include "Defines.h"

enum class GlobalTokenType : u8
{
    _LITTERAL,
    _IDENTIFIER,
    _OPERAND,
    _KEYWORD,
    _DELIMITOR    
};

enum class TokenType : u8
{
    // Litterals
    _VALUE,
    _STRING_LITTERAL,

    // Identifiers
    _SYMBOL,

    _BOOL,
    _I8, _I16, _I32, _I64,
    _U8, _U16, _U32, _U64,
    _I32x8, _I64x4,
    _STRING_IDENTIFIER,
    _BITBOARD, _PIECE,
    _SQUARE, _MOVE,
    _BOARD,

    // Operands
    _EQUAL,
    _ADD, _SUB, _MUL, _DIV, _MOD, _POW,
    _OR_BINARY, _AND_BINARY, _XOR_BINARY,
    _OR_OP, _AND_OP, _NOT_OP,
    _L_SHIFT, _R_SHIFT,
    _GREATER_THAN, _LOWER_THAN,
    _GREATER_EQ_THAN, _LOWER_EQ_THAN,
    _EQUAL_OP, _NOT_EQUAL_OP,

    // Keywords
    _DO, _WHILE,
    _IF,
    _FUNC, _RETURN,
    _TRUE, _FALSE,

    // Delimitors
    _SEMI_COLON,                    // ;
    _L_BRACE, _R_BRACE,             // {, }
    _L_PARENTHESE, _R_PARENTHESE,   // (, )
    _COLON,                         // :
    _COMMA,                         // ,
    _DOUBLE_QUOTE,                   // "
    _COMMENTARY,                    // //
    _CR, _LF,                       // \r, \n
    _END_LINE,                       // \r\n

    _ANY,
    _DEFAULT
};

struct Token
{
    str_view view;
    u32 line;
    u32 column;
    GlobalTokenType globalType;
    TokenType type;

    void Print();
};