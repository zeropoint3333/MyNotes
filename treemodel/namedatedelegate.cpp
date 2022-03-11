#include "namedatedelegate.h"

#include <QString>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QTreeView>
#include <QDebug>

static QTreeView *parentTree;
void expandAllChild(QModelIndex index);
NameDateDelegate::NameDateDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    parentTree=qobject_cast<QTreeView*>(parent);
}

QWidget *NameDateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    return nullptr;
}

void NameDateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int grade=-1;
    QModelIndex parent=index;
    while (parent!=QModelIndex()) {
        grade++;
        parent=parent.parent();
    }
    switch (grade){
    case 1:{
        if (option.state & QStyle::State_MouseOver) {
            painter->fillRect(option.rect,(QColor(229,243,255)));
        }
        if (option.state & QStyle::State_Selected){
            painter->fillRect(option.rect,QColor(204,232,255));
        }
        if (option.state & QStyle::State_HasFocus){
            painter->fillRect(option.rect,(QColor(189,221,255)));
        }
        QString text="";
        //获取值
        bool checked = index.model()->data(index,Qt::WhatsThisRole).toBool();
        //按钮的风格选项
        QStyleOptionButton *theButtonOption1=new QStyleOptionButton();
        theButtonOption1->state |= QStyle::State_Enabled;
        if(checked)
        {
            theButtonOption1->state |= QStyle::State_On;
            text=index.model()->data(index).toString();
            QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, theButtonOption1);
            QPoint checkBoxPoint(option.rect.x() /*+ option.rect.width() - checkBoxRect.width()*/,
                                 option.rect.y() + option.rect.height() / 2 - checkBoxRect.height() / 2);
            theButtonOption1->rect = QRect(checkBoxPoint, checkBoxRect.size());
            //绘制QCheckBox
            QApplication::style()->drawControl(QStyle::CE_CheckBox,theButtonOption1,painter);
            QApplication::style()->drawItemText(painter,option.rect,Qt::AlignLeft,QApplication::palette(),true,"     "+text);
        }
        else
        {
            theButtonOption1->state = QStyle::State_Off;
            QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, theButtonOption1);
            QPoint checkBoxPoint(option.rect.x(),
                                 option.rect.y() + option.rect.height() / 2 - checkBoxRect.height() / 2);
            theButtonOption1->rect = QRect(checkBoxPoint, checkBoxRect.size());
            //绘制QCheckBox
            QApplication::style()->drawControl(QStyle::CE_CheckBox,theButtonOption1,painter);
        }
        delete theButtonOption1;
        break;
    }
    case 2:{
        if (option.state & QStyle::State_MouseOver) {
            painter->fillRect(option.rect,(QColor(229,243,255)));
        }
        if (option.state & QStyle::State_Selected){
            painter->fillRect(option.rect,QColor(204,232,255));
        }
        if (option.state & QStyle::State_HasFocus){
            painter->fillRect(option.rect,(QColor(189,221,255)));
        }
        QString text="";
        //获取值
        bool checked = index.model()->data(index,Qt::WhatsThisRole).toBool();
        //按钮的风格选项
        QStyleOptionButton *theButtonOption1=new QStyleOptionButton();
        theButtonOption1->state |= QStyle::State_Enabled;
        if(checked)
        {
            theButtonOption1->state |= QStyle::State_On;
            text=index.model()->data(index).toString();
            QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, theButtonOption1);
            QPoint checkBoxPoint(option.rect.x() /*+ option.rect.width() - checkBoxRect.width()*/,
                                 option.rect.y() + option.rect.height() / 2 - checkBoxRect.height() / 2);
            theButtonOption1->rect = QRect(checkBoxPoint, checkBoxRect.size());
            //绘制QCheckBox
            QApplication::style()->drawControl(QStyle::CE_CheckBox,theButtonOption1,painter);

            QStyleOptionButton *theButtonOption2=new QStyleOptionButton();
            theButtonOption2->text = "展";
            theButtonOption2->rect = option.rect.adjusted(checkBoxRect.width()+1, 0, checkBoxRect.width()+24-option.rect.width(), -0);
            theButtonOption2->state |= QStyle::State_Enabled;

            QApplication::style()->drawItemText(painter,option.rect,Qt::AlignLeft,QApplication::palette(),true,"          "+text);
            QApplication::style()->drawControl(QStyle::CE_PushButton,theButtonOption2,painter);
            delete  theButtonOption2;
        }
        else
        {
            theButtonOption1->state = QStyle::State_Off;
            QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, theButtonOption1);
            QPoint checkBoxPoint(option.rect.x(),
                                 option.rect.y() + option.rect.height() / 2 - checkBoxRect.height() / 2);
            theButtonOption1->rect = QRect(checkBoxPoint, checkBoxRect.size());
            //绘制QCheckBox
            QApplication::style()->drawControl(QStyle::CE_CheckBox,theButtonOption1,painter);
        }
        delete theButtonOption1;
        break;
    }
    default:{
        if (option.state & QStyle::State_HasFocus){
            painter->fillRect(option.rect,(QColor(240,244,255)));
        }
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }
    }
}

