#include <bitset>
#include <tuple>
#include <vector>
#include <ctime>
#include <array>
#include <stdexcept>

template <size_t width, size_t height, size_t winLength>
class Solver
{
private:
    std::tuple<std::bitset<width * height>, std::bitset<width * height>> boards;
    std::vector<std::bitset<width * height>> winningPositions;
    void makeListOfWinningPositions();
    std::tuple<int, int, int> alphaBeta(int alpha, int beta, int depthLeft, int turn, time_t end);
    bool inBounds(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; }
public:
    Solver();
    int whoWins() const;
    bool isBoardFull() const;
    int squaresLeft() const;
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
bool Solver<width, height, winLength>::isBoardFull() const
{
    return squaresLeft() == 0;
}

template <size_t width, size_t height, size_t winLength>
int Solver<width, height, winLength>::squaresLeft() const
{
    return width * height - (std::get<0>(boards) | std::get<1>(boards)).count();
}

template <size_t width, size_t height, size_t winLength>
std::tuple<int, int, int> Solver<width, height, winLength>::alphaBeta(int alpha, int beta, int depthLeft, int turn, time_t end)
{
    int win = whoWins();
    if (win != 0 || isBoardFull() || depthLeft == 0 || end < std::time(nullptr))
        return std::make_tuple(0, 0, turn * win);

    int bestValue = -2;
    int x = -1;
    int y = -1;
    for (size_t moveX = 0; moveX < width; moveX++)
        for (size_t moveY = 0; moveY < height; moveY++)
            if (!std::get<0>(boards).test(moveX + moveY * width) && !std::get<1>(boards).test(moveX + moveY * width))
            {
                if (turn == 1)
                    std::get<0>(boards).set(moveX + moveY * width);
                else
                    std::get<1>(boards).set(moveX + moveY * width);

                int score = -std::get<2>(alphaBeta(-beta, -alpha, depthLeft - 1, -turn, end));

                if (turn == 1)
                    std::get<0>(boards).reset(moveX + moveY * width);
                else
                    std::get<1>(boards).reset(moveX + moveY * width);

                if (score == 1)
                    return std::make_tuple(moveX, moveY, 1);

                if (score > bestValue)
                {
                    bestValue = score;
                    if (score > alpha)
                    {
                        alpha = score;
                        x = moveX;
                        y = moveY;
                    }
                }
                if (score >= beta)
                    return std::make_tuple(x, y, bestValue);
            }
    return std::make_tuple(x, y, bestValue);
}

template <size_t width, size_t height, size_t winLength>
std::tuple<int, int, int> Solver<width, height, winLength>::getBestMove(int turn, int depth, time_t end)
{
    return alphaBeta(-2, 2, depth, turn, end);
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