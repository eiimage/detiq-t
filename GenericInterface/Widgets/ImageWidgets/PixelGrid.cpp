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
#include "ImageWidget.h"

using namespace std;
using namespace genericinterface;
using namespace imagein;

PixelGrid::PixelGrid(const imagein::Image* image) : _image(image), _offset(0,0), _channel(0)  {
    _pixmap.convertFromImage(ImageWidget::convertImage(_image));
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
    return sum / im->getNbChannels();
}

void PixelGrid::resizeEvent(QResizeEvent* event) {
    emit resized(event->size()/PIXEL_S);
}

void PixelGrid::paintEvent (QPaintEvent* /*event*/ ) {
    QPainter painter(this);
    
    QSize srcSize(this->width()/PIXEL_S-1, this->height()/PIXEL_S-1);
    QSize dstSize(srcSize.width()*PIXEL_S, srcSize.height()*PIXEL_S);
    
    /* draw the image's pixmap */
    painter.drawPixmap(QRect(QPoint(PIXEL_S,PIXEL_S), dstSize), _pixmap, QRect(_offset, srcSize));
    
    /* draw the grid's lines */
    painter.setPen(Qt::black);
    for(int i = 1; i <= srcSize.width()+1; ++i) {
        painter.drawLine(i*PIXEL_S, 0, i*PIXEL_S, (srcSize.height()+1)*PIXEL_S);
    }
    for(int i = 1; i <= srcSize.height()+1; ++i) {
        painter.drawLine(0, i*PIXEL_S, (srcSize.width()+1)*PIXEL_S, i*PIXEL_S);
    }

    painter.setFont(QFont("arial", 8));
    int offsetY = (PIXEL_S+painter.fontMetrics().height())/2;
    for(int i = 0; i < srcSize.width(); ++i) {
        QString string = QString("%1").arg(_offset.x()+i);
        const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
        painter.drawText(QPointF((i+1)*PIXEL_S+offsetX, offsetY), string);
    }
    for(int j = 0; j < srcSize.height(); ++j) {
        QString string = QString("%1").arg(_offset.y()+j);
        const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
        painter.drawText(QPointF(offsetX, (j+1)*PIXEL_S+offsetY), string);
    }

    /* draw the text */
    painter.setFont(QFont("arial", 8));
    offsetY = (PIXEL_S+painter.fontMetrics().height())/2;
    for(int j = 0; j < srcSize.height(); ++j) {
        for(int i = 0; i < srcSize.width(); ++i) {
            try {
                Image::depth_t value = _image->getPixel(i+_offset.x(), j+_offset.y(), _channel);
                painter.setPen( (sum(_image, i+_offset.x(), j+_offset.y()) >= 127) ? Qt::black : Qt::white );
                QString string = QString("%1").arg(value);
                const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
                painter.drawText(QPointF((i+1)*PIXEL_S+offsetX, (j+1)*PIXEL_S+offsetY), string);
            }
            catch(out_of_range&) {}
        }
    }
}

DoublePixelGrid::DoublePixelGrid(const imagein::Image_t<double>* dataImg, const imagein::Image* displayImg)
    : PixelGrid(displayImg), _dataImg(dataImg) {
}

