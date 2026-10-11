#pragma once
#include "SymbolType.h"
#include "Symbol.h"

template <SymbolType K> struct SymbolTrait;
template <> struct SymbolTrait<SymbolType::_VAR>             { using type = VarSymbol; };
template <> struct SymbolTrait<SymbolType::_FUNC>            { using type = FuncSymbol; };

template <SymbolType K>
typename SymbolTrait<K>::type* SymbolCast(Symbol* e) {
    if (!e || e->type != K) return nullptr;
    return static_cast<typename SymbolTrait<K>::type*>(e);
}