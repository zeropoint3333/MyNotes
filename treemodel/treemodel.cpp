#include "treemodel.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem=new TreeItem(nullptr,-1);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation!=Qt::Horizontal||role!=Qt::DisplayRole)return QVariant();
    if(headers.length()<=section)return  QVariant();
    return headers.at(section);
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(orientation!=Qt::Horizontal||role!=Qt::EditRole)return false;
    for(int i=headers.length();i<=section;i++)headers.append("");
    headers[section]=value;
    emit headerDataChanged(orientation, section, section);
    return true;
}

bool TreeModel::setHeaders(const QList<QVariant> &values,Qt::Orientation orientation,int role)
{
    if(orientation!=Qt::Horizontal||role!=Qt::EditRole)return false;
    headers=values;
    emit headerDataChanged(orientation,0,headers.length()-1);
    return true;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!rootItem||row<0||column<0)return QModelIndex();
    TreeItem* parentNode =nodeFromIndex(parent);
    //判断索引所在行数是否为空，为空则增加空行
    for(int i=parentNode->children.length();i<=row;i++){
        TreeItem *child =new TreeItem(parentNode,i);
        parentNode->children.append({child});
    }
    //判断索引所在行所在列是否为空，为空则增加Item
    for(int i=parentNode->children.at(row).length();i<=column;i++){
        TreeItem *child =new TreeItem(parentNode,row);
        parentNode->children[row].append(child);
    }
    //找到childNode
    TreeItem* childNode = parentNode->children.at(row).at(column);
    if(!childNode)return QModelIndex();
    return createIndex(row,column,childNode);
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    TreeItem* node = nodeFromIndex(index);
    //如果当前节点的node为空，则返回无效index
    if(!node)return QModelIndex();
    //获取当前节点的父节点值，如果为空，返回无效index
    TreeItem* parentNode = node->getparent();
    if(!parentNode)
        return QModelIndex();
    //找到当前节点的父节点在它的父节点下的行号
    int row = parentNode->inrow;
    //生成对应行号的index。
    return createIndex(row,0,parentNode);
}

TreeItem* TreeModel::nodeFromIndex(const QModelIndex& index) const {
    if(index.isValid())
        return static_cast<TreeItem*>(index.internalPointer());
    else
        return rootItem;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column() > 0)
        return 0;
    TreeItem* parentNode = nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.length();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return  headers.length();
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
    TreeItem* item=nodeFromIndex(parent);
    if(item->children.length()==0||item->nameIsShowed==false)return false;
    return true;
}


bool TreeModel::canFetchMore(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return false;
}

void TreeModel::fetchMore(const QModelIndex &parent)
{
    // FIXME: Implement me!
}


QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    TreeItem *item=nodeFromIndex(index);
    if(!index.isValid())return  QVariant();
    if(role==Qt::DisplayRole)return item->data();
    if(role==Qt::WhatsThisRole)return item->nameIsShowed;
    return  QVariant();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index) == value)return false;
    TreeItem *item = nodeFromIndex(index);
    if (role==Qt::EditRole){
        bool result = item->setData(value);
        emit dataChanged(index,index,{role});
        return result;
    }
    if (role==Qt::WhatsThisRole)
    {
        item->nameIsShowed=value.toBool();
        return true;
    }
    return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable |Qt::ItemIsUserCheckable;
}

void TreeModel::appendRow(const QModelIndex &parent,const QList<QVariant> value)
{
    TreeItem *par=nodeFromIndex(parent);
    int theAddrow=par->children.length();
    for(int i=0;i<value.length();i++){
        setData(index(theAddrow,i,parent),value.at(i));
    }
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return false;
}

bool TreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return false;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return false;
}

bool TreeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return false;
}
