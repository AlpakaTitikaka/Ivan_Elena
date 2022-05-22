#include "fairy_tail.hpp"

Fairyland::Fairyland()
    : mOutput("output.txt")
    , mTurnCount(0)
{
    std::ifstream file("input.txt");
    check(file.is_open(), "File input.txt not found");

    mMaze.resize(gSize);
    for (int y = 0; y < gSize; ++y)
    {
        mMaze[y].resize(gSize);
        for (int x = 0; x < gSize; ++x)
        {
            char c = 0;
            file >> c;
            check(file.good(), "Invalid input file");

            bool passage = true;
            switch (c)
            {
            case '#':
                passage = false;
                break;

            case '@':
                mIvanPos.first = x;
                mIvanPos.second = y;
                break;

            case '&':
                mElenaPos.first = x;
                mElenaPos.second = y;
                break;

            default:
                check(c == '.', "Invalid input file");
            }

            mMaze[y][x] = passage;
        }
    }
}

Fairyland::~Fairyland()
{
    mOutput << "XX" << std::endl;
}

void Fairyland::check(bool expression, const char* message)
{
    if (!expression)
    {
        std::cerr << message << std::endl;
        throw std::runtime_error(message);
    }
}

int Fairyland::getTurnCount() const
{
    return mTurnCount;
}

bool Fairyland::move(Position& position, Direction direction)
{
    switch (direction)
    {
    case Direction::Up:
        position.second -= 1;
        return position.second >= 0;

    case Direction::Down:
        position.second += 1;
        return position.second < gSize;

    case Direction::Left:
        position.first -= 1;
        return position.first >= 0;

    case Direction::Right:
        position.first += 1;
        return position.first < gSize;

    default:
        return true;
    }
}

bool Fairyland::canGo(Character name, Direction direction) const
{
    Position position = (name == Character::Ivan) ? mIvanPos : mElenaPos;
    return move(position, direction) && mMaze[position.second][position.first];
}

bool Fairyland::go(Direction directionIvan, Direction directionElena)
{
    check(canGo(Character::Ivan, directionIvan), "Invalid Ivan's direction");
    check(canGo(Character::Elena, directionElena), "Invalid Elena's direction");

    mOutput << static_cast<char>(directionIvan) << static_cast<char>(directionElena);
    check(mOutput.good(), "Cannot write to file output.txt");

    mTurnCount += 1;
    check(mTurnCount < 1000000, "Too many turns");

    const Position lastIvanPos = mIvanPos;
    const Position lastElenaPos = mElenaPos;

    move(mIvanPos, directionIvan);
    move(mElenaPos, directionElena);

    return mIvanPos == mElenaPos || lastIvanPos == mElenaPos && lastElenaPos == mIvanPos;
}