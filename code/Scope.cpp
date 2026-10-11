#include "Scope.h"

Scope::Scope(Scope* parent) : m_parent(parent)
{
    if (m_parent != nullptr)
        parent->m_children.insert(this);
}

Scope::~Scope()
{
    for (auto& it : m_symbols) 
    {
        delete it.second; 
    }

    while (!m_children.empty()) // should not happen
    {
        delete *m_children.begin();
    }

    if (m_parent == nullptr)
        return;
    m_parent->m_children.erase(this);
}

bool Scope::HasSymbol(str_view name)
{
    return m_symbols.count(name) || (m_parent != nullptr && m_parent->HasSymbol(name));
}

Symbol* Scope::GetSymbol(str_view name)
{
    auto symbol = m_symbols.find(name);
    if (symbol != m_symbols.end())
        return symbol->second;
        
    if (m_parent != nullptr)
        return m_parent->GetSymbol(name);
    return nullptr;
}

Symbol *Scope::Declare(str_view name, Symbol *sym)
{
    if (HasSymbol(name))
        return nullptr;

    sym->m_name  = name;            
    sym->m_scope = this;
    m_symbols.emplace(name, sym);
    return sym;
}