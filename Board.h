#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <utility>

struct Position {
    int r; 
    int c; 

    bool operator==(const Position& other) const {
        return r == other.r && c == other.c;
    }
};

enum class Cell {
    Empty,
    Removed,
    P1, 
    P2  
};

struct TurnAction {
   
    Position to;
    Position remove;
};

class Board {
public:
    static constexpr int N = 7;

    Board(); 

   
    Position getP1Pos() const { return p1; }
    Position getP2Pos() const { return p2; }
    Cell getCell(int r, int c) const { return grid[r][c]; }

 
    bool inBounds(const Position& p) const;
    bool isNeighbor8(const Position& from, const Position& to) const;

    bool canMove(int player, const Position& to) const;        
    bool canRemove(const Position& sq) const;

    bool hasAnyMove(int player) const; 

    
    bool applyAction(int player, const TurnAction& act);

   
    struct Snapshot {
        std::vector<std::vector<Cell>> grid;
        Position p1;
        Position p2;
    };

    Snapshot snapshot() const;
    void restore(const Snapshot& snap);

   
    std::vector<TurnAction> generateActions(int player) const;

   
    void print() const;

    
    static bool parseCoord(const std::string& s, Position& out);
    static std::string coordToString(const Position& p);

private:
    std::vector<std::vector<Cell>> grid;
    Position p1;
    Position p2;

    Position currentPos(int player) const;
    void setCurrentPos(int player, const Position& p);

    bool isOccupiedByAnyPiece(const Position& p) const;
};

#endif
