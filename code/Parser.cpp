#include "Parser.h"

un_set<TokenType> Parser::s_typeTokens = {
    TokenType::_BOOL, TokenType::_STRING_IDENTIFIER,
    TokenType::_I8, TokenType::_I16, TokenType::_I32, TokenType::_I64,
    TokenType::_U8, TokenType::_U16, TokenType::_U32, TokenType::_U64,
    TokenType::_I32x8, TokenType::_I64x4,
    TokenType::_BITBOARD, TokenType::_PIECE,
    TokenType::_SQUARE, TokenType::_MOVE,
    TokenType::_BOARD,
};

un_set<TokenType> Parser::s_lineTokens = {
    TokenType::_END_LINE, TokenType::_CR, TokenType::_LF
};

Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::Parse(std::vector<Token>& tokens)
{
	m_tokens = tokens;
    try
    {
        m_program = ParseProgram();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Parse error: " << e.what() << std::endl;
    }
}

ProgramExpression* Parser::GetProgram()
{
    return m_program;
}

Token& Parser::Consume(TokenType type, bool jumpLineToken)
{
    if (m_tokenIndex >= static_cast<int>(m_tokens.size()))
        throw std::runtime_error("token out of range parser.Consume()");

    Token& t = m_tokens[m_tokenIndex];

    if (jumpLineToken && s_lineTokens.count(t.type))
    {
        m_tokenIndex++;
        return Consume(type,true);
    }

    if (t.type != type)
        throw std::runtime_error("unexpected token type got : " + std::to_string((int)t.type) + " expected : " + std::to_string((int)type) + " line : " + std::to_string(t.line) + " column : " + std::to_string(t.column));
    m_tokenIndex++;
    return t;
}

Token& Parser::Peek(int indexOffset, bool jumpLineToken)
{
    int i = m_tokenIndex + indexOffset;

    while (jumpLineToken && i < static_cast<int>(m_tokens.size()) && s_lineTokens.count(m_tokens[i].type))
    {
        i++;
    }

    if (i >= static_cast<int>(m_tokens.size()))
        throw std::runtime_error("token out of range in parser.Peek()");

    return m_tokens[i];
}

ProgramExpression* Parser::ParseProgram()
{
    ProgramExpression* programExp = new ProgramExpression();
    programExp->SetLineCol(0,0);
    while (m_tokenIndex < m_tokens.size())
    {
        Expression* exp = ParseDeclaration();
        programExp->content.push_back(exp);
    }
    return programExp;
}

Expression* Parser::ParseDeclaration()
{
    Token& token = Peek();
    switch (token.type)
    {
    case TokenType::_FUNC: return ParseFunctionDeclaration();
    case TokenType::_COMMENTARY: return ParseComment();
    default:
    {
        Expression* expr = ParseVariableDeclaration();
        Consume(TokenType::_SEMI_COLON);
        return expr;
    }
    }
}

BlockExpression* Parser::ParseBlock()
{
    Token& t = Consume(TokenType::_L_BRACE);

    BlockExpression* block = new BlockExpression();
    block->SetLineCol(t.line,t.column);

    while (Peek().type != TokenType::_R_BRACE)
    {
        Expression* expr = ParseExpression();
        
        if (expr->type != ExpressionType::_IF && expr->type != ExpressionType::_COMMENT)
            Consume(TokenType::_SEMI_COLON);

        block->content.push_back(expr);
    }

    Consume(TokenType::_R_BRACE);
    return block;
}

FunctionDeclarationExpression* Parser::ParseFunctionDeclaration()
{
    FunctionDeclarationExpression* func = new FunctionDeclarationExpression();
    Token& t = Consume(TokenType::_FUNC);
    func->SetLineCol(t.line,t.column);

    func->functionName = new IdentifierExpression(Consume(TokenType::_SYMBOL));

    Consume(TokenType::_L_PARENTHESE);

    if (Peek().type != TokenType::_R_PARENTHESE)
    {
        func->parameters.push_back(ParseParameter());
        while (Peek().type == TokenType::_COMMA)
        {
            Consume(TokenType::_COMMA);
            func->parameters.push_back(ParseParameter());
        }
    }

    Consume(TokenType::_R_PARENTHESE);

    if (Peek().type == TokenType::_COLON)
    {
        Consume(TokenType::_COLON);
        func->returnType = ParseType();
    }

    func->content = ParseBlock();

    return func;
}

