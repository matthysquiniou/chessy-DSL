#pragma once
#include "Defines.h"
#include "Token.h"

class Lexer
{
private:
    static un_map<str, TokenType> s_Identifier;
    static un_map<str, TokenType> s_Keyword;

private:
    str m_File;
    vector<Token> m_Tokens;

    int m_Start;
    int m_Current;
    u32 m_Line;
    u32 m_Column;

public:
    Lexer() = default;
    ~Lexer() = default;

    bool ReadFile(str _path);
    void Tokenize();

    vector<Token>& GetTokens() { return m_Tokens; }

private:
    void ScanToken();
    void AddToken(GlobalTokenType globalType, TokenType type);

    bool IsAtEnd() { return m_Current == m_File.size(); }
    bool Match(char expected);
    char& NextChar();
    char Peek();

    void String();
    void Number();
    void Symbol();
};