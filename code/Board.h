#pragma once
#include "Defines.h"
#include "Piece.h"

enum CastlingRight : u8
{
    CastleWhiteKing  = 1 << 0,
    CastleWhiteQueen = 1 << 1,
    CastleBlackKing  = 1 << 2,
    CastleBlackQueen = 1 << 3
};

struct Board
{
    u64 pieces[12];
    u64 occupancy[2];
    u64 allOccupancy;
    Color sideToMove;
    u8 castlingRights;
    u8 enPassantSquare;
    u8 halfmoveClock;
    u16 fullmoveNumber;
};

struct BoardState
{
    Board board;
};