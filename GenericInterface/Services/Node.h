#ifndef QTINTERFACE_NODE_H
#define QTINTERFACE_NODE_H

#include <iostream>
#include <QString>
#include <QList>
#include <QMdiSubWindow>
#include <QMetaType>

#include <Image.h>


namespace genericinterface
{
    struct NodeId {
      public:
        inline NodeId(const imagein::Image* id = NULL) : _id(id) {}
        inline bool operator==(const NodeId& other) { return _id==other._id; }
        inline bool operator<(const NodeId& other) const { return _id<other._id; }
      private:
        const imagein::Image* _id;    
    };
    
    struct Node {
        inline Node() : image(NULL), path("") {}
        inline Node(const imagein::Image* img, QString path_) : image(img), path(path_) {}
        NodeId getId() const { return image; }
        const imagein::Image* image;
        QString path;
        QList<QMdiSubWindow*> windows;
        ~Node() {
        }
    };
}
Q_DECLARE_METATYPE(const genericinterface::Node*);

#endif
