#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QPushButton>
#include <stdlib.h>
#include "treemodel/treemodel.h"
#include "treemodel/namedatedelegate.h"
#include "treemodel/commomtextdelegate.h"

namespace Ui {
class TimeTable;
}

class TimeTable : public QWidget
{
    Q_OBJECT
private:
    TreeModel *thatModel;

public:
    explicit TimeTable(QWidget *parent = nullptr);
    ~TimeTable();

private slots:


    void on_actionOpen_in_Excel_triggered();

    void on_ExpandAll_clicked();

    void on_CollapseAll_clicked();

    void on_actionOpenXlsx_triggered();

    void on_actionNewRow_triggered();

    void on_checkBox_clicked(bool checked);

    void on_pushButton_lastwidth_clicked();

    void on_epdButton_clicked();

private:
    Ui::TimeTable *ui;
};

#endif // TIMETABLE_H