FunctionCallExpression* Parser::ParseFunctionCall()
{
    FunctionCallExpression* call = new FunctionCallExpression();

    Token& t = Consume(TokenType::_SYMBOL);
    call->SetLineCol(t.line,t.column);

    call->name = new IdentifierExpression(t);

    Consume(TokenType::_L_PARENTHESE);

    if (Peek().type != TokenType::_R_PARENTHESE)
    {
        call->arguments.push_back(ParseExpression());
        while (Peek().type == TokenType::_COMMA)
        {
            Consume(TokenType::_COMMA);
            call->arguments.push_back(ParseExpression());
        }
    }

    Consume(TokenType::_R_PARENTHESE);

    return call;
}

ParameterExpression* Parser::ParseParameter() 
{
    ParameterExpression* param = new ParameterExpression();
    param->paramType = ParseType();
    Token& t = Consume(TokenType::_SYMBOL);
    param->SetLineCol(t.line,t.column);
    param->name = new IdentifierExpression(t);
    return param;
}

VariableDeclarationExpression* Parser::ParseVariableDeclaration()
{
    VariableDeclarationExpression* variableDec = new VariableDeclarationExpression();
    variableDec->varType = ParseType();
    Token& t = Consume(TokenType::_SYMBOL);
    variableDec->SetLineCol(t.line,t.column);
    variableDec->name = new IdentifierExpression(t);
    if (Peek().type == TokenType::_EQUAL)
    {
        Consume(TokenType::_EQUAL);
        variableDec->value = ParseExpression();
    }

    return variableDec;
}

VariableAssignmentExpression *Parser::ParseVariableAssignment()
{
    VariableAssignmentExpression* variableAssign = new VariableAssignmentExpression();
    Token& t = Consume(TokenType::_SYMBOL);
    variableAssign->SetLineCol(t.line,t.column);
    variableAssign->name = new IdentifierExpression(t);

    Consume(TokenType::_EQUAL);
    variableAssign->value = ParseExpression();

    return variableAssign;
}

TypeExpression* Parser::ParseType()
{
    Token& type = Peek();
    if (s_typeTokens.count(type.type) == 0)
        throw std::runtime_error("unexpected token type got : " + std::string(type.view) + " which is not a valid type");

    TypeExpression* typeExpr = new TypeExpression(type);
    Consume(type.type);

    return typeExpr;
}

ReturnExpression* Parser::ParseReturn()
{
    ReturnExpression* ret = new ReturnExpression();
    Token& t = Consume(TokenType::_RETURN);
    ret->SetLineCol(t.line,t.column);
    if (Peek().type == TokenType::_SEMI_COLON)
        return ret;

    ret->value = ParseExpression();
    return ret;
}

DoWhileExpression* Parser::ParseDoWhile()
{
    DoWhileExpression* doWhile = new DoWhileExpression();
    Token& t = Consume(TokenType::_DO);
    doWhile->SetLineCol(t.line,t.column);
    doWhile->block = ParseBlock();
    Consume(TokenType::_WHILE);
    Consume(TokenType::_L_PARENTHESE);
    doWhile->condition = ParseExpression();
    Consume(TokenType::_R_PARENTHESE);
    return doWhile;
}

IfExpression* Parser::ParseIf()
{
    IfExpression* ifExpression = new IfExpression();
    Token& t = Consume(TokenType::_IF);
    ifExpression->SetLineCol(t.line,t.column);
    Consume(TokenType::_L_PARENTHESE);
    ifExpression->condition = ParseExpression();
    Consume(TokenType::_R_PARENTHESE);
    ifExpression->block = ParseBlock();

    return ifExpression;
}

CommentExpression* Parser::ParseComment()
{
    CommentExpression* comment = new CommentExpression();
    comment->comment = "";

    Token& t = Consume(TokenType::_COMMENTARY);
    comment->SetLineCol(t.line,t.column);

    while (Peek().type != TokenType::_END_LINE)
    {
        Token& tok = Peek();
        comment->comment += std::string(tok.view);
        Consume(tok.type);
    }

    Consume(TokenType::_END_LINE);

    return comment;
}

