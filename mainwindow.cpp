#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainmenu.h"

void ui_init(QTabWidget*ui_ptr);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->maintabWidget);
    MainMenu *mainmenu=new MainMenu(ui->maintabWidget);
    mainmenu->setAttribute(Qt::WA_DeleteOnClose);
    int cur=ui->maintabWidget->addTab(mainmenu,QString::asprintf("MainMenu"));
    ui->maintabWidget->setCurrentIndex(cur);
    //新建主菜单Tab
    ui->maintabWidget->setVisible(true);
    //取消主菜单Tab关闭按钮
    ui->maintabWidget->tabBar()->setTabButton(cur,QTabBar::RightSide,nullptr);
    //将TabWidget的指针传入mainmenu.cpp
    ui_init(ui->maintabWidget);
    //parent->addAction(ui->actionCloseTab);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_maintabWidget_tabCloseRequested(int index)
{
    //if(ui->maintabWidget->count()==1)ui->maintabWidget->setVisible(false);
    if(index<0) return;
    QWidget* tab=ui->maintabWidget->widget(index);
    tab->close();
}

void MainWindow::on_actionCloseTab_triggered()
{
    if(ui->maintabWidget->currentIndex()==0) return;
    ui->maintabWidget->widget(ui->maintabWidget->currentIndex())->close();
}
