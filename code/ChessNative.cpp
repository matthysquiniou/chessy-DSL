#include "ChessNative.h"
#include "VM.h"
#include "Bitboard.h"
#include "Square.h"
#include "Piece.h"
#include "Builtins.h"
#include <chrono>
#include <cstring>
#include "BitUtils.h"

static u64 g_KnightAttacks[64];
static u64 g_KingAttacks[64];
static u64 g_PawnAttacks[2][64];
static bool g_TablesInitialized = false;

static void InitializeTables();
static u64 ComputeBishopAttacks(u8 squareIndex, u64 occupancy);
static u64 ComputeRookAttacks(u8 squareIndex, u64 occupancy);
static void GenerateMovesInternal(VM& vm);
static void MakeMoveInternal(VM& vm, const Move& move);
static void UnmakeMoveInternal(VM& vm);
static Piece PieceOnSquare(const Board& board, u8 squareIndex);
static bool IsSquareAttacked(const Board& board, u8 squareIndex, Color byColor);
static u64 AttackersTo(const Board& board, u8 squareIndex, u64 occupancy);
static u64 SlidingAttackRay(u8 from, int fileDelta, int rankDelta, u64 occupancy);
static void PushPawnMoves(const Board& board, vector<Move>& moveList, Color color);
static void PushKnightMoves(const Board& board, vector<Move>& moveList, Color color);
static void PushSlidingMoves(const Board& board, vector<Move>& moveList, Color color, PieceType pieceType);
static void PushKingMoves(const Board& board, vector<Move>& moveList, Color color);
static void PushCastlingMoves(const Board& board, vector<Move>& moveList, Color color);
static int PieceValue(Piece p);
static int MoveScore(const Board& board, const Move& m);
static void GenerateCapturesInternal(VM& vm);

void InvokeNativeImpl(u8 nativeIndex, const char* name, VM& vm, const ConstantContainer* arguments, u8 argumentCount);

static void InitializeTables()
{
    if (g_TablesInitialized) { return; }
    g_TablesInitialized = true;

    for (u8 squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        u8 file = squareIndex & 7;
        u8 rank = squareIndex >> 3;

        int knightDeltas[8][2] = { {1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2} };
        int kingDeltas[8][2]   = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };

        u64 knightMask = 0;
        u64 kingMask = 0;

        for (int deltaIndex = 0; deltaIndex < 8; deltaIndex++)
        {
            int knightFile = static_cast<int>(file) + knightDeltas[deltaIndex][0];
            int knightRank = static_cast<int>(rank) + knightDeltas[deltaIndex][1];
            if (knightFile >= 0 && knightFile < 8 && knightRank >= 0 && knightRank < 8)
            {
                knightMask |= 1ULL << (knightRank * 8 + knightFile);
            }

            int kingFile = static_cast<int>(file) + kingDeltas[deltaIndex][0];
            int kingRank = static_cast<int>(rank) + kingDeltas[deltaIndex][1];
            if (kingFile >= 0 && kingFile < 8 && kingRank >= 0 && kingRank < 8)
            {
                kingMask |= 1ULL << (kingRank * 8 + kingFile);
            }
        }

        g_KnightAttacks[squareIndex] = knightMask;
        g_KingAttacks[squareIndex] = kingMask;

        u64 whitePawnMask = 0;
        u64 blackPawnMask = 0;
        if (rank < 7)
        {
            if (file > 0) { whitePawnMask |= 1ULL << ((rank + 1) * 8 + file - 1); }
            if (file < 7) { whitePawnMask |= 1ULL << ((rank + 1) * 8 + file + 1); }
        }
        if (rank > 0)
        {
            if (file > 0) { blackPawnMask |= 1ULL << ((rank - 1) * 8 + file - 1); }
            if (file < 7) { blackPawnMask |= 1ULL << ((rank - 1) * 8 + file + 1); }
        }
        g_PawnAttacks[0][squareIndex] = whitePawnMask;
        g_PawnAttacks[1][squareIndex] = blackPawnMask;
    }
}

static u64 SlidingAttackRay(u8 from, int fileDelta, int rankDelta, u64 occupancy)
{
    u64 attacks = 0;
    int file = (from & 7) + fileDelta;
    int rank = (from >> 3) + rankDelta;
    while (file >= 0 && file < 8 && rank >= 0 && rank < 8)
    {
        u8 square = static_cast<u8>(rank * 8 + file);
        attacks |= 1ULL << square;
        if (occupancy & (1ULL << square)) { break; }
        file += fileDelta;
        rank += rankDelta;
    }
    return attacks;
}

static u64 ComputeBishopAttacks(u8 squareIndex, u64 occupancy)
{
    return SlidingAttackRay(squareIndex, 1, 1, occupancy)
         | SlidingAttackRay(squareIndex, 1, -1, occupancy)
         | SlidingAttackRay(squareIndex, -1, 1, occupancy)
         | SlidingAttackRay(squareIndex, -1, -1, occupancy);
}

