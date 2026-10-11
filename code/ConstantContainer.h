#pragma once
#include "Defines.h"
#include "Bitboard.h"
#include "Piece.h"
#include "Square.h"
#include "Move.h"
#include "Board.h"

struct VectorI32x8 { i32 lanes[8]; };
struct VectorI64x4 { i64 lanes[4]; };

enum class ConstantType : u8
{
    _BOOL,
    _I8, _I16, _I32, _I64,
    _U8, _U16, _U32, _U64,
    _I32x8, _I64x4,
    _STRING,
    _BITBOARD, _PIECE,
    _SQUARE, _MOVE,

    _COUNT
};

struct ConstantContainer
{
public:
    ConstantType type;
    union
    {
        bool as_bool;

        i8 as_i8;
        i16 as_i16;
        i32 as_i32;
        i64 as_i64;

        u8 as_u8;
        u16 as_u16;
        u32 as_u32;
        u64 as_u64;

        VectorI32x8 as_i32x8;
        VectorI64x4 as_i64x4;

        str* as_str;

        Bitboard as_bitboard;
        Piece as_piece;
        Square as_square;
        Move as_move;
    } data;
};