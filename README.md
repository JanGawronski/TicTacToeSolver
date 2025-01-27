# Tic Tac Toe Solver
This repo uses techniques known in chess programming such as MinMax, Alpha Beta Pruning and Iterative Deepening to solve the game of Tic Tac Toe for any width and height of the board as well as number of ticks in row needed to win.

## How to run
There is a Makefile for linux in the repo. To compile the program run `make` in the terminal. This will compile the program with the following command:

`g++ $(pkg-config --cflags --libs Qt6Widgets) -fPIC -Ofast gui.cpp`.

And this to compile console programs:

`g++ -fPIC -Ofast main.cpp`.

[Qt](https://www.qt.io/) library is needed for gui programs. I use Qt6 but it should work with Qt5 as well.