#ifndef NAMEDATEDELEGATE_H
#define NAMEDATEDELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionButton>

class NameDateDelegate :public QStyledItemDelegate
{
public:
    Q_OBJECT
public:
    explicit NameDateDelegate(QObject *parent = nullptr);

public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void SIGNAL_Clicked(int _iRow, int _iColumn, QString _sButtonContent);

private:
    QPoint m_mousePoint;
};

#endif // NAMEDATEDELEGATE_H
