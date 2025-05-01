#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Tile.h"
#include <QObject>
#include <QVector>


class QRandomGenerator;

class GameBoard : public QObject
{
    Q_OBJECT
public:
    GameBoard(int size = 4);
    ~GameBoard();

    int getScore() const;
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    bool isGameOver() const;
    const Tile& getTile(int row, int col) const;
    void reset();
    int getSize() const;
    bool has2048() const;

private:
    void spawnRandomTile();
    bool canMove() const;
    bool processRow(int row, Tile* line, int size);
    bool shiftTiles(int direction);
    bool performMove(int direction);
    QVector<QVector<Tile>> grid;
    int score;
    int size;
    QRandomGenerator* rng;
    enum MoveDirection { LEFT, RIGHT, UP, DOWN };

signals:
    void scoreChanged(int score);
    void gameOver();
};

#endif // GAMEBOARD_H
