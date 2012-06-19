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

#ifndef QTINTERFACE_WIDGET_NAVIGATIONBAR_H
#define QTINTERFACE_WIDGET_NAVIGATIONBAR_H

#include <QListView>

#include "ImageDelegate.h"
#include "../../Services/Node.h"

namespace genericinterface
{
    class NavigationBar : public QListView
    {
      Q_OBJECT
      public:
        NavigationBar(QSize itemSize, Qt::Orientation);
        QSize sizeHintForIndex(const QModelIndex& index) const;
        QSize sizeHint() const;
      protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event );
        void resizeEvent(QResizeEvent* e);
        QSize _itemSize;
        QPoint _downPos;
        bool _mouseDown;
      public slots:
        void setOrientation(Qt::Orientation);
      signals:
        void removeNode(NodeId);
        void itemClicked(const QModelIndex& index, QPoint downPos, QPoint upPos);
        void resized();
     
    };
}

#endif
