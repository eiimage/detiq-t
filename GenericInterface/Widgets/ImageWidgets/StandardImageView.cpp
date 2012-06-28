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

#include <QScrollBar>

#include "StandardImageView.h"

using namespace genericinterface;
using namespace imagein;
using namespace std;

StandardImageView::StandardImageView(QWidget* parent, Image* image): QScrollArea(parent), _parent(parent), _image(image)
{
    _mode = MODE_MOUSE;
    _selectMode = SELECTMODE_NONE;
    _select = QRect(0,0,image->getWidth(), image->getHeight());
    _oldSelect = _select;
    _selectSrc = NULL;
    //_visibleArea = Rectangle(0, 0, _image->getWidth(), _image->getHeight());
	//_scene = new QGraphicsScene();
    //_view = new QGraphicsView();

	//this->setAcceptHoverEvents(true);
    this->setMouseTracking(true);
    
    _ctrlDown = false;
  
    _originX = false;
    _originY = false;
    _vLine = false;
    _hLine = false;
  
    _downPos = QPoint(-1, -1);
    
    _imgWidget = new ImgWidget(this, image);
    _imgWidget->setFixedSize(QSize(image->getWidth(), image->getHeight()));
    _imgWidget->setMouseTracking(true);
    this->setWidget(_imgWidget);
    this->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    
    _rubberBand = new QRubberBand(QRubberBand::Rectangle, _imgWidget);
    //redrawSelect();
    _rubberBand->hide();
/*
    initMenu();

    showImage();
*/
}

StandardImageView::~StandardImageView()
{
	//delete _menu;
    //delete _imgWidget;
}

void StandardImageView::initMenu()
{
}

void StandardImageView::showImage()
{
/*
    //Qt ne peut pas afficher les QImage directement, on en fait un QPixmap...
    this->setPixmap(QPixmap::fromImage(convertImage(this->getImage())));
    
    //this->setPixmap(_pixmap_img);
	//_scene->addItem(this);
	
	_highlight = new QGraphicsRectItem(((int)_selection.x), ((int)_selection.y), ((int)_selection.w), ((int)_selection.h));
	_highlight->setPen(QPen(QBrush(QColor(255, 0, 0, 32)), 1));
	
	//_scene->addItem(_highlight);
	
	//_view->setScene(_scene);
*/
}

/*void StandardImageView::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (_ctrlPressed && event->orientation() == Qt::Vertical)
	{
        zoom(event->delta());
	}
}*/

StandardImageView::ImgWidget::ImgWidget(QWidget* parent, const imagein::Image* img) : QWidget(parent) {
    _pixmap.convertFromImage(convertImage(img));
}

void StandardImageView::ImgWidget::setImage(const imagein::Image* img) {
    _pixmap.convertFromImage(convertImage(img));
}

void StandardImageView::ImgWidget::paintEvent (QPaintEvent* event ) {
    QPainter painter(this);
    //QSize size = _pixmap.size();
    //size.scale(this->rect().size(), Qt::KeepAspectRatio);
    //painter.drawPixmap(QRect(this->rect().topLeft(), size), _pixmap);
    painter.drawPixmap(this->rect(), _pixmap);
}

