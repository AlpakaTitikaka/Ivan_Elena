#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

enum class Character
{
    Ivan,
    Elena,
};

enum class Direction : char
{
    Pass = 'P',
    Up = 'U',
    Down = 'D',
    Left = 'L',
    Right = 'R',
};

class Fairyland
{
    using Position = std::pair<int, int>;

public:
    explicit Fairyland();
    ~Fairyland();

public:
    int getTurnCount() const;
    bool canGo(Character name, Direction direction) const;
    bool go(Direction directionIvan, Direction directionElena);

private:
    static void check(bool expression, const char* message);
    static bool move(Position& position, Direction direction);

private:
    static const std::size_t gSize = 10;
    std::vector<std::vector<bool>> mMaze;
    Position mIvanPos;
    Position mElenaPos;
    std::ofstream mOutput;
    int mTurnCount;
};
