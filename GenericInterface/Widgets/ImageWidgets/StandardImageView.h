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
#include <iostream>

#include "ImageContextMenu.h"
#include "GenericHistogramView.h"
#include "ImageWindow.h"

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
  class StandardImageView : public QObject, public QGraphicsPixmapItem
  {
  Q_OBJECT

  public:
  
    enum Mode {MODE_MOUSE, MODE_SELECT};
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
        
		//! Returns the selection rectangle
    inline imagein::Rectangle getRectangle() const { return _selection; }
        
	/*	//! Returns the graphics view
    inline QGraphicsView* getGraphicsView() const { return _view; }*/
    inline QGraphicsRectItem* getHighlightItem() { return _highlight; }
    
    
    inline void setMode(Mode mode) { _mode = mode; }

	public slots:
    void ctrlPressed();
    void showHighlightRect(imagein::Rectangle rect, ImageWindow* source);
    //void zoom(int delta);
		
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
    
  private slots:
    void toggleSelection();
    
  private:
    QWidget* _parent;
    //QGraphicsScene* _scene;
    //QGraphicsView* _view;
    QGraphicsRectItem* _highlight;
    GenericHistogramView* _sourceHighlight;
    
    imagein::Image* _image;
    imagein::Rectangle _selection;
    imagein::Rectangle _visibleArea;
    
    bool _selectionOn;
    bool _ctrlPressed;
    bool _mouseButtonPressed;
    QPoint _pixelClicked;
    imagein::Rectangle _originalHighlight;
    bool _resize;
    bool _move;
    bool _originX;
    bool _originY;
    bool _vLine;
    bool _hLine;
    
    Mode _mode;
    
    void initMenu();
    void showImage();
    Qt::CursorShape mouseOverHighlight(int x, int y);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
    //void wheelEvent(QGraphicsSceneWheelEvent* event);
  };
}

#endif // STANDARDIMAGEVIEW_H
