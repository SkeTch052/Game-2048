#include <QApplication>
#include <QIcon>
#include "MainMenu.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/gameIcon.ico"));
    MainMenu menu;
    menu.show();
    return a.exec();
}
