#include "Compiler.h"
#include "ExpressionTrait.h"
#include "Symbol.h"
#include "SymbolTrait.h"

Compiler::Compiler()
{
    m_chunk = Chunk();
}

void Compiler::Compile(ProgramExpression *program)
{
    m_program = program;
    CompileProgram();
    ResolveLabels();
}

Chunk &Compiler::GetChuck()
{
   return m_chunk;
}

void Compiler::CompileProgram()
{
    for (size_t i = 0; i < m_program->content.size(); i++)
    {
        Expression* expr = m_program->content[i];
        switch (expr->type)
        {
        case ExpressionType::_FUNCTION_DECLARATION:
        {
            FunctionDeclarationExpression* fnDecExpre = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(expr);
            FunctionInfo functionInfo = FunctionInfo();
            functionInfo.frame_size = fnDecExpre->symb->slot_count;
            functionInfo.arg_count = fnDecExpre->parameters.size();
            size_t functionIndex = m_chunk.push_function(functionInfo);
            fnDecExpre->symb->function_index = functionIndex;
            break;
        }
        default:
            break;
        }
    }

    int mainIndex = FindMainFunctionIndex();
    if (mainIndex < 0)
    {
        // TODO :: make it a error later 
        return;
    }

    size globalSlotCount = 0;
    for (size_t i = 0; i < m_program->content.size(); i++)
    {
    Expression* expr = m_program->content[i];
    if (expr->type != ExpressionType::_VARIABLE_DECLARATION)
        continue;
    VariableDeclarationExpression* varDec = ExpressionCast<ExpressionType::_VARIABLE_DECLARATION>(expr);
    if (varDec->symbol == nullptr)
        continue;
    size slotEnd = static_cast<size>(varDec->symbol->m_slot + 1);
    if (slotEnd > globalSlotCount)
        globalSlotCount = slotEnd;
    }
    m_chunk.set_global_slot_count(globalSlotCount);

    for (size_t i = 0; i < m_program->content.size(); i++)
    {
        Expression* expr = m_program->content[i];
        if (expr->type != ExpressionType::_VARIABLE_DECLARATION)
            continue;
        CompileDeclaration(expr);
    }

    Emit(OpCode::_PUSH, 0);   
    Emit(OpCode::_CALL, static_cast<u8>(mainIndex), 0);
    Emit(OpCode::_HALT);

    for (size_t i = 0; i < m_program->content.size(); i++)
    {
        Expression* expr = m_program->content[i];
        if (expr->type != ExpressionType::_FUNCTION_DECLARATION)
            continue;
        CompileDeclaration(expr);
    }
}

void Compiler::CompileDeclaration(Expression *expr)
{
    switch (expr->type)
    {
    case ExpressionType::_FUNCTION_DECLARATION: CompileFunctionDeclaration(ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(expr)); break;
    case ExpressionType::_VARIABLE_DECLARATION: CompileVariableDeclaration(ExpressionCast<ExpressionType::_VARIABLE_DECLARATION>(expr)); break;
    default:
        break;
    }
}

void Compiler::CompileExpression(Expression *expr)
{
        if (expr == nullptr)
        return;

    switch (expr->type)
    {
        case ExpressionType::_BLOCK:                CompileBlock(ExpressionCast<ExpressionType::_BLOCK>(expr)); break;
        case ExpressionType::_RETURN:               CompileReturn(ExpressionCast<ExpressionType::_RETURN>(expr)); break;
        case ExpressionType::_DO_WHILE:             CompileDoWhile(ExpressionCast<ExpressionType::_DO_WHILE>(expr)); break;
        case ExpressionType::_IF:                   CompileIf(ExpressionCast<ExpressionType::_IF>(expr)); break;
        case ExpressionType::_FUNCTION_DECLARATION: CompileFunctionDeclaration(ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(expr)); break;
        case ExpressionType::_FUNCTION_CALL:        CompileFunctionCall(ExpressionCast<ExpressionType::_FUNCTION_CALL>(expr)); break;
        case ExpressionType::_VARIABLE_DECLARATION: CompileVariableDeclaration(ExpressionCast<ExpressionType::_VARIABLE_DECLARATION>(expr)); break;
        case ExpressionType::_VARIABLE_ASSIGNMENT:  CompileVariableAssignment(ExpressionCast<ExpressionType::_VARIABLE_ASSIGNMENT>(expr)); break;
        case ExpressionType::_IDENTIFIER:           CompileIdentifier(ExpressionCast<ExpressionType::_IDENTIFIER>(expr)); break;
        case ExpressionType::_LITERAL:              CompileLiteral(ExpressionCast<ExpressionType::_LITERAL>(expr)); break;
        case ExpressionType::_UNARY:                CompileUnary(ExpressionCast<ExpressionType::_UNARY>(expr)); break;
        case ExpressionType::_BINARY:               CompileBinary(ExpressionCast<ExpressionType::_BINARY>(expr)); break;

        default:
            return;
    }      
    return;
}

