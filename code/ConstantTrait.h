#pragma once
#include "ConstantContainer.h"


// Allow to pass from Chessy types to C++ type (ex: i32 => int)
template <ConstantType T> struct ConstantTrait;

#define REGISTER_TYPE(ConstType, CppType, MemberName)           \
template<> struct ConstantTrait<ConstType>                      \
{                                                               \
    using cpp = CppType;                                        \
                                                                \
    static CppType get(const ConstantContainer& rContainer)     \
    {                                                           \
        return rContainer.data.MemberName;                      \
    }                                                           \
                                                                \
    static ConstantContainer make(CppType val)                  \
    {                                                           \
        ConstantContainer container {};                         \
        container.type = ConstType;                             \
        container.data.MemberName = val;                        \
        return container;                                       \
    }                                                           \
}; 

///////////////////Definition of needed//////////////////////////
REGISTER_TYPE(ConstantType::_BOOL, bool, as_bool)

REGISTER_TYPE(ConstantType::_I8, i8, as_i8)
REGISTER_TYPE(ConstantType::_I16, i16, as_i16)
REGISTER_TYPE(ConstantType::_I32, i32, as_i32)
REGISTER_TYPE(ConstantType::_I64, i64, as_i64)

REGISTER_TYPE(ConstantType::_U8, u8, as_u8)
REGISTER_TYPE(ConstantType::_U16, u16, as_u16)
REGISTER_TYPE(ConstantType::_U32, u32, as_u32)
REGISTER_TYPE(ConstantType::_U64, u64, as_u64)
REGISTER_TYPE(ConstantType::_I32x8, VectorI32x8, as_i32x8)
REGISTER_TYPE(ConstantType::_I64x4, VectorI64x4, as_i64x4)

REGISTER_TYPE(ConstantType::_STRING, str*, as_str)

REGISTER_TYPE(ConstantType::_BITBOARD, Bitboard, as_bitboard)
REGISTER_TYPE(ConstantType::_PIECE, Piece, as_piece)
REGISTER_TYPE(ConstantType::_SQUARE, Square, as_square)
REGISTER_TYPE(ConstantType::_MOVE, Move, as_move)

/////////////////////////////////////////////////////////////////


// Allow to pass from C++ types to Chessy type (ex: int => i32)
template <typename T> struct ToEnum;

#define TO_ENUM(ConstType, CppType)                             \
template<> struct ToEnum<CppType>                               \
{                                                               \
    static constexpr ConstantType value = ConstType;            \
};

///////////////////Definition of needed//////////////////////////
TO_ENUM(ConstantType::_BOOL, bool)

TO_ENUM(ConstantType::_I8, i8)
TO_ENUM(ConstantType::_I16, i16)
TO_ENUM(ConstantType::_I32, i32)
TO_ENUM(ConstantType::_I64, i64)

TO_ENUM(ConstantType::_U8, u8)
TO_ENUM(ConstantType::_U16, u16)
TO_ENUM(ConstantType::_U32, u32)
TO_ENUM(ConstantType::_U64, u64)
TO_ENUM(ConstantType::_I32x8, VectorI32x8)
TO_ENUM(ConstantType::_I64x4, VectorI64x4)

TO_ENUM(ConstantType::_STRING, str*)

TO_ENUM(ConstantType::_BITBOARD, Bitboard)
TO_ENUM(ConstantType::_PIECE, Piece)
TO_ENUM(ConstantType::_SQUARE, Square)
TO_ENUM(ConstantType::_MOVE, Move)
/////////////////////////////////////////////////////////////////