#include <bitset>
#include <tuple>
#include <vector>
#include <ctime>
#include <array>
#include <stdexcept>
#include <unordered_map>

template <size_t width, size_t height, size_t winLength>
class Solver
{
private:
    std::tuple<std::bitset<width * height>, std::bitset<width * height>> boards;
    std::vector<std::bitset<width * height>> winningPositions;
    std::unordered_map<std::bitset<width * height>, std::unordered_map<std::bitset<width * height>, int>> endgameCache;
    std::unordered_map<std::bitset<width * height>, std::unordered_map<std::bitset<width * height>, int>> tempCache;
    void makeListOfWinningPositions();
    int alphaBeta(int alpha, int beta, int depthLeft, int turn, time_t end);
    bool inBounds(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; };
    std::vector<std::tuple<int, int>> getPossibleMoves() const;
public:
    Solver();
    int whoWins() const;
    bool isBoardFull() const { return (std::get<0>(boards) | std::get<1>(boards)).all(); };
    int squaresLeft() const { return width * height - std::get<0>(boards).count() - std::get<1>(boards).count(); };
    std::tuple<int, int, int> getBestMove(int turn, int depth, time_t end);
    void makeMove(int x, int y, int turn);
    int getPosition(int x, int y) const;
};

template <size_t width, size_t height, size_t winLength>
Solver<width, height, winLength>::Solver()
{
    std::get<0>(boards) = std::bitset<width * height>();
    std::get<1>(boards) = std::bitset<width * height>();
    makeListOfWinningPositions();
}

template <size_t width, size_t height, size_t winLength>
void Solver<width, height, winLength>::makeListOfWinningPositions()
{
    for (size_t i = 0; i < width - winLength + 1; i++)
        for (size_t j = 0; j < height; j++)
        {
            std::bitset<width * height> position;
            for (size_t k = 0; k < winLength; k++)
                position.set(i + j * width + k);
            winningPositions.push_back(position);
        }
    
    for (size_t i = 0; i < width; i++)
        for (size_t j = 0; j < height - winLength + 1; j++)
        {
            std::bitset<width * height> position;
            for (size_t k = 0; k < winLength; k++)
                position.set(i + (j + k) * width);
            winningPositions.push_back(position);
        }
    
    for (size_t i = 0; i < width - winLength + 1; i++)
        for (size_t j = 0; j < height - winLength + 1; j++)
        {
            std::bitset<width * height> position;
            for (size_t k = 0; k < winLength; k++)
                position.set(i + (j + k) * width + k);
            winningPositions.push_back(position);
        }
    
    for (size_t i = 0; i < width - winLength + 1; i++)
        for (size_t j = 0; j < height - winLength + 1; j++)
        {
            std::bitset<width * height> position;
            for (size_t k = 0; k < winLength; k++)
                position.set(i + (j + k) * width + winLength - k - 1);
            winningPositions.push_back(position);
        }
        
}

template <size_t width, size_t height, size_t winLength>
int Solver<width, height, winLength>::whoWins() const
{
    for (const auto &position : winningPositions)
    {
        if ((std::get<0>(boards) & position) == position)
            return 1;
        if ((std::get<1>(boards) & position) == position)
            return -1;
    }
    return 0;
}

