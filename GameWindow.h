#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTimer>
#include "GameBoardWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(int boardSize = 4, QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void onMenuClicked();
    void onRestartClicked();
    void updateTimer();
    void onGameOver();
    void onGameWon();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::GameWindow *ui;
    GameBoardWidget *boardWidget;
    QTimer *timer;
    int seconds;
    bool timerStarted;
    int boardSize;
    bool gameEnded;
    bool won;
    void saveRecord();
};

#endif // GAMEWINDOW_H
