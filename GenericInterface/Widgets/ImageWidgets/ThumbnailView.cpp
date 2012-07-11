#include <QMouseEvent>

#include "ThumbnailView.h"

using namespace std;
using namespace genericinterface;
using namespace imagein;


ThumbnailView::ThumbnailView(QWidget* parent, const Image* image) 
  : ImageWidget(parent, image), _rubberBand(QRubberBand::Rectangle, this) {
    this->setMouseTracking(false);
    _rubberBand.show();
}


void ThumbnailView::mouseMoveEvent(QMouseEvent * event) {
    QPoint pos = QPoint(event->pos().x()*pixmap().width()/width(), event->pos().y()*pixmap().height()/height());
    int x = max(0, pos.x()-_select.width()/2);
    int y = max(0, pos.y()-_select.height()/2);
    x = min(x, pixmap().width() - _select.width());
    y = min(y, pixmap().height() - _select.height());
    _select.moveTo(QPoint(x,y));
    _rubberBand.move(QPoint(x*width()/pixmap().width(), y*height()/pixmap().height()));
    emit positionChanged(QPoint(x,y));
}

void ThumbnailView::setRectSize(QSize size) {
    _select.setSize(size);
    _rubberBand.resize(QSize(size.width()*width()/pixmap().width(),size.height()*height()/pixmap().height()));
}

