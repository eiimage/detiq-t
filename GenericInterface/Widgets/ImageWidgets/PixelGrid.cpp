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

#include <QPainter>

#include "PixelGrid.h"

using namespace std;
using namespace genericinterface;
using namespace imagein;

PixelGrid::PixelGrid(const imagein::Image* image) : _image(image), _offset(0,0), _channel(0)  {
    _pixmap.convertFromImage(convertImage(_image));
}

void PixelGrid::setOffset(QPoint offset) {
    _offset = QPoint(max(0,offset.x()), max(0,offset.y()));
    this->update();
}

void PixelGrid::setChannel(int c) {
    _channel = c;
    this->update();
}

inline uintmax_t sum(const Image* im, int i, int j) {
    uintmax_t sum = 0;
    for(unsigned int c=0; c<im->getNbChannels(); ++c) {
        sum += static_cast<uintmax_t>(im->getPixel(i, j, c));
    }
    return sum;
}

void PixelGrid::resizeEvent(QResizeEvent* event) {
    emit resized(event->size()/PIXEL_S);
}

void PixelGrid::paintEvent (QPaintEvent* event ) {
    QPainter painter(this);
    
    QSize srcSize(this->width()/PIXEL_S, this->height()/PIXEL_S);
    QSize dstSize(srcSize.width()*PIXEL_S, srcSize.height()*PIXEL_S);
    
    /* draw the image's pixmap */
    painter.drawPixmap(QRect(QPoint(0,0), dstSize), _pixmap, QRect(_offset, srcSize));
    
    /* draw the grid's lines */
    painter.setPen(Qt::black);
    for(int i = 0; i <= srcSize.width(); ++i) {
        painter.drawLine(i*PIXEL_S, 0, i*PIXEL_S, srcSize.height()*PIXEL_S); 
    }
    for(int i = 0; i <= srcSize.height(); ++i) {
        painter.drawLine(0, i*PIXEL_S, srcSize.width()*PIXEL_S, i*PIXEL_S); 
    }

    /* draw the text */
    painter.setFont(QFont("arial", 8));
    const int offsetY = (PIXEL_S+painter.fontMetrics().height())/2;
    for(int j = 0; j < srcSize.height(); ++j) {
        for(int i = 0; i < srcSize.width(); ++i) {
            try {
                Image::depth_t value = _image->getPixel(i+_offset.x(), j+_offset.y(), _channel);
                painter.setPen( (sum(_image, i+_offset.x(), j+_offset.y()) > 3*127) ? Qt::black : Qt::white ); 
                QString string = QString("%1").arg(value);
                const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
                painter.drawText(QPointF(i*PIXEL_S+offsetX, j*PIXEL_S+offsetY), string);
            }
            catch(out_of_range&) {}
        }
    }
}
