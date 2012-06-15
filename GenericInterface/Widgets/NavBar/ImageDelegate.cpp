#include <iostream>

#include <QToolTip>
#include <QAbstractItemView>

#include <Image.h>

#include "ImageDelegate.h"
#include "../ImageWidgets/StandardImageView.h"
#include "NodeListModel.h"
#include "../../Services/Node.h"

using namespace genericinterface;


ImageDelegate::ImageDelegate(QSize itemSize) : _itemSize(itemSize) {
}

const QPixmap* ImageDelegate::getPixmap(const QModelIndex &index) const {
    QVariant data = index.data();
    if(!data.canConvert<const Node*>()) {
        return NULL;
    }
    const Node* node = data.value<const Node*>();
    if(node == NULL) return NULL;
    return &node->pixmap;
}

QSize ImageDelegate::getItemSize(const QModelIndex &index) const {
    const QPixmap* pixmap = getPixmap(index);
    if(pixmap != NULL) {
        QSize size = pixmap->size();
        size.scale(_itemSize, Qt::KeepAspectRatio);
        return size;
    }
    return QSize();
}

QRect ImageDelegate::getCloseRect(const QModelIndex &index, QRect visualRect) const {    
    QRect rect = QRect(visualRect.topLeft(), getItemSize(index));
    rect.adjust(4, 4, -4, -4);
    
    QRect imgRect = rect.adjusted(4, 4, -4, -4);
    
    QSize closeSize = QSize(12, 12);
    QPoint closeTopLeft = QPoint(imgRect.right()-closeSize.width()/2, imgRect.top()-closeSize.height()/2);
    QRect closeRect = QRect(closeTopLeft, closeSize);
    QRect inCloseRect = closeRect.adjusted(2, 2, -1, -1);
    return inCloseRect;
}

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    //std::cout << "paint : " << option.rect.x() << ":" << option.rect.y() << ":" << option.rect.width() << ":" << option.rect.height() << std::endl;
    QRect rect = QRect(option.rect.topLeft(), getItemSize(index));
    rect.adjust(4, 4, -4, -4);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, option.palette.highlight());
    }    

    painter->setRenderHint(QPainter::Antialiasing, true);
    
    QRect imgRect = rect.adjusted(4, 4, -4, -4);
    const QPixmap* pixmap = getPixmap(index);
    if(pixmap != NULL) {
        painter->drawPixmap(imgRect, *pixmap);
    }
    
    QSize closeSize = QSize(12, 12);
    QPoint closeTopLeft = QPoint(imgRect.right()-closeSize.width()/2, imgRect.top()-closeSize.height()/2);
    QRect closeRect = QRect(closeTopLeft, closeSize);
    painter->setBrush(QBrush(QColor(128,128,128)));
    painter->setPen(QPen(QColor(40,40,40), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawEllipse(closeRect);
    painter->setPen(QPen(QColor(40,40,40), 2.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRect inCloseRect = closeRect.adjusted(2, 2, -1, -1);
    painter->drawLine(inCloseRect.topLeft(), inCloseRect.bottomRight());
    painter->drawLine(inCloseRect.bottomLeft(), inCloseRect.topRight());

    painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return getItemSize(index);
}

bool ImageDelegate::helpEvent(QHelpEvent* event, QAbstractItemView* view, const QStyleOptionViewItem& option, const QModelIndex& index ) {
    if ( !event || !view ) {
        return false;
    }
    if ( event->type() == QEvent::ToolTip ) {
        QVariant data = index.data();
        if(data.canConvert<const Node*>()) {
            const Node* node = data.value<const Node*>();
            QToolTip::showText( event->globalPos(), node->path, view);
            return true;
        }
    }
    return false;
}