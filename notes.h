#ifndef NOTES_H
#define NOTES_H

#include <QWidget>
#include <QStringListModel>
#include <QMessageBox>
#include <QFileDialog>
#include "JlCompress.h"

namespace Ui {
class Notes;
}

class Notes : public QWidget
{
    Q_OBJECT
private:
    QStringListModel *theModel;
    QStringList txt_list;

public:
    explicit Notes(QWidget *parent = nullptr);
    ~Notes();

private slots:

    void on_listView_clicked(const QModelIndex &index);

    void on_actionSave_triggered();

    void on_actionNew_triggered();

private:
    Ui::Notes *ui;
};

#endif // NOTES_H