static u64 ComputeRookAttacks(u8 squareIndex, u64 occupancy)
{
    return SlidingAttackRay(squareIndex, 1, 0, occupancy)
         | SlidingAttackRay(squareIndex, -1, 0, occupancy)
         | SlidingAttackRay(squareIndex, 0, 1, occupancy)
         | SlidingAttackRay(squareIndex, 0, -1, occupancy);
}

static Piece PieceOnSquare(const Board& board, u8 squareIndex)
{
    u64 squareBit = 1ULL << squareIndex;
    for (u8 pieceIndex = 0; pieceIndex < 12; pieceIndex++)
    {
        if (board.pieces[pieceIndex] & squareBit)
        {
            return static_cast<Piece>(pieceIndex);
        }
    }
    return Piece::None;
}

static u64 AttackersTo(const Board& board, u8 squareIndex, u64 occupancy)
{
    u64 attackers = 0;
    attackers |= g_KnightAttacks[squareIndex] & (board.pieces[static_cast<u8>(Piece::WhiteKnight)] | board.pieces[static_cast<u8>(Piece::BlackKnight)]);
    attackers |= g_KingAttacks[squareIndex] & (board.pieces[static_cast<u8>(Piece::WhiteKing)] | board.pieces[static_cast<u8>(Piece::BlackKing)]);
    attackers |= g_PawnAttacks[static_cast<u8>(Color::Black)][squareIndex] & board.pieces[static_cast<u8>(Piece::WhitePawn)];
    attackers |= g_PawnAttacks[static_cast<u8>(Color::White)][squareIndex] & board.pieces[static_cast<u8>(Piece::BlackPawn)];
    attackers |= ComputeBishopAttacks(squareIndex, occupancy) & (board.pieces[static_cast<u8>(Piece::WhiteBishop)] | board.pieces[static_cast<u8>(Piece::BlackBishop)] | board.pieces[static_cast<u8>(Piece::WhiteQueen)] | board.pieces[static_cast<u8>(Piece::BlackQueen)]);
    attackers |= ComputeRookAttacks(squareIndex, occupancy) & (board.pieces[static_cast<u8>(Piece::WhiteRook)] | board.pieces[static_cast<u8>(Piece::BlackRook)] | board.pieces[static_cast<u8>(Piece::WhiteQueen)] | board.pieces[static_cast<u8>(Piece::BlackQueen)]);
    return attackers;
}

static bool IsSquareAttacked(const Board& board, u8 squareIndex, Color byColor)
{
    u64 attackers = AttackersTo(board, squareIndex, board.allOccupancy);
    u64 byColorMask = board.occupancy[static_cast<u8>(byColor)];
    return (attackers & byColorMask) != 0;
}

static void PushPawnMoves(const Board& board, vector<Move>& moveList, Color color)
{
    u64 pawns = board.pieces[static_cast<u8>(MakePiece(color, PieceType::Pawn))];
    u64 occupancy = board.allOccupancy;
    int pushDirection = (color == Color::White) ? 8 : -8;
    u64 promotionRankMask = (color == Color::White) ? 0xFF00000000000000ULL : 0x00000000000000FFULL;
    u64 startRankMask = (color == Color::White) ? 0x000000000000FF00ULL : 0x00FF000000000000ULL;
    u64 emptySquares = ~occupancy;
    u64 enemyOccupancy = board.occupancy[static_cast<u8>(OppositeColor(color))];

    while (pawns)
    {
        u8 from = static_cast<u8>(CountTrailingZeros(pawns));
        pawns &= pawns - 1;
        u8 to = static_cast<u8>(static_cast<int>(from) + pushDirection);
        if (!(emptySquares & (1ULL << to))) { continue; }
        if (promotionRankMask & (1ULL << to))
        {
            for (u8 promoType = 1; promoType <= 4; promoType++)
            {
                Move move = { from, to, static_cast<u8>(MoveFlagPromotion | ((promoType - 1) << 4)) };
                moveList.push_back(move);
            }
        }
        else
        {
            Move move = { from, to, MoveFlagNone };
            moveList.push_back(move);
            if ((startRankMask & (1ULL << from)) && (emptySquares & (1ULL << (to + pushDirection))))
            {
                Move doublePush = { from, static_cast<u8>(to + pushDirection), MoveFlagNone };
                moveList.push_back(doublePush);
            }
        }
    }

    u64 captureTargets = enemyOccupancy;
    if (board.enPassantSquare != NoSquare)
    {
        captureTargets |= 1ULL << board.enPassantSquare;
    }
    pawns = board.pieces[static_cast<u8>(MakePiece(color, PieceType::Pawn))];
    while (pawns)
    {
        u8 from = static_cast<u8>(CountTrailingZeros(pawns));
        pawns &= pawns - 1;
        u64 attacks = g_PawnAttacks[static_cast<u8>(color)][from] & captureTargets;
        while (attacks)
        {
            u8 to = static_cast<u8>(CountTrailingZeros(attacks));
            attacks &= attacks - 1;
            u8 flag = (to == board.enPassantSquare) ? MoveFlagEnPassant : MoveFlagCapture;
            if (promotionRankMask & (1ULL << to))
            {
                for (u8 promoType = 1; promoType <= 4; promoType++)
                {
                    Move move = { from, to, static_cast<u8>(flag | MoveFlagPromotion | ((promoType - 1) << 4)) };
                    moveList.push_back(move);
                }
            }
            else
            {
                Move move = { from, to, flag };
                moveList.push_back(move);
            }
        }
    }
}

