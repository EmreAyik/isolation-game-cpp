#include "AI.h"
#include <algorithm>
#include <vector>


static const int WIN_SCORE  = 1000000;
static const int LOSE_SCORE = -1000000;


static std::vector<TurnAction> generateActionsFixed(const Board& board, int player) {
    std::vector<TurnAction> actions;

    Position from = (player == 1) ? board.getP1Pos() : board.getP2Pos();
    Position opp  = (player == 1) ? board.getP2Pos() : board.getP1Pos();

   
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            Position to{from.r + dr, from.c + dc};

            
            if (to.r < 0 || to.r >= Board::N || to.c < 0 || to.c >= Board::N) continue;
            if (board.getCell(to.r, to.c) != Cell::Empty) continue;

            
            for (int r = 0; r < Board::N; r++) {
                for (int c = 0; c < Board::N; c++) {
                    Position rem{r, c};

                   
                    if (rem == to) continue;

                    
                    if (rem == opp) continue;

                   
                    Cell cell = board.getCell(r, c);
                    bool removableNowEmpty = (cell == Cell::Empty);
                    bool removableFrom = (rem == from);

                    
                    if (removableNowEmpty || removableFrom) {
                        
                        if (cell != Cell::Removed && !(rem == opp)) {
                            actions.push_back({to, rem});
                        }
                    }
                }
            }
        }
    }

    return actions;
}

AI::AI(int depth) : maxDepth(depth) {}

TurnAction AI::chooseBestMove(const Board& board) {
    Board b = board; 

    auto actions = generateActionsFixed(b, 1);
    if (actions.empty()) {
        
        return TurnAction{{-1, -1}, {-1, -1}};
    }

    int bestScore = std::numeric_limits<int>::min();
    TurnAction bestAct = actions[0];

    int alpha = std::numeric_limits<int>::min();
    int beta  = std::numeric_limits<int>::max();

    for (const auto& act : actions) {
        auto snap = b.snapshot();
        bool ok = b.applyAction(1, act);
        if (!ok) {
            b.restore(snap);
            continue;
        }

        int score = minimax(b, maxDepth - 1, /*maximizingPlayer=*/false, alpha, beta);
        b.restore(snap);

        if (score > bestScore) {
            bestScore = score;
            bestAct = act;
        }
        alpha = std::max(alpha, bestScore);
    }

    return bestAct;
}

int AI::minimax(Board& board, int depth, bool maximizingPlayer, int alpha, int beta) {
    int player = maximizingPlayer ? 1 : 2;

    
    if (!board.hasAnyMove(player)) {
       
        return maximizingPlayer ? LOSE_SCORE : WIN_SCORE;
    }

    if (depth == 0) {
        return evaluate(board);
    }

    
    auto actions = generateActionsFixed(board, player);

    if (maximizingPlayer) {
        int best = std::numeric_limits<int>::min();

        for (const auto& act : actions) {
            auto snap = board.snapshot();
            if (!board.applyAction(player, act)) {
                board.restore(snap);
                continue;
            }

            int val = minimax(board, depth - 1, /*maximizingPlayer=*/false, alpha, beta);
            board.restore(snap);

            best = std::max(best, val);
            alpha = std::max(alpha, best);
            if (beta <= alpha) break; // prune
        }
        return best;
    } else {
        int best = std::numeric_limits<int>::max();

        for (const auto& act : actions) {
            auto snap = board.snapshot();
            if (!board.applyAction(player, act)) {
                board.restore(snap);
                continue;
            }

            int val = minimax(board, depth - 1, /*maximizingPlayer=*/true, alpha, beta);
            board.restore(snap);

            best = std::min(best, val);
            beta = std::min(beta, best);
            if (beta <= alpha) break; // prune
        }
        return best;
    }
}

int AI::mobilityScore(const Board& board, int player) const {
    Position pos = (player == 1) ? board.getP1Pos() : board.getP2Pos();
    int moves = 0;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            Position to{pos.r + dr, pos.c + dc};
            if (to.r < 0 || to.r >= Board::N || to.c < 0 || to.c >= Board::N) continue;
            if (board.getCell(to.r, to.c) == Cell::Empty) moves++;
        }
    }
    return moves;
}

int AI::evaluate(const Board& board) const {
    
    int m1 = mobilityScore(board, 1);
    int m2 = mobilityScore(board, 2);

    // main term
    int score = (m1 - m2) * 100;

    
    if (m2 <= 2) score += 150;
    if (m2 == 1) score += 200;

    
    if (m1 <= 2) score -= 150;
    if (m1 == 1) score -= 200;

    return score;
}