bool NameDateDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    int grade=-1;
    QModelIndex parent=index;
    QApplication::restoreOverrideCursor();

    while (parent!=QModelIndex()) {
        grade++;
        parent=parent.parent();
    }

    QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
    m_mousePoint = pEvent->pos();

    switch (grade) {
    case 1:{
        QStyleOptionButton *theButtonOption1=new QStyleOptionButton();
        QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, theButtonOption1);
        QPoint checkBoxPoint(option.rect.x(),
                             option.rect.y() + option.rect.height() / 2 - checkBoxRect.height() / 2);
        theButtonOption1->rect = QRect(checkBoxPoint, checkBoxRect.size());

        if (!theButtonOption1->rect.contains(m_mousePoint))return false;
        if(event->type()==QEvent::MouseButtonRelease) {
            model->setData(index,!model->data(index,Qt::WhatsThisRole).toBool(),Qt::WhatsThisRole);
            return true;
        }
        delete theButtonOption1;
        break;
    }
    case 2:{
        QStyleOptionButton *theButtonOption1=new QStyleOptionButton();
        QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, theButtonOption1);
        QPoint checkBoxPoint(option.rect.x(),
                             option.rect.y() + option.rect.height() / 2 - checkBoxRect.height() / 2);
        theButtonOption1->rect = QRect(checkBoxPoint, checkBoxRect.size());

        QStyleOptionButton *theButtonOption2=new QStyleOptionButton();
        theButtonOption2->text = "展";
        theButtonOption2->rect = option.rect.adjusted(checkBoxRect.width()+1, 0, checkBoxRect.width()+24-option.rect.width(), -0);

        if(event->type()==QEvent::MouseButtonRelease&&theButtonOption1->rect.contains(m_mousePoint)) {
            model->setData(index,!model->data(index,Qt::WhatsThisRole).toBool(),Qt::WhatsThisRole);
            return true;
        }

        if(index.model()->data(index,Qt::WhatsThisRole).toBool()&&theButtonOption2->rect.contains(m_mousePoint)){
            if(event->type()==QEvent::MouseMove){
                QApplication::setOverrideCursor(Qt::PointingHandCursor);
                return true;
            }
            if(event->type()==QEvent::MouseButtonRelease){
                expandAllChild(index);
                return true;
            }
        }
        delete theButtonOption2;
        delete theButtonOption1;
        break;
    }
    }
    return false;
}

void expandAllChild(QModelIndex index){
    parentTree->collapse(index);
    parentTree->expand(index);
    int times=index.model()->rowCount(index);
    for(int i=0;i<times;i++){
        expandAllChild(index.model()->index(i,0,index));
    }
}
