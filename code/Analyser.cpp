#include "Analyser.h"
#include "ExpressionTrait.h"
#include "Scope.h"
#include "SymbolTrait.h"
#include "Builtins.h"

Analyser::Analyser(ProgramExpression *program) : m_program(program)
{
}

void Analyser::Analyse()
{
    GlobalAnalyse();
}

std::vector<AnalyserError> Analyser::GetErrors()
{
    return m_errors;
}

bool Analyser::HasErrors()
{
    return !m_errors.empty();
}

void Analyser::PrintErrors()
{
    for (size_t i = 0; i < m_errors.size(); i++)
    {
        AnalyserError error = m_errors[i];
        error.Print();
    }
}

void Analyser::GlobalAnalyse()
{
    m_scopes.push_back(new Scope(nullptr));
    AddBuiltin();
    CollectGlobals();
    CheckMain();
    AnalyseBodies();
    PopScope();
}

void Analyser::CollectGlobals()
{
    if (m_program == nullptr)
        return;

    for (size_t i = 0; i < m_program->content.size(); i++)
    {
        Expression* expr = m_program->content[i];

        switch (expr->type)
        {
        case ExpressionType::_FUNCTION_DECLARATION:
        {
            FunctionDeclarationExpression* funcExpr = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(expr);
            FuncSymbol* funcSymbol = new FuncSymbol(); 
            if (m_scopes.back()->Declare(funcExpr->functionName->token.view, funcSymbol) == nullptr)
            {
                delete funcSymbol;
                m_errors.push_back({std::string(funcExpr->functionName->token.view) + " may have already been declared", funcExpr});
                funcExpr->hasError = true;
            }
            break;
        }
        case ExpressionType::_VARIABLE_DECLARATION:
        {
            VariableDeclarationExpression* varExpr = ExpressionCast<ExpressionType::_VARIABLE_DECLARATION>(expr);
            VarSymbol* varSymbol = new VarSymbol(varExpr->varType->token.type);
            varSymbol->m_slot = m_nextSlot++;
            varSymbol->isGlobal = true;
            if (m_scopes.back()->Declare(varExpr->name->token.view, varSymbol) == nullptr)
            {
                delete varSymbol;
                m_errors.push_back({std::string(varExpr->name->token.view) + " may have already been declared", varExpr});
                varExpr->hasError = true;
            }
            break;
        }
        default: break;
        }
    }
    
}

void Analyser::CheckMain()
{
    Symbol* sym = m_scopes.back()->GetSymbol("Main");
    if (sym == nullptr)
    {
        m_errors.push_back({"Main function is not defined", nullptr});
        return;
    }
    FuncSymbol* fn = SymbolCast<SymbolType::_FUNC>(sym);
    if (fn == nullptr)
    {
        m_errors.push_back({"Main is not a function", nullptr});
        return;
    }
    if (!fn->paramsType.empty())
        m_errors.push_back({"Main must not take arguments", nullptr});
    if (fn->returnType != TokenType::_DEFAULT)
        m_errors.push_back({"Main must return nothing", nullptr});
}

void Analyser::AnalyseBodies()
{
    if (m_program == nullptr)
        return;

    for (size_t i = 0; i < m_program->content.size(); i++)
    {
        Expression* expr = m_program->content[i];

        switch (expr->type)
        {
        case ExpressionType::_FUNCTION_DECLARATION:
            FunctionAnalyse(ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(expr)); break;
        case ExpressionType::_VARIABLE_DECLARATION:
            VarDeclarationAnalyse(ExpressionCast<ExpressionType::_VARIABLE_DECLARATION>(expr)); break;
        default: break;
        }
    }
}

void Analyser::AddBuiltin()
{
    const std::vector<BuiltinDesc>& builtins = GetBuiltins();
    for (size_t builtinIndex = 0; builtinIndex < builtins.size(); builtinIndex++)
    {
        const BuiltinDesc& desc = builtins[builtinIndex];
        FuncSymbol* funcSymbol = new FuncSymbol();
        funcSymbol->returnType = desc.hasReturn ? desc.returnType : TokenType::_DEFAULT;
        funcSymbol->paramsType = desc.paramTypes;
        funcSymbol->function_index = builtinIndex;
        funcSymbol->isBuiltin = true;
        funcSymbol->hasReturn = desc.hasReturn;

        if (m_scopes.back()->Declare(desc.name, funcSymbol) == nullptr)
        {
            delete funcSymbol;
            m_errors.push_back({ std::string("Builtin name collision: ") + desc.name, nullptr });
        }
    }
}

