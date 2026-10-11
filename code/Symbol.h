#pragma once
#include "Defines.h"
#include "SymbolType.h"
#include "Token.h"

class Scope;

struct Symbol
{
public:
    virtual ~Symbol() = default;
    SymbolType type;
    str_view m_name;
    Scope* m_scope = nullptr;
};

struct VarSymbol : public Symbol
{
public:
    int m_slot = -1;
    TokenType varType;
    bool isGlobal = false; 
    VarSymbol(TokenType vType) : varType(vType)
    {
        type = SymbolType::_VAR;
    }
};

struct FuncSymbol : public Symbol
{
public:
    int slot_count = 0;
    vector<TokenType> paramsType;
    TokenType returnType = TokenType::_DEFAULT;
    size function_index = -1;
    bool isBuiltin = false;
    bool hasReturn = true;
    FuncSymbol() 
    { 
        type = SymbolType::_FUNC;
    }
};