# Tic Tac Toe Solver
This repo uses techniques known in chess programming such as MinMax, Alpha Beta Pruning and Iterative Deepening to solve the game of Tic Tac Toe for any width and height of the board as well as number of ticks in row needed to win.


There are 3 programs in this repo:
- main.cpp - which is bare backend with minimal UI,
- gui.cpp - same as main.cpp but with GUI,
- guiSimplified.cpp - my failed attempt to optimize backend.

## How to run
I use this command to compile programs with gui:

`g++ $(pkg-config --cflags --libs Qt6Widgets) -fPIC -Ofast gui.cpp`.

And this to compile console programs:

`g++ -fPIC -Ofast main.cpp`.

[Qt](https://www.qt.io/) library is needed for gui programs.