TokenType Analyser::AnalyseExpression(Expression *expr)
{
    if (expr == nullptr)
        return TokenType::_DEFAULT;

    switch (expr->type)
    {
        case ExpressionType::_BLOCK:                BlockAnalyse(ExpressionCast<ExpressionType::_BLOCK>(expr)); return TokenType::_DEFAULT;
        case ExpressionType::_RETURN:               ReturnAnalyse(ExpressionCast<ExpressionType::_RETURN>(expr));  return TokenType::_DEFAULT;
        case ExpressionType::_DO_WHILE:             DoWhileAnalyse(ExpressionCast<ExpressionType::_DO_WHILE>(expr));  return TokenType::_DEFAULT;
        case ExpressionType::_IF:                   IfAnalyse(ExpressionCast<ExpressionType::_IF>(expr));  return TokenType::_DEFAULT;
        case ExpressionType::_FUNCTION_DECLARATION: FunctionAnalyse(ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(expr));  return TokenType::_DEFAULT;
        case ExpressionType::_PARAMETER:            return ParameterAnalyse(ExpressionCast<ExpressionType::_PARAMETER>(expr));
        case ExpressionType::_FUNCTION_CALL:        return FunctionCallAnalyse(ExpressionCast<ExpressionType::_FUNCTION_CALL>(expr));
        case ExpressionType::_VARIABLE_DECLARATION: VarDeclarationAnalyse(ExpressionCast<ExpressionType::_VARIABLE_DECLARATION>(expr));  return TokenType::_DEFAULT;
        case ExpressionType::_VARIABLE_ASSIGNMENT:  VarAssignmentAnalyse(ExpressionCast<ExpressionType::_VARIABLE_ASSIGNMENT>(expr));  return TokenType::_DEFAULT;
        case ExpressionType::_IDENTIFIER:           return IdentifierAnalyse(ExpressionCast<ExpressionType::_IDENTIFIER>(expr));
        case ExpressionType::_TYPE:                 TypeAnalyse(ExpressionCast<ExpressionType::_TYPE>(expr));  return TokenType::_DEFAULT;
        case ExpressionType::_LITERAL:              return LiteralAnalyse(ExpressionCast<ExpressionType::_LITERAL>(expr));
        case ExpressionType::_UNARY:                return UnaryAnalyse(ExpressionCast<ExpressionType::_UNARY>(expr));
        case ExpressionType::_BINARY:               return BinaryAnalyse(ExpressionCast<ExpressionType::_BINARY>(expr));
        case ExpressionType::_COMMENT:              CommentAnalyse(ExpressionCast<ExpressionType::_COMMENT>(expr)); return TokenType::_DEFAULT;

        default:
            return TokenType::_DEFAULT;
    }      
    return TokenType::_DEFAULT;
}

void Analyser::BlockAnalyse(BlockExpression *block)
{
    AddScope();

    for (size_t i = 0; i < block->content.size(); i++)
    {
        Expression* expr = block->content[i];
        AnalyseExpression(expr);
    }

    PopScope();
}

void Analyser::ReturnAnalyse(ReturnExpression *ret)
{
    if (m_currentFunc == nullptr)
    {
        m_errors.push_back({"return out of function",ret});
        ret->hasError = true;
        return;
    }
    
    if (ret->value == nullptr && m_currentFunc->returnType == nullptr)
        return;

    TokenType retType = AnalyseExpression(ret->value);
    if (!IsAssignable(retType, m_currentFunc->returnType->token.type))
    {
        m_errors.push_back({"return statement and function return type don't match on " + std::string(m_currentFunc->functionName->token.view), ret});
        ret->hasError = true;
    }
        
}

void Analyser::DoWhileAnalyse(DoWhileExpression *doWhile)
{
    BlockAnalyse(ExpressionCast<ExpressionType::_BLOCK>(doWhile->block));
    AnalyseExpression(doWhile->condition);
}

void Analyser::IfAnalyse(IfExpression *ifStmt)
{
    AnalyseExpression(ifStmt->condition);
    BlockAnalyse(ExpressionCast<ExpressionType::_BLOCK>(ifStmt->block));
}

