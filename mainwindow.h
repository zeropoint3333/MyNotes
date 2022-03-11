#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_maintabWidget_tabCloseRequested(int index);

    void on_actionCloseTab_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
