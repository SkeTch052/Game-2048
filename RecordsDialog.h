#ifndef RECORDSDIALOG_H
#define RECORDSDIALOG_H

#include <QDialog>
#include <QList>
#include <QPair>


class QTableWidget;
class QPushButton;

class RecordsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RecordsDialog(QWidget *parent = nullptr);
    static void readRecords(QList<QPair<QPair<int, int>, bool>>& records4x4, QList<QPair<QPair<int, int>, bool>>& records5x5);
    static void writeRecords(const QList<QPair<QPair<int, int>, bool>>& records4x4, const QList<QPair<QPair<int, int>, bool>>& records5x5);

private slots:
    void onCloseClicked();

private:
    void loadRecords();
    QTableWidget *table; // Таблица для отображения рекордов
    QPushButton *closeButton;
};

#endif // RECORDSDIALOG_H
