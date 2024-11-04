#include<iostream>
#include <ctime> 

const int width = 3;
const int height = 3;
int winLength = 3;
int secondsToThink = 1;
int bestMoveY = 0;
int bestMoveX = 0;
int squaresLeft = width * height;
int board[width][height] = {};

int whoWins() {
    if (width >= winLength)
        for (int y = 0; y < height; y++) {
            int counterP = 0;
            int counterN = 0;
            for (int x = 0; x < width; x++) {
                counterP += board[y][x] == 1;
                counterN += board[y][x] == -1;
            }
            if (counterP >= winLength)
                return 1;
            if (counterN >= winLength)
                return -1;
        }
    if (height >= winLength)
        for (int x = 0; x < width; x++) {
            int counterP = 0;
            int counterN = 0;
            for (int y = 0; y < height; y++) {
                counterP += board[y][x] == 1;
                counterN += board[y][x] == -1;
            }
            if (counterP >= winLength)
                return 1;
            if (counterN >= winLength)
                return -1;
        }
    if (width >= winLength && height >= winLength) {
        int perpendicular = width;
        if (height < width)
            perpendicular = height;

        for (int y = 0; y <= height - perpendicular; y++)
            for (int x = 0; x <= width - perpendicular; x++) {
            int counterP = 0;
            int counterN = 0;
                for (int p = 0; p < perpendicular; p++) {
                    counterP += board[p + y][p + x] == 1;
                    counterN += board[p + y][p + x] == -1;
                }
                if (counterP >= winLength)
                    return 1;
                if (counterN >= winLength)
                    return -1;
                }
        for (int y = 0; y <= height - perpendicular; y++)
            for (int x = 0; x <= width - perpendicular; x++) {
            int counterP = 0;
            int counterN = 0;
                for (int p = 0; p < perpendicular; p++) {
                    counterP += board[p + y][width - (p + x) - 1] == 1;
                    counterN += board[p + y][width - (p + x) - 1] == -1;
                }
                if (counterP >= winLength)
                    return 1;
                if (counterN >= winLength)
                    return -1;
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
                if (score >= beta)
                    return bestValue;
        }
    if (firstIteration) {
        bestMoveX = x;
        bestMoveY = y;
        }
    return bestValue;
}


void takeTurn(int turn) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
            std::cout<<board[y][x]<<"\t";
        std::cout<<"\n";
        }
    
    time_t start, end; 
    time(&start);
    time(&end);
    int evaluation = 0;
    int depth = 1;
    while (double(end - start) < secondsToThink && squaresLeft >= depth) {
        evaluation = alphaBeta(-2, 2, depth, true, -1);
        depth++;
        time(&end);
    }
    std::cout<<"Evaluation: "<<evaluation<<" Best move: "<<bestMoveX<<" "<<bestMoveY<<"\n"; 
    
    std::cout<<"Enter your move as zero-based coordinates: ";

    int moveY = 0, moveX = 0;
    std::cin>>moveX>>moveY;

    while (!(moveY >= 0 && moveY < height && moveX >= 0 && moveX < width && board[moveY][moveX] == 0)) {
        std::cout<<"Incorrect coordinates. Enter coordinates for not taken position within bounds: ";
        std::cin>>moveX>>moveY;
    }

    board[moveY][moveX] = turn;
}


int main() {
    int turn = 1;
    while (squaresLeft != 0 && whoWins() == 0) {
        takeTurn(turn);
        turn *= -1;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
            std::cout<<board[y][x]<<"\t";
        std::cout<<"\n";
        }
        
    if (whoWins() == 0)
        std::cout<<"Draw";
    else
        std::cout<<-turn<<" won";
}