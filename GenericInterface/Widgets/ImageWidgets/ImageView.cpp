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
#include <QSpinBox>

#include "ImageView.h"

using namespace genericinterface;
using namespace imagein;
using namespace std;

ImageView::ImageView(QWidget* parent, const Image* image)
    : QScrollArea(parent), _parent(parent)
{
    _mode = MODE_MOUSE;
    _selectMode = SELECTMODE_NONE;

    this->setMouseTracking(true);

    _select.setRect(0, 0, 0, 0);
    _oldSelect = _select;
    _selectSrc = NULL;

    _originX = false;
    _originY = false;
    _vLine = false;
    _hLine = false;
  
    _downPos = QPoint(-1, -1);
    
    _imgWidget = new ImageWidget(this, image);
    _imgWidget->setFixedSize(pixmap().size());
    _imgWidget->setMouseTracking(true);
    this->setWidget(_imgWidget);
    this->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    
    _rubberBand = new QRubberBand(QRubberBand::Rectangle, _imgWidget);
    redrawSelect();
    _rubberBand->hide();

    if(image != NULL) {
        this->setImage(image);
    }
}

ImageView::~ImageView()
{
}


void ImageView::mousePressEvent(QMouseEvent * event)
{
    QPoint pos = mapToPixmap(event->pos());
    if(event->button() == Qt::LeftButton)
    {
        _downPos = pos;
        _oldSelect = _select;
       
        if(_mode == MODE_SELECT) {
            if(_selectMode == SELECTMODE_NONE) {
            
                const bool ctrlDown = event->modifiers().testFlag(Qt::ControlModifier);
                SelectMode newMode = ctrlDown ? SELECTMODE_MOVE : SELECTMODE_RESIZE;
                
                int posX = QCursor::pos().x();
                int posY = QCursor::pos().y();
                QRect rect = _rubberBand->geometry();
                QPoint p = event->pos() - _imgWidget->geometry().topLeft();
                
                int delta;
                
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
}

void ImageView::mouseDoubleClickEvent(QMouseEvent * event) {
    if(event->button() == Qt::LeftButton)
    {
        if(_imgWidget->geometry().contains(event->pos()))
        {
            emit startDrag();
        }
    }
}

void ImageView::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        _selectMode = SELECTMODE_NONE;
        _oldSelect = _select;
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

void ImageView::selectionMove(QPoint pos) {
    int x = max(0, _oldSelect.x() + pos.x() - _downPos.x());
    int y = max(0, _oldSelect.y() + pos.y() - _downPos.y());
    x = min(x, _imgWidget->pixmap().width() - _oldSelect.width());
    y = min(y, _imgWidget->pixmap().height() - _oldSelect.height());
    _select.moveTo(x, y);
    redrawSelect();
//    if(_selectSrc != NULL) _selectSrc->update(this->getImage(), imagein::Rectangle(_select.x(), _select.y(), _select.width(), _select.height()));
    if(_selectSrc != NULL) {
        emit updateSrc(_selectSrc, imagein::Rectangle(_select.x(), _select.y(), _select.width(), _select.height()));
    }
}

void ImageView::selectionResize(QPoint pos) {
    
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
    
    _select.setLeft(std::max(0, _select.left()));
    _select.setRight(std::min(pixmap().width()-1, _select.right()));
    _select.setTop(std::max(0, _select.top()));
    _select.setBottom(std::min(pixmap().height()-1, _select.bottom()));
    redrawSelect();
    if(_selectSrc != NULL) {
        emit updateSrc(_selectSrc, imagein::Rectangle(_select.x(), _select.y(), _select.width(), _select.height()));
    }
}

void ImageView::selectionMake(QPoint pos) {
    _selectSrc = NULL;
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


void ImageView::mouseMoveEvent(QMouseEvent* event)
{
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
                this->setCursor(mouseOverHighlight(event));
        }
    }
    else if(_mode == MODE_MOUSE) {
        if(event->buttons().testFlag(Qt::LeftButton)) {
            QScrollBar* hsb = this->horizontalScrollBar();
            QScrollBar* vsb = this->verticalScrollBar();
            int offsetX = (pos.x()-_downPos.x())*_imgWidget->width()/_imgWidget->pixmap().width();
            int offsetY = (pos.y()-_downPos.y())*_imgWidget->width()/_imgWidget->pixmap().width();
            hsb->setValue(hsb->value() - offsetX);
            vsb->setValue(vsb->value() - offsetY);
        }
    }
}

void ImageView::wheelEvent(QWheelEvent* event) {
    event->ignore();
}   

Qt::CursorShape ImageView::mouseOverHighlight(QMouseEvent* event)
{
    const QPoint pos = event->pos();
    const bool ctrlDown = event->modifiers().testFlag(Qt::ControlModifier);
    const int x = pos.x() - _imgWidget->geometry().x();
    const int y = pos.y() - _imgWidget->geometry().y();
    const QRect rect = _rubberBand->geometry();
    const bool inX = ( x >= rect.left()-2 ) && ( x <= rect.right()+2 );
    const bool inY = ( y >= rect.top()-2 ) && ( y <= rect.bottom()+2 );
    const bool nearLeft = inY && ( abs(x - rect.left()) <= 2 );
    const bool nearRight = inY && ( abs(x - rect.right()) <= 2 );
    const bool nearTop = inX && ( abs(y - rect.top()) <= 2 );
    const bool nearBottom = inX && ( abs(y - rect.bottom()) <= 2 );
  
    Qt::CursorShape res;
    if(ctrlDown && (nearLeft || nearRight || nearTop || nearBottom) ) res = Qt::SizeAllCursor;
    else if( (nearLeft && nearTop) || (nearRight && nearBottom) ) res = Qt::SizeFDiagCursor;
    else if( (nearLeft && nearBottom) || (nearRight && nearTop) ) res = Qt::SizeBDiagCursor;
    else if(nearLeft || nearRight) res = Qt::SizeHorCursor;
    else if(nearTop || nearBottom) res = Qt::SizeVerCursor;
    else res = Qt::CrossCursor;
    
    _originX = nearLeft;
    _originY = nearTop;
    
    return res;
}

void ImageView::scale(double scaleW, double scaleH) {
    if(_selectMode == SELECTMODE_NONE) {

        QPoint mousePos = mapToWidget(mapFromGlobal(QCursor::pos()));
        QPoint pixelPos = _imgWidget->mapToPixmap(mousePos);

        _imgWidget->setFixedSize(pixmap().width()*scaleW, pixmap().height()*scaleH);

        QPoint offset = _imgWidget->mapFromPixmap(pixelPos) - mousePos;
        QScrollBar* hsb = this->horizontalScrollBar();
        QScrollBar* vsb = this->verticalScrollBar();
        hsb->setValue(hsb->value() + offset.x());
        vsb->setValue(vsb->value() + offset.y());

        redrawSelect();
    }
}

void ImageView::showSelectRect(imagein::Rectangle rect, GenericHistogramView* source)
{
    _select.setRect(((int)rect.x), ((int)rect.y), ((int)rect.w), ((int)rect.h));
    redrawSelect();
    _oldSelect = _select;
  
    _vLine = (_oldSelect.width() == 0 && _oldSelect.height() == pixmap().height());
    _hLine = (_oldSelect.height() == 0 && _oldSelect.width() == pixmap().width());
    
    _selectSrc = source;
}

void ImageView::selectAll()
{
    _selectSrc = NULL;
    _select = QRect(0, 0, pixmap().width(), pixmap().height());
    redrawSelect();
}

void ImageView::switchMode(Mode mode) {
    _mode = mode; 
    if(_mode == MODE_MOUSE) {
        _rubberBand->hide();
    }
    else if(_mode == MODE_SELECT) {
        _rubberBand->show();
    }
}

void ImageView::setImage(const imagein::Image* image)
{
    _select.setRect(0, 0, image->getWidth(), image->getHeight());
    _oldSelect = _select;

    _selectSrc = NULL;
    _originX = false;
    _originY = false;
    _vLine = false;
    _hLine = false;

    _downPos = QPoint(-1, -1);
    _imgWidget->setImage(image);
    _imgWidget->setFixedSize(pixmap().size());
    this->updateGeometry();
    redrawSelect();
}