void StandardImageView::mousePressEvent(QMouseEvent * event)
{
    //QPoint pos = (event->pos() - _imgWidget->geometry().topLeft())*_imgWidget->scale();
    QPoint pos = mapToPixmap(event->pos());
    //std::cout << "pos = (" << pos.x() << "," << pos.y() << ")" << std::endl;
    if(event->button() == Qt::LeftButton)
    {
        _downPos = pos;
        _oldSelect = _select;
       
        if(_mode == MODE_SELECT) {
            if(_selectMode == SELECTMODE_NONE) {
            
                SelectMode newMode = _ctrlDown ? SELECTMODE_MOVE : SELECTMODE_RESIZE;
                
                int posX = QCursor::pos().x();
                int posY = QCursor::pos().y();
                QRect rect = _rubberBand->geometry();
                //QPoint p = event->globalPos() - mapToGlobal(_imgWidget->geometry().topLeft());
                QPoint p = event->pos() - _imgWidget->geometry().topLeft();
                
                int delta;
                //std::cout << "_move" << std::endl;
                
                if( abs( delta = p.x() - rect.left() ) <= 2) {
                    posX -= delta;
                    _selectMode = newMode;
                }
                else if( abs( delta = p.x() - (rect.right()) ) <= 2) {
                    posX -= delta;
                    _selectMode = newMode;
                }

                if( abs( delta = p.y() - rect.top()) <= 2) {
                    posY -= delta;
                    _selectMode = newMode;
                }
                else if( abs( delta = p.y() - (rect.bottom())) <= 2) {
                    posY -= delta;
                    _selectMode = newMode;
                }

                QCursor::setPos(posX, posY);
            }
            
            if(_mode == MODE_SELECT && _selectMode == SELECTMODE_NONE) {
                _selectMode = SELECTMODE_MAKE;
            }
        }
        else if(_mode == MODE_MOUSE) {
            this->setCursor(Qt::ClosedHandCursor);
        }
        
        
    }
    //std::cout << "selectMode = " << (_selectMode==SELECTMODE_MAKE ? "SELECTMODE_MAKE" : (_selectMode==SELECTMODE_MOVE ? "SELECTMODE_MOVE" : (_selectMode==SELECTMODE_RESIZE ? "SELECTMODE_RESIZE" : "SELECTMODE_NONE"))) << std::endl;
}

void StandardImageView::mouseDoubleClickEvent(QMouseEvent * event) {
    if(event->button() == Qt::LeftButton)
    {
        if(_imgWidget->geometry().contains(event->pos()))
        {
            emit startDrag();
        }
    }
}

void StandardImageView::mouseReleaseEvent(QMouseEvent * event)
{
    //std::cout << "mouseReleaseEvent" << std::endl;
    if(event->button() == Qt::LeftButton)
    {
        _selectMode = SELECTMODE_NONE;
        _oldSelect = _select;
        //QPoint pos = (event->pos() - _imgWidget->geometry().topLeft())*_imgWidget->scale();
        QPoint pos = mapToPixmap(event->pos());
        if(_imgWidget->pixmap().rect().contains(pos) && pos == _downPos)
        {
            emit pixelClicked(pos.x(), pos.y());
        }
        _downPos = QPoint(-1, -1);
        if(_mode == MODE_MOUSE) {
            this->setCursor(Qt::OpenHandCursor);
        }
    }
}

void StandardImageView::selectionMove(QPoint pos) {
    int x = max(0, _oldSelect.x() + pos.x() - _downPos.x());
    int y = max(0, _oldSelect.y() + pos.y() - _downPos.y());
    x = min(x, _imgWidget->pixmap().width() - _oldSelect.width());
    y = min(y, _imgWidget->pixmap().height() - _oldSelect.height());
    _select.moveTo(x, y);
    redrawSelect();
    if(_selectSrc != NULL) _selectSrc->update(this->getImage(), imagein::Rectangle(_select.x(), _select.y(), _select.width(), _select.height()));
}

void StandardImageView::selectionResize(QPoint pos) {
    
    if(this->cursor().shape() == Qt::SizeHorCursor || this->cursor().shape() == Qt::SizeFDiagCursor || this->cursor().shape() == Qt::SizeBDiagCursor)
    {
        if(_originX) {
            _select.setLeft(pos.x());
        }
        else {
            _select.setRight(pos.x());
        }
    }
    
    if(this->cursor().shape() == Qt::SizeVerCursor || this->cursor().shape() == Qt::SizeFDiagCursor || this->cursor().shape() == Qt::SizeBDiagCursor)
    {
        if(_originY) {
            _select.setTop(pos.y());
        }
        else {
            _select.setBottom(pos.y());
        }
    }
    
    if(_select.width() < 0) {
        _originX = !_originX;
        int x = _select.x();
        _select.setX(_select.right()+1);
        _select.setRight(x-1);
    }
    
    if(_select.height() < 0) {
        _originY = !_originY;
        int y = _select.y();
        _select.setY(_select.bottom()+1);
        _select.setBottom(y-1);
    }
    
    
    //_select = _select.normalized();
    //_select &= _imgWidget->rect();
    _select.setLeft(std::max(0, _select.left()));
    _select.setRight(std::min((int)this->getImage()->getWidth()-1, _select.right()));
    _select.setTop(std::max(0, _select.top()));
    _select.setBottom(std::min((int)this->getImage()->getHeight()-1, _select.bottom()));
    redrawSelect();
    if(_selectSrc != NULL) _selectSrc->update(this->getImage(), imagein::Rectangle(_select.x(), _select.y(), _select.width(), _select.height()));
}