void Compiler::CompileBlock(BlockExpression *block)
{
    for (size_t i = 0; i < block->content.size(); i++)
    {
        Expression* expr = block->content[i];
        CompileExpression(expr);
    }
    
}

void Compiler::CompileReturn(ReturnExpression *ret)
{
    if (ret->value != nullptr)
    {
        CompileExpression(ret->value);
        Emit(OpCode::_STORE,0,0);
    }

    Emit(OpCode::_RET);
}

void Compiler::CompileDoWhile(DoWhileExpression *doWhile)
{
    size_t labelStart = NewLabel();
    BindLabel(labelStart, m_chunk.instruction_size());
    CompileBlock(doWhile->block);
    CompileExpression(doWhile->condition); 
    EmitJump(OpCode::_JNZ, 0, labelStart);       
}

void Compiler::CompileIf(IfExpression *ifStmt)
{
    size_t lEnd = NewLabel();
    CompileExpression(ifStmt->condition); 
    EmitJump(OpCode::_JZ, 0, lEnd);       
    CompileBlock(ifStmt->block);
    BindLabel(lEnd, m_chunk.instruction_size());
}

void Compiler::CompileFunctionDeclaration(FunctionDeclarationExpression *functionDeclaration)
{
    size index = functionDeclaration->symb->function_index;
    FunctionInfo& fnInfo = m_chunk.read_functions(index);
    fnInfo.entry = m_chunk.instruction_size();
    CompileBlock(functionDeclaration->content);
    Emit(OpCode::_RET); 
}

void Compiler::CompileFunctionCall(FunctionCallExpression *functionCall)
{
    FuncSymbol* functionSymbol = functionCall->symb;

    if (!functionSymbol->isBuiltin)
    {
        Emit(OpCode::_PUSH, 0);  
    }

    for (size_t argumentIndex = 0; argumentIndex < functionCall->arguments.size(); argumentIndex++)
    {
        Expression* argument = functionCall->arguments[argumentIndex];
        CompileExpression(argument);
        Emit(OpCode::_PUSH, 0);
    }

    if (functionSymbol->isBuiltin)
    {
        Emit(OpCode::_CALL_NATIVE, static_cast<u8>(functionSymbol->function_index), static_cast<u8>(functionCall->arguments.size()));
        return;
    }

    Emit(OpCode::_CALL, static_cast<u8>(functionSymbol->function_index), static_cast<u8>(functionCall->arguments.size()));
}

void Compiler::CompileVariableDeclaration(VariableDeclarationExpression *variableDeclaration)
{
    if (variableDeclaration->value == nullptr)
        return;

    CompileExpression(variableDeclaration->value);

    VarSymbol* var = SymbolCast<SymbolType::_VAR>(variableDeclaration->symbol);
    
    if (var->isGlobal)
        Emit(OpCode::_STOREG, static_cast<u8>(var->m_slot), 0);
    else
        Emit(OpCode::_STORE, static_cast<u8>(var->m_slot), 0);
}

