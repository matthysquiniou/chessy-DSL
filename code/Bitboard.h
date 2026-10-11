#pragma once
#include "Defines.h"

struct Bitboard
{
    u64 board;
};

inline bool operator==(const Bitboard& a, const Bitboard& b)
{
    return a.board == b.board;
}

inline bool operator!=(const Bitboard& a, const Bitboard& b)
{
    return !(a == b);
}
