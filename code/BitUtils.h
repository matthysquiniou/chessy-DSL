#pragma once
#include "Defines.h"

#if defined(_MSC_VER)
#include <intrin.h>

inline u32 PopCount(u64 value)
{
#if defined(_M_X64) || defined(_M_ARM64)
    return static_cast<u32>(__popcnt64(value));
#else
    return static_cast<u32>(__popcnt(static_cast<u32>(value)))
         + static_cast<u32>(__popcnt(static_cast<u32>(value >> 32)));
#endif
}

inline u32 CountTrailingZeros(u64 value)
{
#if defined(_M_X64) || defined(_M_ARM64)
    unsigned long index;
    _BitScanForward64(&index, value);
    return static_cast<u32>(index);
#else
    unsigned long index;
    u32 low = static_cast<u32>(value);
    if (_BitScanForward(&index, low))
    {
        return static_cast<u32>(index);
    }
    u32 high = static_cast<u32>(value >> 32);
    _BitScanForward(&index, high);
    return static_cast<u32>(index) + 32;
#endif
}

#else

inline u32 PopCount(u64 value)
{
    return static_cast<u32>(__builtin_popcountll(value));
}

inline u32 CountTrailingZeros(u64 value)
{
    return static_cast<u32>(__builtin_ctzll(value));
}

#endif