#include "StandardImageView.h"

using namespace genericinterface;
using namespace imagein;
using namespace std;

StandardImageView::StandardImageView(QWidget* parent, Image* image): QGraphicsPixmapItem(), _parent(parent), _image(image)
{
    _mode = MODE_MOUSE;
    _selection = Rectangle(0, 0, _image->getWidth(), _image->getHeight());
    _visibleArea = Rectangle(0, 0, _image->getWidth(), _image->getHeight());
	_scene = new QGraphicsScene();
    _view = new QGraphicsView();

	this->setAcceptHoverEvents(true);
    
	_zoomFactor = 1;
    _ctrlPressed = false;
  
    _sourceHighlight = NULL;
    _originalHighlight = Rectangle(0, 0, _image->getWidth(), _image->getHeight());
    _resize = false;
    _originX = false;
    _originY = false;
    _vLine = false;
    _hLine = false;
  
    _mouseButtonPressed = false;
    _pixelClicked = QPoint(-1, -1);
    
    _selectionOn = false;

    initMenu();

    showImage();
}

StandardImageView::~StandardImageView()
{
	delete _menu;
	delete _pixmap_img;
}

void StandardImageView::initMenu()
{
	_menu = new ImageContextMenu(_view);
	_view->setContextMenuPolicy(Qt::CustomContextMenu);
	
	_menu->addAction("Histogram", _parent, SLOT(showHistogram()));
	_menu->addAction("Horizontal Projection Histogram", _parent, SLOT(showHProjectionHistogram()));
	_menu->addAction("Vertical Projection Histogram", _parent, SLOT(showVProjectionHistogram()));
	_menu->addAction("Pixels Grid", _parent, SLOT(showPixelsGrid()));
	_menu->addAction("Column Profile", _parent, SLOT(showColumnProfile()));
	_menu->addAction("Line Profile", _parent, SLOT(showLineProfile()));

	QObject::connect(_view, SIGNAL(customContextMenuRequested(const QPoint&)), _menu, SLOT(showContextMenu(const QPoint&)));
}

void StandardImageView::showImage()
{
    QImage im(_image->getWidth(), _image->getHeight(), QImage::Format_ARGB32);

    //on récupère les bits de l'image qt, qu'on cast en QRgb (qui fait 32 bits -> une image RGBA)
    QRgb* data = reinterpret_cast<QRgb*>(im.bits());
    Image::const_iterator it = _image->begin();
    
    for(unsigned int i = 0 ; i < _image->getHeight()*_image->getWidth() ; ++i)
    {
        //Pour chaque pixel de l'image Qt, on récupère les données correspondantes de l'image ImageIn grace à l'itérateur
        if(_image->getNbChannels() == 4)
        {
			unsigned char red = *(it++);
			unsigned char green = *(it++);
			unsigned char blue = *(it++);
			unsigned char alpha = *(it++);

			//On utilise la fonction qRgba pour en faire un pointeur de qRgb
			data[i] = qRgba(red, green, blue, alpha);
		}
		else if(_image->getNbChannels() == 3)
		{
			unsigned char red = *(it++);
			unsigned char green = *(it++);
			unsigned char blue = *(it++);

			//On utilise la fonction qRgba pour en faire un pointeur de qRgb
			data[i] = qRgb(red, green, blue);		
		}
		else if(_image->getNbChannels() == 2)
		{
			unsigned char gray = *(it++);
			unsigned char alpha = *(it++);

			//On utilise la fonction qRgba pour en faire un pointeur de qRgb
			data[i] = qRgba(gray, gray, gray, alpha);		
		}
		else if(_image->getNbChannels() == 1)
		{
			unsigned char gray = *(it++);

			//On utilise la fonction qRgba pour en faire un pointeur de qRgb
			data[i] = qRgb(gray, gray, gray);			
		}
    }

    //Qt ne peut pas afficher les QImage directement, on en fait un QPixmap...
    _pixmap_img = new QPixmap();
    _pixmap_img->convertFromImage(im);
    
    this->setPixmap(*_pixmap_img);
	_scene->addItem(this);
	
	_highlight = new QGraphicsRectItem(((int)_selection.x), ((int)_selection.y), ((int)_selection.w), ((int)_selection.h));
	_highlight->setPen(QPen(QBrush(QColor(255, 0, 0, 200)), 1));
	
	_scene->addItem(_highlight);
	
	_view->setScene(_scene);
}

