/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of DETIQ-T.
 *
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QPainter>

#include "ImageWidget.h"

using namespace std;
using namespace imagein;


ImageWidget::ImageWidget(QWidget* parent, const imagein::Image* img) : QWidget(parent) {
    if(img != NULL) {
        this->setImage(img);
    }
}

void ImageWidget::setImage(const imagein::Image* img) {
    _pixmap.convertFromImage(convertImage(img));
}

void ImageWidget::paintEvent (QPaintEvent* /*event*/ ) {
    QPainter painter(this);
    painter.drawPixmap(this->rect(), _pixmap);
}

QImage ImageWidget::convertImage(const imagein::Image* img)
{
    unsigned int channels = img->getNbChannels();
//    bool hasAlpha = (channels== 4 || channels == 2);
//    QImage qImg(img->getWidth(), img->getHeight(), (hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32));
    QImage qImg(img->getWidth(), img->getHeight(), QImage::Format_RGB32);
    //on récupère les bits de l'image qt, qu'on cast en QRgb (qui fait 32 bits -> une image RGB(A))
    QRgb* data = reinterpret_cast<QRgb*>(qImg.bits());
    //Pour chaque pixel de l'image Qt, on récupère les données correspondantes de l'image ImageIn grace �  l'itérateur
    Image::const_iterator it = img->begin();
    int size = qImg.width()*qImg.height();
    switch(channels) {
        case 0: break;
        case 1:
        {
            for(int i = 0; i < size; ++i) {
                const imagein::Image::depth_t gray = *(it);
                data[i] = qRgb(gray, gray, gray);
                ++it;
            }
            break;
        }
        case 2:
        {
            for(int i = 0; i < size; ++i) {
                const imagein::Image::depth_t gray = *(it);
                const imagein::Image::depth_t alpha = *(it + size);
                data[i] = qRgba(gray, gray, gray, alpha);
                ++it;
            }
            break;
        }
        case 3:
        {
            for(int i = 0; i < size; ++i) {
                const imagein::Image::depth_t red = *(it);
                const imagein::Image::depth_t green = *(it + size );
                const imagein::Image::depth_t blue = *(it + size*2 );
                data[i] = qRgb(red, green, blue);
                ++it;
            }
            break;
        }
        default:
        {
            for(int i = 0; i < size; ++i) {
                const imagein::Image::depth_t red = *(it);
                const imagein::Image::depth_t green = *(it + size );
                const imagein::Image::depth_t blue = *(it + size*2 );
//                const imagein::Image::depth_t alpha = *(it + size*3 );
//                data[i] = qRgba(red, green, blue, alpha);
                data[i] = qRgb(red, green, blue);
                ++it;
            }
        }
    }
    return qImg;
}
