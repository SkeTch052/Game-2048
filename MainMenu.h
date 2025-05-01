#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include <QCloseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QDialog
{
    Q_OBJECT
public:
    explicit MainMenu(int lastSize = 4,QWidget *parent = nullptr);
    ~MainMenu();
    int getSelectedBoardSize() const;

private slots:
    void onStartClicked();
    void onRecordsClicked();
    void onExitClicked();

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainMenu *ui;
    int lastSize;
};

#endif // MAINMENU_H
