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

const Image* getImage(const QModelIndex &index) {
    QVariant data = index.data();
    if(!data.canConvert<const Node*>()) {
        return NULL;
    }
    const Node* node = data.value<const Node*>();
    if(node == NULL) return NULL;
    return node->image;
}

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  painter->save();

  //std::cout << option.rect.x() << ":" << option.rect.y() << ":" << option.rect.width() << ":" << option.rect.height() << std::endl;
  QRect rect = option.rect;
  rect.adjust(4, 4, -4, -4);
  
  if (option.state & QStyle::State_Selected)
    painter->fillRect(rect, option.palette.highlight());
    

  painter->setRenderHint(QPainter::Antialiasing, true);
  rect.adjust(4, 4, -4, -4);
  const Image* dataImg = getImage(index);
  if(dataImg != NULL) {
      QImage img = getQImage(dataImg);
      
      /*if(img.width() <= rect.width()) {
        painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
      }
      else {
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
      }

      if (img.width() > img.height())
      {
        double scale = (double)rect.width() / (double)img.width();
        int size = (int) ((double)img.height() * scale);
        int adjust = (rect.height() - size) / 2;
        rect.adjust(0, adjust, 0, -adjust);
      }
      else if (img.height() > img.width()) 
      {
        double scale = (double)rect.height() / (double)img.height();
        int size = (int) ((double)img.width() * scale);
        //int adjust = size / 2;
        int adjust = (rect.width() - size) / 2;
        rect.adjust(adjust, 0, -adjust, 0);
      }*/

      painter->drawImage(rect, img);
  }
  /*painter->restore();
  painter->save();*/
  
  //painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(QColor(128,128,128)));
  painter->setPen(QPen(QColor(40,40,40), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawEllipse (option.rect.right()-13, option.rect.top()+1, 12, 12);
  painter->setPen(QPen(QColor(40,40,40), 2.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawLine(option.rect.right()-11, option.rect.top()+3, option.rect.right()-3, option.rect.top()+11);
  painter->drawLine(option.rect.right()-11, option.rect.top()+11, option.rect.right()-3, option.rect.top()+3);
  
  painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  const Image* dataImg = getImage(index);
  if(dataImg == NULL) {
    std::cout << "NULL img" << std::endl;
    return QSize(0,0);
  }
  double ratio = (double)dataImg->getHeight() / (double)dataImg->getWidth();
  
  return QSize(_width, ratio*_width);
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