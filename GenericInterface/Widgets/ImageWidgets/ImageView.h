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
#include "HistogramWindow.h"
#include "ImageWidget.h"

#include <Image.h>
#include <Rectangle.h>

namespace genericinterface
{
  /*!
   * \brief Display an image from imagein
   *
   * Display an image from imagein and manages mouse events
   */
  class ImageView : public QScrollArea
  {
  Q_OBJECT

  public:
  
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
    ImageView(QWidget* parent, const imagein::Image* image = NULL);

    /*!
     * \brief StandardImageView destructor.
     */
    virtual ~ImageView();
    
    void setImage(const imagein::Image* image);
    
		//! Returns the pixmap
    inline QPixmap pixmap() const { return _imgWidget->pixmap(); }
    inline const ImageWidget* widget() { return _imgWidget; }
        
		//! Returns the selection rectangle
    inline imagein::Rectangle getRectangle() const { 
        return imagein::Rectangle(_select.x(), _select.y(), _select.width(), _select.height()); 
    }
   inline QRect select() const { return _select; } 
    inline void setSelectSrc(GenericHistogramView* src) { _selectSrc = src; }
        
    void switchMode(Mode mode);
    inline Mode mode() const { return _mode; }

    public slots:

    void showSelectRect(imagein::Rectangle rect, GenericHistogramView* source);
    void moveSelection(QRect rect);
    void selectAll();
    void scale(double, double);
    virtual QSize sizeHint() const { return pixmap().size()+QSize(frameWidth()*2,frameWidth()*2); }
    
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event); 
    void wheelEvent(QWheelEvent* event);
		
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
        
        void startDrag();
        void updateSrc(GenericHistogramView*, imagein::Rectangle);
        void selectionMoved(QRect);

  protected:
    QWidget* _parent;

    ImageWidget* _imgWidget;
    QRect _select;
    QRect _oldSelect;
    GenericHistogramView* _selectSrc;
    
    bool _ctrlDown;
    SelectMode _selectMode;
    bool _originX;
    bool _originY;
    bool _vLine;
    bool _hLine;
    QPoint _downPos;
    QRubberBand* _rubberBand;
    
    Mode _mode;
    
    Qt::CursorShape mouseOverHighlight(QMouseEvent*);
    void selectionMove(QPoint);
    void selectionResize(QPoint);
    void selectionMake(QPoint);
    inline void redrawSelect() {
        _rubberBand->setGeometry(_imgWidget->mapFromPixmap(_select));
    }

    inline QPoint mapToWidget(QPoint p) const {
        return p - _imgWidget->geometry().topLeft();
    }
    inline QPoint mapFromWidget(QPoint p) const {
        return p + _imgWidget->geometry().topLeft();
    }
    inline QPoint mapToPixmap(QPoint p) const {
        return _imgWidget->mapToPixmap(mapToWidget(p)); 
    }
    inline QSize mapToPixmap(QSize s) const {
        return _imgWidget->mapToPixmap(s); 
    }
    inline QPoint mapFromPixmap(QPoint p) const {
        return mapFromWidget(_imgWidget->mapFromPixmap(p)); 
    }
    inline QSize mapFromPixmap(QSize s) const {
        return _imgWidget->mapFromPixmap(s); 
    }
    inline QRect mapFromPixmap(QRect r) const {
        return QRect(mapFromPixmap(r.topLeft()), mapFromPixmap(r.size())); 
    }
    inline QRect mapToPixmap(QRect r) const {
        return QRect(mapToPixmap(r.topLeft()), mapToPixmap(r.size()));
    }
  };
}

#endif // STANDARDIMAGEVIEW_H