void StandardImageView::selectionMake(QPoint pos) {
    _selectSrc = NULL;
    /*if(_pixelClicked.x() < pos.x())
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
    }*/
    _select = QRect(_downPos, pos);
    if(_select.width() < 0) {
        int x = _select.x();
        _select.setX(_select.right()+1);
        _select.setRight(x-1);
    }
    
    if(_select.height() < 0) {
        int y = _select.y();
        _select.setY(_select.bottom()+1);
        _select.setBottom(y-1);
    }
    _select = _select.intersected(_imgWidget->pixmap().rect());
    redrawSelect();
}


void StandardImageView::mouseMoveEvent(QMouseEvent* event)
{
    //QPoint pos = (event->pos() - _imgWidget->geometry().topLeft())*_imgWidget->scale();
    QPoint pos = mapToPixmap(event->pos());
    
    if(_imgWidget->rect().contains(pos))
    {
        emit pixelHovered(pos.x(), pos.y());
    }
   
    if(_mode == MODE_SELECT) {

        switch(_selectMode) {
            case SELECTMODE_MOVE:
            {
                selectionMove(pos);
                break;
            }
            case SELECTMODE_RESIZE:
            {
                selectionResize(pos);
                break;
            }
            case SELECTMODE_MAKE:
            {
                selectionMake(pos);
                break;
            }
            default:
                this->setCursor(mouseOverHighlight(event->pos()));
        }
    }
    else if(_mode == MODE_MOUSE) {
        if(event->buttons().testFlag(Qt::LeftButton)) {
            QScrollBar* hsb = this->horizontalScrollBar();
            QScrollBar* vsb = this->verticalScrollBar();
            int offsetX = (pos.x()-_downPos.x())*_imgWidget->width()/_imgWidget->pixmap().width();
            int offsetY = (pos.y()-_downPos.y())*_imgWidget->width()/_imgWidget->pixmap().width();
            //std::cout << sb->minimum() << ":" << sb->value() << ":" << sb->maximum() << std::endl;
            hsb->setValue(hsb->value() - offsetX);
            vsb->setValue(vsb->value() - offsetY);
        }
    }
        /*
        if(_mouseButtonPressed)
        {
            if(_move)
            {
                this->selectionMove(pos);
            }
            else if(_resize && this->cursor().shape() != Qt::ArrowCursor )
            {
                this->selectionResize(pos);
            }
            else
            {
                if(_mode == MODE_SELECT) {
                    this->selectionMake(pos);
                }
                else if( (_downPos - pos).manhattanLength() > 8){
                    emit startDrag();
                    _mouseButtonPressed = false;
                }
            }
            //_highlight->setRect(_selection.x, _selection.y, _selection.w, _selection.h);
            _rubberBand->setGeometry(_select);
        }*/
    //}
    
}

void StandardImageView::wheelEvent(QWheelEvent* event) {
    event->ignore();
}   
/*
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
*/
Qt::CursorShape StandardImageView::mouseOverHighlight(QPoint pos)
{
    int x = pos.x() - _imgWidget->geometry().x();
    int y = pos.y() - _imgWidget->geometry().y();
    QRect rect = _rubberBand->geometry();
    bool inX = ( x >= rect.left()-2 ) && ( x <= rect.right()+2 );
    bool inY = ( y >= rect.top()-2 ) && ( y <= rect.bottom()+2 );
    bool nearLeft = inY && ( abs(x - rect.left()) <= 2 );
    bool nearRight = inY && ( abs(x - rect.right()) <= 2 );
    bool nearTop = inX && ( abs(y - rect.top()) <= 2 );
    bool nearBottom = inX && ( abs(y - rect.bottom()) <= 2 );
  
    Qt::CursorShape res;
    if(_ctrlDown && (nearLeft || nearRight || nearTop || nearBottom) ) res = Qt::SizeAllCursor;
    else if( (nearLeft && nearTop) || (nearRight && nearBottom) ) res = Qt::SizeFDiagCursor;
    else if( (nearLeft && nearBottom) || (nearRight && nearTop) ) res = Qt::SizeBDiagCursor;
    else if(nearLeft || nearRight) res = Qt::SizeHorCursor;
    else if(nearTop || nearBottom) res = Qt::SizeVerCursor;
    else res = Qt::CrossCursor;
    
    _originX = nearLeft;
    _originY = nearTop;
    
    return res;
}

