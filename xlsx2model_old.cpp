#include "xlsxdocument.h"
#include <QString>
#include <QDate>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QPushButton>

#define NAME_XLSX_CLOUMN 1
#define DATE_XLSX_CLOUMN 2
#define START_XLSX_CLOUMN 3
#define END_XLSX_CLOUMN 4
#define DESCRIPTION_XLSX_CLOUMN 6
#define NOTE_XLSX_CLOUMN 9
#define MAX_BLANK_ROWS 20

#define NAME_MODEL_CLOUMN 0
#define DATE_MODEL_CLOUMN 0
#define START_MODEL_CLOUMN 1
#define END_MODEL_CLOUMN 2
#define TIMEDIFF_MODEL_CLOUMN 3
#define DESCRIPTION_MODEL_CLOUMN 4
#define NOTE_MODEL_CLOUMN 5

QString time_diff(QString a_time,QString b_time);
QString timediff_add(QString a_timedif,QString b_timedif);
QString hmm_divide(QString time,int num);

extern int xlsx2model_old(QString filePath,QStandardItemModel* theModel,int xlsx_number)
{
    theModel->setHorizontalHeaderLabels(QStringList()
                                        <<QStringLiteral("项目名/日期")
                                        <<QStringLiteral("开始时间")
                                        <<QStringLiteral("结束时间")
                                        <<QStringLiteral("持续时间")
                                        <<QStringLiteral("描述")
                                        <<QStringLiteral("杂记"));

    //截取文件名(带.xlsx)
    QString filetitle=filePath.mid(filePath.lastIndexOf("/")+1,filePath.length());
    //filetitle=filetitle.mid(0,filetitle.indexOf(".xlsx"));

    //添加一级节点(根节点)
    theModel->setItem(xlsx_number,0,new QStandardItem(filetitle.toStdString().data()));
    //theModel->setItem(xlsx_number,1,new QStandardItem());

    //对curXlsx进行赋值
    QXlsx::Document curXlsx(filePath);

    //声明一些循环需要的变量
    int xlsx_row_i=2,now_blank_rows=0,name_i=0,name_date_i=0;
    QString name,sumtime,name_date;


    //初始化name_list和isNull，并且将第一行的数据储存
    QStringList name_list;
    QList<QList<int>> isFull;
    name_list<<curXlsx.read(xlsx_row_i,NAME_XLSX_CLOUMN).toString();
    isFull.append({1,1});
    xlsx_row_i++;

    //将name这一列的数据全部读出储存在name_list中，并且在isFull中储存它是否为空行
    while(now_blank_rows<MAX_BLANK_ROWS) {
        name=curXlsx.read(xlsx_row_i,NAME_XLSX_CLOUMN).toString();
        if(name==""){
            now_blank_rows++;
            isFull.append({0});
        }else{
            isFull.append({1});
            now_blank_rows=0;
            if(name!=name_list.at(name_list.length()-1))
                name_list.append(name);
        }
        xlsx_row_i++;
    }

    //由于设置MAX_BLANK_ROWS，将isNull最后MAX_BLANK_ROWS个QList全部删除
    for(int i=0;i<MAX_BLANK_ROWS;i++)
        isFull.removeLast();

    /*
     * 初始化name_detail
     * 将name_date是否为空存入isFull(0,1,2)，将name_date的这一行的其他数据存入name_detail
     */
    QList<QList<QString>> name_detail;
    name_detail.append({curXlsx.read(2,DATE_XLSX_CLOUMN).toDate().toString("yyyy.MM.dd"),
                        curXlsx.read(2,START_XLSX_CLOUMN).toTime().toString("h:mm"),
                        curXlsx.read(2,END_XLSX_CLOUMN).toTime().toString("h:mm"),
                        time_diff(curXlsx.read(2,END_XLSX_CLOUMN).toTime().toString("h:mm"),curXlsx.read(2,START_XLSX_CLOUMN).toTime().toString("h:mm")),
                        curXlsx.read(2,DESCRIPTION_XLSX_CLOUMN).toString(),
                        curXlsx.read(2,NOTE_XLSX_CLOUMN).toString()});

    for(int i=1;i<isFull.length();i++){
        if(isFull.at(i).at(0)==1){
            isFull[i].append(1);
            name_date=curXlsx.read(i+2,DATE_XLSX_CLOUMN).toDate().toString("yyyy.MM.dd");
            int jj=i-1;
            while(name_date==name_detail.at(jj).at(0)&&jj>=0&&isFull.at(jj).at(0)==1){
                isFull[jj][1]=isFull.at(jj).at(1)+1;
                jj--;
            }
            QString starttime=curXlsx.read(i+2,START_XLSX_CLOUMN).toTime().toString("h:mm");
            QString endtime=curXlsx.read(i+2,END_XLSX_CLOUMN).toTime().toString("h:mm");
            name_detail.append({name_date,
                                starttime,
                                endtime,
                                time_diff(endtime,starttime),
                                curXlsx.read(i+2,DESCRIPTION_XLSX_CLOUMN).toString(),
                                curXlsx.read(i+2,NOTE_XLSX_CLOUMN).toString()});
        }else{
            name_detail.append({""});
            isFull[i].append(0);
        }
    }


    /*
     * 将name_list中的数据导入theModel，建立二级目录
     */
    for(int i=0;i<name_list.length();i++)
        theModel->item(xlsx_number,0)->setChild(i,NAME_MODEL_CLOUMN,new QStandardItem(name_list.at(i)));

    /*
     * 将name_detail的第1列数据导入theModel，建立三级目录
     */
    isFull.append({0});
    name_i=0;
    for(int i=0;i<isFull.length();i++){
        if(isFull.at(i).at(0)==0)continue;
        if(isFull.at(i).at(1)>1)i=i+isFull.at(i).at(1)-1;
        theModel->item(xlsx_number,0)->child(name_i,0)->appendRow(new QStandardItem(name_detail.at(i).at(0)));
        if(isFull.at(i+1).at(0)==0)name_i++;
    }

    /*
     * 将name_detail的第2,3列数据导入theModel，补充三级目录的栏目
     * 根据isNull第2列中数据是否>1，判断是否建立四级目录并将数据导入
     */
    name_i=0;
    name_date_i=0;
    for(int i=0;i<isFull.length();i++){
        if(isFull.at(i).at(0)==0)continue;
        if(isFull.at(i).at(1)==1){
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,START_MODEL_CLOUMN,new QStandardItem(name_detail.at(i).at(1)));
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,END_MODEL_CLOUMN,new QStandardItem(name_detail.at(i).at(2)));
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,TIMEDIFF_MODEL_CLOUMN,new QStandardItem(name_detail.at(i).at(3)));
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,DESCRIPTION_MODEL_CLOUMN,new QStandardItem(name_detail.at(i).at(4)));
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,NOTE_MODEL_CLOUMN,new QStandardItem(name_detail.at(i).at(5)));
        }else{
            sumtime="0h0min";
            for(int j=0;j<isFull.at(i).at(1);j++){
                theModel->item(xlsx_number,0)->child(name_i,0)->child(name_date_i,0)->setChild(j,START_MODEL_CLOUMN,new QStandardItem(name_detail.at(i+j).at(1)));
                theModel->item(xlsx_number,0)->child(name_i,0)->child(name_date_i,0)->setChild(j,END_MODEL_CLOUMN,new QStandardItem(name_detail.at(i+j).at(2)));
                theModel->item(xlsx_number,0)->child(name_i,0)->child(name_date_i,0)->setChild(j,TIMEDIFF_MODEL_CLOUMN,new QStandardItem(name_detail.at(i+j).at(3)));
                theModel->item(xlsx_number,0)->child(name_i,0)->child(name_date_i,0)->setChild(j,DESCRIPTION_MODEL_CLOUMN,new QStandardItem(name_detail.at(i+j).at(4)));
                theModel->item(xlsx_number,0)->child(name_i,0)->child(name_date_i,0)->setChild(j,NOTE_MODEL_CLOUMN,new QStandardItem(name_detail.at(i+j).at(5)));
                sumtime=timediff_add(sumtime,name_detail.at(i+j).at(3));
            }
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,START_MODEL_CLOUMN,new QStandardItem("-------"));
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,END_MODEL_CLOUMN,new QStandardItem("-------"));
            theModel->item(xlsx_number,0)->child(name_i,0)->setChild(name_date_i,TIMEDIFF_MODEL_CLOUMN,new QStandardItem(sumtime));
            i=i+isFull.at(i).at(1)-1;
        }
        name_date_i++;
        if(isFull.at(i+1).at(0)==0){
            name_i++;
            name_date_i=0;
        }
    }

    /*
     * 将总时间和平均时间导入theModel，补充三级目录的项目
     */
    name_i=name_date_i=0;
    sumtime="0h0min";
    for(int i=0;i<isFull.length();i++){
        if(isFull.at(i).at(0)==0)continue;
        sumtime=timediff_add(sumtime,name_detail.at(i).at(3));
        if(isFull.at(i).at(1)==1)name_date_i++;
        if(isFull.at(i+1).at(0)==0)
        {
            theModel->item(xlsx_number,0)->setChild(name_i,TIMEDIFF_MODEL_CLOUMN,new QStandardItem(sumtime));
            theModel->item(xlsx_number,0)->setChild(name_i,4,new QStandardItem("平均:"+hmm_divide(sumtime,name_date_i)));
            sumtime="0h0min";
            name_i++;
            name_date_i=0;
        }
    }
    return 0;
}

