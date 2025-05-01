#ifndef TILE_H
#define TILE_H

#include <QColor>
#include <QPainter>


class Tile
{
public:
    Tile(int value = 0);
    int getValue() const;
    QColor getColor() const;
    void draw(QPainter &painter, int x, int y, int width, int height) const;

private:
    int value;
};

#endif // TILE_H
