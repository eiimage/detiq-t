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
