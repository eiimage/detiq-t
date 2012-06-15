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
    delete _view;
    delete _scene;
}


void StandardImageWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {


     }
     ImageWindow::mousePressEvent(event);
}

void StandardImageWindow::startDrag() {
    QDrag *drag = new QDrag(this);
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

    /*Qt::DropAction dropAction = */drag->exec();
}

void StandardImageWindow::mouseMoveEvent(QMouseEvent *event) {
    //std::cout << "mouseMoveEvent " << event->x() << ":" << event->y() << std::endl;
     ImageWindow::mouseMoveEvent(event);
}

void StandardImageWindow::init()
{
    _zoomFactor = 1;
	_scene = new QGraphicsScene(this);
    _imageView = new StandardImageView(this, _image);
	_scene->addItem(_imageView);
	_scene->addItem(_imageView->getHighlightItem());
    
    _view = new QGraphicsView(_scene);
    
    
	_menu = new ImageContextMenu(_view);
	_view->setContextMenuPolicy(Qt::CustomContextMenu);
	_menu->addAction("Histogram", this, SLOT(showHistogram()));
	_menu->addAction("Horizontal Projection Histogram", this, SLOT(showHProjectionHistogram()));
	_menu->addAction("Vertical Projection Histogram", this, SLOT(showVProjectionHistogram()));
	_menu->addAction("Pixels Grid", this, SLOT(showPixelsGrid()));
	_menu->addAction("Column Profile", this, SLOT(showColumnProfile()));
	_menu->addAction("Line Profile", this, SLOT(showLineProfile()));
	QObject::connect(_view, SIGNAL(customContextMenuRequested(const QPoint&)), _menu, SLOT(showContextMenu(const QPoint&)));
    
    _selectedPixel = new QPoint();

    initStatusBar();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(_view);
	layout->addWidget(_statusBar);
    this->setLayout(layout);

	QObject::connect(_imageView, SIGNAL(pixelClicked(int, int)), this, SLOT(showSelectedPixelInformations(int, int)));
	QObject::connect(_imageView, SIGNAL(pixelHovered(int, int)), this, SLOT(showHoveredPixelInformations(int, int)));
	//QObject::connect(_imageView, SIGNAL(zoomChanged(double)), this, SLOT(updateZoom(double)));
	QObject::connect(_imageView, SIGNAL(startDrag()), this, SLOT(startDrag()));
	QObject::connect(this, SIGNAL(ctrlPressed()), _imageView, SLOT(ctrlPressed()));
	QObject::connect(this, SIGNAL(highlightRectChange(imagein::Rectangle, ImageWindow*)), _imageView, SLOT(showHighlightRect(imagein::Rectangle, ImageWindow*)));
}

void StandardImageWindow::showHistogram()
{
    //QString path = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE))->getWidgetId(this);
    const Image* im = _imageView->getImage();
    HistogramWindow* histo = new HistogramWindow(im, _imageView->getRectangle(), this);

    AlternativeImageView* view = histo->getView();
    GenericHistogramView* source;
    if(view != NULL && (source = dynamic_cast<GenericHistogramView*>(view)))
        QObject::connect(source, SIGNAL(updateApplicationArea(imagein::Rectangle)), histo, SLOT(setApplicationArea(imagein::Rectangle)));
        QObject::connect(histo, SIGNAL(highlightRectChange(imagein::Rectangle, ImageWindow*)), this, SLOT(showHighlightRect(imagein::Rectangle, ImageWindow*)));
   
    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addWidget(ws->getNodeId(this), histo);
}

void StandardImageWindow::showHProjectionHistogram()
{
    //QString path = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE))->getWidgetId(this);
    const Image* im = _imageView->getImage();

    bool ok;
    int value = QInputDialog::getInt(this, "Select value", "What Value (0..255)?", 0, 0, 255, 1, &ok);

    if (ok)
    {
        ProjectionHistogramWindow* histo = new ProjectionHistogramWindow(im, _imageView->getRectangle(), this, value);

        AlternativeImageView* view = histo->getView();
        GenericHistogramView* source;
        if (view != NULL && (source = dynamic_cast<GenericHistogramView*>(view)))
            QObject::connect(source, SIGNAL(updateApplicationArea(imagein::Rectangle)), histo, SLOT(setApplicationArea(imagein::Rectangle)));

        WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
        ws->addWidget(ws->getNodeId(this), histo);
    }
}

