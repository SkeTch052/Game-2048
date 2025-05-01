#include "GameBoardWidget.h"
#include <QPainter>
#include <QKeyEvent>
#include "Tile.h"


GameBoardWidget::GameBoardWidget(int boardSize, QWidget *parent) :
    QWidget(parent),
    board(boardSize),
    firstMove(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(350, 350);
}

bool GameBoardWidget::hasMoved() const { return firstMove; }

void GameBoardWidget::reset()
{
    board.reset();
    firstMove = false;
    movesBlocked = false;
    update();
}

void GameBoardWidget::blockMoves() { movesBlocked = true; }

GameBoard& GameBoardWidget::getBoard() { return board; }

int GameBoardWidget::getScore() const { return board.getScore(); }

void GameBoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Arial", 20, QFont::Bold));

    // Размер поля и отступы
    int boardSize = board.getSize();
    int spacing = (boardSize == 4) ? 6 : 5; // Отступ: 6 пикселей для 4x4, 5 пикселей для 5x5
    int totalSpacing = spacing * (boardSize + 1); // Общий отступ
    int tileSize = (350 - totalSpacing) / boardSize; // Размер плитки

    // Фон поля
    painter.fillRect(0, 0, width(), height(), QColor(187, 173, 160));

    // Отрисовка плиток
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            // Координаты с учётом отступов
            int x = spacing + col * (tileSize + spacing);
            int y = spacing + row * (tileSize + spacing);
            Tile tile = board.getTile(row, col);
            tile.draw(painter, x, y, tileSize, tileSize);
        }
    }
}

void GameBoardWidget::keyPressEvent(QKeyEvent *event)
{
    if (movesBlocked) {
            return;
        }
    bool moved = false;
    bool was2048 = board.has2048();

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        moved = board.moveUp();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        moved = board.moveDown();
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        moved = board.moveLeft();
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        moved = board.moveRight();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    if (moved) {
        if (!firstMove) {
            firstMove = true;
            emit firstMoveMade();
        }
        update();
        if (!was2048 && board.has2048()) {
                    emit gameWon();
                }
    }
}
