#include <QApplication>
#include <QCoreApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QFont>
#include <ctime> 

const int width = 3;
const int height = 3;
int winLength = 3;
int secondsToThink = 1;
time_t start, end;
int bestMoveY = 0;
int bestMoveX = 0;
int squaresLeft = width * height;
int board[height][width] = {};

int whoWins() {
    if (width >= winLength)
        for (int y = 0; y < height; y++) {
            int counterP = 0;
            int counterN = 0;
                for (int x = 0; x < width; x++) {
                    if (board[y][x] == 1){
                        counterP++;
                        if (counterP == winLength)
                            return 1;
                    } else
                        counterP = 0;

                    if (board[y][x] == -1){
                        counterN++;
                        if (counterN == winLength)
                            return -1;
                    } else
                        counterN = 0;
                }
            }
            
    if (height >= winLength)
        for (int x = 0; x < width; x++) {
            int counterP = 0;
            int counterN = 0;
                for (int y = 0; y < height; y++) { 
                    if (board[y][x] == 1){
                        counterP++;
                        if (counterP == winLength)
                            return 1;
                    } else
                        counterP = 0;

                    if (board[y][x] == -1){
                        counterN++;
                        if (counterN == winLength)
                            return -1;
                    } else
                        counterN = 0;
                }
            }

    if (width >= winLength && height >= winLength) {    
        for (int y = 0; y <= height - winLength; y++)
            for (int x = 0; x <= width - winLength; x++) {
            int counterP = 0;
            int counterN = 0;
                for (int p = 0; p < winLength; p++) {
                    if (board[p + y][p + x] == 1){
                        counterP++;
                        if (counterP == winLength)
                            return 1;
                    } else
                        counterP = 0;

                    if (board[p + y][p + x] == -1){
                        counterN++;
                        if (counterN == winLength)
                            return -1;
                    } else
                        counterN = 0;
                }
            }

        for (int y = 0; y <= height - winLength; y++)
            for (int x = 0; x <= width - winLength; x++) {
                int counterP = 0;
                int counterN = 0;
                for (int p = 0; p < winLength; p++) {
                    if (board[p + y][width - (p + x) - 1] == 1){
                        counterP++;
                        if (counterP == winLength)
                            return 1;
                    } else
                        counterP = 0;

                    if (board[p + y][width - (p + x) - 1] == -1){
                        counterN++;
                        if (counterN == winLength)
                            return -1;
                    } else
                        counterN = 0;
                }
            }
    }
    return 0;
}

int alphaBeta(int alpha, int beta, int depthLeft, bool firstIteration, int turn) {
    int win = whoWins();
    if (win != 0 || squaresLeft == 0)
        return turn * win;
    
    if (depthLeft == 0)
        return 0;

    time(&end);
    if (double(end - start) > secondsToThink)
        return 0;

    int bestValue = -2;
    int x = 0;
    int y = 0;
    for (int moveY = 0; moveY < height; moveY++)
        for (int moveX = 0; moveX < width; moveX++)
            if (board[moveY][moveX] == 0) {
                board[moveY][moveX] = turn;
                squaresLeft--;
                int score = -alphaBeta(-beta, -alpha, depthLeft - 1, false, -turn);
                board[moveY][moveX] = 0;
                squaresLeft++;
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
    window.setFixedSize(50 + 100 * width, 50 + 100 * height + 20);

    QVBoxLayout *mainLayout = new QVBoxLayout(&window);

    QLabel *dynamicLabel = new QLabel("", &window);
    dynamicLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(dynamicLabel);
    dynamicLabel->setFixedHeight(20);
    
    QFont buttonFont;
    buttonFont.setPointSize(80);
    
    QGridLayout *layout = new QGridLayout;
    QVector<QVector<QPushButton*>> buttons(height, QVector<QPushButton*>(width));

    int turn = 1;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            QPushButton *button = new QPushButton("", &window);
            layout->addWidget(button, row, col);
            button->setFixedSize(100, 100);
            button->setFont(buttonFont);
            buttons[row][col] = button;

            QObject::connect(button, &QPushButton::clicked, [button, row, col, &buttons, &dynamicLabel]() {
                board[row][col] = 1;
                squaresLeft--;
                button->setText("O");
                for (int r = 0; r < height; ++r) {
                    for (int c = 0; c < width; ++c) {
                        buttons[r][c]->setEnabled(false);
                    }
                }

                int win = whoWins();
                if (win == 1)
                    dynamicLabel->setText("You won");
                else if (win == -1)
                    dynamicLabel->setText("You lost");
                else if (squaresLeft == 0)
                    dynamicLabel->setText("Draw");

                QCoreApplication::processEvents();

                time(&start);
                time(&end);
                
                int depth = 1;
                while (double(end - start) < secondsToThink && squaresLeft >= depth) {
                    int evaluation = alphaBeta(-2, 2, depth, true, -1);
                    dynamicLabel->setText(QString("Depth: %1 \t Evaluation: %2").arg(depth - 1).arg(-evaluation));
                    QCoreApplication::processEvents();
                    depth++;
                    time(&end);
                }

                board[bestMoveY][bestMoveX] = -1;
                squaresLeft--;
                buttons[bestMoveY][bestMoveX]->setText("X");

                
                win = whoWins();
                if (win == 1)
                    dynamicLabel->setText("You won");
                else if (win == -1)
                    dynamicLabel->setText("You lost");
                else if (squaresLeft == 0)
                    dynamicLabel->setText("Draw");
                else {
                    for (int r = 0; r < height; ++r) {
                        for (int c = 0; c < width; ++c) {
                            if (board[r][c] == 0)
                                buttons[r][c]->setEnabled(true);
                        }
                    }
                }

                QCoreApplication::processEvents();

            });
        }
    }

    mainLayout->addLayout(layout);

    window.setLayout(mainLayout);
    window.show();

    return app.exec();
}