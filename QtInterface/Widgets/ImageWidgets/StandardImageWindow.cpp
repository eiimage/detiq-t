#include "StandardImageWindow.h"

using namespace genericinterface;
using namespace imagein;
using namespace std;

StandardImageWindow::StandardImageWindow(const QString & path, GenericInterface* gi): ImageWindow(), _gi(gi)
{
    _path = path;
    Image* im = new Image(path.toStdString());
    this->setWindowTitle("Image - " + path);
    _imageView = new StandardImageView(this, im);
	
    init();

    this->show();
}

void StandardImageWindow::init()
{
	_selectedPixel = new QPoint();
	
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setWidget(_imageView);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setAlignment(Qt::AlignCenter);
	
	initStatusBar();
    
	QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(scrollArea);
	layout->addWidget(_statusBar);
	this->setLayout(layout);
	
    connect(_imageView, SIGNAL(pixelClicked(int, int)), this, SLOT(showSelectedPixelInformations(int, int)));
    connect(_imageView, SIGNAL(pixelHovered(int, int)), this, SLOT(showHoveredPixelInformations(int, int)));
    connect(_imageView, SIGNAL(zoomChanged(double)), this, SLOT(updateZoom(double)));
    connect(this, SIGNAL(ctrlPressed()), _imageView, SLOT(ctrlPressed()));
}

void StandardImageWindow::showHistogram()
{
  list<HistogramWindow*> histos = getHistogram();

  for(list<HistogramWindow*>::iterator it = histos.begin(); it != histos.end(); ++it)
  {
    dynamic_cast<WindowService*>(_gi->getService(0))->addWidget(_path, *it);
  }
}

list<HistogramWindow*> StandardImageWindow::getHistogram()
{
  //Rectangle rect = _imageView->getRectangle();
  Rectangle rect;

  Image* im = _imageView->getImage();
  list<HistogramWindow*> histos;

  HistogramWindow* histo = new HistogramWindow(im, &rect, this);
  histos.push_back(histo);

  return histos;
}

void StandardImageWindow::showPixelsGrid()
{
	GridWindow* grid = new GridWindow(_imageView->getImage(), this);
	_area->addSubWindow(grid);
	grid->show();
}

void StandardImageWindow::showLineProfile()
{
	Image* im = _imageView->getImage();
	imagein::Rectangle* rect = new Rectangle(0, _selectedPixel->y(), im->getWidth(), 1);
	//RowWindow* histo = new RowWindow(im, rect, this);
  RowWindow* histo = new RowWindow(im, rect, _path, _gi);

  dynamic_cast<WindowService*>(_gi->getService(0))->addWidget(_path, histo);
			
	//_area->addSubWindow(histo);
	//histo->show();
}

void StandardImageWindow::showColumnProfile()
{
	Image* im = _imageView->getImage();
	imagein::Rectangle* rect = new Rectangle(0, _selectedPixel->y(), im->getWidth(), 1);
	//RowWindow* histo = new RowWindow(im, rect, this);
  RowWindow* histo = new RowWindow(im, rect, _path, _gi);

  dynamic_cast<WindowService*>(_gi->getService(0))->addWidget(_path, histo);

	//Image* im = _imageView->getImage();
	//imagein::Rectangle* rect = new Rectangle(_selectedPixel->x(), 0, 1, im->getHeight());
	//RowWindow* histo = new RowWindow(im, rect, this, true);
			
	//_area->addSubWindow(histo);
	//histo->show();
}

void StandardImageWindow::initStatusBar()
{
	std::ostringstream oss;
    oss << _imageView->getPixmap()->height();
    std::string height = oss.str();
    oss.str("");
    oss << _imageView->getPixmap()->width();
    std::string width = oss.str();
	
	QFont font;
    _statusBar = new QStatusBar();
    
    _lImageName = new QLabel("Name: Nom_Image");
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

	_statusBar->addWidget(_lImageName);
    _statusBar->addWidget(_lImageSize);
	_statusBar->addWidget(_lSelectedPixelInfo);
	_statusBar->addWidget(_lSelectedPixelPosition);
	_statusBar->addWidget(_lSelectedPixelColor);
	_statusBar->addWidget(_lHoveredPixelInfo);
	_statusBar->addWidget(_lHoveredPixelPosition);
	_statusBar->addWidget(_lHoveredPixelColor);
	_statusBar->addWidget(_lZoom);
}

void StandardImageWindow::showSelectedPixelInformations(int x, int y)
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
}

void StandardImageWindow::showHoveredPixelInformations(int x, int y)
{
	std::ostringstream oss;
    oss << x;
    std::string xs = oss.str();
    oss.str("");
    oss << y;
    std::string ys = oss.str();
	_lHoveredPixelPosition->setText(QString::fromStdString(xs + " * " + ys));
}

void StandardImageWindow::updateZoom(double z)
{
	std::ostringstream oss;
    oss << z;
    std::string zs = oss.str();
	_lZoom->setText(QString::fromStdString("Zoom: " + zs + "%"));
}

void StandardImageWindow::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Control)
		emit ctrlPressed();
}

void StandardImageWindow::keyReleaseEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Control)
		emit ctrlPressed();
}

/*void StandardImageWindow::showHighlightRect(Rectangle* rect)
{
	
}*/