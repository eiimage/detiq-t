/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of EIImage.
 *
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
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

void ImageWidget::paintEvent (QPaintEvent* event ) {
    QPainter painter(this);
    painter.drawPixmap(this->rect(), _pixmap);
}

QImage ImageWidget::convertImage(const imagein::Image* img)
{
    bool hasAlpha = (img->getNbChannels() == 4 || img->getNbChannels() == 2);
    QImage qImg(img->getWidth(), img->getHeight(), (hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32));
    //on récupère les bits de l'image qt, qu'on cast en QRgb (qui fait 32 bits -> une image RGB(A))
    QRgb* data = reinterpret_cast<QRgb*>(qImg.bits());
    //Pour chaque pixel de l'image Qt, on récupère les données correspondantes de l'image ImageIn grace à l'itérateur
    Image::const_iterator it = img->begin();
    for(int i = 0; i < qImg.width()*qImg.height(); ++i) {
        int red, green, blue, alpha;
        if(img->getNbChannels() < 3) {
            red = green = blue = *(it++);
        }
        else {
            red = *(it++);
            green = *(it++);
            blue = *(it++);
        }
        if(hasAlpha) {
            alpha = *(it++);
            //On utilise la fonction qRgb(a) pour en faire un pointeur de qRgb
            data[i] = qRgba(red, green, blue, alpha);
        }
        else {
            data[i] = qRgb(red, green, blue);
        }
    }
    return qImg;
}