void StandardImageView::scale(double scaleW, double scaleH) {
    if(_selectMode == SELECTMODE_NONE) {

        QPoint mousePos = mapToWidget(mapFromGlobal(QCursor::pos()));
        QPoint pixelPos = _imgWidget->mapToPixmap(mousePos);

        _imgWidget->setFixedSize(_image->getWidth()*scaleW, _image->getHeight()*scaleH);

        QPoint offset = _imgWidget->mapFromPixmap(pixelPos) - mousePos;
        QScrollBar* hsb = this->horizontalScrollBar();
        QScrollBar* vsb = this->verticalScrollBar();
        //std::cout << sb->minimum() << ":" << sb->value() << ":" << sb->maximum() << std::endl;
        hsb->setValue(hsb->value() + offset.x());
        vsb->setValue(vsb->value() + offset.y());
        //std::cout << offset.x() << ":" << offset.y() << std::endl;
        
        redrawSelect();
    }
}

void StandardImageView::ctrlPressed(bool isDown)
{
	_ctrlDown = isDown;
}

void StandardImageView::showSelectRect(imagein::Rectangle rect, GenericHistogramView* source)
{
    _select.setRect(((int)rect.x), ((int)rect.y), ((int)rect.w), ((int)rect.h));
    redrawSelect();
    _oldSelect = _select;
  
    _vLine = (_oldSelect.width() == 0 && _oldSelect.height() == (int)_image->getHeight());
    _hLine = (_oldSelect.height() == 0 && _oldSelect.width() == (int)_image->getWidth());
    
    _selectSrc = source;
}

void StandardImageView::selectAll()
{
    _selectSrc = NULL;
    _select = QRect(0, 0, (int)_image->getWidth(), (int)_image->getHeight());
    redrawSelect();
}

void StandardImageView::switchMode(Mode mode) { 
    _mode = mode; 
    if(_mode == MODE_MOUSE) {
        //_selectSrc = NULL;
        //_select = QRect(0, 0, _image->getWidth(), _image->getHeight());
        _rubberBand->hide();
    }
    else if(_mode == MODE_SELECT) {
        _rubberBand->show();
        //redrawSelect();
    }
}



void StandardImageView::setImage(imagein::Image* image)
{
    _select.setRect(0, 0, image->getWidth(), image->getHeight());
    //_visibleArea = Rectangle(0, 0, image->getWidth(), image->getHeight());

    _selectSrc = NULL;
    _oldSelect = _select;
    _originX = false;
    _originY = false;
    _vLine = false;
    _hLine = false;

    _downPos = QPoint(-1, -1);

    //QImage im(getQImage(image));

    //Qt ne peut pas afficher les QImage directement, on en fait un QPixmap...
    //_pixmap_img = new QPixmap();
    //this->setPixmap(QPixmap::fromImage(convertImage(this->getImage())));
    _image = image;
    //delete _imgWidget;
    //_imgWidget = new ImgWidget(this, image);
    _imgWidget->setImage(image);
    _imgWidget->setFixedSize(QSize(image->getWidth(), image->getHeight()));
    this->updateGeometry();
    if(_mode == MODE_SELECT) {
        redrawSelect();
    }
    //_pixmap.convertFromImage(convertImage(image));

    //this->setPixmap(_pixmap_img);
    //_scene->addItem(this);

    //_highlight->setRect(((int)_selection.x), ((int)_selection.y), ((int)_selection.w), ((int)_selection.h));
    //redrawSelect();
    //_rubberBand->hide();

    //_scene->addItem(_highlight);
    //_view->setScene(_scene);

}


QImage genericinterface::convertImage(const imagein::Image* img) 
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
