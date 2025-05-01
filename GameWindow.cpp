#include "GameWindow.h"
#include "ui_GameWindow.h"
#include "MainMenu.h"
#include "RecordsDialog.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QPair>
#include <algorithm>


GameWindow::GameWindow(int boardSize, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::GameWindow),
        boardWidget(nullptr),
        boardSize(boardSize),
        timer(nullptr),
        seconds(0),
        timerStarted(false),
        gameEnded(false),
        won(false)
{
    ui->setupUi(this);
    setFixedSize(400, 450);
    setWindowTitle("Игра-2048");
    setStyleSheet("QMainWindow { background-color: rgb(238, 228, 218); }");

    // Замена gameBoardContainer на GameBoardWidget
    boardWidget = new GameBoardWidget(boardSize, this);
    QVBoxLayout *layout = new QVBoxLayout(ui->gameBoardContainer);
    layout->addWidget(boardWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->gameBoardContainer->setLayout(layout);

    // Настройка таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateTimer);
    connect(boardWidget, &GameBoardWidget::firstMoveMade, this, [this]() {
        if (!timerStarted) {
            timerStarted = true;
            timer->start(1000);
        }
    });
    ui->timerLabel->setText("00:00");

    ui->menuButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #FF9640;"
        "   color: white;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #A64B00;"
        "}"
    );

    ui->restartButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #FF9640;"
        "   color: white;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #A64B00;"
        "}"
    );

    connect(ui->menuButton, &QPushButton::clicked, this, &GameWindow::onMenuClicked);
    connect(ui->restartButton, &QPushButton::clicked, this, &GameWindow::onRestartClicked);
    connect(&boardWidget->getBoard(), &GameBoard::gameOver, this, &GameWindow::onGameOver);
    connect(boardWidget, &GameBoardWidget::gameWon, this, &GameWindow::onGameWon);

    boardWidget->setFocus();
}

GameWindow::~GameWindow() { delete ui; }

void GameWindow::onMenuClicked()
{
    if (!gameEnded &&boardWidget->hasMoved()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Выход в меню");
        msgBox.setText("Выйти в меню? Прогресс будет потерян.");
        QAbstractButton *yesButton = msgBox.addButton("Да", QMessageBox::YesRole);
        QAbstractButton *noButton = msgBox.addButton("Нет", QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton() == noButton) {
            return;
        }
    }
    MainMenu *menu = new MainMenu(boardSize, nullptr);
    menu->show();
    hide();
}

void GameWindow::onRestartClicked()
{
    if (boardWidget->hasMoved()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Рестарт");
        msgBox.setText("Перезапустить игру? Прогресс будет потерян.");
        QAbstractButton *yesButton = msgBox.addButton("Да", QMessageBox::YesRole);
        QAbstractButton *noButton = msgBox.addButton("Нет", QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton() == noButton) {
            return;
        }
    }
    seconds = 0;
    timerStarted = false;
    timer->stop();
    ui->timerLabel->setText("00:00");
    boardWidget->reset();
    gameEnded = false;
    won = false;
    boardWidget->setFocus();
}

void GameWindow::updateTimer()
{
    seconds++;
    int minutes = seconds / 60;
    int secs = seconds % 60;
    ui->timerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0')));
}

void GameWindow::saveRecord()
{
    QList<QPair<QPair<int, int>, bool>> records4x4, records5x5;
    RecordsDialog::readRecords(records4x4, records5x5);

    // Добавление нового рекорда
    QPair<QPair<int, int>, bool> newRecord = {{boardWidget->getScore(), seconds}, won};
    if (boardSize == 4) {
        records4x4.append(newRecord);
        std::sort(records4x4.begin(), records4x4.end(),
            [](const QPair<QPair<int, int>, bool>& a, const QPair<QPair<int, int>, bool>& b) {
                return a.first.first > b.first.first;
            });
        if (records4x4.size() > 10) records4x4.resize(10);
    } else {
        records5x5.append(newRecord);
        std::sort(records5x5.begin(), records5x5.end(),
            [](const QPair<QPair<int, int>, bool>& a, const QPair<QPair<int, int>, bool>& b) {
                return a.first.first > b.first.first;
            });
        if (records5x5.size() > 10) records5x5.resize(10);
    }

    RecordsDialog::writeRecords(records4x4, records5x5);
}

void GameWindow::onGameOver()
{
    timer->stop();
    boardWidget->blockMoves();
    gameEnded = true;
    won = false;
    saveRecord();
    QMessageBox::information(this, "Игра окончена", QString("Счёт: %1, Время: %2").arg(boardWidget->getScore()).arg(ui->timerLabel->text()));
}

void GameWindow::onGameWon()
{
    timer->stop();
    boardWidget->blockMoves();
    gameEnded = true;
    won = true;
    saveRecord();
    QMessageBox::information(this, "Победа!", QString("Счёт: %1, Время: %2").arg(boardWidget->getScore()).arg(ui->timerLabel->text()));
}

void GameWindow::closeEvent(QCloseEvent *event)
{
    if (boardWidget->hasMoved()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Закрытие");
        msgBox.setText("Закрыть программу? Прогресс будет потерян.");
        QAbstractButton *yesButton = msgBox.addButton("Да", QMessageBox::YesRole);
        QAbstractButton *noButton = msgBox.addButton("Нет", QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton() == noButton) {
            event->ignore();
            return;
        }
    }
    QApplication::quit();
    event->accept();
}
