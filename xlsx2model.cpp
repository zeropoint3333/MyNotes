#include "xlsxdocument.h"
#include <QString>
#include <QDate>
#include <QItemDelegate>
#include <QPushButton>

#include "treemodel/treemodel.h"

#define NAME_XLSX_COLUMN 1
#define DATE_XLSX_COLUMN 2
#define START_XLSX_COLUMN 3
#define END_XLSX_COLUMN 4
#define DESCRIPTION_XLSX_COLUMN 6
#define NOTE_XLSX_COLUMN 9
#define MAX_BLANK_ROWS 20

#define NAME_MODEL_COLUMN 0
#define DATE_MODEL_COLUMN 0
#define START_MODEL_COLUMN 1
#define END_MODEL_COLUMN 2
#define TIMEDIFF_MODEL_COLUMN 3
#define DESCRIPTION_MODEL_COLUMN 4
#define NOTE_MODEL_COLUMN 5

QString time_diff(QString a_time,QString b_time);
QString timediff_add(QString a_timedif,QString b_timedif);
QString hmm_divide(QString time,int num);

extern int xlsx2model(QString filePath,TreeModel* thatModel,int xlsx_number)
{
    //截取文件名(带.xlsx)
    QString filetitle=filePath.mid(filePath.lastIndexOf("/")+1,filePath.length());

    //对curXlsx进行赋值
    QXlsx::Document curXlsx(filePath);

    //声明一些循环需要的变量
    int xlsx_row_i=2,now_blank_rows=0,name_i=0,name_date_i=0;
    QString name,sumtime,name_date;


    //初始化name_list和isNull，并且将第一行的数据储存
    QStringList name_list;
    QList<QList<int>> isFull;
    name_list<<curXlsx.read(xlsx_row_i,NAME_XLSX_COLUMN).toString();
    isFull.append({1,1});
    xlsx_row_i++;

    //将name这一列的数据全部读出储存在name_list中，并且在isFull中储存它是否为空行
    while(now_blank_rows<MAX_BLANK_ROWS) {
        name=curXlsx.read(xlsx_row_i,NAME_XLSX_COLUMN).toString();
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
    QList<QList<QVariant>> name_detail;
    name_detail.append({curXlsx.read(2,DATE_XLSX_COLUMN).toDate().toString("yyyy.MM.dd"),
                        curXlsx.read(2,START_XLSX_COLUMN).toTime(),
                        curXlsx.read(2,END_XLSX_COLUMN).toTime(),
                        time_diff(curXlsx.read(2,END_XLSX_COLUMN).toTime().toString("h:mm"),curXlsx.read(2,START_XLSX_COLUMN).toTime().toString("h:mm")),
                        curXlsx.read(2,DESCRIPTION_XLSX_COLUMN).toString(),
                        curXlsx.read(2,NOTE_XLSX_COLUMN).toString()});

    for(int i=1;i<isFull.length();i++){
        if(isFull.at(i).at(0)==1){
            isFull[i].append(1);
            name_date=curXlsx.read(i+2,DATE_XLSX_COLUMN).toDate().toString("yyyy.MM.dd");
            int jj=i-1;
            while(name_date==name_detail.at(jj).at(0)&&jj>=0&&isFull.at(jj).at(0)==1){
                isFull[jj][1]=isFull.at(jj).at(1)+1;
                jj--;
            }
            QTime starttime=curXlsx.read(i+2,START_XLSX_COLUMN).toTime();
            QTime endtime=curXlsx.read(i+2,END_XLSX_COLUMN).toTime();
            name_detail.append({name_date,
                                starttime,
                                endtime,
                                time_diff(endtime.toString("h:mm"),starttime.toString("h:mm")),
                                curXlsx.read(i+2,DESCRIPTION_XLSX_COLUMN).toString(),
                                curXlsx.read(i+2,NOTE_XLSX_COLUMN).toString()});
        }else{
            name_detail.append({""});
            isFull[i].append(0);
        }
    }

    //添加一级节点(根节点)
    thatModel->setData(thatModel->index(xlsx_number,0),filetitle.toStdString().data());
    if(filetitle.contains("study"))thatModel->setData(thatModel->index(xlsx_number,0),false,Qt::WhatsThisRole);

    /*
     * 将name_list中的数据导入thatModel，建立二级目录
     */
    QModelIndex parent=thatModel->index(xlsx_number,0);
    QModelIndex child;
    for(int i=0;i<name_list.length();i++){
        child = thatModel->index(i,NAME_MODEL_COLUMN,parent);
        thatModel->setData(child,name_list.at(i));
    }

    /*
     * 将name_detail的第1列数据导入thatModel，建立三级目录
     */
    isFull.append({0});
    name_i=0;
    for(int i=0;i<isFull.length();i++){
        if(isFull.at(i).at(0)==0)continue;
        if(isFull.at(i).at(1)>1)i=i+isFull.at(i).at(1)-1;
        child=thatModel->index(name_i,DATE_MODEL_COLUMN,parent);
        thatModel->appendRow(child,{name_detail.at(i).at(0)});
        if(isFull.at(i+1).at(0)==0)name_i++;
    }

    /*
     * 将name_detail的第2,3列数据导入thatModel，补充三级目录的栏目
     * 根据isNull第2列中数据是否>1，判断是否建立四级目录并将数据导入
     */
    name_i=0;
    name_date_i=0;
    for(int i=0;i<isFull.length();i++){
        if(isFull.at(i).at(0)==0)continue;
        if(isFull.at(i).at(1)==1){
            child=thatModel->index(name_i,0,parent);
            thatModel->setData(thatModel->index(name_date_i,START_MODEL_COLUMN,child),name_detail.at(i).at(1));
            thatModel->setData(thatModel->index(name_date_i,END_MODEL_COLUMN,child),name_detail.at(i).at(2));
            thatModel->setData(thatModel->index(name_date_i,TIMEDIFF_MODEL_COLUMN,child),name_detail.at(i).at(3));
            thatModel->setData(thatModel->index(name_date_i,DESCRIPTION_MODEL_COLUMN,child),name_detail.at(i).at(4));
            thatModel->setData(thatModel->index(name_date_i,NOTE_MODEL_COLUMN,child),name_detail.at(i).at(5));
        }else{
            sumtime="0h0min";
            child=thatModel->index(name_i,0,parent);
            thatModel->setData(thatModel->index(name_date_i,START_MODEL_COLUMN,child),"-------");
            thatModel->setData(thatModel->index(name_date_i,END_MODEL_COLUMN,child),"-------");
            child=thatModel->index(name_date_i,0,child);
            for(int j=0;j<isFull.at(i).at(1);j++){
                //直接增加一行而不直接使用setData，清除多余的第一项中的文字
                thatModel->appendRow(child,name_detail.at(i+j));
                thatModel->setData(thatModel->index(j,DATE_MODEL_COLUMN,child),"");
                sumtime=timediff_add(sumtime,name_detail.at(i+j).at(3).toString());
            }
            thatModel->setData(thatModel->index(name_date_i,TIMEDIFF_MODEL_COLUMN,child.parent()),sumtime);
            i=i+isFull.at(i).at(1)-1;
        }
        name_date_i++;
        if(isFull.at(i+1).at(0)==0){
            name_i++;
            name_date_i=0;
        }
    }

    /*
     * 将总时间和平均时间导入thatModel，补充三级目录的项目
     */
    name_i=name_date_i=0;
    sumtime="0h0min";
    for(int i=0;i<isFull.length();i++){
        if(isFull.at(i).at(0)==0)continue;
        sumtime=timediff_add(sumtime,name_detail.at(i).at(3).toString());
        if(isFull.at(i).at(1)==1)name_date_i++;
        if(isFull.at(i+1).at(0)==0)
        {
            thatModel->setData(thatModel->index(name_i,TIMEDIFF_MODEL_COLUMN,parent),sumtime);
            thatModel->setData(thatModel->index(name_i,DESCRIPTION_MODEL_COLUMN,parent),"平均:"+hmm_divide(sumtime,name_date_i));
            sumtime="0h0min";
            name_i++;
            name_date_i=0;
        }
    }

    return 0;
}