static void PushKnightMoves(const Board& board, vector<Move>& moveList, Color color)
{
    u64 knights = board.pieces[static_cast<u8>(MakePiece(color, PieceType::Knight))];
    u64 ownOccupancy = board.occupancy[static_cast<u8>(color)];
    u64 enemyOccupancy = board.occupancy[static_cast<u8>(OppositeColor(color))];
    while (knights)
    {
        u8 from = static_cast<u8>(CountTrailingZeros(knights));
        knights &= knights - 1;
        u64 attacks = g_KnightAttacks[from] & ~ownOccupancy;
        while (attacks)
        {
            u8 to = static_cast<u8>(CountTrailingZeros(attacks));
            attacks &= attacks - 1;
            u8 flag = (enemyOccupancy & (1ULL << to)) ? MoveFlagCapture : MoveFlagNone;
            moveList.push_back({ from, to, flag });
        }
    }
}

static void PushSlidingMoves(const Board& board, vector<Move>& moveList, Color color, PieceType pieceType)
{
    u64 sliders = board.pieces[static_cast<u8>(MakePiece(color, pieceType))];
    u64 ownOccupancy = board.occupancy[static_cast<u8>(color)];
    u64 enemyOccupancy = board.occupancy[static_cast<u8>(OppositeColor(color))];
    u64 occupancy = board.allOccupancy;
    while (sliders)
    {
        u8 from = static_cast<u8>(CountTrailingZeros(sliders));
        sliders &= sliders - 1;
        u64 attacks = 0;
        if (pieceType == PieceType::Bishop) { attacks = ComputeBishopAttacks(from, occupancy); }
        else if (pieceType == PieceType::Rook) { attacks = ComputeRookAttacks(from, occupancy); }
        else { attacks = ComputeBishopAttacks(from, occupancy) | ComputeRookAttacks(from, occupancy); }
        attacks &= ~ownOccupancy;
        while (attacks)
        {
            u8 to = static_cast<u8>(CountTrailingZeros(attacks));
            attacks &= attacks - 1;
            u8 flag = (enemyOccupancy & (1ULL << to)) ? MoveFlagCapture : MoveFlagNone;
            moveList.push_back({ from, to, flag });
        }
    }
}

static void PushKingMoves(const Board& board, vector<Move>& moveList, Color color)
{
    u64 king = board.pieces[static_cast<u8>(MakePiece(color, PieceType::King))];
    if (king == 0) { return; }
    u8 from = static_cast<u8>(CountTrailingZeros(king));
    u64 ownOccupancy = board.occupancy[static_cast<u8>(color)];
    u64 enemyOccupancy = board.occupancy[static_cast<u8>(OppositeColor(color))];
    u64 attacks = g_KingAttacks[from] & ~ownOccupancy;
    while (attacks)
    {
        u8 to = static_cast<u8>(CountTrailingZeros(attacks));
        attacks &= attacks - 1;
        u8 flag = (enemyOccupancy & (1ULL << to)) ? MoveFlagCapture : MoveFlagNone;
        moveList.push_back({ from, to, flag });
    }
}

static void PushCastlingMoves(const Board& board, vector<Move>& moveList, Color color)
{
    u64 occupancy = board.allOccupancy;
    if (color == Color::White)
    {
        if ((board.castlingRights & CastleWhiteKing) && !(occupancy & 0x60ULL))
        {
            if (!IsSquareAttacked(board, 4, Color::Black) && !IsSquareAttacked(board, 5, Color::Black) && !IsSquareAttacked(board, 6, Color::Black))
            {
                moveList.push_back({ 4, 6, MoveFlagCastle });
            }
        }
        if ((board.castlingRights & CastleWhiteQueen) && !(occupancy & 0x0EULL))
        {
            if (!IsSquareAttacked(board, 4, Color::Black) && !IsSquareAttacked(board, 3, Color::Black) && !IsSquareAttacked(board, 2, Color::Black))
            {
                moveList.push_back({ 4, 2, MoveFlagCastle });
            }
        }
        return;
    }

    if ((board.castlingRights & CastleBlackKing) && !(occupancy & 0x6000000000000000ULL))
    {
        if (!IsSquareAttacked(board, 60, Color::White) && !IsSquareAttacked(board, 61, Color::White) && !IsSquareAttacked(board, 62, Color::White))
        {
            moveList.push_back({ 60, 62, MoveFlagCastle });
        }
    }
    if ((board.castlingRights & CastleBlackQueen) && !(occupancy & 0x0E00000000000000ULL))
    {
        if (!IsSquareAttacked(board, 60, Color::White) && !IsSquareAttacked(board, 59, Color::White) && !IsSquareAttacked(board, 58, Color::White))
        {
            moveList.push_back({ 60, 58, MoveFlagCastle });
        }
    }
}