void StandardImageView::zoom(int delta) {
    double wOrigin = _pixmap_img->width();
    double hOrigin = _pixmap_img->height();
    if(delta < 0 && _zoomFactor > 0.05) //Zoom out
    {
        _zoomFactor -= 0.05;
        
        double wActual = wOrigin * (_zoomFactor + 0.05);
        double hActual = hOrigin * (_zoomFactor + 0.05);
        
        double zoomW = (_zoomFactor * wOrigin) / wActual;
        double zoomH = (_zoomFactor * hOrigin) / hActual;
        
        _view->scale(zoomW, zoomH);
    }
    else if(delta > 0)//Zoom in
    {
        _zoomFactor += 0.05;
        
        double wActual = wOrigin * (_zoomFactor - 0.05);
        double hActual = hOrigin * (_zoomFactor - 0.05);
        
        double zoomW = (_zoomFactor * wOrigin) / wActual;
        double zoomH = (_zoomFactor * hOrigin) / hActual;
        
        _view->scale(zoomW, zoomH);
    }		
    
    emit zoomChanged(_zoomFactor*100);
}

void StandardImageView::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (_ctrlPressed && event->orientation() == Qt::Vertical)
	{
        zoom(event->delta());
	}
}

void StandardImageView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QPoint pos = event->pos().toPoint();
    if(pixmap().rect().contains(pos))
    {
        if(event->button() == Qt::LeftButton)
        {
            _mouseButtonPressed = true;
            _pixelClicked = pos;

            if(_move) {
                int posX = QCursor::pos().x();
                int posY = QCursor::pos().y();
                
                int delta;
                
                if( abs( delta = pos.x() - _highlight->rect().left() ) <= 2) {
                    posX -= delta;
                }
                else if( abs( delta = pos.x() - _highlight->rect().right() ) <= 2) {
                    posX -= delta;
                }

                if( abs( delta = pos.y() - _highlight->rect().top()) <= 2) {
                    posY -= delta;
                }
                else if( abs( delta = pos.y() - _highlight->rect().bottom()) <= 2) {
                    posY -= delta;
                }

                QCursor::setPos(posX, posY);
            }
            else {
                emit startDrag();
            }
            
            
        }
    }
}

void StandardImageView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    std::cout << "mouseReleaseEvent" << std::endl;
    QPoint pos = event->pos().toPoint();
    if(event->button() == Qt::LeftButton)
    {
        if(pixmap().rect().contains(pos))
        {
            if(pos == _pixelClicked) {
                emit pixelClicked(pos.x(), pos.y());
            }
        }
        _mouseButtonPressed = false;
        _pixelClicked = QPoint(-1, -1);
        _originalHighlight = _selection;
    }
}

