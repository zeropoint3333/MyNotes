#ifndef NEWROW_H
#define NEWROW_H

#include <QWidget>
#include "xlsxdocument.h"

namespace Ui {
class NewRow;
}

class NewRow : public QWidget
{
    Q_OBJECT

public:
    explicit NewRow(QWidget *parent = nullptr);
    ~NewRow();

private:
    QStringList allname_list;

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_end_clicked();

    void on_comboBox_xlsx_currentIndexChanged(const QString &arg1);

    void on_pushButton_add_clicked();

    void on_pushButton_confirm_clicked();

private:
    Ui::NewRow *ui;
};

#endif // NEWROW_H