static void GenerateMovesInternal(VM& vm)
{
    Board& board = vm.GetBoard();
    vector<Move>& moveList = vm.GetCurrentMoveList();
    moveList.clear();
    Color color = board.sideToMove;
    PushPawnMoves(board, moveList, color);
    PushKnightMoves(board, moveList, color);
    PushSlidingMoves(board, moveList, color, PieceType::Bishop);
    PushSlidingMoves(board, moveList, color, PieceType::Rook);
    PushSlidingMoves(board, moveList, color, PieceType::Queen);
    PushKingMoves(board, moveList, color);
    PushCastlingMoves(board, moveList, color);
}

static void MakeMoveInternal(VM& vm, const Move& move)
{
    Board& board = vm.GetBoard();
    vector<BoardState>& states = vm.GetBoardStates();
    BoardState state;
    state.board = board;
    states.push_back(state);

    Piece movingPiece = PieceOnSquare(board, move.from);
    if (move.flags & MoveFlagEnPassant)
    {
        u8 capturedSquare = (board.sideToMove == Color::White) ? static_cast<u8>(move.to - 8) : static_cast<u8>(move.to + 8);
        Piece capturedPiece = PieceOnSquare(board, capturedSquare);
        board.pieces[static_cast<u8>(capturedPiece)] &= ~(1ULL << capturedSquare);
        board.occupancy[static_cast<u8>(OppositeColor(board.sideToMove))] &= ~(1ULL << capturedSquare);
    }
    else if (move.flags & MoveFlagCapture)
    {
        Piece capturedPiece = PieceOnSquare(board, move.to);
        board.pieces[static_cast<u8>(capturedPiece)] &= ~(1ULL << move.to);
        board.occupancy[static_cast<u8>(OppositeColor(board.sideToMove))] &= ~(1ULL << move.to);
    }

    board.pieces[static_cast<u8>(movingPiece)] &= ~(1ULL << move.from);
    board.occupancy[static_cast<u8>(board.sideToMove)] &= ~(1ULL << move.from);

    if (move.flags & MoveFlagPromotion)
    {
        PieceType promotionType = static_cast<PieceType>(((move.flags >> 4) & 0x3) + 1);
        u8 promotionPieceIndex = static_cast<u8>(MakePiece(board.sideToMove, promotionType));
        board.pieces[promotionPieceIndex] |= 1ULL << move.to;
    }
    else
    {
        board.pieces[static_cast<u8>(movingPiece)] |= 1ULL << move.to;
    }
    board.occupancy[static_cast<u8>(board.sideToMove)] |= 1ULL << move.to;

    if (move.flags & MoveFlagCastle)
    {
        if (move.to == 6)  { board.pieces[static_cast<u8>(Piece::WhiteRook)] &= ~(1ULL << 7); board.pieces[static_cast<u8>(Piece::WhiteRook)] |= 1ULL << 5; board.occupancy[0] &= ~(1ULL << 7); board.occupancy[0] |= 1ULL << 5; }
        if (move.to == 2)  { board.pieces[static_cast<u8>(Piece::WhiteRook)] &= ~(1ULL << 0); board.pieces[static_cast<u8>(Piece::WhiteRook)] |= 1ULL << 3; board.occupancy[0] &= ~(1ULL << 0); board.occupancy[0] |= 1ULL << 3; }
        if (move.to == 62) { board.pieces[static_cast<u8>(Piece::BlackRook)] &= ~(1ULL << 63); board.pieces[static_cast<u8>(Piece::BlackRook)] |= 1ULL << 61; board.occupancy[1] &= ~(1ULL << 63); board.occupancy[1] |= 1ULL << 61; }
        if (move.to == 58) { board.pieces[static_cast<u8>(Piece::BlackRook)] &= ~(1ULL << 56); board.pieces[static_cast<u8>(Piece::BlackRook)] |= 1ULL << 59; board.occupancy[1] &= ~(1ULL << 56); board.occupancy[1] |= 1ULL << 59; }
    }

    board.allOccupancy = board.occupancy[0] | board.occupancy[1];
    board.enPassantSquare = NoSquare;
    if (TypeOfPiece(movingPiece) == PieceType::Pawn && (move.to == move.from + 16 || move.to == move.from + 8))
    {
        if (move.to - move.from == 16)
        {
            board.enPassantSquare = static_cast<u8>((move.from + move.to) / 2);
        }
    }
    if (TypeOfPiece(movingPiece) == PieceType::Pawn && (move.from > move.to) && (move.from - move.to == 16))
    {
        board.enPassantSquare = static_cast<u8>((move.from + move.to) / 2);
    }

    board.sideToMove = OppositeColor(board.sideToMove);
    board.halfmoveClock = (TypeOfPiece(movingPiece) != PieceType::Pawn) ? board.halfmoveClock + 1 : 0;
    if (board.sideToMove == Color::White) { board.fullmoveNumber++; }
}