void StandardImageView::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QPoint pos = event->pos().toPoint();
    if(pixmap().rect().contains(pos))
    {
        if(_mouseButtonPressed)
        {
            if(_move)
            {
                int dx = pos.x() - _pixelClicked.x();
                int dy = pos.y() - _pixelClicked.y();
                _pixelClicked = pos;
                _selection.x = min( max((int)_selection.x + dx, 0), (int)_image->getWidth() - (int)_selection.w - 1);
                _selection.y = min( max((int)_selection.y + dy, 0), (int)_image->getHeight() - (int)_selection.h - 1);
                if(_sourceHighlight != NULL) _sourceHighlight->update(_selection);
            }
            else if(_resize && this->hasCursor())
            {
                if(this->cursor().shape() == Qt::SizeHorCursor || this->cursor().shape() == Qt::SizeFDiagCursor || this->cursor().shape() == Qt::SizeBDiagCursor)
                {
                    if(_originX && pos.x() < (int)(_originalHighlight.x  + _originalHighlight.w))
                    {
                        _selection.w = _originalHighlight.x - pos.x() + _originalHighlight.w;
                        _selection.x = pos.x();
                    }
                    else if(_originX)
                    {
                        _originalHighlight.x = _originalHighlight.x + _originalHighlight.w;
                        _originalHighlight.w = pos.x() - _originalHighlight.x;
                        _originX = false;
                    }

                    if(!_originX && (int)_originalHighlight.x >= pos.x())
                    {
                        _selection.w = _originalHighlight.x - pos.x();
                        _selection.x = pos.x();
                    }
                    else if(!_originX)
                    {
                        _selection.w = pos.x() - _originalHighlight.x;
                        _selection.x = _originalHighlight.x;
                    }
                }

                if(this->cursor().shape() == Qt::SizeVerCursor || this->cursor().shape() == Qt::SizeFDiagCursor || this->cursor().shape() == Qt::SizeBDiagCursor)
                {
                    if(_originY && pos.y() < (int)(_originalHighlight.y  + _originalHighlight.h))
                    {
                        _selection.h = _originalHighlight.y - pos.y() + _originalHighlight.h;
                        _selection.y = pos.y();
                    }
                    else if(_originY)
                    {
                        _originalHighlight.y = _originalHighlight.y + _originalHighlight.h;
                        _originalHighlight.h = pos.y() - _originalHighlight.y;
                        _originY = false;
                    }

                    if(!_originY && (int)_originalHighlight.y >= pos.y())
                    {
                        _selection.h = _originalHighlight.y - pos.y();
                        _selection.y = pos.y();
                    }
                    else if(!_originY)
                    {
                        _selection.h = pos.y() - _originalHighlight.y;
                        _selection.y = _originalHighlight.y;
                    }
                }
                if(_sourceHighlight != NULL) _sourceHighlight->update(_selection);
            }
            else
            {
                if(_mode == MODE_SELECT) {
                    _sourceHighlight = NULL;
                    if(_pixelClicked.x() < pos.x())
                    {
                        _selection.w = pos.x() - _pixelClicked.x();
                        _selection.x = _pixelClicked.x();
                    }
                    else
                    {
                        _selection.x = pos.x();
                        _selection.w = _pixelClicked.x() - pos.x();
                    }

                    if(_pixelClicked.y() < pos.y())
                    {
                        _selection.y = _pixelClicked.y();
                        _selection.h = pos.y() - _pixelClicked.y();
                    }
                    else
                    {
                        _selection.y = pos.y();
                        _selection.h = _pixelClicked.y() - pos.y();
                    }
                }
            }
            _highlight->setRect(_selection.x, _selection.y, _selection.w, _selection.h);
        }
    }
}

void StandardImageView::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    QPoint pos = event->pos().toPoint();
    
    if(pixmap().rect().contains(pos)) {
        emit pixelHovered(pos.x(), pos.y());
    }
    
    if(!_mouseButtonPressed) {
        this->setCursor(mouseOverHighlight(pos.x(), pos.y()));
    }
}

Qt::CursorShape StandardImageView::mouseOverHighlight(int x, int y)
{
    const QRectF& rect = _highlight->rect();
    bool nearLeft = ( abs(x - rect.left()) <= 2 );
    bool nearRight = ( abs(x - rect.right()) <= 2 );
    bool nearTop = ( abs(y - rect.top()) <= 2 );
    bool nearBottom = ( abs(y - rect.bottom()) <= 2 );
    bool inX = ( x >= rect.left()-2 ) && ( x <= rect.right()+2 );
    bool inY = ( y >= rect.top()-2 ) && ( y <= rect.bottom()+2 );
  
    Qt::CursorShape res;
    if(_ctrlPressed && (nearLeft || nearRight || nearTop || nearBottom) ) res = Qt::SizeAllCursor;
    else if( (nearLeft && nearTop) || (nearRight && nearBottom) ) res = Qt::SizeFDiagCursor;
    else if( (nearLeft && nearBottom) || (nearRight && nearTop) ) res = Qt::SizeBDiagCursor;
    else if(inY && (nearLeft || nearRight)) res = Qt::SizeHorCursor;
    else if(inX && (nearTop || nearBottom)) res = Qt::SizeVerCursor;
    else res = Qt::ArrowCursor;
  
    _move = (res == Qt::SizeAllCursor);
    _resize = (res != Qt::ArrowCursor);
    
    _originX = nearLeft;
    _originY = nearTop;
    
    return res;
}

void StandardImageView::ctrlPressed()
{
	_ctrlPressed = !_ctrlPressed;
}

