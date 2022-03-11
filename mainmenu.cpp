#include "mainmenu.h"
#include "mainwindow.h"
#include "ui_mainmenu.h"
#include "notes.h"
#include "timetable.h"
#include "newrow.h"

//#include <QMessageBox>
#include <QDateTime>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

static QTabWidget* ui2  =nullptr;

MainMenu::~MainMenu()
{
    delete ui;
}

void ui_init(QTabWidget*ui_ptr){
    ui2=ui_ptr;
}

void MainMenu::on_pushButtonStatistics_clicked()
{
    QMessageBox::information(this,"TITLE",qApp->applicationDirPath()+"/"+qAppName());
}

void MainMenu::on_pushButtonNotes_clicked()
{
    Notes *notes=new Notes(this);
    notes->setAttribute(Qt::WA_DeleteOnClose);
    int cur2=ui2->addTab(notes,QString::asprintf("Notes"));
    ui2->setCurrentIndex(cur2);
    ui2->setVisible(true);
}

void ui_init_timetable(QTabWidget*ui_ptr);
void MainMenu::on_pushButtonSchedule_clicked()
{
    TimeTable *timetable=new TimeTable(this);
    timetable->setAttribute(Qt::WA_DeleteOnClose);
    int cur2=ui2->addTab(timetable,QString::asprintf("TimeTable"));
    ui2->setCurrentIndex(cur2);
    ui2->setVisible(true);
    ui_init_timetable(ui2);
}

void ui_closenewrow(QTabWidget*ui_ptr);
void MainMenu::on_pushButtonNewRow_clicked()
{
    NewRow *newrow=new NewRow(this);
    newrow->setAttribute(Qt::WA_DeleteOnClose);
    int cur2=ui2->addTab(newrow,QString::asprintf("AddNewRow"));
    ui2->setCurrentIndex(cur2);
    ui2->setVisible(true);
    ui_closenewrow(ui2);
}

void MainMenu::on_pushButtonImport_clicked()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString dlgTitle="导入数据";
    QString fliter="压缩包(*.zip)";
    QString filePath=QFileDialog::getOpenFileName(this,dlgTitle,curPath,fliter);
    //弹出一个对话框获取zip数据的路径
    if(filePath.count()==0)return;
    //判断是否获取到
    datazip.setZipName(filePath);
    datafile.setZip(&datazip);
    //获取到后给datazip和datafile赋值
    JlCompress::extractDir(filePath,(qApp->applicationDirPath()+"/"+qAppName()));
}

void MainMenu::on_pushButtonExport_clicked()
{
    //弹出一个对话框选择将zip数据保存的路径
    QString curPath=QDir::currentPath();
    QString dlgTitle="选择保存目录";
    QString dirPath=QFileDialog::getSaveFileName(this,tr("导出数据"),
                                                 QCoreApplication::applicationDirPath()+
                                                 "/MyNotes"+
                                                 QDateTime::currentDateTime().toString("yyyyMMddhhmmss")+".zip","压缩包(*.zip)");
    if(dirPath.isNull())return;
    //保存数据为zip
    JlCompress::compressDir(dirPath,qApp->applicationDirPath()+"/"+qAppName());
}