extern QString time_diff(QString a_time,QString b_time){
    int hour_diff,min_diff;
    hour_diff=a_time.mid(0,a_time.indexOf(":")).toInt()-b_time.mid(0,b_time.indexOf(":")).toInt();
    min_diff=a_time.mid(a_time.indexOf(":")+1,2).toInt()-b_time.mid(b_time.indexOf(":")+1,2).toInt();
    if(hour_diff<0)hour_diff+=24;
    if(min_diff<0){
        hour_diff--;
        min_diff+=60;
    }
    return QString::number(hour_diff)+"h"+QString::number(min_diff)+"min";
}

QString timediff_add(QString a_timedif,QString b_timedif){
    int hour=a_timedif.mid(0,a_timedif.indexOf("h")).toInt()+b_timedif.mid(0,b_timedif.indexOf("h")).toInt();
    int min=a_timedif.mid(a_timedif.indexOf("h")+1,a_timedif.length()-a_timedif.indexOf("h")-4).toInt()
            +b_timedif.mid(b_timedif.indexOf("h")+1,b_timedif.length()-b_timedif.indexOf("h")-4).toInt();
    if(min>=60){
        hour++;
        min=min-60;
    }
    return QString::number(hour)+"h"+QString::number(min)+"min";
}

QString hmm_divide(QString time,int num){
    int hmm[2];
    hmm[0]=time.mid(0,time.indexOf("h")).toInt();
    hmm[1]=time.mid(time.indexOf("h")+1,time.length()-time.indexOf("h")-4).toInt();
    return QString::number(hmm[0]/num)+"h"+QString::number((hmm[0]*60+hmm[1])/num%60)+"min";
}
