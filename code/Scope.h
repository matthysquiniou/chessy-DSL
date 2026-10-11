#pragma once
#include "Defines.h"
#include "Symbol.h"

class Scope
{
public:
    Scope(Scope* parent = nullptr);
    ~Scope();

    bool HasSymbol(str_view name);
    Symbol* GetSymbol(str_view name);
    Symbol* Declare(str_view name, Symbol* sym);
public:
    Scope* m_parent = nullptr;
    un_set<Scope*> m_children;
    un_map<str_view, Symbol*> m_symbols;
};