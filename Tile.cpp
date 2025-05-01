#include "tile.h"


Tile::Tile(int value) : value(value) {}

int Tile::getValue() const { return value; }

QColor Tile::getColor() const
{
    // Цвета для разных значений
    switch (value) {
        case 2:    return QColor(238, 228, 218);
        case 4:    return QColor(237, 224, 200);
        case 8:    return QColor(242, 177, 121);
        case 16:   return QColor(245, 149, 99);
        case 32:   return QColor(246, 124, 95);
        case 64:   return QColor(246, 94, 59);
        case 128:  return QColor(237, 207, 114);
        case 256:  return QColor(237, 204, 97);
        case 512:  return QColor(237, 200, 80);
        case 1024: return QColor(237, 197, 63);
        case 2048: return QColor(237, 194, 46);
        default:   return QColor(205, 193, 180); // Пустая плитка
    }
}

void Tile::draw(QPainter &painter, int x, int y, int width, int height) const
{
    // Заливка плитки цветом
    painter.setBrush(getColor());
    painter.drawRect(x, y, width, height);

    // Текст числа
    if (value != 0) {
        painter.setPen(Qt::black);
        painter.drawText(x, y, width, height, Qt::AlignCenter, QString::number(value));
    }
}