void Analyser::FunctionAnalyse(FunctionDeclarationExpression *func)
{
    FuncSymbol* fnSmb;
    if (m_scopes.size() == 1)
    {
        fnSmb = SymbolCast<SymbolType::_FUNC>(m_scopes.back()->GetSymbol(func->functionName->token.view));
        if (fnSmb == nullptr)
        {
            m_errors.push_back({std::string(func->functionName->token.view) + " is not a function", func});
            func->hasError = true;
            return;
        }
    }
    else
    {
        m_errors.push_back({std::string(func->functionName->token.view) + " can't be declared inside another function", func});
        func->hasError = true;
        return;
    }

    AddScope(func);
    m_nextSlot++; 
    
    for (size_t i = 0; i < func->parameters.size(); i++)
    {
        ParameterExpression* param = func->parameters[i];
        TokenType paramType = ParameterAnalyse(param);
        fnSmb->paramsType.push_back(paramType);
    }

    if (func->returnType != nullptr)
    {
        fnSmb->returnType = func->returnType->token.type;
    }

    for (size_t i = 0; i < func->content->content.size(); i++)
    {
        Expression* expr = func->content->content[i];
        AnalyseExpression(expr);
    }
    
    fnSmb->slot_count = m_nextSlot;
    func->symb = fnSmb;

    PopScope(true);
}

TokenType Analyser::ParameterAnalyse(ParameterExpression *param)
{
    VarSymbol* sym = new VarSymbol(param->paramType->token.type);
    sym->m_slot = m_nextSlot++;
    if(m_scopes.back()->Declare(param->name->token.view, sym) == nullptr)
    {
        delete sym;
        m_errors.push_back({std::string(param->name->token.view) + " may have already been declared", param});
        param->hasError = true;
        return TokenType::_DEFAULT;
    }

    return param->paramType->token.type;
}

TokenType Analyser::FunctionCallAnalyse(FunctionCallExpression *call)
{
    Symbol* sym = m_scopes.back()->GetSymbol(call->name->token.view);
    if (sym == nullptr)
    {
        m_errors.push_back({"function " + std::string(call->name->token.view) + " could not be found", call});
        call->hasError = true;
        return TokenType::_DEFAULT;
    }

    FuncSymbol* fnSmb = SymbolCast<SymbolType::_FUNC>(sym);
    if (fnSmb == nullptr) {
        m_errors.push_back({std::string(call->name->token.view) + " is not a function", call});
        call->hasError = true;
        return TokenType::_DEFAULT;
    }
    
    if (call->arguments.size() != fnSmb->paramsType.size())
    {
        m_errors.push_back({"function " + std::string(call->name->token.view) + " expect" + std::to_string(fnSmb->paramsType.size()) + ", " + std::to_string(call->arguments.size()) + " were given", call});
        call->hasError = true;
        return TokenType::_DEFAULT;
    }

    for (size_t i = 0; i < call->arguments.size(); i++)
    {
        Expression* expr = call->arguments[i];
        TokenType argType = AnalyseExpression(expr);
        TokenType paramType = fnSmb->paramsType[i];

        if (paramType == TokenType::_ANY)
            continue; 

        if (!IsAssignable(argType, paramType))
        {
            m_errors.push_back({"function call arguments does not match type of " + std::string(fnSmb->m_name) + " parameters", call});
            call->hasError = true;
            return TokenType::_DEFAULT;
        }
    }
    
    call->symb = fnSmb;
    return fnSmb->returnType;
}

void Analyser::VarDeclarationAnalyse(VariableDeclarationExpression *varDec)
{
    VarSymbol* varSmb;
    if (m_scopes.size() == 1)
    {
        Symbol* smb = m_scopes.back()->GetSymbol(varDec->name->token.view);
        varSmb = SymbolCast<SymbolType::_VAR>(smb);
        if (varSmb == nullptr)
        {
            m_errors.push_back({std::string(varDec->name->token.view) + " is not a variable", varDec});
            varDec->hasError = true;
            return;
        }
    }
    else
    {
        varSmb = new VarSymbol(varDec->varType->token.type);
        varSmb->m_slot = m_nextSlot++;
        if (m_scopes.back()->Declare(varDec->name->token.view,varSmb) == nullptr)
        {
            delete varSmb;
            m_errors.push_back({std::string(varDec->name->token.view) + " may have already been declared", varDec});
            varDec->hasError = true;
            return;
        }
    }

    if (varDec->value != nullptr)
    {
        TokenType valueType = AnalyseExpression(varDec->value);
        if (!IsAssignable(valueType, varDec->varType->token.type))
        {
            m_errors.push_back({"could not assign value to " + std::string(varSmb->m_name) + " because of type difference " + std::to_string((int)valueType) + " and " + std::to_string((int)varSmb->varType), varDec});
            varDec->hasError = true;
            return;
        }
    }

    varDec->symbol = varSmb;
}

