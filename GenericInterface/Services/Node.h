#ifndef QTINTERFACE_NODE_H
#define QTINTERFACE_NODE_H

#include <iostream>
#include <QString>
#include <QList>
#include <QMdiSubWindow>
#include <QMetaType>
#include <QPixmap>

#include <Image.h>


namespace genericinterface
{
    QImage convertImage(const imagein::Image* image);
    struct NodeId {
      public:
        inline NodeId(const imagein::Image* id = NULL) : _id(id) {}
        inline bool operator==(const NodeId& other) { return _id==other._id; }
        inline bool operator<(const NodeId& other) const { return _id<other._id; }
        inline bool isValid() { return _id != NULL;}
      private:
        const imagein::Image* _id;    
    };
    
    struct Node {
        inline Node() : image(NULL), path("") {}
        inline Node(const imagein::Image* img, QString path_) : image(img), path(path_), pixmap(QPixmap::fromImage(convertImage(img))) {}
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
