#include "RecordsDialog.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QPushButton>


RecordsDialog::RecordsDialog(QWidget *parent) :
        QDialog(parent),
        table(new QTableWidget(this)),
        closeButton(new QPushButton("Закрыть", this))
{
    setWindowTitle("Рекорды");
    setFixedSize(420, 379);

    table->setRowCount(0);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"4x4 Счёт", "4x4 Время", "5x5 Счёт", "5x5 Время"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(table);
    layout->addWidget(closeButton);
    setLayout(layout);

    connect(closeButton, &QPushButton::clicked, this, &RecordsDialog::onCloseClicked);
    loadRecords();
}

void RecordsDialog::onCloseClicked() { accept(); }

void RecordsDialog::readRecords(QList<QPair<QPair<int, int>, bool>>& records4x4, QList<QPair<QPair<int, int>, bool>>& records5x5)
{
    QFile file("stats.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split("=");
            if (parts.size() != 2) continue;
            QStringList values = parts[1].split(",");
            if (values.size() < 2) continue;
            int score = values[0].toInt();
            int seconds = values[1].toInt();
            bool won = (values.size() >= 3) ? (values[2].toInt() != 0) : false; // Обратная совместимость
            QPair<QPair<int, int>, bool> record = {{score, seconds}, won};
            if (line.startsWith("4x4=")) {
                records4x4.append(record);
            } else if (line.startsWith("5x5=")) {
                records5x5.append(record);
            }
        }
        file.close();
    }
}

void RecordsDialog::writeRecords(const QList<QPair<QPair<int, int>, bool>>& records4x4, const QList<QPair<QPair<int, int>, bool>>& records5x5)
{
    QFile file("stats.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        for (const auto& record : records4x4) {
            out << "4x4=" << record.first.first << "," << record.first.second << "," << (record.second ? 1 : 0) << "\n";
        }
        for (const auto& record : records5x5) {
            out << "5x5=" << record.first.first << "," << record.first.second << "," << (record.second ? 1 : 0) << "\n";
        }
        file.close();
    }
}

void RecordsDialog::loadRecords()
{
  table->setRowCount(0); // Очистка таблицы
  QList<QPair<QPair<int, int>, bool>> records4x4, records5x5;
  readRecords(records4x4, records5x5);


  int maxRows = qMax(records4x4.size(), records5x5.size());
  for (int i = 0; i < qMin(maxRows, 10); ++i) {
    int row = table->rowCount();
    table->insertRow(row);

    // 4x4 Счёт и Время
    if (i < records4x4.size()) {
      int score = records4x4[i].first.first;
      int seconds = records4x4[i].first.second;
      bool won = records4x4[i].second;
      int minutes = seconds / 60;
      int secs = seconds % 60;
      QString time = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));

      QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(score));
      QTableWidgetItem *timeItem = new QTableWidgetItem(time);

      if (won) {
        scoreItem->setBackground(QColor(247, 204, 66));
        timeItem->setBackground(QColor(247, 204, 66));
      }

      table->setItem(row, 0, scoreItem);
      table->setItem(row, 1, timeItem);
    }

    // 5x5 Счёт и Время
    if (i < records5x5.size()) {
      int score = records5x5[i].first.first;
      int seconds = records5x5[i].first.second;
      bool won = records5x5[i].second;
      int minutes = seconds / 60;
      int secs = seconds % 60;
      QString time = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));

      QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(score));
      QTableWidgetItem *timeItem = new QTableWidgetItem(time);

      if (won) {
        scoreItem->setBackground(QColor(237, 194, 46));
        timeItem->setBackground(QColor(237, 194, 46));
      }

      table->setItem(row, 2, scoreItem);
      table->setItem(row, 3, timeItem);
    }
  }
}