void Analyser::VarAssignmentAnalyse(VariableAssignmentExpression *varAssign)
{
    Symbol* smb = m_scopes.back()->GetSymbol(varAssign->name->token.view);
    if (smb == nullptr)
    {
        m_errors.push_back({"variable " + std::string(varAssign->name->token.view) + " has not been declared", varAssign});
        varAssign->hasError = true;
        return;
    }

    VarSymbol* varSmb = SymbolCast<SymbolType::_VAR>(smb);
    if (varSmb == nullptr)
    {
        m_errors.push_back({std::string(varAssign->name->token.view) + " is not a viariable", varAssign});
        varAssign->hasError = true;
        return;
    }

    TokenType varType = AnalyseExpression(varAssign->value);
    if (!IsAssignable(varType, varSmb->varType))
    {
        m_errors.push_back({"could not assign value to " + std::string(varSmb->m_name) + " because of type difference " + std::to_string((int)varType) + " and " + std::to_string((int)varSmb->varType), varAssign});
        varAssign->hasError = true;
        return;
    }

    varAssign->symbol = varSmb;
}

TokenType Analyser::IdentifierAnalyse(IdentifierExpression *id)
{
    Symbol* sym = m_scopes.back()->GetSymbol(id->token.view);
    if (sym == nullptr)
    {
        m_errors.push_back({"undefined Identifier " + std::string(id->token.view), id});
        id->hasError = true;
        return TokenType::_DEFAULT;
    }

    id->symbol = sym; 

    switch (sym->type)
    {
    case SymbolType::_FUNC:   
    {
        FuncSymbol* fnSmb = SymbolCast<SymbolType::_FUNC>(sym);
        return fnSmb->returnType;
    }
    case SymbolType::_VAR:
    {
        VarSymbol* varSmb = SymbolCast<SymbolType::_VAR>(sym);
        return varSmb->varType;
    }
    default:
        return TokenType::_DEFAULT;
    }
}

void Analyser::TypeAnalyse(TypeExpression *type)
{
}

TokenType Analyser::LiteralAnalyse(LiteralExpression *literal)
{
    return literal->token.type;
}

TokenType Analyser::UnaryAnalyse(UnaryExpression *unary)
{
    return AnalyseExpression(unary->left);
}

TokenType Analyser::BinaryAnalyse(BinaryExpression *binary)
{
    TokenType leftType = AnalyseExpression(binary->left);
    TokenType rightType = AnalyseExpression(binary->right);

    std::pair<bool,TokenType> typeInfer = TypeInference(leftType, rightType);
    if (typeInfer.first == false)
    {
        m_errors.push_back({"can't do operation between " + std::to_string((int)leftType) + " and " + std::to_string((int)rightType), binary});
        binary->hasError = true;
        return TokenType::_DEFAULT;
    }

    return typeInfer.second;
}

