#include "Board.h"
#include "AI.h"
#include <iostream>
#include <string>
#include <cstdlib>

static void printHelp() {
    std::cout << "Coordinate format: a1 .. g7\n";
    std::cout << "Example move: a4\n";
    std::cout << "You will enter 2 coords each turn:\n";
    std::cout << "  1) where to MOVE your piece\n";
    std::cout << "  2) which square to REMOVE\n";
    std::cout << "Type 'q' at any prompt to quit the game.\n";
}

static Position askCoord(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::cin >> s;
        if (s == "q" || s == "Q") {
    std::cout << "Exiting game...\n";
    exit(0);
}

        Position p;
        if (Board::parseCoord(s, p)) return p;

        std::cout << "Invalid coord. Use a1..g7\n";
    }
}

int main() {
    std::cout << "CSE462 Term Project - Strategic Board Game (Min-Max AI)\n";
    printHelp();

    Board board;
    AI ai(/*depth=*/3);

    
    int turn = 1;

    while (true) {
        board.print();

        
        if (!board.hasAnyMove(turn)) {
            if (turn == 1) std::cout << "AI cannot move. Human wins!\n";
            else std::cout << "Human cannot move. AI wins!\n";
            break;
        }

        if (turn == 1) {
            std::cout << "[AI] Thinking...\n";
            TurnAction act = ai.chooseBestMove(board);

            if (act.to.r == -1) {
                std::cout << "AI has no legal actions. Human wins!\n";
                break;
            }

            bool ok = board.applyAction(1, act);
            if (!ok) {
                std::cout << "Internal error: AI produced an illegal move.\n";
                break;
            }

            std::cout << "[AI] Moved to " << Board::coordToString(act.to)
                      << ", removed " << Board::coordToString(act.remove) << "\n";
        } else {
            // Human turn
            std::cout << "[Human] Your turn.\n";

            while (true) {
                Position to = askCoord("  Move to: ");
                Position rem = askCoord("  Remove: ");

                TurnAction act{to, rem};

                
                auto snap = board.snapshot();
                bool ok = board.applyAction(2, act);
                if (ok) {
                    break; 
                }
                board.restore(snap);
                std::cout << "Illegal action. Rules reminder:\n";
                std::cout << "- Move must be to one of 8 neighboring squares\n";
                std::cout << "- Move cannot go to removed square / outside / opponent\n";
                std::cout << "- Remove must be an empty square and cannot contain any piece\n";
                std::cout << "Try again.\n";
            }
        }

      
        turn = (turn == 1 ? 2 : 1);
    }

    std::cout << "Game over.\n";
    return 0;
}
