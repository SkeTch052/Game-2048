#include "MainMenu.h"
#include "ui_MainMenu.h"
#include "GameWindow.h"
#include "RecordsDialog.h"
#include <QButtonGroup>
#include <QApplication>
#include <QIcon>


MainMenu::MainMenu(int lastSize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainMenu),
    lastSize(lastSize)
{
    ui->setupUi(this);
    setFixedSize(400, 350);
    setWindowTitle("Игра-2048");
    setStyleSheet("QDialog { background-color: rgb(238, 228, 218); }");

    QButtonGroup *sizeGroup = new QButtonGroup(this);
    sizeGroup->addButton(ui->size4x4Button);
    sizeGroup->addButton(ui->size5x5Button);
    sizeGroup->setExclusive(true);
    ui->size4x4Button->setChecked(true);

    ui->startButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F65E3B;"
        "   color: white;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #A64B00;"
        "}"
    );

    ui->recordsButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #846F5E;"
        "   color: white;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #584F47;"
        "}"
    );

    ui->exitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #846F5E;"
        "   color: white;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #584F47;"
        "}"
    );

    connect(ui->startButton, &QPushButton::clicked, this, &MainMenu::onStartClicked);
    connect(ui->recordsButton, &QPushButton::clicked, this, &MainMenu::onRecordsClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainMenu::onExitClicked);
}

MainMenu::~MainMenu() { delete ui; }

int MainMenu::getSelectedBoardSize() const
{
    if (ui->size4x4Button->isChecked()) return 4;
    if (ui->size5x5Button->isChecked()) return 5;
    return 4;
}

void MainMenu::onStartClicked()
{
    lastSize = getSelectedBoardSize();
    GameWindow *game = new GameWindow(lastSize, nullptr);
    game->show();
    hide();
}

void MainMenu::onRecordsClicked()
{
    RecordsDialog *dialog = new RecordsDialog(this);
    dialog->exec();
}

void MainMenu::onExitClicked() { QApplication::quit(); }

void MainMenu::showEvent(QShowEvent *event)
{
    if (lastSize == 4) {
        ui->size4x4Button->setChecked(true);
    } else if (lastSize == 5) {
        ui->size5x5Button->setChecked(true);
    }
    QDialog::showEvent(event);
}

void MainMenu::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
    event->accept();
}