std::pair<bool,TokenType> Analyser::TypeInference(TokenType typeA, TokenType typeB)
{
    if (typeA > typeB)
    {
        std::swap(typeA,typeB);
    }

    static un_map<TokenType,vector<std::pair<TokenType,TokenType>>> typeInferenceMap =
    {
        {TokenType::_STRING_LITTERAL,
            {
                {TokenType::_STRING_LITTERAL, TokenType::_STRING_IDENTIFIER},
                {TokenType::_STRING_IDENTIFIER, TokenType::_STRING_IDENTIFIER},
            }
        },
        {TokenType::_VALUE,
            {
                {TokenType::_BOOL, TokenType::_BOOL},
                {TokenType::_I8, TokenType::_I8},
                {TokenType::_I16, TokenType::_I16},
                {TokenType::_I32, TokenType::_I32},
                {TokenType::_I64, TokenType::_I64},
                {TokenType::_U8, TokenType::_U8},
                {TokenType::_U16, TokenType::_U16},
                {TokenType::_U32, TokenType::_U32},
                {TokenType::_U64, TokenType::_U64},
            }
        },
        {TokenType::_I8,
            {
                {TokenType::_I8, TokenType::_I8},
                {TokenType::_I16, TokenType::_I16},
                {TokenType::_I32, TokenType::_I32},
                {TokenType::_I64, TokenType::_I64},
            }
        },
        {TokenType::_I16,
            {
                {TokenType::_I16, TokenType::_I16},
                {TokenType::_I32, TokenType::_I32},
                {TokenType::_I64, TokenType::_I64},
            }
        },
        {TokenType::_I32,
            {
                {TokenType::_I32, TokenType::_I32},
                {TokenType::_I64, TokenType::_I64},
            }
        },
        {TokenType::_I64,
            {
                {TokenType::_I64, TokenType::_I64},
            }
        },
        {TokenType::_U8,
            {
                {TokenType::_U8, TokenType::_U8},
                {TokenType::_U16, TokenType::_U16},
                {TokenType::_U32, TokenType::_U32},
                {TokenType::_U64, TokenType::_U64},
            }
        },
        {TokenType::_U16,
            {
                {TokenType::_U16, TokenType::_U16},
                {TokenType::_U32, TokenType::_U32},
                {TokenType::_U64, TokenType::_U64},
            }
        },
        {TokenType::_U32,
            {
                {TokenType::_U32, TokenType::_I32},
                {TokenType::_U64, TokenType::_U64},
            }
        },
        {TokenType::_U64,
            {
                {TokenType::_U64, TokenType::_U64},
            }
        },
        {TokenType::_BOOL,
            {
                {TokenType::_BOOL, TokenType::_BOOL},
            }
        },
        {TokenType::_BITBOARD,
            {
                {TokenType::_BITBOARD, TokenType::_BITBOARD},
            }
        },
        {TokenType::_PIECE,
            {
                {TokenType::_PIECE, TokenType::_PIECE},
            }
        },
        {TokenType::_SQUARE,
            {
                {TokenType::_SQUARE, TokenType::_SQUARE},
            }
        },
        {TokenType::_BOARD,
            {
                 {TokenType::_BOARD, TokenType::_BOARD}
            }
        },
        {TokenType::_MOVE,
            {
                {TokenType::_MOVE, TokenType::_MOVE},
            }
        },
        {TokenType::_I32x8,
            {
                {TokenType::_I32x8, TokenType::_I32x8},
            }
        },
        {TokenType::_I64x4,
            {
                {TokenType::_I64x4, TokenType::_I64x4},
            }
        },
    };

    auto it = typeInferenceMap.find(typeA);
    if (it == typeInferenceMap.end())
    {
        return {false, TokenType::_DEFAULT};
    }

    for (size_t i = 0; i < it->second.size(); i++)
    {
        std::pair<TokenType,TokenType>& pair = it->second[i];
        if (pair.first == typeB)
            return {true, pair.second};
    }

    return {false, TokenType::_DEFAULT};
    
}

void Analyser::CommentAnalyse(CommentExpression *comment)
{
}

void Analyser::AddScope(FunctionDeclarationExpression* func)
{
    Scope* newScope = new Scope(m_scopes.back());
    m_scopes.push_back(newScope);



    if (func != nullptr)
    {
        m_previousNextSlot.push_back(m_nextSlot);
        m_nextSlot = 0;

        m_previousFunc.push_back(m_currentFunc);
        m_currentFunc = func;
    }
}

void Analyser::PopScope(bool popFunc)
{
    m_scopes.pop_back();


    if (popFunc)
    {
        m_nextSlot = m_previousNextSlot.back();
        m_previousNextSlot.pop_back();

        m_currentFunc = m_previousFunc.back();
        m_previousFunc.pop_back();
    }
}

bool Analyser::IsAssignable(TokenType fromType, TokenType toType)
{
    if (fromType == toType)
    {
        return true;
    }

    static un_set<TokenType> numericTypes = {
        TokenType::_BOOL,
        TokenType::_I8, TokenType::_I16, TokenType::_I32, TokenType::_I64,
        TokenType::_U8, TokenType::_U16, TokenType::_U32, TokenType::_U64,
    };

    if (fromType == TokenType::_VALUE && numericTypes.count(toType))
    {
        return true;
    }

    if (fromType == TokenType::_STRING_LITTERAL && toType == TokenType::_STRING_IDENTIFIER)
    {
        return true;
    }

    return false;
}