#include <QMimeData>

#include "../ImageWidgets/StandardImageWindow.h"
#include "NodeListModel.h"

using namespace genericinterface;

NodeListModel::NodeListModel(QObject *parent) : QAbstractListModel(parent)
{
}

int NodeListModel::rowCount(const QModelIndex & /*parent*/) const
{
    return _nodes.size();
}

QVariant NodeListModel::data(const QModelIndex &index, int /*role*/) const {
    if(index.row() < 0 || index.row() >= rowCount()) {
        return QVariant();
    }
    const Node* node = _nodes[index.row()];
    if(node == NULL) {
        return QVariant();
    }
    return QVariant::fromValue(node);
}

void NodeListModel::setList(QList<const Node*> list) {
    beginResetModel();
    _nodes = list;
    endResetModel();
}

bool NodeListModel::insertRows(int row, int count, const QModelIndex& parent) {
    if(row < 0 || row > rowCount() || count < 1) {
        return false;
    }
    beginInsertRows(parent, row, row+count-1);
    for(int i = 0; i < count; ++i) {
        _nodes.insert(row, NULL);
    }
    endInsertRows();
    return true;
}

bool NodeListModel::setData(const QModelIndex& index, const QVariant& value, int /*role*/) {
    if(index.row() < 0 || index.row() >= rowCount()) {
        std::cout << "Set data out of bound" << std::endl;
        return false;
    }
    
    if(!value.canConvert<const Node*>()) {
        std::cout << "Set data can't convert to const Node*" << std::endl;
        return false;
    }
    
    _nodes[index.row()] = value.value<const Node*>();
    emit dataChanged(index, index);
    return true;
}

bool NodeListModel::removeRows(int row, int count, const QModelIndex& parent) {
    if(row < 0 || count < 1 || row+count > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row+count-1);
    for(int i = 0; i < count; ++i) {
        _nodes.removeAt(row);
    }
    endRemoveRows();
    return true;
}

Qt::DropActions NodeListModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags NodeListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if (index.isValid())
        return (Qt::ItemIsDragEnabled | defaultFlags) & ~Qt::ItemIsDropEnabled;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList NodeListModel::mimeTypes() const {
    QStringList types;
    types << "application/detiqt.genericinterface.node";
    types << "application/detiqt.genericinterface.stdimgwnd";
    return types;
}

QMimeData* NodeListModel::mimeData(const QModelIndexList& indexes ) const {
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            const Node* node = data(index, Qt::DisplayRole).value<const Node*>();
            stream << reinterpret_cast<uintptr_t>(node);
        }
    }

    mimeData->setData("application/detiqt.genericinterface.node", encodedData);
    return mimeData;
}

bool NodeListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/detiqt.genericinterface.node")
        && !data->hasFormat("application/detiqt.genericinterface.stdimgwnd"))
        return false;
    if (column > 0)
        return false;
        
    int beginRow;
    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = parent.row();
    else
        beginRow = rowCount(QModelIndex());
        
    if(data->hasFormat("application/detiqt.genericinterface.node")) {
        QByteArray encodedData = data->data("application/detiqt.genericinterface.node");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        QList<QVariant> newNodes;
        int rows = 0;

        while (!stream.atEnd()) {
            uintptr_t data;
            stream >> data;
            const Node* node = reinterpret_cast<const Node*>(data);
            newNodes << QVariant::fromValue(node);
            ++rows;
        }
        
        insertRows(beginRow, rows, QModelIndex());
        for(QList<QVariant> ::iterator it = newNodes.begin(); it != newNodes.end(); ++it) {
            QModelIndex idx = index(beginRow, 0);
            setData(idx, *it);
            beginRow++;
        }
    }
    else if(data->hasFormat("application/detiqt.genericinterface.stdimgwnd")){
        QByteArray encodedData = data->data("application/detiqt.genericinterface.stdimgwnd");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        uintptr_t data;
        stream >> data;
        StandardImageWindow* siw = reinterpret_cast<StandardImageWindow*>(data);
        if(siw == NULL) {
            return false;
        }
        emit windowDropped(siw);
    }

    return true;
}