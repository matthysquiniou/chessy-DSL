#pragma once
#include "Defines.h"
#include "ConstantContainer.h"
#include "VM_Operation.h"

using OperationPtr = ConstantContainer(*)(const ConstantContainer&, const ConstantContainer&);
using OperationPtrSolo = ConstantContainer(*)(const ConstantContainer&);
using NoReturnOperationPtrSolo = void(*)(const ConstantContainer&);

// Add operand
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateAddLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateAddTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Sub operand
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateSubLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateSubTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Mul type
template <size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateMulLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateMulTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Div type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateDivLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateDivTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Mod type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateModLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateModTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Pow type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreatePowLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreatePowTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Neg type
template<size... iSize>
constexpr array<OperationPtrSolo, sizeof...(iSize)> CreateNegTable(std::index_sequence<iSize...>);


// And type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateAndLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateAndTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Or type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateOrLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateOrTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Xor type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateXorLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateXorTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Not type
template<size... iSize>
constexpr array<OperationPtrSolo, sizeof...(iSize)> CreateNotTable(std::index_sequence<iSize...>);

// Shift-left type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateShlLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateShlTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Shift-right type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateShrLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateShrTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Is Equal type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpEqLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpEqTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Is Different type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpNeLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpNeTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Lesser Than type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpLtLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpLtTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Lesser or Equal type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpLeLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpLeTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Greater Than type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpGtLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpGtTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);

// Greater or Equal type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpGeLine(std::index_sequence<jSize...>);

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpGeTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js);


// Print type
template<size... iSize>
constexpr array<NoReturnOperationPtrSolo, sizeof...(iSize)> CreatePrintTable(std::index_sequence<iSize...>);

#include "VM_Table.inl"