template <size_t width, size_t height, size_t winLength>
int Solver<width, height, winLength>::alphaBeta(int alpha, int beta, int depthLeft, int turn, time_t end)
{
    if (endgameCache.contains(std::get<0>(boards)) && endgameCache[std::get<0>(boards)].contains(std::get<1>(boards)))
        return endgameCache[std::get<0>(boards)][std::get<1>(boards)];

    if (tempCache.contains(std::get<0>(boards)) && tempCache[std::get<0>(boards)].contains(std::get<1>(boards)))
        return tempCache[std::get<0>(boards)][std::get<1>(boards)];

    int win = whoWins();
    if (win != 0 || isBoardFull()) {
        endgameCache[std::get<0>(boards)][std::get<1>(boards)] = turn * win;
        return turn * win;
    }

    if (depthLeft == 0 || end < std::time(nullptr)) {
        tempCache[std::get<0>(boards)][std::get<1>(boards)] = 0;
        return 0;
    }

    int bestValue = -2;
    for (const std::tuple<int, int> &move : getPossibleMoves()) {
        int moveX = std::get<0>(move);
        int moveY = std::get<1>(move);
        if (turn == 1)
            std::get<0>(boards).set(moveX + moveY * width);
        else
            std::get<1>(boards).set(moveX + moveY * width);

        int score = -alphaBeta(-beta, -alpha, depthLeft - 1, -turn, end);

        if (turn == 1)
            std::get<0>(boards).reset(moveX + moveY * width);
        else
            std::get<1>(boards).reset(moveX + moveY * width);

        if (score == 1) {
            endgameCache[std::get<0>(boards)][std::get<1>(boards)] = 1;
            return 1;
        }
        if (score > bestValue)
        {
            bestValue = score;
            if (score > alpha)
                alpha = score;

        }
        if (score >= beta) {
            if (bestValue == 1 || bestValue == -1)
                endgameCache[std::get<0>(boards)][std::get<1>(boards)] = bestValue;
            else
                tempCache[std::get<0>(boards)][std::get<1>(boards)] = bestValue;
            return bestValue;
        }
    }
    if (bestValue == 1 || bestValue == -1)
        endgameCache[std::get<0>(boards)][std::get<1>(boards)] = bestValue;
    else
        tempCache[std::get<0>(boards)][std::get<1>(boards)] = bestValue;
    return bestValue;
}

template <size_t width, size_t height, size_t winLength>
void Solver<width, height, winLength>::makeMove(int x, int y, int turn)
{
    if (!inBounds(x, y))
        throw std::out_of_range("Out of bounds");
    if (getPosition(x, y) != 0)
        throw std::invalid_argument("Position already taken");
    if (turn == 1)
        std::get<0>(boards).set(x + y * width);
    else
        std::get<1>(boards).set(x + y * width);
}

template <size_t width, size_t height, size_t winLength>
int Solver<width, height, winLength>::getPosition(int x, int y) const
{
    if (!inBounds(x, y))
        throw std::out_of_range("Out of bounds");
    if (std::get<0>(boards).test(x + y * width))
        return 1;
    if (std::get<1>(boards).test(x + y * width))
        return -1;
    return 0;
}

template <size_t width, size_t height, size_t winLength>
std::vector<std::tuple<int, int>> Solver<width, height, winLength>::getPossibleMoves() const
{
    std::vector<std::tuple<int, int>> moves;
    for (size_t x = 0; x < width; x++)
        for (size_t y = 0; y < height; y++)
            if (!std::get<0>(boards).test(x + y * width) && !std::get<1>(boards).test(x + y * width))
                moves.push_back(std::make_tuple(x, y));
    return moves;
}

template <size_t width, size_t height, size_t winLength>
std::tuple<int, int, int> Solver<width, height, winLength>::getBestMove(int turn, int depth, time_t end)
{
    tempCache.clear();
    int bestValue = -2;
    int bestX = -1;
    int bestY = -1;
    for (const std::tuple<int, int> &move : getPossibleMoves()) {
        int moveX = std::get<0>(move);
        int moveY = std::get<1>(move);
        if (turn == 1)
            std::get<0>(boards).set(moveX + moveY * width);
        else
            std::get<1>(boards).set(moveX + moveY * width);

        int score = -alphaBeta(-2, 2, depth - 1, -turn, end);

        if (turn == 1)
            std::get<0>(boards).reset(moveX + moveY * width);
        else
            std::get<1>(boards).reset(moveX + moveY * width);

        if (score == 1)
            return std::make_tuple(moveX, moveY, 1);
        if (score > bestValue)
        {
            bestValue = score;
            bestX = moveX;
            bestY = moveY;
        }
    }
    return std::make_tuple(bestX, bestY, bestValue);
}