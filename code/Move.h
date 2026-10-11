#pragma once
#include "Defines.h"

enum MoveFlag : u8
{
    MoveFlagNone       = 0,
    MoveFlagCapture    = 1 << 0,
    MoveFlagEnPassant  = 1 << 1,
    MoveFlagCastle     = 1 << 2,
    MoveFlagPromotion  = 1 << 3
};

struct Move
{
    u8 from;
    u8 to;
    u8 flags;

    bool IsNull() const { return from == 0 && to == 0 && flags == 0; }
};

inline bool operator==(const Move& a, const Move& b) { return a.from == b.from && a.to == b.to && a.flags == b.flags; }
inline bool operator!=(const Move& a, const Move& b) { return !(a == b); }