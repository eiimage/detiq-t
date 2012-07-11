#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include "ImageWidget.h"
#include <QRubberBand>


namespace genericinterface 
{
  class ThumbnailView : public ImageWidget
  {
    Q_OBJECT

    public:

      ThumbnailView(QWidget* parent, const imagein::Image* image);
    
    signals:
      void positionChanged(QPoint pos);


    public slots:
      void setRectSize(QSize size);

    protected:
      void mouseMoveEvent(QMouseEvent * event);

      QRubberBand _rubberBand;
      QRect _select;

    
  };
}
#endif
