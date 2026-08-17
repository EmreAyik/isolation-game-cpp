# isolation-game-cpp

A terminal-based Isolation board game implemented in C++ with a Minimax AI opponent.

## What is Isolation?

Isolation is a two-player strategy game played on a 7x7 board. Each turn, a player moves their piece to an adjacent square, then removes any empty square from the board. The player who cannot move loses.

## Features

- 7x7 board with terminal display
- Human vs AI gameplay
- Minimax algorithm (depth 3) for AI decisions
- Illegal move detection and validation

## How to Compile

```bash
g++ -o game main.cpp Board.cpp AI.cpp
```

## How to Play

```bash
./game
```

- Enter coordinates in `a1..g7` format
- Each turn: first move your piece, then remove a square
- Type `q` to quit

## Tech Stack

- C++
- Minimax AI algorithm
