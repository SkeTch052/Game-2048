#ifndef GAMEBOARDWIDGET_H
#define GAMEBOARDWIDGET_H

#include <QWidget>
#include "GameBoard.h"


class GameBoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameBoardWidget(int boardSize, QWidget *parent = nullptr);
    bool hasMoved() const;
    void reset();
    void blockMoves();
    GameBoard& getBoard();
    int getScore() const;

signals:
    void firstMoveMade();
    void gameWon();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    GameBoard board;
    bool firstMove;
    bool movesBlocked;
};

#endif // GAMEBOARDWIDGET_H
