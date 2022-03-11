#ifndef TREEITEM_H
#define TREEITEM_H

#include<QVariant>
#include<QList>
#include<QDebug>

class TreeItem
{
public:
    TreeItem(TreeItem*par ,int in_row);

    int inrow;
    QList<QList<TreeItem*>> children;
    bool nameIsShowed=true;

    bool setData(const QVariant &value);
    QVariant data();
    TreeItem* getparent();

private:
    QVariant content;
    TreeItem* parent;
};

#endif // TREEITEM_H
