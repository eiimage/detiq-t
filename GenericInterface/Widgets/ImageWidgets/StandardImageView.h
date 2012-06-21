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

#ifndef STANDARDIMAGEVIEW_H
#define STANDARDIMAGEVIEW_H

#include <QCursor>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QString>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QObject>
#include <QRubberBand>
//#include <QGLWidget>
#include <iostream>

#include "ImageContextMenu.h"
#include "GenericHistogramView.h"
#include "ImageWindow.h"
#include "HistogramWindow.h"

#include <Image.h>
#include <Rectangle.h>

namespace genericinterface
{
  QImage convertImage(const imagein::Image* image);
  /*!
   * \brief Display an image from imagein
   *
   * Display an image from imagein and manages mouse events
   */
  class StandardImageView : public QScrollArea
  {
  Q_OBJECT

  public:
  
    class ImgWidget : public QWidget  {
      public:
        ImgWidget(QWidget* parent, const imagein::Image* img);
        inline QPixmap pixmap() const { return _pixmap; }
        virtual QSize sizeHint() const { return this->size(); }
        inline double scale() const { return static_cast<double>(_pixmap.width()) / static_cast<double>(this->width()); }
      protected:
        void paintEvent (QPaintEvent* event );
        QPixmap _pixmap;
    };
  
    enum Mode {MODE_MOUSE, MODE_SELECT};
    enum SelectMode {SELECTMODE_NONE, SELECTMODE_MAKE, SELECTMODE_RESIZE, SELECTMODE_MOVE};
		/*!
		 * \brief Default constructor
		 * 
		 * Display image.
		 * 
		 * \param parent The parent widget
		 * \param image The image to display
		 */
    StandardImageView(QWidget* parent, imagein::Image* image);
        
		/*!
		 * \brief StandardImageView destructor.
		 */
		virtual ~StandardImageView();
    
    void setImage(imagein::Image* image);
    
		//! Returns the image
    inline const imagein::Image* getImage() const { return _image; }
        
		//! Returns the pixmap
    //inline const QPixmap* getPixmap() const { return _pixmap_img; }
    inline QPixmap pixmap() const { return _imgWidget->pixmap(); }
        
		//! Returns the selection rectangle
    inline imagein::Rectangle getRectangle() const { 
        return imagein::Rectangle(_select.x(), _select.y(), _select.width(), _select.height()); 
    }
    
    inline void setSelectSrc(GenericHistogramView* src) { _selectSrc = src; }
        
	/*	//! Returns the graphics view
    inline QGraphicsView* getGraphicsView() const { return _view; }*/
    //inline QGraphicsRectItem* getHighlightItem() { return _highlight; }
    
    
    void switchMode(Mode mode);

	public slots:
    void ctrlPressed(bool);
    void showSelectRect(imagein::Rectangle rect, GenericHistogramView* source);
    void selectAll();
    void scale(double);
    virtual QSize sizeHint() const { return _imgWidget->size()+QSize(frameWidth()*2,frameWidth()*2); }
		
	signals:
		/*!
		 * \brief Signal emits when the mouse left button is clicked
		 * 
		 * \param x X coordinate of click
		 * \param y Y coordinate of click
		 */
		void pixelClicked(int x, int y) const;
		
		/*!
		 * \brief Signal emits when the mouse move over the view
		 * 
		 * \param x X coordinate of mouse
		 * \param y Y coordinate of mouse
		 */
		void pixelHovered(int x, int y) const;
		
		/*!
		 * \brief Signal emits when zoom is changed
		 * 
		 * \param z The zoom factor
		 */
		//void zoomChanged(double z) const;
        
        void startDrag();
    
  protected:
    QWidget* _parent;
    //QGraphicsScene* _scene;
    //QGraphicsView* _view;
    //QGraphicsRectItem* _highlight;
    
    imagein::Image* _image;
    ImgWidget* _imgWidget;
    QRect _select;
    QRect _oldSelect;
    GenericHistogramView* _selectSrc;
    //imagein::Rectangle _visibleArea;
    
    bool _ctrlDown;
    SelectMode _selectMode;
    bool _originX;
    bool _originY;
    bool _vLine;
    bool _hLine;
    QPoint _downPos;
    QRubberBand* _rubberBand;
    
    Mode _mode;
    
    void initMenu();
    void showImage();
    Qt::CursorShape mouseOverHighlight(QPoint pos);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event); 
    void wheelEvent(QWheelEvent* event);
    void selectionMove(QPoint);
    void selectionResize(QPoint);
    void selectionMake(QPoint);
    inline void redrawSelect() {
        _rubberBand->setGeometry(QRect(
            _select.x()*_imgWidget->width()/_imgWidget->pixmap().width(), 
            _select.y()*_imgWidget->height()/_imgWidget->pixmap().height(), 
            _select.width()*_imgWidget->width()/_imgWidget->pixmap().width(), 
            _select.height()*_imgWidget->height()/_imgWidget->pixmap().height()
        ));
    }
  };
}

#endif // STANDARDIMAGEVIEW_H