void StandardImageWindow::showVProjectionHistogram()
{
    //QString path = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE))->getWidgetId(this);
    const Image* im = _imageView->getImage();
	
	bool ok;
	int value = QInputDialog::getInt(this, "Select value", "What Value (0..255)?", 0, 0, 255, 1, &ok);
	
	if(ok)
	{
        ProjectionHistogramWindow* histo = new ProjectionHistogramWindow(im, _imageView->getRectangle(), this, value, false);
    
		AlternativeImageView* view = histo->getView();
		GenericHistogramView* source;
		if (view != NULL && (source = dynamic_cast<GenericHistogramView*>(view)))
			QObject::connect(source, SIGNAL(updateApplicationArea(imagein::Rectangle)), histo, SLOT(setApplicationArea(imagein::Rectangle)));

        WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
        ws->addWidget(ws->getNodeId(this), histo);
	} 
}

void StandardImageWindow::showPixelsGrid()
{
 
    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    GridWindow* grid = new GridWindow(_imageView->getImage(), this);
    ws->addWidget(ws->getNodeId(this), grid);
}

void StandardImageWindow::showLineProfile()
{
    //QString path = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE))->getWidgetId(this);

	const Image* im = _imageView->getImage();
	imagein::Rectangle rect(0, _selectedPixel->y(), im->getWidth(), 0);
    RowWindow* histo = new RowWindow(im, rect, _gi, this);
    
	AlternativeImageView* view = histo->getView();
	GenericHistogramView* source;
	if (view != NULL && (source = dynamic_cast<GenericHistogramView*>(view)))
		QObject::connect(source, SIGNAL(updateApplicationArea(imagein::Rectangle)), histo, SLOT(setApplicationArea(imagein::Rectangle)));

    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addWidget(ws->getNodeId(this), histo);
}

void StandardImageWindow::showColumnProfile()
{
    //QString path = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE))->getWidgetId(this);

    const Image* im = _imageView->getImage();
	imagein::Rectangle rect(_selectedPixel->x(), 0, 0, im->getHeight());
    RowWindow* histo = new RowWindow(im, rect, _gi, this, true);
    
	AlternativeImageView* view = histo->getView();
	GenericHistogramView* source;
	if (view != NULL && (source = dynamic_cast<GenericHistogramView*>(view)))
		QObject::connect(source, SIGNAL(updateApplicationArea(imagein::Rectangle)), histo, SLOT(setApplicationArea(imagein::Rectangle)));

    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addWidget(ws->getNodeId(this), histo);
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

  _selectButton = new QPushButton("SEL");
  _mouseButton = new QPushButton("MOU");
  _selectButton->setCheckable(true);
  _mouseButton->setCheckable(true);
  //QObject::connect(_selectButton, SIGNAL(pressed()), _imageView, SLOT(selectionOn()));
  QObject::connect(_mouseButton, SIGNAL(toggled(bool)), this, SLOT(toggleMouseMode(bool)));
  QObject::connect(_selectButton, SIGNAL(toggled(bool)), this, SLOT(toggleSelectMode(bool)));
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
	layoutImage->addWidget(_mouseButton);
	layoutImage->addWidget(_selectButton);
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
        _imageView->setMode(StandardImageView::MODE_MOUSE);
    }
}

void StandardImageWindow::toggleSelectMode(bool checked) {
    if(checked) {
        _mouseButton->setChecked(false);
        _imageView->setMode(StandardImageView::MODE_SELECT);
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
		oss << (unsigned int) im->getPixel(x, y, i);
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
		emit ctrlPressed();
    }
}

void StandardImageWindow::keyReleaseEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Control) {
        _ctrlPressed = false;
		emit ctrlPressed();
    }
}

void StandardImageWindow::showHighlightRect(Rectangle rect, ImageWindow* source)
{
	emit(highlightRectChange(rect, source));
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
