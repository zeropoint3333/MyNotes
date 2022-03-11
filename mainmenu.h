#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QFileDialog>
#include <QTabWidget>
#include "quazip.h"
#include "quazipfile.h"
#include "JlCompress.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private:
    QuaZip datazip;
    QuaZipFile datafile;


private slots:
    void on_pushButtonStatistics_clicked();

    void on_pushButtonNotes_clicked();

    void on_pushButtonSchedule_clicked();

    void on_pushButtonImport_clicked();

    void on_pushButtonExport_clicked();

    void on_pushButtonNewRow_clicked();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H
