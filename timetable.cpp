#include "newrow.h"
#include "timetable.h"
#include "ui_timetable.h"

int xlsx2model(QString filePath,TreeModel* theModel,int number);

static QTabWidget* ui3  =nullptr;

TimeTable::TimeTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeTable)
{
    ui->setupUi(this);
    ui->lastwidth->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));

    QDir dir(qApp->applicationDirPath()+"/"+qAppName());
    QStringList xlsx_list;
    //导入路径下的所有文件
    xlsx_list = dir.entryList();
    //筛选其中的xlsx文件
    xlsx_list = xlsx_list.filter(".xlsx");

    thatModel = new TreeModel(ui->treeView);
    ui->treeView->setModel(thatModel);

    thatModel->setHeaders({"项目名/日期","开始时间","结束时间","持续时间","描述","杂记"});
    for(int i=0;i<xlsx_list.size();i++){
        xlsx2model(qApp->applicationDirPath()+"/"+qAppName()+"/"+xlsx_list.at(i),thatModel,i);
    }

    ui->treeView->setMouseTracking(true);
    NameDateDelegate *thede=new NameDateDelegate(ui->treeView);
    ui->treeView->setItemDelegateForColumn(0,thede);
    for(int i=1;i<6;i++){
        ui->treeView->setItemDelegateForColumn(i,new CommomTextDelegate());
    }

    //ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->header()->setMinimumSectionSize(90);
    ui->treeView->setColumnWidth(0,210);
    ui->treeView->setColumnWidth(1,90);
    ui->treeView->setColumnWidth(2,90);
    ui->treeView->setColumnWidth(3,90);
    ui->treeView->setColumnWidth(4,110);
    ui->treeView->setColumnWidth(5,800);

    //ui->treeView->expandAll();
}

void ui_init_timetable(QTabWidget*ui_ptr){
    ui3=ui_ptr;
}

TimeTable::~TimeTable()
{
    delete ui;
}


void TimeTable::on_actionOpen_in_Excel_triggered()
{
    QModelIndex index=ui->treeView->currentIndex();
    while (index.parent().parent()!=QModelIndex()) {
        index=index.parent();
    }
    index=index.sibling(index.row(),0);
    QString index_xlsx_path=qApp->applicationDirPath()+"/"+qAppName()+"/"+index.data().toString();
    system(("start "+index_xlsx_path).toStdString().data());
}

void TimeTable::on_actionOpenXlsx_triggered()
{

}

void ui_closenewrow(QTabWidget*ui_ptr);
void TimeTable::on_actionNewRow_triggered()
{
    NewRow *newrow=new NewRow(this);
    newrow->setAttribute(Qt::WA_DeleteOnClose);
    int cur2=ui3->addTab(newrow,QString::asprintf("AddNewRow"));
    ui3->setCurrentIndex(cur2);
    ui3->setVisible(true);
    ui_closenewrow(ui3);
}

void TimeTable::on_ExpandAll_clicked()
{
    ui->treeView->expandAll();
}

void TimeTable::on_CollapseAll_clicked()
{
    ui->treeView->collapseAll();
}

void TimeTable::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        ui->treeView->showColumn(5);
        ui->lastwidth->show();
        ui->pushButton_lastwidth->show();
    }else{
        ui->treeView->hideColumn(5);
        ui->lastwidth->hide();
        ui->pushButton_lastwidth->hide();
    }
}

void TimeTable::on_pushButton_lastwidth_clicked()
{
    ui->treeView->setColumnWidth(5,ui->lastwidth->text().toInt());
}

void TimeTable::on_epdButton_clicked()
{
    ui->treeView->expandToDepth(ui->spinBox_epdGrade->value()-1);
}
