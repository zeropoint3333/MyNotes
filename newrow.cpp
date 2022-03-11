#include "newrow.h"
#include "ui_newrow.h"

#include <QDate>
#include <QDir>
#include <QMessageBox>


#define MAX_BLANK_ROWS 20
#define NAME_XLSX_CLOUMN 1

static QTabWidget* ui4  =nullptr;
void ui_closenewrow(QTabWidget*ui_ptr){
    ui4=ui_ptr;
}

QString time_diff(QString a_time,QString b_time);
NewRow::NewRow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewRow)
{
    ui->setupUi(this);
    ui->pushButton_confirm->setVisible(0);
    ui->pushButton_end->hide();
    ui->comboBox_xlsx->addItem("-----");

    QDir dir(qApp->applicationDirPath()+"/"+qAppName());
    QStringList xlsx_list;
    //导入路径下的所有文件
    xlsx_list = dir.entryList();
    //筛选其中的xlsx文件
    xlsx_list = xlsx_list.filter(".xlsx");
    ui->comboBox_xlsx->addItems(xlsx_list);

    ui->radioButton_exist->setChecked(true);
    ui->lineEdit_newthing->setEnabled(false);
    connect(ui->radioButton_new,&QRadioButton::clicked,[=](){
        ui->comboBox_thing->setEnabled(false);
        ui->lineEdit_newthing->setEnabled(true);
    });
    connect(ui->radioButton_exist,&QRadioButton::clicked,[=](){
        ui->lineEdit_newthing->setEnabled(false);
        ui->comboBox_thing->setEnabled(true);
    });
}

NewRow::~NewRow()
{
    delete ui;
}


void NewRow::on_pushButton_start_clicked()
{
    ui->pushButton_start->hide();
    ui->pushButton_end->show();
    ui->label_date->setText(QDate::currentDate().toString("yyyy.MM.dd"));
    ui->timeEdit_start->setTime(QTime::currentTime());
    ui->timeEdit_start->setEnabled(true);
}

void NewRow::on_pushButton_end_clicked()
{
    ui->pushButton_end->hide();
    ui->timeEdit_end->setTime(QTime::currentTime());
    ui->pushButton_confirm->setVisible(1);
    ui->timeEdit_end->setEnabled(true);
}

void NewRow::on_comboBox_xlsx_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_thing->clear();
    allname_list.clear();
    if(arg1=="-----")return;

    QString xlsx_path=qApp->applicationDirPath()+"/"+qAppName()+"/"+arg1;
    QXlsx::Document curXlsx(xlsx_path);
    QStringList name_list;
    QString name;
    int xlsx_row_i=2,now_blank_rows=0;
    name_list<<curXlsx.read(xlsx_row_i,NAME_XLSX_CLOUMN).toString();
    allname_list<<curXlsx.read(xlsx_row_i,NAME_XLSX_CLOUMN).toString();
    xlsx_row_i++;

    //将name这一列的数据全部读出储存在name_list和allname_list中(name_list中无重复)
    while(now_blank_rows<MAX_BLANK_ROWS) {
        name=curXlsx.read(xlsx_row_i,NAME_XLSX_CLOUMN).toString();
        allname_list.append(name);
        if(name==""){
            now_blank_rows++;
        }else{
            now_blank_rows=0;
            if(name!=name_list.at(name_list.length()-1))
                name_list.append(name);
        }
        xlsx_row_i++;
    }
    //逆转name_list排序
    for(int k = 0; k < (name_list.size()/2); k++) name_list.swap(k,name_list.size()-(1+k));
    ui->comboBox_thing->addItems(name_list);
}

void NewRow::on_pushButton_add_clicked()
{
    if(ui->comboBox_xlsx->currentText()=="-----")
    {
        ui->label_tips->setText("未选择xlsx!");
        return;
    }

    if(ui->label_lasttime->text()=="")
    {
        ui->label_tips->setText("未确定时间!");
        return;
    }

    int max=allname_list.length()-MAX_BLANK_ROWS+1;
    int i=allname_list.lastIndexOf(ui->comboBox_thing->currentText())+3;
    QString xlsx_path=qApp->applicationDirPath()+"/"+qAppName()+"/"+ui->comboBox_xlsx->currentText();
    //ui->label_tips->setText(QString::number(i));
    QXlsx::Document curXlsx(xlsx_path);

    bool ifaddsuccess;
    if(ui->radioButton_exist->isChecked()){
        curXlsx.write(i,1,ui->comboBox_thing->currentText());
        curXlsx.write(i,2,QDate::fromString(ui->label_date->text(),"yyyy.MM.dd"));
        curXlsx.write(i,3,ui->timeEdit_start->time());
        curXlsx.write(i,4,ui->timeEdit_end->time());
        curXlsx.write(i,6,ui->lineEdit_note->text());
        curXlsx.write(i,9,ui->textEdit_feeling->toPlainText());
        for(int j=max;j>i;j--){
            curXlsx.write(j+1,1,curXlsx.read(j,1));
            curXlsx.write(j+1,2,curXlsx.read(j,2));
            curXlsx.write(j+1,3,curXlsx.read(j,3));
            curXlsx.write(j+1,4,curXlsx.read(j,4));
            curXlsx.write(j+1,6,curXlsx.read(j,6));
            curXlsx.write(j+1,9,curXlsx.read(j,9));
        }
        curXlsx.write(i+1,1,"");
        curXlsx.write(i+1,2,"");
        curXlsx.write(i+1,3,"");
        curXlsx.write(i+1,4,"");
        curXlsx.write(i+1,6,"");
        curXlsx.write(i+1,9,"");
        ifaddsuccess = curXlsx.saveAs(qApp->applicationDirPath()+"/"+qAppName()+"/"+ui->comboBox_xlsx->currentText());
    }else{
        curXlsx.write(max+2,1,ui->lineEdit_newthing->text());
        curXlsx.write(max+2,2,QDate::fromString(ui->label_date->text(),"yyyy.MM.dd"));
        curXlsx.write(max+2,3,ui->timeEdit_start->time());
        curXlsx.write(max+2,4,ui->timeEdit_end->time());
        curXlsx.write(max+2,6,ui->lineEdit_note->text());
        curXlsx.write(max+2,9,ui->textEdit_feeling->toPlainText());
        ifaddsuccess = curXlsx.saveAs(qApp->applicationDirPath()+"/"+qAppName()+"/"+ui->comboBox_xlsx->currentText());
    }
    if(ifaddsuccess){
        ui4->widget(ui4->currentIndex())->close();
        QMessageBox::information(this,"Success","Successfully Added!");
    }else{
        ui->label_tips->setText("添加失败，可能该文件已经被打开");
    }
}


void NewRow::on_pushButton_confirm_clicked()
{
    ui->label_lasttime->setText(time_diff(ui->timeEdit_end->time().toString("h:mm"),ui->timeEdit_start->time().toString("h:mm")));
}
