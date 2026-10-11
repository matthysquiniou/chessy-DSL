#pragma once
#include "Token.h"
#include "ExpressionType.h"

struct Symbol;
struct VarSymbol;
struct FuncSymbol;

struct Expression
{
	virtual ~Expression() = default;
	ExpressionType type;
	bool hasError = false;
	int line = 0;
	int col = 0;

	void SetLineCol(int l, int c)
	{
		line = l;
		col = c;
	}
};

struct ProgramExpression : public Expression
{
	ProgramExpression()
	{
		type = ExpressionType::_PROGRAM;
	}
	~ProgramExpression() override
	{
		for (Expression* e : content) 
			delete e;
	}
	std::vector<Expression*> content;
};

struct IdentifierExpression : public Expression
{
	IdentifierExpression(Token& tok) : token(tok)
	{
		type = ExpressionType::_IDENTIFIER;
		SetLineCol(tok.line,tok.column);
	}
	Token token;
	Symbol* symbol = nullptr;
};

struct TypeExpression : public Expression
{
	TypeExpression(Token& tok) : token(tok)
	{
		type = ExpressionType::_TYPE;
		SetLineCol(tok.line,tok.column);
	}
	Token token;
};

struct LiteralExpression : public Expression
{
	LiteralExpression(Token& tok) : token(tok)
	{
		type = ExpressionType::_LITERAL;
		SetLineCol(tok.line,tok.column);
	}
	Token token;
};

struct UnaryExpression : public Expression
{
	UnaryExpression(Token& tok, Expression* l) : token(tok), left(l)
	{
		type = ExpressionType::_UNARY;
		SetLineCol(tok.line,tok.column);
	}
	~UnaryExpression() override { delete left; }
	Token token;
	Expression* left = nullptr;
};

struct BinaryExpression : public Expression
{
	BinaryExpression(Token& tok, Expression* l, Expression* r) : token(tok), left(l), right(r)
	{
		type = ExpressionType::_BINARY;
		SetLineCol(tok.line,tok.column);
	}
	~BinaryExpression() override { delete left; delete right; }
	Token token;
	Expression* left = nullptr;
	Expression* right = nullptr;
};

struct BlockExpression : public Expression
{
	BlockExpression()
	{
		type = ExpressionType::_BLOCK;
	}
	~BlockExpression() override
	{
		for (Expression* e : content) delete e;
	}
	std::vector<Expression*> content;
};

struct ParameterExpression : public Expression
{
	ParameterExpression()
	{
		type = ExpressionType::_PARAMETER;
	}
	~ParameterExpression() override { delete name; delete paramType; }
	IdentifierExpression* name = nullptr;
	TypeExpression* paramType = nullptr;
};

struct FunctionDeclarationExpression : public Expression
{
	FunctionDeclarationExpression()
	{
		type = ExpressionType::_FUNCTION_DECLARATION;
	}
	~FunctionDeclarationExpression() override
	{
		delete functionName;
		for (ParameterExpression* p : parameters) delete p;
		delete returnType;
		delete content;
	}
	IdentifierExpression* functionName = nullptr;
	std::vector<ParameterExpression*> parameters;
	TypeExpression* returnType = nullptr;
	BlockExpression* content = nullptr;
	FuncSymbol* symb = nullptr;	
};

struct FunctionCallExpression : public Expression
{
	FunctionCallExpression()
	{
		type = ExpressionType::_FUNCTION_CALL;
	}
	~FunctionCallExpression() override
	{
		delete name;
		for (Expression* e : arguments) delete e;
	}
	IdentifierExpression* name = nullptr;
	std::vector<Expression*> arguments;
	FuncSymbol* symb = nullptr;	
};

struct VariableDeclarationExpression : public Expression
{
	VariableDeclarationExpression()
	{
		type = ExpressionType::_VARIABLE_DECLARATION;
	}
	~VariableDeclarationExpression() override
	{
		delete varType;
		delete name;
		delete value;
	}
	TypeExpression* varType = nullptr;
	IdentifierExpression* name = nullptr;
	Expression* value = nullptr;
	VarSymbol* symbol = nullptr;
};

struct ReturnExpression : public Expression
{
	ReturnExpression()
	{
		type = ExpressionType::_RETURN;
	}
	~ReturnExpression() override { delete value; }
	Expression* value = nullptr;
};

struct DoWhileExpression : public Expression 
{
	DoWhileExpression()
	{
		type = ExpressionType::_DO_WHILE;
	}
	~DoWhileExpression()
	{
		delete block;
		delete condition;
	}
	BlockExpression* block;
	Expression* condition;
};

struct VariableAssignmentExpression : public Expression 
{
	VariableAssignmentExpression()
	{
		type = ExpressionType::_VARIABLE_ASSIGNMENT;
	}
	~VariableAssignmentExpression()
	{
		delete name;
		delete value;
	}
	IdentifierExpression* name;
	Expression* value;
	VarSymbol* symbol = nullptr;
};

struct IfExpression : public Expression
{
	IfExpression()
	{
		type = ExpressionType::_IF;
	}
	~IfExpression(){
		delete condition;
		delete block;
	}
	Expression* condition;
	BlockExpression* block;
};

struct CommentExpression : public Expression 
{
	CommentExpression()
	{
		type = ExpressionType::_COMMENT;
	}
	std::string comment;
};