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

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include <cmath>
#include <QWidget>
#include <QPixmap>

#include <Image.h>

class ImageWidget : public QWidget  {
  public:

    static QImage convertImage(const imagein::Image* image);

    ImageWidget(QWidget* parent, const imagein::Image* img = NULL);

    void setImage(const imagein::Image* img);

    inline QPixmap pixmap() const { return _pixmap; }

    virtual QSize sizeHint() const { return this->size(); }

    //inline double scale() const { return static_cast<double>(_pixmap.width()) / static_cast<double>(this->width()); }

    inline QPoint mapToPixmap(QPoint p) const {
        if(this->size().isEmpty()) return QPoint();
        const int x = std::floor( p.x() * pixmap().width() / width() );
        const int y = std::floor( p.y() * pixmap().height() / height() );
        return QPoint(x, y);
    }

    inline QSize mapToPixmap(QSize s) const {
        if(this->size().isEmpty()) return QSize();
        const int w = std::floor( s.width() * pixmap().width() / width() );
        const int h = std::floor( s.height() * pixmap().height() / height() );
        return QSize(w, h);
    }

    inline QPoint mapFromPixmap(QPoint p) const {
        if(pixmap().size().isEmpty()) return QPoint();
        const int x = std::floor( p.x() * width() / pixmap().width() );
        const int y = std::floor( p.y() * height() / pixmap().height() );
        return QPoint(x, y);
    }

    inline QSize mapFromPixmap(QSize s) const {
        if(pixmap().size().isEmpty()) return QSize();
        const int w = std::floor( s.width() * width() / pixmap().width() );
        const int h = std::floor( s.height() * height() / pixmap().height() );
        return QSize(w, h);
    }

    inline QRect mapFromPixmap(QRect r) const {
        return QRect(mapFromPixmap(r.topLeft()), mapFromPixmap(r.size()));
    }

    inline QRect mapToPixmap(QRect r) const {
        return QRect(mapToPixmap(r.topLeft()), mapToPixmap(r.size()));
    }

  protected:
    void paintEvent (QPaintEvent* event );
    QPixmap _pixmap;
};

#endif // IMAGEWIDGET_H
