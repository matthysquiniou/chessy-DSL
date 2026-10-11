#pragma once
#include "Defines.h"

struct Square
{
    u8 index;

    u8 File() const { return index & 7; }
    u8 Rank() const { return index >> 3; }
};

constexpr u8 NoSquare = 64;

inline Square MakeSquare(u8 file, u8 rank) { return Square{ static_cast<u8>(rank * 8 + file) }; }

inline bool operator==(const Square& a, const Square& b) { return a.index == b.index; }
inline bool operator!=(const Square& a, const Square& b) { return !(a == b); }