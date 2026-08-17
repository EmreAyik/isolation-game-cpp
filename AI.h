#ifndef AI_H
#define AI_H

#include "Board.h"
#include <limits>

class AI {
public:
    
    explicit AI(int depth = 3);

    
    TurnAction chooseBestMove(const Board& board);

private:
    int maxDepth;

    
    int minimax(Board& board, int depth, bool maximizingPlayer,
                int alpha, int beta);

    
    int evaluate(const Board& board) const;

   
    int mobilityScore(const Board& board, int player) const;
};

#endif
