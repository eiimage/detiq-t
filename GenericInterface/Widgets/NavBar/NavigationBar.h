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
        void mouseReleaseEvent(QMouseEvent* event );
      signals:
        void removeNode(NodeId);
    };
}

#endif
