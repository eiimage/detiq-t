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

#include "StandardImageWindow.h"
#include "UnknownFormatException.h"

#include "Algorithm/RgbToGrayscale.h"
#include "Algorithm/Otsu.h"

#include <QPushButton>
#include <QMessageBox>

using namespace genericinterface;
using namespace imagein;
using namespace std;
using namespace imagein::algorithm;

StandardImageWindow::StandardImageWindow(const QString path, GenericInterface* gi)
    : ImageWindow(path), _gi(gi), _ctrlPressed(false)
{
    bool error = false;
    QString msg = "";
    try {
        _image = new Image(path.toStdString());
    }
    catch(const imagein::UnknownFormatException& e) {
        error = true;
        msg = tr("Unknown file format !");
    }
    catch(const imagein::ImageFileException& e) {
        error = true;
        msg = QString::fromStdString(e.getMsg());
    }
    catch(...) {
        error = true;
        msg = tr("Unknown exception");
    }
    
    if(error) {
        QMessageBox::critical(this, tr("Error while opening file"), msg);
        _image = new Image();
    }
    
    this->setWindowTitle(ImageWindow::getTitleFromPath(path));
    
    init();
}

StandardImageWindow::StandardImageWindow(const QString path, GenericInterface* gi, Image* image)
    : ImageWindow(path), _gi(gi), _ctrlPressed(false)
{
    _image = image;

    this->setWindowTitle(ImageWindow::getTitleFromPath(path));

    init();
}

StandardImageWindow::StandardImageWindow(const StandardImageWindow& siw, bool crop) 
    : ImageWindow(siw.getPath()), _gi(siw._gi) , _ctrlPressed(false)
{
    if(crop) {
        _image = siw._image->crop(siw._imageView->getRectangle());
    }
    else {
        _image = new Image(*siw._image);
    }
    this->setWindowTitle(siw.windowTitle());

    init();
}

StandardImageWindow::~StandardImageWindow()
{
	//delete _imageView;
    delete _selectedPixel;
    delete _image;
    delete _menu;
    //delete _view;
    //delete _scene;
}


void StandardImageWindow::mousePressEvent(QMouseEvent *event) {
    //if (event->button() == Qt::LeftButton) {
        //_imageView->mousePressEvent(event);   
    //}
    //ImageWindow::mousePressEvent(event);
    event->ignore();
}

void StandardImageWindow::mouseReleaseEvent(QMouseEvent *event) {
    //if (event->button() == Qt::LeftButton) {
        //_imageView->mouseReleaseEvent(event);   
    //}
    //ImageWindow::mouseReleaseEvent(event);
    event->ignore();
}