Expression* Parser::ParseExpression(int minLeftBindingPower)
{
    
    Expression* left = ParsePrefix();
    while (m_tokenIndex < static_cast<int>(m_tokens.size()))
    {
        Token& token = m_tokens[m_tokenIndex];
        BindingPower bindingPower = GetBindingPower(token);
        if (bindingPower.left < minLeftBindingPower || bindingPower.left == 0)
            break;
        m_tokenIndex++;
        Expression* right = ParseExpression(bindingPower.right);
        left = new BinaryExpression(token, left, right);
    }
    return left;
}

Expression* Parser::ParsePrefix()
{
    Token& token = Peek();

    if (token.globalType == GlobalTokenType::_LITTERAL)
    {
        Consume(token.type);
        return new LiteralExpression(token);
    }

    if (token.globalType == GlobalTokenType::_KEYWORD && (token.type == TokenType::_TRUE || token.type == TokenType::_FALSE))
    {
        Consume(token.type);
        return new LiteralExpression(token);
    }

    if (token.type == TokenType::_SYMBOL)
    {
        if (Peek(1).type == TokenType::_L_PARENTHESE)
            return ParseFunctionCall();

        if (Peek(1).type == TokenType::_EQUAL)
            return ParseVariableAssignment();

        Consume(TokenType::_SYMBOL);
        return new IdentifierExpression(token);
    }

    if (token.type == TokenType::_L_PARENTHESE)
    {
        Consume(TokenType::_L_PARENTHESE);
        Expression* inner = ParseExpression();
        Consume(TokenType::_R_PARENTHESE);
        return inner;
    }

    if (token.type == TokenType::_DO)
    {
        return ParseDoWhile();
    }

    if (token.type == TokenType::_COMMENTARY)
    {
        return ParseComment();
    }

    if (token.type == TokenType::_IF)
    {
        return ParseIf();
    }

    if (s_typeTokens.count(token.type) && Peek(1).type == TokenType::_SYMBOL)
    {
        return ParseVariableDeclaration();
    }

    if (token.type == TokenType::_RETURN)
    {
        return ParseReturn();
    }

    if (token.type == TokenType::_NOT_OP || token.type == TokenType::_SUB || token.type == TokenType::_ADD)
    {
        Consume(token.type);
        Expression* operand = ParseExpression(20);
        return new UnaryExpression(token, operand);
    }

    throw std::runtime_error("parser couldn't parse token " + std::string(token.view) + " as expression");
}

BindingPower Parser::GetBindingPower(Token& token)
{
    switch (token.globalType)
    {
    case GlobalTokenType::_OPERAND:
        switch (token.type)
        {
        case TokenType::_EQUAL:            return { 3, 3 };    
        case TokenType::_OR_OP:            return { 4, 5 };    
        case TokenType::_AND_OP:           return { 5, 6 };    
        case TokenType::_OR_BINARY:        return { 6, 7 };    
        case TokenType::_XOR_BINARY:       return { 7, 8 };    
        case TokenType::_AND_BINARY:       return { 8, 9 };    
        case TokenType::_EQUAL_OP:
        case TokenType::_NOT_EQUAL_OP:     return { 9, 10 };   
        case TokenType::_GREATER_THAN:
        case TokenType::_GREATER_EQ_THAN:
        case TokenType::_LOWER_THAN:
        case TokenType::_LOWER_EQ_THAN:    return { 10, 11 };  
        case TokenType::_L_SHIFT:
        case TokenType::_R_SHIFT:          return { 11, 12 };  
        case TokenType::_ADD:
        case TokenType::_SUB:              return { 12, 13 };  
        case TokenType::_MUL:
        case TokenType::_DIV:
        case TokenType::_MOD:              return { 13, 14 };  
        case TokenType::_POW:              return { 14, 14 };  
        case TokenType::_NOT_OP:           return { 15, 15 };  
        default:                           return { 0, 0 };
        }

    case GlobalTokenType::_DELIMITOR:
        switch (token.type)
        {
        case TokenType::_L_PARENTHESE: return { 18, 0 };
        default:                       return { 0, 0 };
        }

    default:
        return { 0, 0 };
    }
}
