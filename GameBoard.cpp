#include "GameBoard.h"
#include <QRandomGenerator>
#include <QTime>
#include <QDebug>
#include <algorithm>
#include <stdexcept>
#include <QVector>


GameBoard::GameBoard(int size) :
    score(0),
    size(size),
    rng(new QRandomGenerator(QTime::currentTime().msec()))
{
    if (size < 4 || size > 5) {
        throw std::invalid_argument("Board size must be 4 or 5");
    }
    grid.resize(size);
    for (int i = 0; i < size; ++i) {
        grid[i].resize(size);
    }
    reset();
}

GameBoard::~GameBoard() { delete rng; }

void GameBoard::reset()
{
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            grid[i][j] = Tile(0);
        }
    }
    score = 0;
    spawnRandomTile();
    spawnRandomTile();
}

bool GameBoard::processRow(int row, Tile* line, int size)
{
    bool moved = false;
    int pos = 0;
    for (int i = 0; i < size; ++i) {
        if (line[i].getValue() != 0) {
            if (pos != i) {
                line[pos] = line[i];
                line[i] = Tile(0);
                moved = true;
            }
            pos++;
        }
    }
    for (int i = 0; i < size - 1; ++i) {
        if (line[i].getValue() != 0 && line[i].getValue() == line[i + 1].getValue()) {
            line[i] = Tile(line[i].getValue() * 2);
            line[i + 1] = Tile(0);
            score += line[i].getValue();
            moved = true;
        }
    }
    pos = 0;
    for (int i = 0; i < size; ++i) {
        if (line[i].getValue() != 0) {
            if (pos != i) {
                line[pos] = line[i];
                line[i] = Tile(0);
                moved = true;
            }
            pos++;
        }
    }
    return moved;
}

bool GameBoard::shiftTiles(int direction)
{
    bool moved = false;
    Tile temp[5]; // Максимум 5 для 5x5

    if (direction == LEFT) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) temp[j] = grid[i][j];
            if (processRow(i, temp, size)) moved = true;
            for (int j = 0; j < size; ++j) grid[i][j] = temp[j];
        }
    } else if (direction == RIGHT) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) temp[j] = grid[i][size - 1 - j];
            if (processRow(i, temp, size)) moved = true;
            for (int j = 0; j < size; ++j) grid[i][size - 1 - j] = temp[j];
        }
    } else if (direction == UP) {
        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) temp[i] = grid[i][j];
            if (processRow(j, temp, size)) moved = true;
            for (int i = 0; i < size; ++i) grid[i][j] = temp[i];
        }
    } else if (direction == DOWN) {
        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) temp[i] = grid[size - 1 - i][j];
            if (processRow(j, temp, size)) moved = true;
            for (int i = 0; i < size; ++i) grid[size - 1 - i][j] = temp[i];
        }
    }

    return moved;
}

bool GameBoard::performMove(int direction)
{
    bool moved = shiftTiles(direction);
    if (moved) {
        spawnRandomTile();
        emit scoreChanged(score);
        if (isGameOver()) {
            emit gameOver();
        }
    }
    return moved;
}

bool GameBoard::moveLeft() { return performMove(LEFT); }

bool GameBoard::moveRight() { return performMove(RIGHT); }

bool GameBoard::moveUp() { return performMove(UP); }

bool GameBoard::moveDown() { return performMove(DOWN); }

bool GameBoard::isGameOver() const { return !canMove(); }

int GameBoard::getScore() const { return score; }

int GameBoard::getSize() const { return size; }

bool GameBoard::canMove() const
{
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (grid[i][j].getValue() == 0) return true;
            if (j < size - 1 && grid[i][j].getValue() == grid[i][j + 1].getValue()) return true;
            if (i < size - 1 && grid[i][j].getValue() == grid[i + 1][j].getValue()) return true;
        }
    }
    return false;
}

const Tile& GameBoard::getTile(int row, int col) const
{
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw std::out_of_range("Tile index out of bounds");
    }
    return grid[row][col];
}

void GameBoard::spawnRandomTile()
{
    QVector<QPoint> emptyCells;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (grid[i][j].getValue() == 0) {
                emptyCells.push_back(QPoint(i, j));
            }
        }
    }
    if (!emptyCells.isEmpty()) {
        QPoint pos = emptyCells[rng->bounded(emptyCells.size())];
        grid[pos.x()][pos.y()] = Tile(rng->bounded(10) < 9 ? 2 : 4);
    }
}

bool GameBoard::has2048() const
{
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (grid[i][j].getValue() == 2048) {
                return true;
            }
        }
    }
    return false;
}
