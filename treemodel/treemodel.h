#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include "treeitem.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation=Qt::Horizontal, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaders(const QList<QVariant> &values,Qt::Orientation orientation=Qt::Horizontal,int role = Qt::EditRole);

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    TreeItem* nodeFromIndex(const QModelIndex& index) const;
    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    void appendRow(const QModelIndex &parent,const QList<QVariant> value);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    TreeItem *rootItem;
    QList<QVariant> headers;
};

#endif // TREEMODEL_H
