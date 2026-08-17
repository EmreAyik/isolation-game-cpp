#include "Board.h"
#include <iostream>
#include <iomanip>
#include <cctype>

Board::Board() {
    grid.assign(N, std::vector<Cell>(N, Cell::Empty));

    
    p1 = {0, 3};
    p2 = {6, 3};

    grid[p1.r][p1.c] = Cell::P1;
    grid[p2.r][p2.c] = Cell::P2;
}

bool Board::inBounds(const Position& p) const {
    return p.r >= 0 && p.r < N && p.c >= 0 && p.c < N;
}

bool Board::isNeighbor8(const Position& from, const Position& to) const {
    int dr = std::abs(from.r - to.r);
    int dc = std::abs(from.c - to.c);
    return (dr <= 1 && dc <= 1 && !(dr == 0 && dc == 0));
}

Position Board::currentPos(int player) const {
    return (player == 1) ? p1 : p2;
}

void Board::setCurrentPos(int player, const Position& p) {
    if (player == 1) p1 = p;
    else p2 = p;
}

bool Board::isOccupiedByAnyPiece(const Position& p) const {
    return grid[p.r][p.c] == Cell::P1 || grid[p.r][p.c] == Cell::P2;
}

bool Board::canMove(int player, const Position& to) const {
    Position from = currentPos(player);

    if (!inBounds(to)) return false;
    if (!isNeighbor8(from, to)) return false;
    if (grid[to.r][to.c] != Cell::Empty) return false;

    return true;
}

bool Board::canRemove(const Position& sq) const {
    if (!inBounds(sq)) return false;
    if (grid[sq.r][sq.c] != Cell::Empty) return false;
    return true;
}

bool Board::hasAnyMove(int player) const {
    Position from = currentPos(player);

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            Position to{from.r + dr, from.c + dc};
            if (inBounds(to) && grid[to.r][to.c] == Cell::Empty)
                return true;
        }
    }
    return false;
}

bool Board::applyAction(int player, const TurnAction& act) {
    if (!canMove(player, act.to)) return false;
    if (!canRemove(act.remove)) return false;

    Position from = currentPos(player);

    // move piece
    grid[from.r][from.c] = Cell::Empty;
    setCurrentPos(player, act.to);
    grid[act.to.r][act.to.c] = (player == 1 ? Cell::P1 : Cell::P2);

    // remove square
    grid[act.remove.r][act.remove.c] = Cell::Removed;

    return true;
}

Board::Snapshot Board::snapshot() const {
    Snapshot s;
    s.grid = grid;
    s.p1 = p1;
    s.p2 = p2;
    return s;
}

void Board::restore(const Snapshot& snap) {
    grid = snap.grid;
    p1 = snap.p1;
    p2 = snap.p2;
}

std::vector<TurnAction> Board::generateActions(int player) const {
    std::vector<TurnAction> actions;
    Position from = currentPos(player);

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;

            Position to{from.r + dr, from.c + dc};
            if (!inBounds(to)) continue;
            if (grid[to.r][to.c] != Cell::Empty) continue;

            // After moving, any empty square can be removed
            for (int r = 0; r < N; r++) {
                for (int c = 0; c < N; c++) {
                    if (grid[r][c] == Cell::Empty) {
                        Position rem{r, c};
                        actions.push_back({to, rem});
                    }
                }
            }
        }
    }
    return actions;
}

void Board::print() const {
    std::cout << "\n    1 2 3 4 5 6 7\n";
    for (int r = 0; r < N; r++) {
        char rowChar = 'a' + r;
        std::cout << " " << rowChar << "  ";
        for (int c = 0; c < N; c++) {
            char ch = '.';
            if (grid[r][c] == Cell::Removed) ch = 'x';
            else if (grid[r][c] == Cell::P1) ch = 'A'; 
            else if (grid[r][c] == Cell::P2) ch = 'H'; 
            std::cout << ch << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool Board::parseCoord(const std::string& s, Position& out) {
    if (s.size() != 2) return false;

    char row = std::tolower(s[0]);
    char col = s[1];

    if (row < 'a' || row > 'g') return false;
    if (col < '1' || col > '7') return false;

    out.r = row - 'a';
    out.c = col - '1';
    return true;
}

std::string Board::coordToString(const Position& p) {
    std::string s;
    s.push_back(char('a' + p.r));
    s.push_back(char('1' + p.c));
    return s;
}