void StandardImageView::showHighlightRect(imagein::Rectangle rect, ImageWindow* source)
{
	_highlight->setRect(((int)rect.x), ((int)rect.y), ((int)rect.w), ((int)rect.h));
  _originalHighlight = rect;
  
  _selection.x = (int)rect.x;
  _selection.y = (int)rect.y;
  _selection.w = (int)rect.w;
  _selection.h = (int)rect.h;
  
  _vLine = (_originalHighlight.w == 0 && _originalHighlight.h == _image->getHeight());
  _hLine = (_originalHighlight.h == 0 && _originalHighlight.w == _image->getWidth());
  
  AlternativeImageView* view = source->getView();
  if(view != NULL && (_sourceHighlight = dynamic_cast<GenericHistogramView*>(view))) {}
  else _sourceHighlight = NULL;
}
/*
void StandardImageView::selectAll()
{
  _sourceHighlight = NULL;
  _selection.x = 0;
  _selection.y = 0;
  _selection.w = _image->getWidth();
  _selection.h = _image->getHeight();
  
  _highlight->setRect(_selection.x, _selection.y, _selection.w, _selection.h);  
}*/

void StandardImageView::toggleSelection() {
    _selectionOn = !_selectionOn;
    std::cout << "Selection " <<  (_selectionOn ? "on" : "false") << std::endl;
}


void StandardImageView::setImage(imagein::Image* image)
{
  _selection = Rectangle(0, 0, image->getWidth(), image->getHeight());
  _visibleArea = Rectangle(0, 0, image->getWidth(), image->getHeight());
    
	_zoomFactor = 1;
  
  _sourceHighlight = NULL;
  _originalHighlight = Rectangle(0, 0, _image->getWidth(), _image->getHeight());
  _resize = false;
  _originX = false;
  _originY = false;
  _vLine = false;
  _hLine = false;
  
  _pixelClicked = QPoint(-1, -1);
  
  QImage im(getQImage(image));

  //Qt ne peut pas afficher les QImage directement, on en fait un QPixmap...
  _pixmap_img = new QPixmap();
  _pixmap_img->convertFromImage(im);
    
  this->setPixmap(*_pixmap_img);
	//_scene->addItem(this);
	
	_highlight = new QGraphicsRectItem(((int)_selection.x), ((int)_selection.y), ((int)_selection.w), ((int)_selection.h));
	
	//_scene->addItem(_highlight);
	//_view->setScene(_scene);
  
  _image = image;
}


QImage genericinterface::getQImage(const imagein::Image* image) 
{
  QImage im(image->getWidth(), image->getHeight(), QImage::Format_ARGB32);

  //on récupère les bits de l'image qt, qu'on cast en QRgb (qui fait 32 bits -> une image RGBA)
  QRgb* data = reinterpret_cast<QRgb*>(im.bits());
  Image::const_iterator it = image->begin();
  
  for(unsigned int i = 0 ; i < image->getHeight()*image->getWidth() ; ++i)
  {
    //Pour chaque pixel de l'image Qt, on récupère les données correspondantes de l'image ImageIn grace à l'itérateur
    if(image->getNbChannels() == 4)
    {
      unsigned char red = *(it++);
      unsigned char green = *(it++);
      unsigned char blue = *(it++);
      unsigned char alpha = *(it++);

      //On utilise la fonction qRgba pour en faire un pointeur de qRgb
      data[i] = qRgba(red, green, blue, alpha);
    }
    else if(image->getNbChannels() == 3)
    {
      unsigned char red = *(it++);
      unsigned char green = *(it++);
      unsigned char blue = *(it++);

      //On utilise la fonction qRgba pour en faire un pointeur de qRgb
      data[i] = qRgb(red, green, blue);		
    }
    else if(image->getNbChannels() == 2)
    {
      unsigned char gray = *(it++);
      unsigned char alpha = *(it++);

      //On utilise la fonction qRgba pour en faire un pointeur de qRgb
      data[i] = qRgba(gray, gray, gray, alpha);		
    }
    else if(image->getNbChannels() == 1)
    {
      unsigned char gray = *(it++);

      //On utilise la fonction qRgba pour en faire un pointeur de qRgb
      data[i] = qRgb(gray, gray, gray);			
    }
  }
  return im;
}
