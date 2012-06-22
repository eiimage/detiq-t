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

#include <QPushButton>
#include <QMessageBox>

using namespace genericinterface;
using namespace imagein;
using namespace std;

StandardImageWindow::StandardImageWindow(const QString path, GenericInterface* gi)
    : ImageWindow(path), _gi(gi), _ctrlPressed(false)
{
    bool error = false;
    std::string msg = "";
    try {
        _image = new Image(path.toStdString());
    }
    catch(const imagein::UnknownFormatException& e) {
        error = true;
        msg = "Unknown file format !";
    }
    catch(const imagein::ImageFileException& e) {
        error = true;
        msg = e.getMsg();
    }
    catch(...) {
        error = true;
        msg = "Unknown exception";
    }
    
    if(error) {
        QMessageBox::critical(this, "Error while opening file", msg.c_str());
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

StandardImageWindow::StandardImageWindow(const StandardImageWindow& siw) 
    : ImageWindow(siw.getPath()), _gi(siw._gi) , _ctrlPressed(false)
{
    _image = new Image(*siw._image);

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
    if (event->button() == Qt::LeftButton) {


     }
     ImageWindow::mousePressEvent(event);
}

void StandardImageWindow::startDrag() {
    QDrag *drag = new QDrag(_gi);
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    //Node *node = new Node(this->getImage(), this->getPath());
    uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
    stream << ptr;
    mimeData->setData("application/detiqt.genericinterface.stdimgwnd", encodedData);
    
    
    //QImage img = getQImage(this->getImage()).scaled(QSize(76,76), Qt::KeepAspectRatio, Qt::FastTransformation);
     
    drag->setMimeData(mimeData);
    drag->setPixmap(_imageView->pixmap().scaled(QSize(76,76), Qt::KeepAspectRatio, Qt::FastTransformation));
    drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));

    drag->exec();
    
}

void StandardImageWindow::mouseMoveEvent(QMouseEvent *event) {
    //std::cout << "mouseMoveEvent " << event->x() << ":" << event->y() << std::endl;
     ImageWindow::mouseMoveEvent(event);
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
	_menu->addAction("Histogram", this, SLOT(showHistogram()));
	_menu->addAction("Horizontal Projection Histogram", this, SLOT(showHProjectionHistogram()));
	_menu->addAction("Vertical Projection Histogram", this, SLOT(showVProjectionHistogram()));
	_menu->addAction("Pixels Grid", this, SLOT(showPixelsGrid()));
	_menu->addAction("Column Profile", this, SLOT(showColumnProfile()));
	_menu->addAction("Line Profile", this, SLOT(showLineProfile()));
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
    int value = QInputDialog::getInt(this, "Select value", "What Value (0..255)?", 0, 0, 255, 1, &ok);

    if (ok)
    {
        ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_imageView->getImage(), _imageView->getRectangle(), value, true, this->windowTitle());
        showGenericHistogram(histogramWnd);
    }
}

void StandardImageWindow::showVProjectionHistogram()
{
	bool ok;
	int value = QInputDialog::getInt(this, "Select value", "What Value (0..255)?", 0, 0, 255, 1, &ok);
	
	if(ok)
	{
        ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_imageView->getImage(), _imageView->getRectangle(), value, false,  this->windowTitle());
        showGenericHistogram(histogramWnd);
	} 
}