static void UnmakeMoveInternal(VM& vm)
{
    vector<BoardState>& states = vm.GetBoardStates();
    if (states.empty()) { return; }
    Board& board = vm.GetBoard();
    board = states.back().board;
    states.pop_back();
}

void InvokeNativeImpl(u8 nativeIndex, const char* name, VM& vm, const ConstantContainer* arguments, u8 argumentCount)
{
    InitializeTables();

    switch (nativeIndex)
    {
    case 0:  // Print
    {
        const ConstantContainer& value = arguments[0];
        switch (value.type)
        {
        case ConstantType::_BOOL:   std::cout << (value.data.as_bool ? "true" : "false") << std::endl; break;
        case ConstantType::_I8:     std::cout << static_cast<i32>(value.data.as_i8)  << std::endl; break;
        case ConstantType::_I16:    std::cout << static_cast<i32>(value.data.as_i16) << std::endl; break;
        case ConstantType::_I32:    std::cout << value.data.as_i32 << std::endl; break;
        case ConstantType::_I64:    std::cout << value.data.as_i64 << std::endl; break;
        case ConstantType::_U8:     std::cout << static_cast<u32>(value.data.as_u8)  << std::endl; break;
        case ConstantType::_U16:    std::cout << static_cast<u32>(value.data.as_u16) << std::endl; break;
        case ConstantType::_U32:    std::cout << value.data.as_u32 << std::endl; break;
        case ConstantType::_U64:    std::cout << value.data.as_u64 << std::endl; break;
        case ConstantType::_BITBOARD: std::cout << value.data.as_bitboard.board << std::endl; break;
        case ConstantType::_MOVE:
        {
            const Move& m = value.data.as_move;
            std::cout
                << static_cast<char>('a' + (m.from & 7))
                << static_cast<char>('1' + (m.from >> 3))
                << static_cast<char>('a' + (m.to & 7))
                << static_cast<char>('1' + (m.to >> 3));
            if (m.flags & MoveFlagCapture)   std::cout << " capture";
            if (m.flags & MoveFlagPromotion) std::cout << " promo";
            if (m.flags & MoveFlagCastle)    std::cout << " castle";
            if (m.flags & MoveFlagEnPassant) std::cout << " ep";
            std::cout << std::endl;
            break;
        }
        case ConstantType::_I32x8:
        {
            std::cout << "[";
            for (int i = 0; i < 8; i++) { if (i) std::cout << ", "; std::cout << value.data.as_i32x8.lanes[i]; }
            std::cout << "]" << std::endl;
            break;
        }
        case ConstantType::_I64x4:
        {
            std::cout << "[";
            for (int i = 0; i < 4; i++) { if (i) std::cout << ", "; std::cout << value.data.as_i64x4.lanes[i]; }
            std::cout << "]" << std::endl;
            break;
        }
        default: std::cout << "?" << std::endl; break;
        }
        return;
    }
    case 1:  // NowMs
    {
        auto now = std::chrono::steady_clock::now();
        vm.Register(0) = ConstantTrait<ConstantType::_I64>::make(
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
        return;
    }
    case 2:  // PopCount
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(static_cast<i32>(PopCount(arguments[0].data.as_bitboard.board)));
        return;
    case 3:  // Lsb
    {
        u64 bb = arguments[0].data.as_bitboard.board;
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(bb ? static_cast<i32>(CountTrailingZeros(bb)) : -1);
        return;
    }
    case 4:  // Bit
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ 1ULL << static_cast<u8>(arguments[0].data.as_i32) });
        return;
    case 5:  // BitTest
    {
        u64 mask = 1ULL << static_cast<u8>(arguments[1].data.as_i32);
        vm.Register(0) = ConstantTrait<ConstantType::_BOOL>::make((arguments[0].data.as_bitboard.board & mask) != 0);
        return;
    }
    case 6:  // SquareMake
    {
        u8 index = static_cast<u8>(arguments[1].data.as_i32 * 8 + arguments[0].data.as_i32);
        vm.Register(0) = ConstantTrait<ConstantType::_SQUARE>::make(Square{ index });
        return;
    }
    case 7:  // SquareFile
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(arguments[0].data.as_square.index & 7);
        return;
    case 8:  // SquareRank
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(arguments[0].data.as_square.index >> 3);
        return;
    case 9:  // KnightAttacks
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ g_KnightAttacks[arguments[0].data.as_square.index] });
        return;
    case 10: // KingAttacks
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ g_KingAttacks[arguments[0].data.as_square.index] });
        return;
    case 11: // PawnAttacks
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ g_PawnAttacks[static_cast<u8>(arguments[1].data.as_i32)][arguments[0].data.as_square.index] });
        return;
    case 12: // BishopAttacks
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ ComputeBishopAttacks(arguments[0].data.as_square.index, arguments[1].data.as_bitboard.board) });
        return;
    case 13: // RookAttacks
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ ComputeRookAttacks(arguments[0].data.as_square.index, arguments[1].data.as_bitboard.board) });
        return;
    case 14: // QueenAttacks
    {
        u64 attacks = ComputeBishopAttacks(arguments[0].data.as_square.index, arguments[1].data.as_bitboard.board)
                    | ComputeRookAttacks(arguments[0].data.as_square.index, arguments[1].data.as_bitboard.board);
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ attacks });
        return;
    }
    case 15: // NewBoard
    {
        Board& board = vm.GetBoard();
        std::memset(&board, 0, sizeof(Board));
        board.pieces[static_cast<u8>(Piece::WhitePawn)]   = 0x000000000000FF00ULL;
        board.pieces[static_cast<u8>(Piece::WhiteKnight)] = 0x0000000000000042ULL;
        board.pieces[static_cast<u8>(Piece::WhiteBishop)] = 0x0000000000000024ULL;
        board.pieces[static_cast<u8>(Piece::WhiteRook)]   = 0x0000000000000081ULL;
        board.pieces[static_cast<u8>(Piece::WhiteQueen)]  = 0x0000000000000008ULL;
        board.pieces[static_cast<u8>(Piece::WhiteKing)]   = 0x0000000000000010ULL;
        board.pieces[static_cast<u8>(Piece::BlackPawn)]   = 0x00FF000000000000ULL;
        board.pieces[static_cast<u8>(Piece::BlackKnight)] = 0x4200000000000000ULL;
        board.pieces[static_cast<u8>(Piece::BlackBishop)] = 0x2400000000000000ULL;
        board.pieces[static_cast<u8>(Piece::BlackRook)]   = 0x8100000000000000ULL;
        board.pieces[static_cast<u8>(Piece::BlackQueen)]  = 0x0800000000000000ULL;
        board.pieces[static_cast<u8>(Piece::BlackKing)]   = 0x1000000000000000ULL;
        board.occupancy[0] = 0x000000000000FFFFULL;
        board.occupancy[1] = 0xFFFF000000000000ULL;
        board.allOccupancy = 0xFFFF00000000FFFFULL;
        board.sideToMove = Color::White;
        board.castlingRights = 0x0F;
        board.enPassantSquare = NoSquare;
        board.halfmoveClock = 0;
        board.fullmoveNumber = 1;
        return;
    }
    case 16: // SideToMove
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(static_cast<i32>(vm.GetBoard().sideToMove));
        return;
    case 17: // PieceAt
        vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(PieceOnSquare(vm.GetBoard(), arguments[0].data.as_square.index));
        return;
    case 18: // Pieces
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ vm.GetBoard().pieces[static_cast<u8>(arguments[0].data.as_piece)] });
        return;
    case 19: // Occupancy
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{ vm.GetBoard().occupancy[static_cast<u8>(arguments[0].data.as_i32)] });
        return;
    case 20: // KingSquare
    {
        u64 king = vm.GetBoard().pieces[static_cast<u8>(MakePiece(static_cast<Color>(arguments[0].data.as_i32), PieceType::King))];
        vm.Register(0) = ConstantTrait<ConstantType::_SQUARE>::make(Square{ king ? static_cast<u8>(CountTrailingZeros(king)) : NoSquare });
        return;
    }
        case 21: // SortCapturesFirst
    {
        vector<Move>& moveList = vm.GetCurrentMoveList();
        std::partition(moveList.begin(), moveList.end(),
            [](const Move& m) {
                return (m.flags & MoveFlagCapture) != 0 || (m.flags & MoveFlagPromotion) != 0;
            });
        return;
    }
    case 22: // SortMovesMvvLva
    {
        Board& board = vm.GetBoard();
        vector<Move>& moveList = vm.GetCurrentMoveList();
        std::stable_sort(moveList.begin(), moveList.end(),
            [&board](const Move& a, const Move& b) {
                return MoveScore(board, a) > MoveScore(board, b);
            });
        return;
    }
    case 23: // PrintBoard
    {
        Board& board = vm.GetBoard();
        static const char pieceChars[12] = { 'P','N','B','R','Q','K','p','n','b','r','q','k' };
        std::cout << std::endl;
        std::cout << "  +-----------------+" << std::endl;
        for (int rank = 7; rank >= 0; rank--)
        {
            std::cout << " " << (rank + 1) << "| ";
            for (int file = 0; file < 8; file++)
            {
                u8 square = static_cast<u8>(rank * 8 + file);
                u64 bit = 1ULL << square;
                char c = '.';
                for (int p = 0; p < 12; p++)
                {
                    if (board.pieces[p] & bit) { c = pieceChars[p]; break; }
                }
                std::cout << c << ' ';
            }
            std::cout << "|" << std::endl;
        }
        std::cout << "  +-----------------+" << std::endl;
        std::cout << "    a b c d e f g h" << std::endl;
        std::cout << "Side: " << (board.sideToMove == Color::White ? "White" : "Black") << std::endl;
        return;
    }
    case 24: // ReadInt
    {
        i32 value = 0;
        std::cin >> value;
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(value);
        return;
    }
    case 25: // ReadMove
    {
        std::string input;
        std::cin >> input;
        Move m = { 0, 0, MoveFlagNone };
        if (input.size() >= 4)
        {
            u8 fromFile = static_cast<u8>(input[0] - 'a');
            u8 fromRank = static_cast<u8>(input[1] - '1');
            u8 toFile   = static_cast<u8>(input[2] - 'a');
            u8 toRank   = static_cast<u8>(input[3] - '1');
            m.from = static_cast<u8>(fromRank * 8 + fromFile);
            m.to   = static_cast<u8>(toRank * 8 + toFile);
            Board& board = vm.GetBoard();
            u64 toBit = 1ULL << m.to;
            if (board.occupancy[static_cast<u8>(OppositeColor(board.sideToMove))] & toBit)
                m.flags |= MoveFlagCapture;
        }
        vm.Register(0) = ConstantTrait<ConstantType::_MOVE>::make(m);
        return;
    }
    case 26: // NullMove
        vm.Register(0) = ConstantTrait<ConstantType::_MOVE>::make(Move{ 0, 0, 0 });
        return;
    case 27: // Generate
        GenerateMovesInternal(vm);
        return;
    case 28: // MoveCount
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(static_cast<i32>(vm.GetCurrentMoveList().size()));
        return;
    case 29: // MoveAt
        vm.Register(0) = ConstantTrait<ConstantType::_MOVE>::make(vm.GetCurrentMoveList()[arguments[0].data.as_i32]);
        return;
    case 30: // MakeMove
        MakeMoveInternal(vm, arguments[0].data.as_move);
        return;
    case 31: // UnmakeMove
        UnmakeMoveInternal(vm);
        return;
    case 32: // InCheck
    {
        Color color = static_cast<Color>(arguments[0].data.as_i32);
        u64 king = vm.GetBoard().pieces[static_cast<u8>(MakePiece(color, PieceType::King))];
        if (king == 0) { vm.Register(0) = ConstantTrait<ConstantType::_BOOL>::make(false); return; }
        u8 kingSquare = static_cast<u8>(CountTrailingZeros(king));
        vm.Register(0) = ConstantTrait<ConstantType::_BOOL>::make(IsSquareAttacked(vm.GetBoard(), kingSquare, OppositeColor(color)));
        return;
    }
    case 33: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::WhitePawn);   return;
    case 34: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::WhiteKnight); return;
    case 35: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::WhiteBishop); return;
    case 36: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::WhiteRook);   return;
    case 37: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::WhiteQueen);  return;
    case 38: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::WhiteKing);   return;
    case 39: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::BlackPawn);   return;
    case 40: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::BlackKnight); return;
    case 41: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::BlackBishop); return;
    case 42: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::BlackRook);   return;
    case 43: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::BlackQueen);  return;
    case 44: vm.Register(0) = ConstantTrait<ConstantType::_PIECE>::make(Piece::BlackKing);   return;
    case 45: // GenerateCaptures
        GenerateCapturesInternal(vm);
        return;
    case 46: 
    {
        VectorI32x8 v;
        for (int i = 0; i < 8; i++) v.lanes[i] = arguments[i].data.as_i32;
        vm.Register(0) = ConstantTrait<ConstantType::_I32x8>::make(v);
        return;
    }
    case 47:
    {
        VectorI64x4 v;
        for (int i = 0; i < 4; i++) v.lanes[i] = arguments[i].data.as_i64;
        vm.Register(0) = ConstantTrait<ConstantType::_I64x4>::make(v);
        return;
    }
    case 48:
    {
        VectorI32x8 v;
        for (int i = 0; i < 8; i++) v.lanes[i] = arguments[0].data.as_i32;
        vm.Register(0) = ConstantTrait<ConstantType::_I32x8>::make(v);
        return;
    }
    case 49:
    {
        VectorI64x4 v;
        for (int i = 0; i < 4; i++) v.lanes[i] = arguments[0].data.as_i64;
        vm.Register(0) = ConstantTrait<ConstantType::_I64x4>::make(v);
        return;
    }
        case 50: 
    {
        VectorI64x4 v;
        v.lanes[0] = static_cast<i64>(arguments[0].data.as_bitboard.board);
        v.lanes[1] = static_cast<i64>(arguments[1].data.as_bitboard.board);
        v.lanes[2] = static_cast<i64>(arguments[2].data.as_bitboard.board);
        v.lanes[3] = static_cast<i64>(arguments[3].data.as_bitboard.board);
        vm.Register(0) = ConstantTrait<ConstantType::_I64x4>::make(v);
        return;
    }
    case 51: 
    {
        i32 idx = arguments[1].data.as_i32;
        if (idx < 0 || idx > 3) { vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(Bitboard{0}); return; }
        vm.Register(0) = ConstantTrait<ConstantType::_BITBOARD>::make(
            Bitboard{ static_cast<u64>(arguments[0].data.as_i64x4.lanes[idx]) });
        return;
    }
    case 52: 
    {
        __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arguments[0].data.as_i64x4.lanes));
        __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arguments[1].data.as_i64x4.lanes));
        __m256i r = _mm256_and_si256(a, b);
        VectorI64x4 out;
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(out.lanes), r);
        vm.Register(0) = ConstantTrait<ConstantType::_I64x4>::make(out);
        return;
    }
    case 53: 
    {
        __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arguments[0].data.as_i64x4.lanes));
        __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arguments[1].data.as_i64x4.lanes));
        __m256i r = _mm256_or_si256(a, b);
        VectorI64x4 out;
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(out.lanes), r);
        vm.Register(0) = ConstantTrait<ConstantType::_I64x4>::make(out);
        return;
    }
    case 54: 
    {
        __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arguments[0].data.as_i64x4.lanes));
        __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arguments[1].data.as_i64x4.lanes));
        __m256i r = _mm256_xor_si256(a, b);
        VectorI64x4 out;
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(out.lanes), r);
        vm.Register(0) = ConstantTrait<ConstantType::_I64x4>::make(out);
        return;
    }
    case 55:
    {
        __m256i x = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arguments[0].data.as_i64x4.lanes));

        const __m256i m1 = _mm256_set1_epi64x(0x5555555555555555LL);
        const __m256i m2 = _mm256_set1_epi64x(0x3333333333333333LL);
        const __m256i m4 = _mm256_set1_epi64x(0x0F0F0F0F0F0F0F0FLL);

        x = _mm256_sub_epi64(x, _mm256_and_si256(_mm256_srli_epi64(x, 1), m1));
        x = _mm256_add_epi64(
                _mm256_and_si256(x, m2),
                _mm256_and_si256(_mm256_srli_epi64(x, 2), m2));
        x = _mm256_and_si256(
                _mm256_add_epi64(x, _mm256_srli_epi64(x, 4)),
                m4);
        x = _mm256_sad_epu8(x, _mm256_setzero_si256());

        VectorI64x4 out;
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(out.lanes), x);
        vm.Register(0) = ConstantTrait<ConstantType::_I64x4>::make(out);
        return;
    }
    case 56: 
    {
        i32 idx = arguments[1].data.as_i32;
        if (idx < 0 || idx > 3) { vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(0); return; }
        vm.Register(0) = ConstantTrait<ConstantType::_I32>::make(
            static_cast<i32>(arguments[0].data.as_i64x4.lanes[idx]));
        return;
    }

    default:
        std::cout << "Unknown builtin index: " << static_cast<int>(nativeIndex) << std::endl;
        return;
    }
}

