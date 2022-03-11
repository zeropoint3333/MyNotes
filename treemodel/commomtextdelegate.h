#ifndef COMMOMTEXTDELEGATE_H
#define COMMOMTEXTDELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionButton>

class CommomTextDelegate :public QStyledItemDelegate
{
public:
    Q_OBJECT
public:
    explicit CommomTextDelegate(QObject *parent = nullptr);

public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    //void SIGNAL_Clicked(int _iRow, int _iColumn, QString _sButtonContent);

private:
    //QPoint m_mousePoint;
};

#endif // COMMOMTEXTDELEGATE_H
