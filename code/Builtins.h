#pragma once
#include "Defines.h"
#include "Token.h"
#include <vector>
#include <string>

struct BuiltinDesc
{
    const char* name;
    u8 argc;
    bool hasReturn;
    TokenType returnType;
    std::vector<TokenType> paramTypes;
};

inline const std::vector<BuiltinDesc>& GetBuiltins()
{
    static std::vector<BuiltinDesc> builtins = {
        { "Print",            1, false, TokenType::_DEFAULT,   { TokenType::_ANY } },
        { "NowMs",            0, true,  TokenType::_I64,       { } },

        { "PopCount",         1, true,  TokenType::_I32,       { TokenType::_BITBOARD } },
        { "Lsb",              1, true,  TokenType::_I32,       { TokenType::_BITBOARD } },
        { "Bit",              1, true,  TokenType::_BITBOARD,  { TokenType::_I32 } },
        { "BitTest",          2, true,  TokenType::_BOOL,      { TokenType::_BITBOARD, TokenType::_I32 } },

        { "SquareMake",       2, true,  TokenType::_SQUARE,  { TokenType::_I32, TokenType::_I32 } },
        { "SquareFile",       1, true,  TokenType::_I32,       { TokenType::_SQUARE } },
        { "SquareRank",       1, true,  TokenType::_I32,       { TokenType::_SQUARE } },

        { "KnightAttacks",    1, true,  TokenType::_BITBOARD,  { TokenType::_SQUARE } },
        { "KingAttacks",      1, true,  TokenType::_BITBOARD,  { TokenType::_SQUARE } },
        { "PawnAttacks",      2, true,  TokenType::_BITBOARD,  { TokenType::_SQUARE, TokenType::_I32 } },
        { "BishopAttacks",    2, true,  TokenType::_BITBOARD,  { TokenType::_SQUARE, TokenType::_BITBOARD } },
        { "RookAttacks",      2, true,  TokenType::_BITBOARD,  { TokenType::_SQUARE, TokenType::_BITBOARD } },
        { "QueenAttacks",     2, true,  TokenType::_BITBOARD,  { TokenType::_SQUARE, TokenType::_BITBOARD } },

        { "NewBoard",         0, false,  TokenType::_DEFAULT,     { } }, // won't create literal new board for now :) it's complicated :/
        { "SideToMove",       0, true,  TokenType::_I32,       { } },
        { "PieceAt",          1, true,  TokenType::_PIECE,     { TokenType::_SQUARE } },
        { "Pieces",           1, true,  TokenType::_BITBOARD,  { TokenType::_PIECE } },
        { "Occupancy",        1, true,  TokenType::_BITBOARD,  { TokenType::_I32 } },
        { "KingSquare",       1, true,  TokenType::_SQUARE,  { TokenType::_I32 } },

        { "SortCapturesFirst", 0, false, TokenType::_DEFAULT, { } },
        { "SortMovesMvvLva", 0, false, TokenType::_DEFAULT, { } },

        { "PrintBoard",        0, false, TokenType::_DEFAULT, { } },
        { "ReadInt",           0, true,  TokenType::_I32,     { } },
        { "ReadMove",          0, true,  TokenType::_MOVE,    { } },
        { "NullMove",          0, true,  TokenType::_MOVE,    { } },

        { "Generate",         0, false, TokenType::_DEFAULT,   { } },
        { "MoveCount",        0, true,  TokenType::_I32,       { } },
        { "MoveAt",           1, true,  TokenType::_MOVE,      { TokenType::_I32 } },
        { "MakeMove",         1, false, TokenType::_DEFAULT,   { TokenType::_MOVE } },
        { "UnmakeMove",       0, false, TokenType::_DEFAULT,   { } },

        { "InCheck",          1, true,  TokenType::_BOOL,      { TokenType::_I32 } },

        { "PieceWhitePawn",   0, true, TokenType::_PIECE, { } },
        { "PieceWhiteKnight", 0, true, TokenType::_PIECE, { } },
        { "PieceWhiteBishop", 0, true, TokenType::_PIECE, { } },
        { "PieceWhiteRook",   0, true, TokenType::_PIECE, { } },
        { "PieceWhiteQueen",  0, true, TokenType::_PIECE, { } },
        { "PieceWhiteKing",   0, true, TokenType::_PIECE, { } },
        { "PieceBlackPawn",   0, true, TokenType::_PIECE, { } },
        { "PieceBlackKnight", 0, true, TokenType::_PIECE, { } },
        { "PieceBlackBishop", 0, true, TokenType::_PIECE, { } },
        { "PieceBlackRook",   0, true, TokenType::_PIECE, { } },
        { "PieceBlackQueen",  0, true, TokenType::_PIECE, { } },
        { "PieceBlackKing",   0, true, TokenType::_PIECE, { } },

        { "GenerateCaptures", 0, false, TokenType::_DEFAULT, { } },

        { "VectorI32x8Make",      8, true,  TokenType::_I32x8, { TokenType::_I32, TokenType::_I32, TokenType::_I32, TokenType::_I32, TokenType::_I32, TokenType::_I32, TokenType::_I32, TokenType::_I32 } },
        { "VectorI64x4Make",      4, true,  TokenType::_I64x4, { TokenType::_I64, TokenType::_I64, TokenType::_I64, TokenType::_I64 } },
        { "VectorI32x8Broadcast", 1, true,  TokenType::_I32x8, { TokenType::_I32 } },
        { "VectorI64x4Broadcast", 1, true,  TokenType::_I64x4, { TokenType::_I64 } },

        { "VectorBBx4Make",   4, true,  TokenType::_I64x4,     { TokenType::_BITBOARD, TokenType::_BITBOARD, TokenType::_BITBOARD, TokenType::_BITBOARD } },
        { "VectorBBx4Get",    2, true,  TokenType::_BITBOARD,  { TokenType::_I64x4, TokenType::_I32 } },
        { "VectorBBx4And",    2, true,  TokenType::_I64x4,     { TokenType::_I64x4, TokenType::_I64x4 } },
        { "VectorBBx4Or",     2, true,  TokenType::_I64x4,     { TokenType::_I64x4, TokenType::_I64x4 } },
        { "VectorBBx4Xor",    2, true,  TokenType::_I64x4,     { TokenType::_I64x4, TokenType::_I64x4 } },
        { "PopCountBatch",    1, true,  TokenType::_I64x4,     { TokenType::_I64x4 } },
        { "VectorI64x4GetI32", 2, true, TokenType::_I32, { TokenType::_I64x4, TokenType::_I32 } },
    };
    return builtins;
}