static int PieceValue(Piece p)
{
    switch (TypeOfPiece(p))
    {
    case PieceType::Pawn:   return 100;
    case PieceType::Knight: return 320;
    case PieceType::Bishop: return 330;
    case PieceType::Rook:   return 500;
    case PieceType::Queen:  return 900;
    case PieceType::King:   return 20000;
    default:                return 0;
    }
}

static int MoveScore(const Board& board, const Move& m)
{
    int score = 0;
    if (m.flags & MoveFlagCapture)
    {
        int victim = PieceValue(PieceOnSquare(board, m.to));
        int attacker = PieceValue(PieceOnSquare(board, m.from));
        score = 1000000 + victim * 10 - attacker;
    }
    else if (m.flags & MoveFlagPromotion)
    {
        score = 900000;
    }
    return score;
}

static void GenerateCapturesInternal(VM& vm)
{
    Board& board = vm.GetBoard();
    vector<Move>& moveList = vm.GetCurrentMoveList();
    moveList.clear();
    Color color = board.sideToMove;
    
    PushPawnMoves(board, moveList, color);
    PushKnightMoves(board, moveList, color);
    PushSlidingMoves(board, moveList, color, PieceType::Bishop);
    PushSlidingMoves(board, moveList, color, PieceType::Rook);
    PushSlidingMoves(board, moveList, color, PieceType::Queen);
    PushKingMoves(board, moveList, color);
    
    moveList.erase(
        std::remove_if(moveList.begin(), moveList.end(),
            [](const Move& m) {
                return !((m.flags & MoveFlagCapture) || (m.flags & MoveFlagPromotion));
            }),
        moveList.end());
}