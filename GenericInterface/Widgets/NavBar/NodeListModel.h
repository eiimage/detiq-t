#ifndef QTINTERFACE_WIDGET_NODELISTMODEL_H
#define QTINTERFACE_WIDGET_NODELISTMODEL_H

#include <iostream>
#include <QAbstractListModel>
#include <Image.h>

#include "../../Services/Node.h"

namespace genericinterface
{
    class StandardImageWindow;
    
    class NodeListModel : public QAbstractListModel {
      Q_OBJECT
      public:
        
        NodeListModel(QObject *parent);
        
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const;
        bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
        bool setData(const QModelIndex&, const QVariant& value, int role = Qt::EditRole);
        bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
        
        void setList(QList<const Node*> list);
        
        Qt::DropActions supportedDropActions() const;
        Qt::ItemFlags flags(const QModelIndex &) const;
        QStringList mimeTypes() const;
        QMimeData* mimeData (const QModelIndexList&) const;
        bool dropMimeData(const QMimeData*, Qt::DropAction, int row, int column, const QModelIndex &parent = QModelIndex());
        
      signals:
        void windowDropped(StandardImageWindow *siw);

      protected:
        QList<const Node*> _nodes;
        
    };
}

#endif