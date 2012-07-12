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

#ifndef QTINTERFACE_NODE_H
#define QTINTERFACE_NODE_H

#include <iostream>
#include <QString>
#include <QList>
#include <QMdiSubWindow>
#include <QMetaType>
#include <QPixmap>

#include <Image.h>
#include "../Widgets/ImageWidgets/ImageWidget.h"


namespace genericinterface
{
    struct NodeId {
      public:
        inline NodeId() : _id(0) {}
        template<typename D>
        inline NodeId(const imagein::Image_t<D>* id) : _id(reinterpret_cast<uintptr_t>(id)) {}
        inline bool operator==(const NodeId& other) { return _id==other._id; }
        inline bool operator!=(const NodeId& other) { return _id!=other._id; }
        inline bool operator<(const NodeId& other) const { return _id<other._id; }
        inline bool isValid() { return _id != 0;}
      private:
        uintptr_t _id;
    };
    
    struct Node {
        inline Node() : image(NULL), path("") {}
        inline Node(const imagein::Image* img, QString path_) : image(img), path(path_), pixmap(QPixmap::fromImage(ImageWidget::convertImage(img))) {}
        inline Node(QPixmap pixmap_, const imagein::Image* img, QString path_) : image(img), path(path_), pixmap(pixmap_) {}
        NodeId getId() const { return image; }
        inline bool isValid() { return image != NULL;}
        const imagein::Image* image;
        QString path;
        QList<QMdiSubWindow*> windows;
        QPixmap pixmap;
        ~Node() {
        }
    };
}
Q_DECLARE_METATYPE(const genericinterface::Node*);

#endif
