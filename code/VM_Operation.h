#pragma once
#include "Defines.h"
#include "ConstantContainer.h"
#include "ConstantTrait.h"


template <ConstantType LType, ConstantType RType>
ConstantContainer Add(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Sub(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Mul(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Div(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Mod(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Pow(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType>
ConstantContainer Neg(const ConstantContainer& rLeft);


template <ConstantType LType, ConstantType RType>
ConstantContainer And(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Or(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Xor(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType>
ConstantContainer Not(const ConstantContainer& rLeft);

template <ConstantType LType, ConstantType RType>
ConstantContainer Shl(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer Shr(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer CmpEq(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer CmpNe(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer CmpLt(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer CmpLe(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer CmpGt(const ConstantContainer& rLeft, const ConstantContainer& rRight);

template <ConstantType LType, ConstantType RType>
ConstantContainer CmpGe(const ConstantContainer& rLeft, const ConstantContainer& rRight);


template <ConstantType LType>
void Print(const ConstantContainer& rLeft);


#include "VM_Operation.inl"