void StandardImageWindow::showLineProfile()
{
	imagein::Rectangle rect(0, _selectedPixel->y(), _imageView->getImage()->getWidth(), 1);
    RowWindow* histogramWnd = new RowWindow(_imageView->getImage(), rect, false, this->windowTitle() + " - Line Profile");
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showColumnProfile()
{
	imagein::Rectangle rect(_selectedPixel->x(), 0, 1, _imageView->getImage()->getHeight());
    RowWindow* histogramWnd = new RowWindow(_imageView->getImage(), rect, true, this->windowTitle() + " - Line Profile");
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showPixelsGrid()
{
    GridWindow* grid = new GridWindow(_imageView->getImage());
    grid->setWindowTitle(this->windowTitle() + QString::fromStdString(" - Pixels Grid"));
    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addWidget(ws->getNodeId(this), grid);
}

void StandardImageWindow::initStatusBar()
{
  std::ostringstream oss;
  oss << _imageView->pixmap().height();
  std::string height = oss.str();
  oss.str("");
  oss << _imageView->pixmap().width();
  std::string width = oss.str();
	
  QFont font;
  _statusBar = new QStatusBar();

  _lImageName = new QLabel(QString::fromStdString("Image: ") + ImageWindow::getTitleFromPath(_path));
  font = _lImageName->font();
  font.setPointSize(8);
  font.setBold(true);
  _lImageName->setFont(font);
  
  _lImageSize = new QLabel(QString::fromStdString("(" + width + " * " + height + ")"));
  font = _lImageSize->font();
  font.setPointSize(8);
  _lImageSize->setFont(font);
  
  _lSelectedPixelInfo = new QLabel("Selected: ");
  font = _lSelectedPixelInfo->font();
  font.setPointSize(8);
  font.setBold(true);
  _lSelectedPixelInfo->setFont(font);
  
  _lSelectedPixelPosition = new QLabel("");
  font = _lSelectedPixelPosition->font();
  font.setPointSize(8);
  _lSelectedPixelPosition->setFont(font);
  
  _lSelectedPixelColor = new QLabel("Color: ");
  font = _lSelectedPixelColor->font();
  font.setPointSize(8);
  _lSelectedPixelColor->setFont(font);
  
  _lHoveredPixelInfo = new QLabel("Hovered: ");
  font = _lHoveredPixelInfo->font();
  font.setBold(true);
  font.setPointSize(8);
  _lHoveredPixelInfo->setFont(font);
  
  _lHoveredPixelPosition = new QLabel("");
  font = _lHoveredPixelPosition->font();
  font.setPointSize(8);
  _lHoveredPixelPosition->setFont(font);
  
  _lHoveredPixelColor = new QLabel("Color: ");
  font = _lHoveredPixelColor->font();
  font.setPointSize(8);
  _lHoveredPixelColor->setFont(font);
  
  _lZoom = new QLabel("Zoom: 100%");
  font = _lZoom->font();
  font.setPointSize(8);
  _lZoom->setFont(font);

  _selectButton = new QToolButton(this);
  _selectButton->setIcon(QIcon(":/images/tool-rect-select.png"));
  _selectButton->setCheckable(true);
  _selectButton->setAutoRaise(true);
  _selectButton->setIconSize (QSize(24, 24));
  
  _mouseButton = new QToolButton(this);
  _mouseButton->setIcon(QIcon(":/images/tool-smudge.png"));
  _mouseButton->setCheckable(true);
  _mouseButton->setAutoRaise(true);
  _mouseButton->setIconSize (QSize(24, 24));
  
  _selectAllButton = new QToolButton(this);
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
	std::ostringstream oss;
    oss << x;
    std::string xs = oss.str();
    oss.str("");
    oss << y;
    std::string ys = oss.str();
	_lSelectedPixelPosition->setText(QString::fromStdString(xs + " * " + ys));
	
	_lSelectedPixelColor->setText("Color:");
	const Image* im = _imageView->getImage();
	for(unsigned int i = 0; i < im->getNbChannels(); i++)
	{
		oss.str("");
        if(x>0 && y>0 && x<(int)im->getWidth() && y<(int)im->getHeight()) {
            oss << (unsigned int) im->getPixel(x, y, i);
        }
		_lSelectedPixelColor->setText(_lSelectedPixelColor->text() + QString::fromStdString(" " + oss.str()));
	}
}

void StandardImageWindow::showHoveredPixelInformations(int x, int y) const
{
	std::ostringstream oss;
    oss << x;
    std::string xs = oss.str();
    oss.str("");
    oss << y;
    std::string ys = oss.str();
	_lHoveredPixelPosition->setText(QString::fromStdString(xs + " * " + ys));
	
	_lHoveredPixelColor->setText("Color:");
	const Image* im = _imageView->getImage();
	for(unsigned int i = 0; i < im->getNbChannels(); i++)
	{
		oss.str("");
		try {
			oss << (unsigned int) im->getPixel(x, y, i);
		}
		catch(std::out_of_range&) {
		}
		_lHoveredPixelColor->setText(_lHoveredPixelColor->text() + QString::fromStdString(" " + oss.str()));
	}
}


void StandardImageWindow::zoom(int delta) {
    double wOrigin = _imageView->pixmap().width();
    double hOrigin = _imageView->pixmap().height();
    if(delta < 0 && _zoomFactor > 0.05) //Zoom out
    {
        _zoomFactor -= 0.05;
    }
    else if(delta > 0)//Zoom in
    {
        _zoomFactor += 0.05;
    }		
    _imageView->scale(_zoomFactor);
    
    updateZoom(_zoomFactor*100);
}

void StandardImageWindow::updateZoom(double z) const
{
	std::ostringstream oss;
    oss << z;
    std::string zs = oss.str();
	_lZoom->setText(QString::fromStdString("Zoom: " + zs + "%"));
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