void StandardImageWindow::startDrag() {
    QDrag *drag = new QDrag(_gi);
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    //Node *node = new Node(this->getImage(), this->getPath());
    uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
    stream << ptr;
    if(_imageView->mode() == StandardImageView::MODE_MOUSE) {
        mimeData->setData("application/detiqt.genericinterface.stdimgwnd", encodedData);
        drag->setPixmap(_imageView->pixmap().scaled(QSize(76,76), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
    else {
        mimeData->setData("application/detiqt.genericinterface.stdimgwnd.copy", encodedData);
        drag->setPixmap(_imageView->pixmap().copy(_imageView->select()).scaled(QSize(76,76), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
    
    //QImage img = getQImage(this->getImage()).scaled(QSize(76,76), Qt::KeepAspectRatio, Qt::FastTransformation);
     
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));

    drag->exec();
    
}

void StandardImageWindow::mouseMoveEvent(QMouseEvent *event) {
    //std::cout << "mouseMoveEvent " << event->x() << ":" << event->y() << std::endl;
     //ImageWindow::mouseMoveEvent(event);
     event->ignore();
}

void StandardImageWindow::init()
{
    _zoomFactor = 1;
	//_scene = new QGraphicsScene(this);
    _imageView = new StandardImageView(this, _image);
	//_scene->addItem(_imageView);
	//_scene->addItem(_imageView->getHighlightItem());
    
    //_view = new QGraphicsView(_scene);
    
    
	_menu = new ImageContextMenu(_imageView);
	_imageView->setContextMenuPolicy(Qt::CustomContextMenu);
	_menu->addAction(tr("Histogram"), this, SLOT(showHistogram()));
	_menu->addAction(tr("Horizontal Projection Histogram"), this, SLOT(showHProjectionHistogram()));
	_menu->addAction(tr("Vertical Projection Histogram"), this, SLOT(showVProjectionHistogram()));
	_menu->addAction(tr("Pixels Grid"), this, SLOT(showPixelsGrid()));
	_menu->addAction(tr("Column Profile"), this, SLOT(showColumnProfile()));
	_menu->addAction(tr("Line Profile"), this, SLOT(showLineProfile()));
    _menu->addSeparator();
	_menu->addAction(tr("Crop"), this, SLOT(crop()));
	_menu->addAction(tr("Copy & crop"), this, SLOT(copycrop()));
    _menu->addSeparator();
	_menu->addAction(tr("Convert to grayscale"), this, SLOT(convertToGrayscale()));
	_menu->addAction(tr("Convert to binary"), this, SLOT(convertToBinary()));

	QObject::connect(_imageView, SIGNAL(customContextMenuRequested(const QPoint&)), _menu, SLOT(showContextMenu(const QPoint&)));
    
    _selectedPixel = new QPoint();

    initStatusBar();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(_imageView);
	layout->addWidget(_statusBar);
    this->setLayout(layout);

	QObject::connect(_imageView, SIGNAL(pixelClicked(int, int)), this, SLOT(showSelectedPixelInformations(int, int)));
	QObject::connect(_imageView, SIGNAL(pixelHovered(int, int)), this, SLOT(showHoveredPixelInformations(int, int)));
	//QObject::connect(_imageView, SIGNAL(zoomChanged(double)), this, SLOT(updateZoom(double)));
	QObject::connect(_imageView, SIGNAL(startDrag()), this, SLOT(startDrag()));
	QObject::connect(this, SIGNAL(ctrlPressed(bool)), _imageView, SLOT(ctrlPressed(bool)));
}

void StandardImageWindow::showGenericHistogram(GenericHistogramWindow* histogramWnd) {

    _imageView->setSelectSrc(histogramWnd->getView());
    QObject::connect(histogramWnd, SIGNAL(selectRectChange(imagein::Rectangle, GenericHistogramView*)), _imageView, SLOT(showSelectRect(imagein::Rectangle, GenericHistogramView*)));
   
    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addWidget(ws->getNodeId(this), histogramWnd);
}

void StandardImageWindow::showHistogram()
{
    HistogramWindow* histogramWnd = new HistogramWindow( _imageView->getImage(), _imageView->getRectangle(), this->windowTitle());
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showHProjectionHistogram()
{
    bool ok;
    int value = QInputDialog::getInt(this, tr("Select value"), tr("Which value (0..255) ?"), 0, 0, 255, 1, &ok);

    if (ok)
    {
        ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_imageView->getImage(), _imageView->getRectangle(), value, true, this->windowTitle());
        showGenericHistogram(histogramWnd);
    }
}

void StandardImageWindow::showVProjectionHistogram()
{
	bool ok;
	int value = QInputDialog::getInt(this, tr("Select value"), tr("Which value (0..255) ?"), 0, 0, 255, 1, &ok);
	
	if(ok)
	{
        ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_imageView->getImage(), _imageView->getRectangle(), value, false,  this->windowTitle());
        showGenericHistogram(histogramWnd);
	} 
}

void StandardImageWindow::showLineProfile()
{
	imagein::Rectangle rect(0, _selectedPixel->y(), _imageView->getImage()->getWidth(), 1);
    RowWindow* histogramWnd = new RowWindow(_imageView->getImage(), rect, false, this->windowTitle() + QString(" - ") + tr("Line Profile"));
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showColumnProfile()
{
	imagein::Rectangle rect(_selectedPixel->x(), 0, 1, _imageView->getImage()->getHeight());
    RowWindow* histogramWnd = new RowWindow(_imageView->getImage(), rect, true, this->windowTitle() + QString(" - ")  + tr("Line Profile"));
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showPixelsGrid()
{
    GridWindow* grid = new GridWindow(_imageView->getImage());
    grid->setWindowTitle(this->windowTitle() + QString(" - ")  + tr("Pixels Grid"));
    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addWidget(ws->getNodeId(this), grid);
}

void StandardImageWindow::crop() {
    const Image* oldImg = getImage();
    Image* newImg = oldImg->crop(_imageView->getRectangle());
    _imageView->setImage(newImg);
    _image = newImg;
    delete oldImg;
    _imageView->update();
    this->adjustSize();
    this->updateGeometry();
}

void StandardImageWindow::copycrop() {
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, true);
    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addImage(ws->getNodeId(this), newImgWnd);
}

void StandardImageWindow::convertToGrayscale() {
    GrayscaleImage* newImage = RgbToGrayscale()(Converter<RgbImage>::convert(*this->getImage()));
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, true);
    newImgWnd->setImage(newImage);

    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addImage(ws->getNodeId(this), newImgWnd);
}

void StandardImageWindow::convertToBinary() {
    GrayscaleImage* newImage = Otsu()(Converter<GrayscaleImage>::convert(*this->getImage()));
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, true);
    newImgWnd->setImage(newImage);

    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addImage(ws->getNodeId(this), newImgWnd);
}


template<class I>
void StandardImageWindow::convert() {
    I* newImage = Converter<I>::convert(*this->getImage());
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, true);
    newImgWnd->setImage(newImage);

    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addImage(ws->getNodeId(this), newImgWnd);
}

void StandardImageWindow::initStatusBar()
{
  QString width = QString("%1").arg(_imageView->pixmap().width());
  QString height = QString("%1").arg(_imageView->pixmap().height());
	
  QFont font;
  _statusBar = new QStatusBar();

  _lImageName = new QLabel(tr("Image") + " : " + ImageWindow::getTitleFromPath(_path));
  font = _lImageName->font();
  font.setPointSize(8);
  font.setBold(true);
  _lImageName->setFont(font);
  
  _lImageSize = new QLabel(QString("(%1x%2)").arg(width, height));
  font = _lImageSize->font();
  font.setPointSize(8);
  _lImageSize->setFont(font);

  QString stats("min : %1\t max : %2\t mean : %3\t standard deviation : %4");
  QString min="", max="", mean="", dev="";
  for(unsigned int c = 0; c < getImage()->getNbChannels(); ++c) {
        min += QString("%1").arg(getImage()->min(c));
        max += QString("%1").arg(getImage()->max(c));
        mean += QString("%1").arg(getImage()->mean(c), 0, 'f', 1);
        dev += QString("%1").arg(getImage()->deviation(c), 0, 'f', 1);
        if(c < getImage()->getNbChannels()-1)  {
            min+=" "; max+=" "; mean+=" "; dev+=" ";
        }
  }
  stats = stats.arg(min).arg(max).arg(mean).arg(dev);

  QLabel* lStats = new QLabel(stats);
  font = lStats->font();
  font.setPointSize(8);
  lStats->setFont(font);

  _lSelectedPixelInfo = new QLabel(tr("Selected") + " : ");
  font = _lSelectedPixelInfo->font();
  font.setPointSize(8);
  font.setBold(true);
  _lSelectedPixelInfo->setFont(font);
  
  _lSelectedPixelPosition = new QLabel(QString(""));
  font = _lSelectedPixelPosition->font();
  font.setPointSize(8);
  _lSelectedPixelPosition->setFont(font);
  
  _lSelectedPixelColor = new QLabel(tr("Color") + " : ");
  font = _lSelectedPixelColor->font();
  font.setPointSize(8);
  _lSelectedPixelColor->setFont(font);
  
  _lHoveredPixelInfo = new QLabel(tr("Hovered") + " : ");
  font = _lHoveredPixelInfo->font();
  font.setBold(true);
  font.setPointSize(8);
  _lHoveredPixelInfo->setFont(font);
  
  _lHoveredPixelPosition = new QLabel(QString(""));
  font = _lHoveredPixelPosition->font();
  font.setPointSize(8);
  _lHoveredPixelPosition->setFont(font);
  
  _lHoveredPixelColor = new QLabel(tr("Color") + " : ");
  font = _lHoveredPixelColor->font();
  font.setPointSize(8);
  _lHoveredPixelColor->setFont(font);
  
  _lZoom = new QLabel(tr("Zoom") + " : 100%");
  font = _lZoom->font();
  font.setPointSize(8);
  _lZoom->setFont(font);

  _selectButton = new QToolButton(this);
  _selectButton->setToolTip(tr("Selection mode"));
  _selectButton->setIcon(QIcon(":/images/tool-rect-select.png"));
  _selectButton->setCheckable(true);
  _selectButton->setAutoRaise(true);
  _selectButton->setIconSize (QSize(24, 24));
  
  _mouseButton = new QToolButton(this);
  _mouseButton->setToolTip(tr("Hand mode"));
  _mouseButton->setIcon(QIcon(":/images/tool-smudge.png"));
  _mouseButton->setCheckable(true);
  _mouseButton->setAutoRaise(true);
  _mouseButton->setIconSize (QSize(24, 24));
  _mouseButton->setChecked(true);
  
  _selectAllButton = new QToolButton(this);
  _selectAllButton->setToolTip(tr("Select all"));
  _selectAllButton->setIcon(QIcon(":/images/tool-align.png"));
  _selectAllButton->setCheckable(false);
  _selectAllButton->setAutoRaise(true);
  _selectAllButton->setIconSize (QSize(24, 24));
  _selectAllButton->setEnabled(false);
  
  //QObject::connect(_selectButton, SIGNAL(pressed()), _imageView, SLOT(selectionOn()));
  QObject::connect(_mouseButton, SIGNAL(toggled(bool)), this, SLOT(toggleMouseMode(bool)));
  QObject::connect(_selectButton, SIGNAL(toggled(bool)), this, SLOT(toggleSelectMode(bool)));
  QObject::connect(_selectAllButton, SIGNAL(clicked()), _imageView, SLOT(selectAll()));
  //QObject::connect(_selectButton, SIGNAL(clicked()), this, SLOT(makeSelection()));

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
  QWidget* widget = new QWidget();
	
	QHBoxLayout* layoutImage = new QHBoxLayout();
	layoutImage->setContentsMargins(0, 0, 0, 0);
  QWidget* widgetImage = new QWidget();
	layoutImage->addWidget(_lImageName);
  layoutImage->addWidget(_lImageSize);
    layoutImage->addSpacing(30);
	layoutImage->addWidget(_lZoom);
	layoutImage->addSpacing(30);
    layoutImage->setSpacing(0);
	layoutImage->addWidget(_mouseButton);
	layoutImage->addWidget(_selectButton);
	layoutImage->addWidget(_selectAllButton);
	layoutImage->addSpacing(8);
  widgetImage->setLayout(layoutImage);
  layout->addWidget(widgetImage);

  QHBoxLayout* layoutStats = new QHBoxLayout();
  layoutStats->addWidget(lStats);
  layout->addLayout(layoutStats);
	
	QHBoxLayout* layoutSelectedPixel = new QHBoxLayout();
	layoutSelectedPixel->setContentsMargins(0, 0, 0, 0);
  QWidget* widgetSelectedPixel = new QWidget();
	layoutSelectedPixel->addWidget(_lSelectedPixelInfo);
  layoutSelectedPixel->addWidget(_lSelectedPixelPosition);
  layoutSelectedPixel->addWidget(_lSelectedPixelColor);
  widgetSelectedPixel->setLayout(layoutSelectedPixel);
  layout->addWidget(widgetSelectedPixel);
	
	QHBoxLayout* layoutHoveredPixel = new QHBoxLayout();
	layoutHoveredPixel->setContentsMargins(0, 0, 0, 0);
  QWidget* widgetHoveredPixel = new QWidget();
	layoutHoveredPixel->addWidget(_lHoveredPixelInfo);
  layoutHoveredPixel->addWidget(_lHoveredPixelPosition);
  layoutHoveredPixel->addWidget(_lHoveredPixelColor);
  widgetHoveredPixel->setLayout(layoutHoveredPixel);
  layout->addWidget(widgetHoveredPixel);    
  
  widget->setLayout(layout);

  _statusBar->addWidget(widget);
}

void StandardImageWindow::toggleMouseMode(bool checked) {
    if(checked) {
        _selectButton->setChecked(false);
        _imageView->switchMode(StandardImageView::MODE_MOUSE);
        _selectAllButton->setEnabled(false);
    }
    else {
        if(!_selectButton->isChecked()) {
            _mouseButton->setChecked(true);
        }
    }
}

void StandardImageWindow::toggleSelectMode(bool checked) {
    if(checked) {
        _mouseButton->setChecked(false);
        _imageView->switchMode(StandardImageView::MODE_SELECT);
        _selectAllButton->setEnabled(true);
    }
    else {
        if(!_mouseButton->isChecked()) {
            _selectButton->setChecked(true);
        }
    }
}

void StandardImageWindow::showSelectedPixelInformations(int x, int y) const
{
	_selectedPixel->setX(x);
	_selectedPixel->setY(y);
	_lSelectedPixelPosition->setText(QString("%1x%2").arg(x).arg(y));
	
	_lSelectedPixelColor->setText(tr("Color") + " : ");
	const Image* im = _imageView->getImage();
	for(unsigned int i = 0; i < im->getNbChannels(); i++)
	{
		try {
            _lSelectedPixelColor->setText(_lSelectedPixelColor->text() + QString(" %1").arg(im->getPixel(x, y, i)) );
		}
		catch(std::out_of_range&) {
		}
	}
}

void StandardImageWindow::showHoveredPixelInformations(int x, int y) const
{
	_lHoveredPixelPosition->setText(QString("%1x%2").arg(x).arg(y));
	
	_lHoveredPixelColor->setText(tr("Color") + " :");
	const Image* im = _imageView->getImage();
	for(unsigned int i = 0; i < im->getNbChannels(); i++)
	{
		try {
            _lHoveredPixelColor->setText(_lHoveredPixelColor->text() + QString(" %1").arg(im->getPixel(x, y, i)) );
		}
		catch(std::out_of_range&) {
		}
	}
}


void StandardImageWindow::zoom(int delta) {
    //double wOrigin = _imageView->pixmap().width();
    //double hOrigin = _imageView->pixmap().height();
    const double coef = 1.4142135623730950488016887242096980785696718753769480;
    if(delta < 0 && _imageView->widget()->width() > 16) //Zoom out
    {
        _zoomFactor /= coef;
    }
    else if(delta > 0)//Zoom in
    {
        _zoomFactor *= coef;
    }		
    _imageView->scale(_zoomFactor, _zoomFactor);
    //if(delta < 0) {
        //_imageView->resize(_imageView->sizeHint());
        //QApplication::processEvents();
        //this->adjustSize();
        //QApplication::processEvents();
        //this->
    //}
    
    updateZoom(_zoomFactor*100);
}

void StandardImageWindow::updateZoom(double z) const
{
	_lZoom->setText(tr("Zoom") + QString(" : %1\%").arg(z));
}

void StandardImageWindow::wheelEvent (QWheelEvent * event) {
	if (_ctrlPressed && event->orientation() == Qt::Vertical)
	{
        this->zoom(event->delta());
	}
}

void StandardImageWindow::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Control) {
        _ctrlPressed = true;
		emit ctrlPressed(_ctrlPressed);
    }
}

void StandardImageWindow::keyReleaseEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Control) {
        _ctrlPressed = false;
		emit ctrlPressed(_ctrlPressed);
    }
}

const imagein::Image* StandardImageWindow::getImage()
{
  return _imageView->getImage();
}

void StandardImageWindow::setImage(Image* image)
{
    _imageView->setImage(image);
    delete _image;
    _image = image;
}

imagein::Rectangle StandardImageWindow::getSelection()
{
  return _imageView->getRectangle();
}
