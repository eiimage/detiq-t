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

#ifndef WIDGET_PIXELGRID_H
#define WIDGET_PIXELGRID_H

#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <Image.h>
#include "ImageView.h"

/**
* @brief A better pixel grid.
*/

namespace genericinterface
{
  class PixelGrid : public QWidget
  {
  Q_OBJECT
  public:
    PixelGrid(const imagein::Image* image);
    /**
    * @brief Size of one pixel (in pixel... loop !)
    */
    static const int PIXEL_S = 25;

  public slots:
    /**
    * @brief [slot] change the offset ot view
    *
    * @param x x-coordinate of the top left
    * @param y y-coordinate of the top left
    */
    void setOffset(QPoint offset);
    void setChannel(int c);
    virtual QSize sizeHint() const { return QSize(16*PIXEL_S, 16*PIXEL_S); }

  signals:
    void resized(QSize size);

  protected:
  
    void paintEvent (QPaintEvent* event );
    void resizeEvent(QResizeEvent* event);

    
    QPixmap _pixmap;
    const imagein::Image* _image;
    QPoint _offset;
    int _channel;
  };
}

#endif
