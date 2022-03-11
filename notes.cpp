#include "notes.h"
#include "ui_notes.h"

Notes::Notes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notes)
{
    ui->setupUi(this);

    QDir dir(qApp->applicationDirPath()+"/"+qAppName());

    //导入路径下的所有文件
    txt_list = dir.entryList(QDir::AllEntries);
    //逆转txt_list排序
    for(int k = 0; k < (txt_list.size()/2); k++) txt_list.swap(k,txt_list.size()-(1+k));
    //筛选其中的txt文件
    txt_list = txt_list.filter(".txt");
    //给theModel赋值
    theModel = new QStringListModel(this);
    theModel->setStringList(txt_list);
    ui->listView->setModel(theModel);
    //设置列表中的每一项不可编辑
    ui->listView->setEditTriggers(nullptr);

    //设置默认打开第一个文件
    ui->listView->setCurrentIndex(theModel->index(0));
    if(txt_list.length()){
        QFile read_file(qApp->applicationDirPath()+"/"+qAppName()+"/"+txt_list.at(0));
        if (read_file.open(QIODevice::ReadOnly|QIODevice::Text)){
            ui->textEdit->setText(read_file.readAll());
            read_file.close();
        }
    }
}

Notes::~Notes()
{
    delete ui;
}



void Notes::on_listView_clicked(const QModelIndex &index)
{
    QFile read_file(qApp->applicationDirPath()+"/"+qAppName()+"/"+theModel->data(index).toString());
    if (!read_file.open(QIODevice::ReadOnly|QIODevice::Text))return;
    ui->textEdit->setText(read_file.readAll());
    read_file.close();
}

void Notes::on_actionSave_triggered()
{
    QFile save_file(qApp->applicationDirPath()+"/"+qAppName()+"/"+ui->listView->currentIndex().data().toString());
    if(!save_file.open(QIODevice::WriteOnly | QIODevice::Text))return;
    save_file.write((ui->textEdit->toPlainText()+"\n"+QDateTime::currentDateTime().toString("hh:mm")+"\n\n").toUtf8());
    save_file.close();
}

void Notes::on_actionNew_triggered()
{
    QString new_txt=QDateTime::currentDateTime().toString("yyyyMMdd")+".txt";
    if(txt_list.contains(new_txt)){
        QFile read_file(qApp->applicationDirPath()+"/"+qAppName()+"/"+new_txt);
        if (!read_file.open(QIODevice::ReadOnly|QIODevice::Text))return;
        ui->textEdit->setText(read_file.readAll());
        read_file.close();
        ui->listView->setCurrentIndex(theModel->index(txt_list.indexOf(new_txt)));
    }else{
        ui->textEdit->clear();
        QFile save_file(qApp->applicationDirPath()+"/"+qAppName()+"/"+new_txt);
        if(!save_file.open(QIODevice::WriteOnly | QIODevice::Text))return;
        save_file.write("");
        save_file.close();
        txt_list.append(new_txt);
        theModel->setStringList(txt_list);
        ui->listView->setCurrentIndex(theModel->index(txt_list.length()-1));
    }
}
