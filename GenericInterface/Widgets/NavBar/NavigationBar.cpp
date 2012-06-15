#include "NavigationBar.h"
#include <iostream>
#include <QMouseEvent>

using namespace genericinterface;

NavigationBar::NavigationBar() : QListView()
{
    setFixedWidth(96);
    ImageDelegate* im = new ImageDelegate(92);
    setItemDelegate(im);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DragDrop);
    setAcceptDrops(true);
}

void NavigationBar::mouseReleaseEvent(QMouseEvent* event) {
    //std::cout << event->x() << ':' << event->y() << std::endl;
    QModelIndex index = this->indexAt(event->pos());
    QRect rect = this->visualRect(index);
    if(!rect.isValid()) return;
    int posx = event->x() - rect.x();
    int posy = event->y() - rect.y();
    //std::cout << posx << "/" << rect.width() << ':' << posy << "/" << rect.height() << std::endl;
    if(posx>=0 && posx<rect.width() && posy>=0 && posy<rect.width()
    && (rect.width() - posx) < 14 && (posy) < 13  ) {
        QVariant data = index.data();
        if(data.canConvert<const Node*>()) {
            const Node* node = data.value<const Node*>();
            emit removeNode(node->getId());
        }
         
    }
    QListView::mouseReleaseEvent(event);
}