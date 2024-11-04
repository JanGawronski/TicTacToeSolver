#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QVector>
#include <bits/stdc++.h> 
#include <iostream>

const int dim = 3;
int bestMoveY = 0;
int bestMoveX = 0;
int squaresLeft = dim * dim;
int board[dim][dim] = {};
int yCount[dim] = {};
int xCount[dim] = {};
int proPerpendicular = 0;
int antiPerpedicular = 0;

int alphaBeta(int alpha, int beta, int depthsLeft, bool firstIteration, int turn, int movey, int movex) {
    int win = yCount[movey] == dim || xCount[movex] == dim || proPerpendicular == dim || antiPerpedicular == dim;
    if (yCount[movey] == -dim || xCount[movex] == -dim || proPerpendicular == -dim || antiPerpedicular == -dim)
        win = -1;

    if (win != 0 || squaresLeft == 0)
        return turn * win;

    if (depthsLeft == 0)
        return 0;

    int bestValue = -2;
    int x = 0;
    int y = 0;
    
    for (int moveY = 0; moveY < dim; moveY++)
        for (int moveX = 0; moveX < dim; moveX++)
            if (board[moveY][moveX] == 0) {
                board[moveY][moveX] = turn;

                squaresLeft--;
                yCount[moveY] += turn;
                xCount[moveX] += turn;
                if (moveY == moveX)
                    proPerpendicular += turn;
                if (moveY == dim - moveX - 1)
                    antiPerpedicular += turn;

                int score = -alphaBeta(-beta, -alpha, depthsLeft - 1, false, -turn, moveY, moveX);
                board[moveY][moveX] = 0;
                squaresLeft++;
                yCount[moveY] -= turn;
                xCount[moveX] -= turn;

                if (moveY == moveX)
                    proPerpendicular -= turn;
                if (moveY == dim - moveX - 1)
                    antiPerpedicular -= turn;

                if (score > bestValue) {
                    bestValue = score;
                    if (score > alpha) {
                        alpha = score;
                        x = moveX;
                        y = moveY;
                    }
                }
                if (score >= beta) {
                    if (firstIteration) {
                        bestMoveX = x;
                        bestMoveY = y;
                    }
                    return bestValue;
                }
                if (score == 1) {
                    if (firstIteration) {
                        bestMoveX = x;
                        bestMoveY = y;
                    }
                    return 1;
                }    
            }
    if (firstIteration) {
        bestMoveX = x;
        bestMoveY = y;
        }
    return bestValue;
}


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(50 + 100 * dim, 50 + 100 * dim);


    QGridLayout *layout = new QGridLayout;
    QVector<QVector<QPushButton*>> buttons(dim, QVector<QPushButton*>(dim));

    int turn = 1;


    for (int row = 0; row < dim; ++row) {
        for (int col = 0; col < dim; ++col) {
            QPushButton *button = new QPushButton("", &window);
            layout->addWidget(button, row, col);
            button->setFixedSize(100, 100);
            buttons[row][col] = button;

            QObject::connect(button, &QPushButton::clicked, [button, row, col, &buttons]() {
                if (board[row][col] == 0) {
                    board[row][col] = 1;

                    squaresLeft--;

                    yCount[row]++;
                    xCount[col]++;

                    if (row == col)
                        proPerpendicular++;
                    if (row == dim - col - 1)
                        antiPerpedicular++;

                    button->setText("O");

                    time_t start, end; 
                    time(&start);
                    time(&end);
                    int depth = 0;
                    while (double(end - start) < 1 && squaresLeft >= depth) {
                        std::cout << "Depth " << depth << ": " << alphaBeta(-2, 2, depth, true, -1, row, col) << "\n";
                        depth++;
                        time(&end);
                    }
                        

                    board[bestMoveY][bestMoveX] = -1;

                    squaresLeft--;
                    yCount[bestMoveY]--;
                    xCount[bestMoveX]--;

                    if (bestMoveY == bestMoveX)
                        proPerpendicular--;
                    if (bestMoveY == dim - bestMoveX - 1)
                        antiPerpedicular--;

                    buttons[bestMoveY][bestMoveX]->setText("X");
                }
            });
        }
    }


    window.setLayout(layout);

    window.show();
    return app.exec();
}