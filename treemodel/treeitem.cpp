#include "treeitem.h"

TreeItem::TreeItem(TreeItem*par,int in_row)
{
    inrow=in_row;
    parent=par;
}

QVariant TreeItem::data(){
    return content;
}

bool TreeItem::setData(const QVariant &value)
{
    content=value;
    return true;
}

TreeItem* TreeItem::getparent(){
    return parent;
}
