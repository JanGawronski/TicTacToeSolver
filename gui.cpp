#include <QApplication>
#include <QCoreApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QFont>
#include "solver.h"
#include <ctime>
#include <tuple>
#include <array>

int main(int argc, char **argv)
{
    const int width = 5;
    const int height = 5;
    const int winLength = 5;
    const int secondsToThink = 1;
    Solver<width, height, winLength> solver;

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

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            QPushButton *button = new QPushButton("", &window);
            layout->addWidget(button, row, col);
            button->setFixedSize(100, 100);
            button->setFont(buttonFont);
            buttons[row][col] = button;

            QObject::connect(button, &QPushButton::clicked, [button, row, col, &buttons, &dynamicLabel, &solver, &secondsToThink]() {
                solver.makeMove(row, col, 1);
                button->setText("O");
                for (int r = 0; r < height; ++r) {
                    for (int c = 0; c < width; ++c) {
                        buttons[r][c]->setEnabled(false);
                    }
                }

                int win = solver.whoWins();
                if (win == 1)
                    dynamicLabel->setText("You won");
                else if (win == -1)
                    dynamicLabel->setText("You lost");
                else if (solver.isBoardFull())
                    dynamicLabel->setText("Draw");

                QCoreApplication::processEvents();

                if (win != 0 || solver.isBoardFull())
                    return;

                time_t end = time(nullptr) + secondsToThink;                
                int depth = 1;
                std::tuple<int, int, int> bestMove = solver.getBestMove(-1, depth, end);
                while (end > time(nullptr) && depth <= solver.squaresLeft()) {
                    bestMove = solver.getBestMove(-1, depth, end);
                    dynamicLabel->setText(QString("Depth: %1 \t Evaluation: %2").arg(depth - 1).arg(-std::get<2>(bestMove)));
                    QCoreApplication::processEvents();
                    depth++;
                }

                solver.makeMove(std::get<0>(bestMove), std::get<1>(bestMove), -1);
                buttons[std::get<0>(bestMove)][std::get<1>(bestMove)]->setText("X");

                win = solver.whoWins();
                if (win == 1)
                    dynamicLabel->setText("You won");
                else if (win == -1)
                    dynamicLabel->setText("You lost");
                else if (solver.isBoardFull())
                    dynamicLabel->setText("Draw");
                else {
                    for (int r = 0; r < height; r++) {
                        for (int c = 0; c < width; c++) {
                            if (solver.getPosition(r, c) == 0)
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