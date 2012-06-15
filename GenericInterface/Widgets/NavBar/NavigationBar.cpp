#include "NavigationBar.h"
#include <iostream>
#include <QMouseEvent>

using namespace genericinterface;
using namespace imagein;

NavigationBar::NavigationBar(QSize itemSize, Qt::Orientation orientation) : QListView(), _itemSize(itemSize), _mouseDown(false)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DragDrop);
    setAcceptDrops(true);
    
    if(orientation == Qt::Horizontal) {
        this->setFlow(LeftToRight);
        this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    }
    else {
        this->setFlow(TopToBottom);
        this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    }
}

void NavigationBar::mousePressEvent(QMouseEvent* event) {
    std::cout << "NavigationBar::mousePressEvent : " << event->x() << ':' << event->y() << std::endl;
    _mouseDown = true;
    _downPos = event->pos();
    QListView::mousePressEvent(event);
}

void NavigationBar::mouseReleaseEvent(QMouseEvent* event) {
    std::cout << "NavigationBar::mouseReleaseEvent : " << event->x() << ':' << event->y() << std::endl;
    
    if(_mouseDown) {
        QPoint upPos = event->pos();
        QModelIndex downIndex = this->indexAt(_downPos);
        QModelIndex upIndex = this->indexAt(upPos);
        
        if(downIndex.isValid() && upIndex.isValid() && downIndex==upIndex) {
            emit itemClicked(upIndex, _downPos, upPos);
        }
    }
    
    QListView::mouseReleaseEvent(event);
}