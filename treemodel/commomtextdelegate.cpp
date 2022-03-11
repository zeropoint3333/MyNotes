#include "commomtextdelegate.h"
#include <QString>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
CommomTextDelegate::CommomTextDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *CommomTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    return nullptr;
}

void CommomTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.state & QStyle::State_HasFocus){
        painter->fillRect(option.rect,(QColor(240,244,255)));
    }
    QStyledItemDelegate::paint(painter, option, index);
}

bool CommomTextDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QApplication::restoreOverrideCursor();
    return false;
}