void Compiler::CompileVariableAssignment(VariableAssignmentExpression *variableAssignment)
{
    CompileExpression(variableAssignment->value);
    VarSymbol* var = SymbolCast<SymbolType::_VAR>(variableAssignment->symbol);
    
    if (var->isGlobal)
        Emit(OpCode::_STOREG, static_cast<u8>(var->m_slot), 0);
    else
        Emit(OpCode::_STORE, static_cast<u8>(var->m_slot), 0);
}

void Compiler::CompileIdentifier(IdentifierExpression *identifier)
{
    if (identifier->symbol == nullptr)
        return;
    VarSymbol* var = SymbolCast<SymbolType::_VAR>(identifier->symbol);
    if (var == nullptr)
        return;

    if (var->isGlobal)
        Emit(OpCode::_LOADG, 0, static_cast<u8>(var->m_slot));
    else
        Emit(OpCode::_LOAD, 0, static_cast<u8>(var->m_slot));
}

void Compiler::CompileLiteral(LiteralExpression *literal)
{
    ConstantContainer c{};

    switch (literal->token.type)
    {
    case TokenType::_TRUE:
        c.type = ConstantType::_BOOL;
        c.data.as_bool = true;
        break;

    case TokenType::_FALSE:
        c.type = ConstantType::_BOOL;
        c.data.as_bool = false;
        break;

    case TokenType::_VALUE:
    {
        u64 value = std::stoull(std::string(literal->token.view));
        if (value <= static_cast<u64>(std::numeric_limits<i32>::max()))
        {
            c.type = ConstantType::_I32;
            c.data.as_i32 = static_cast<i32>(value);
        }
        else if (value <= static_cast<u64>(std::numeric_limits<i64>::max()))
        {
            c.type = ConstantType::_I64;
            c.data.as_i64 = static_cast<i64>(value);
        }
        else
        {
            c.type = ConstantType::_U64;
            c.data.as_u64 = value;
        }
        break;
    }

    case TokenType::_STRING_LITTERAL:
        c.type = ConstantType::_STRING;
        c.data.as_str = new str(std::string(literal->token.view));
        break;

    default:
        return;  
    }

    int constIndex = GetConstIndex(c);
    Emit(OpCode::_LOADC, 0, static_cast<u8>(constIndex));
}

void Compiler::CompileBinary(BinaryExpression *binary)
{
    CompileExpression(binary->left);       
    Emit(OpCode::_PUSH, 0);                      
    CompileExpression(binary->right);   
    Emit(OpCode::_MOV, 1, 0);                     
    Emit(OpCode::_POP, 0);                        
    OpCode op = GetBinaryOp(binary->token.type);  
    Emit(op, 2, 0, 1);                     
    Emit(OpCode::_MOV, 0, 2);                   
}

void Compiler::CompileUnary(UnaryExpression *unary)
{
    CompileExpression(unary->left);  
    switch (unary->token.type)
    {
    case TokenType::_SUB: Emit(OpCode::_NEG, 0, 0); break;
    case TokenType::_NOT_OP: Emit(OpCode::_NOT, 0, 0); break;
    default:
        break;
    } 
}

size_t Compiler::Emit(OpCode op, u8 a, u8 b, u8 c)
{
    Instruction instr{op, a, b, c};
    return m_chunk.push_instruction(instr);
}

size_t Compiler::EmitJump(OpCode op, u8 reg, size_t label)
{
    size_t instrIdx = Emit(op, reg, 0, 0);
    m_pendingJumps.push_back({instrIdx, label});
    return instrIdx;
}

size_t Compiler::NewLabel()
{
    m_labelPc.push_back(SIZE_MAX); 
    return m_labelPc.size() - 1;
}

void Compiler::BindLabel(size_t label, size_t pc)
{
    m_labelPc[label] = pc;
}

void Compiler::ResolveLabels()
{
    for (const PendingJump& pj : m_pendingJumps)
    {
        size_t targetPc = m_labelPc[pj.labelId];
        if (targetPc == SIZE_MAX)
        {
            //TODO :: make it a compil error later
            continue;
        }
        Instruction& instr = m_chunk.read_instruction(pj.instrIndex);

        instr.regB = (targetPc);
    }
}

