#ifndef STANDARDIMAGEVIEW_H
#define STANDARDIMAGEVIEW_H

#include <QLabel>
#include <QString>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <iostream>

#include "ImageContextMenu.h"

#include <Image.h>
#include <Rectangle.h>

using namespace imagein;

namespace genericinterface
{
    class StandardImageView : public QLabel
    {
		Q_OBJECT
    private:
        QWidget* _parent;
        QPixmap* _pixmap_img;
        double _zoomFactor;
        Image* _image;
        Rectangle* _highlight;
        Rectangle* _selection;
        Rectangle* _visibleArea;
        ImageContextMenu* _menu;
        bool _ctrlPressed;
        
        void init();
        void initMenu();
        void showImage();
        void mousePressEvent(QMouseEvent * event);
        void mouseMoveEvent(QMouseEvent * event);
        void wheelEvent(QWheelEvent* event);

    public:
        StandardImageView(QWidget* parent, Image* image);
        //??? getClickedPixel();
        //ATTENTION A LA FUITE MEMOIRE !
        inline Image* getImage() const {return new Image(*_image);}
        inline QPixmap* getPixmap() const {return new QPixmap(*_pixmap_img);}
        inline Rectangle getRectangle() const { return Rectangle(*_selection); }

	public slots:
        //void showRectangle(Rectangle* rect);
        void ctrlPressed();
		
	signals:
		void pixelClicked(int x, int y);
		void pixelHovered(int x, int y);
		void zoomChanged(double z);
    };
}

#endif // STANDARDIMAGEVIEW_H