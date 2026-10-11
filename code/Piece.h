#pragma once
#include "Defines.h"

enum class Color : u8
{
    White = 0,
    Black = 1,
    Count = 2
};

inline Color OppositeColor(Color color) { return static_cast<Color>(1 - static_cast<u8>(color)); }

enum class PieceType : u8
{
    Pawn = 0,
    Knight = 1,
    Bishop = 2,
    Rook = 3,
    Queen = 4,
    King = 5,
    None = 6,
    Count = 6
};

enum class Piece : u8
{
    WhitePawn = 0, WhiteKnight, WhiteBishop, WhiteRook, WhiteQueen, WhiteKing,
    BlackPawn,     BlackKnight, BlackBishop, BlackRook, BlackQueen, BlackKing,
    None = 12
};

inline Color ColorOfPiece(Piece piece) { return static_cast<Color>(static_cast<u8>(piece) / 6); }
inline PieceType TypeOfPiece(Piece piece) { return static_cast<PieceType>(static_cast<u8>(piece) % 6); }
inline Piece MakePiece(Color color, PieceType type) { return static_cast<Piece>(static_cast<u8>(color) * 6 + static_cast<u8>(type)); }

inline bool operator==(const Piece& a, const Piece& b) { return static_cast<u8>(a) == static_cast<u8>(b); }
inline bool operator!=(const Piece& a, const Piece& b) { return !(static_cast<u8>(a) == static_cast<u8>(b)); }