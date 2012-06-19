/*
 * Copyright 2011-2012 Benoit Averty, Samuel Babin, Matthieu Bergere, Thomas Letan, Sacha Percot-Tétu, Florian Teyssier
 * 
 * This file is part of DETIQ-T.
 * 
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

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