int Compiler::GetConstIndex(ConstantContainer& c)
{
    size_t n = m_chunk.constant_size();
    for (size_t i = 0; i < n; ++i)
    {
        ConstantContainer& existing = m_chunk.read_constants(i);
        if (existing.type != c.type)
            continue;

        bool equal = false;
        switch (c.type)
        {
            case ConstantType::_BOOL:     equal = (existing.data.as_bool == c.data.as_bool); break;
            case ConstantType::_I8:       equal = (existing.data.as_i8 == c.data.as_i8); break;
            case ConstantType::_I16:      equal = (existing.data.as_i16 == c.data.as_i16); break;
            case ConstantType::_I32:      equal = (existing.data.as_i32 == c.data.as_i32); break;
            case ConstantType::_I64:      equal = (existing.data.as_i64  == c.data.as_i64); break;
            case ConstantType::_U8:       equal = (existing.data.as_u8 == c.data.as_u8); break;
            case ConstantType::_U16:      equal = (existing.data.as_u16 == c.data.as_u16); break;
            case ConstantType::_U32:      equal = (existing.data.as_u32 == c.data.as_u32); break;
            case ConstantType::_U64:      equal = (existing.data.as_u64 == c.data.as_u64); break;
            case ConstantType::_STRING:   equal = (existing.data.as_str && c.data.as_str && *existing.data.as_str == *c.data.as_str); break;
            case ConstantType::_BITBOARD: equal = (existing.data.as_bitboard == c.data.as_bitboard); break;
            case ConstantType::_MOVE:     equal = (existing.data.as_move == c.data.as_move); break;
            case ConstantType::_I32x8: equal = (std::memcmp(&existing.data.as_i32x8, &c.data.as_i32x8, sizeof(VectorI32x8)) == 0); break;
            case ConstantType::_I64x4: equal = (std::memcmp(&existing.data.as_i64x4, &c.data.as_i64x4, sizeof(VectorI64x4)) == 0); break;
            default: break;
        }

        if (equal)
            return static_cast<int>(i);
    }

    size_t idx = m_chunk.push_constant(c);
    return static_cast<int>(idx);
}

OpCode Compiler::GetBinaryOp(TokenType token)
{
    switch (token)
    {
    case TokenType::_ADD: return OpCode::_ADD;
    case TokenType::_SUB: return OpCode::_SUB;
    case TokenType::_MUL: return OpCode::_MUL;
    case TokenType::_DIV: return OpCode::_DIV;
    case TokenType::_POW: return OpCode::_POW;
    case TokenType::_EQUAL_OP: return OpCode::_CMP_EQ;
    case TokenType::_NOT_EQUAL_OP: return OpCode::_CMP_NE;
    case TokenType::_LOWER_THAN: return OpCode::_CMP_LT;
    case TokenType::_LOWER_EQ_THAN: return OpCode::_CMP_LE;
    case TokenType::_GREATER_THAN: return OpCode::_CMP_GT;
    case TokenType::_GREATER_EQ_THAN: return OpCode::_CMP_GE;
    case TokenType::_AND_BINARY: return OpCode::_AND;
    case TokenType::_OR_BINARY: return OpCode::_OR;
    case TokenType::_XOR_BINARY: return OpCode::_XOR;
    case TokenType::_L_SHIFT: return OpCode::_SHL;
    case TokenType::_R_SHIFT: return OpCode::_SHR;
    default:
        return OpCode::_HALT; // TODO :: make a compil error later
    }
}

int Compiler::FindMainFunctionIndex()
{
    for (Expression* expr : m_program->content)
    {
        if (expr->type != ExpressionType::_FUNCTION_DECLARATION) continue;
        auto* fn = ExpressionCast<ExpressionType::_FUNCTION_DECLARATION>(expr);
        if (fn->functionName->token.view == "Main")
            return fn->symb->function_index;
    }
    return -1;
}