void DoublePixelGrid::paintEvent (QPaintEvent* /*event */) {
//    QPainter painter(this);

//    QSize srcSize(this->width()/PIXEL_S, this->height()/PIXEL_S);
//    QSize dstSize(srcSize.width()*PIXEL_S, srcSize.height()*PIXEL_S);

//    /* draw the image's pixmap */
//    painter.drawPixmap(QRect(QPoint(0,0), dstSize), _pixmap, QRect(_offset, srcSize));

//    /* draw the grid's lines */
//    painter.setPen(Qt::black);
//    for(int i = 0; i <= srcSize.width(); ++i) {
//        painter.drawLine(i*PIXEL_S, 0, i*PIXEL_S, srcSize.height()*PIXEL_S);
//    }
//    for(int i = 0; i <= srcSize.height(); ++i) {
//        painter.drawLine(0, i*PIXEL_S, srcSize.width()*PIXEL_S, i*PIXEL_S);
//    }

//    /* draw the text */
//    painter.setFont(QFont("arial", 7));
//    const int offsetY = (PIXEL_S+painter.fontMetrics().height())/2;
//    for(int j = 0; j < srcSize.height(); ++j) {
//        for(int i = 0; i < srcSize.width(); ++i) {
//            try {
//                double value = _dataImg->getPixel(i+_offset.x(), j+_offset.y(), _channel);
//                painter.setPen( (sum(_image, i+_offset.x(), j+_offset.y()) > 127) ? Qt::black : Qt::white );
//                QString string = QString("%1").arg(value,0,'f',2);
//                const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
//                painter.drawText(QPointF(i*PIXEL_S+offsetX, j*PIXEL_S+offsetY), string);
//            }
//            catch(out_of_range&) {}
//        }
//    }
    QPainter painter(this);

    QSize srcSize(this->width()/PIXEL_S-1, this->height()/PIXEL_S-1);
    QSize dstSize(srcSize.width()*PIXEL_S, srcSize.height()*PIXEL_S);

    /* draw the image's pixmap */
    painter.drawPixmap(QRect(QPoint(PIXEL_S,PIXEL_S), dstSize), _pixmap, QRect(_offset, srcSize));

    /* draw the grid's lines */
    painter.setPen(Qt::black);
    for(int i = 1; i <= srcSize.width()+1; ++i) {
        painter.drawLine(i*PIXEL_S, 0, i*PIXEL_S, (srcSize.height()+1)*PIXEL_S);
    }
    for(int i = 1; i <= srcSize.height()+1; ++i) {
        painter.drawLine(0, i*PIXEL_S, (srcSize.width()+1)*PIXEL_S, i*PIXEL_S);
    }

    painter.setFont(QFont("arial", 8));
    int offsetY = (PIXEL_S+painter.fontMetrics().height())/2;
    for(int i = 0; i < srcSize.width(); ++i) {
        QString string = QString("%1").arg(_offset.x()+i);
        const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
        painter.drawText(QPointF((i+1)*PIXEL_S+offsetX, offsetY), string);
    }
    for(int j = 0; j < srcSize.height(); ++j) {
        QString string = QString("%1").arg(_offset.y()+j);
        const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
        painter.drawText(QPointF(offsetX, (j+1)*PIXEL_S+offsetY), string);
    }

    /* draw the text */
    painter.setFont(QFont("arial", 8));
    offsetY = (PIXEL_S+painter.fontMetrics().height())/2;
    const double max = _dataImg->max();
    char format;
    int precision;
    if(max < 0.01) {
        format = 'e';
        precision = 0;
    }
    else if(max < 10) {
        format = 'f';
        precision = 3;
    }
    else if(max < 100) {
        format = 'f';
        precision = 2;
    }
    else if(max < 1000) {
        format = 'f';
        precision = 1;
    }
    else if(max < 100000) {
        format = 'f';
        precision = 0;
    }
    else {
        format = 'e';
        precision = 0;
    }
    for(int j = 0; j < srcSize.height(); ++j) {
        for(int i = 0; i < srcSize.width(); ++i) {
            try {
                double value = _dataImg->getPixel(i+_offset.x(), j+_offset.y(), _channel);
                painter.setPen( (sum(_image, i+_offset.x(), j+_offset.y()) >= 127) ? Qt::black : Qt::white );
                QString string = QString("%1").arg(value, 0, format, precision);
                const int offsetX = (PIXEL_S-painter.fontMetrics().width(string))/2;
                painter.drawText(QPointF((i+1)*PIXEL_S+offsetX, (j+1)*PIXEL_S+offsetY), string);
            }
            catch(out_of_range&) {}
        }
    }
}
