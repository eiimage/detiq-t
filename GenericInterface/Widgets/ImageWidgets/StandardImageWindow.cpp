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
#include "GenericInterface.h"
#include "StandardImageWindow.h"
#include "UnknownFormatException.h"
#include "GraphicalHistogram.h"

#include "Algorithm/RgbToGrayscale.h"
#include "Algorithm/Otsu.h"

#include <QPushButton>
#include <QMessageBox>

using namespace genericinterface;
using namespace imagein;
using namespace std;
using namespace imagein::algorithm;

StandardImageWindow::StandardImageWindow(const QString path)
    : ImageWindow(path)
{
    bool error = false;
    QString msg = "";
    imagein::Image* image;
    try {
        image = new Image(path.toStdString());
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
        image = new Image();
    }

    _image = image;
    this->setDisplayImage(image);
    
    this->setWindowTitle(ImageWindow::getTitleFromPath(path));
    
    init();
}

StandardImageWindow::StandardImageWindow(Image* image, const QString path)
    : ImageWindow(path, image)
{
    _image = image;

    this->setWindowTitle(ImageWindow::getTitleFromPath(path));

    init();
}

StandardImageWindow::StandardImageWindow(const StandardImageWindow& siw, imagein::Image* image)
    : ImageWindow(siw.getPath())
{
    if(image == NULL) {
        image = new Image(*siw._image);
    }

    _image = image;
    this->setDisplayImage(image);

    this->setWindowTitle(siw.windowTitle());

    init();
}

StandardImageWindow::~StandardImageWindow()
{
    delete _image;
}


void StandardImageWindow::init()
{
    QObject::connect(this->view(), SIGNAL(updateSrc(GenericHistogramView*,imagein::Rectangle)), this, SLOT(updateSrc(GenericHistogramView*,imagein::Rectangle)));

    menu()->addAction(tr("Histogram"), this, SLOT(showHistogram()));
    menu()->addAction(tr("Horizontal Projection Histogram"), this, SLOT(showHProjectionHistogram()));
    menu()->addAction(tr("Vertical Projection Histogram"), this, SLOT(showVProjectionHistogram()));
    menu()->addAction(tr("Pixels Grid"), this, SLOT(showPixelsGrid()));
    menu()->addAction(tr("Column Profile"), this, SLOT(showColumnProfile()));
    menu()->addAction(tr("Line Profile"), this, SLOT(showLineProfile()));
    menu()->addSeparator();
    menu()->addAction(tr("Crop"), this, SLOT(crop()));
    menu()->addAction(tr("Copy & crop"), this, SLOT(copycrop()));
    menu()->addSeparator();
    menu()->addAction(tr("Convert to grayscale"), this, SLOT(convertToGrayscale()));
    menu()->addAction(tr("Convert to binary"), this, SLOT(convertToBinary()));

    updateStatusBar();

}

void StandardImageWindow::updateStatusBar()
{
    QFont font;

    //Statistics
    QString stats("min : %1\t max : %2\t mean : %3\t standard deviation : %4");
    QString min="", max="", mean="", dev="";
    for(unsigned int c = 0; c < _image->getNbChannels(); ++c) {
        min += QString("%1").arg(_image->min(c));
        max += QString("%1").arg(_image->max(c));
        mean += QString("%1").arg(_image->mean(c), 0, 'f', 1);
        dev += QString("%1").arg(_image->deviation(c), 0, 'f', 1);
        if(c < _image->getNbChannels()-1)  {
            min+=" "; max+=" "; mean+=" "; dev+=" ";
        }
    }
    stats = stats.arg(min).arg(max).arg(mean).arg(dev);
    QLabel* lStats = new QLabel(stats);
    font = lStats->font();
    font.setPointSize(8);
    lStats->setFont(font);

    //Selected pixel informations
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

    //Hovered pixel informations
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

    //Widget placing
    QHBoxLayout* layoutStats = new QHBoxLayout();
    layoutStats->addWidget(lStats);

    QHBoxLayout* layoutSelectedPixel = new QHBoxLayout();
    layoutSelectedPixel->setContentsMargins(0, 0, 0, 0);
    layoutSelectedPixel->addWidget(_lSelectedPixelInfo);
    layoutSelectedPixel->addWidget(_lSelectedPixelPosition);
    layoutSelectedPixel->addWidget(_lSelectedPixelColor);

    QHBoxLayout* layoutHoveredPixel = new QHBoxLayout();
    layoutHoveredPixel->setContentsMargins(0, 0, 0, 0);
    layoutHoveredPixel->addWidget(_lHoveredPixelInfo);
    layoutHoveredPixel->addWidget(_lHoveredPixelPosition);
    layoutHoveredPixel->addWidget(_lHoveredPixelColor);

    QWidget* infoWidget = new QWidget();
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->addLayout(layoutStats);
    infoLayout->addLayout(layoutSelectedPixel);
    infoLayout->addLayout(layoutHoveredPixel);
    _statusBar->layout()->addWidget(infoWidget);

}


void StandardImageWindow::showGenericHistogram(GenericHistogramWindow* histogramWnd) {

    _imageView->setSelectSrc(histogramWnd->getView());
    QObject::connect(histogramWnd, SIGNAL(selectRectChange(imagein::Rectangle, GenericHistogramView*)), _imageView, SLOT(showSelectRect(imagein::Rectangle, GenericHistogramView*)));
   
//    WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
//    ws->addWidget(ws->getNodeId(this), histogramWnd);
    emit addWidget(this, histogramWnd);
}

void StandardImageWindow::showHistogram()
{
    HistogramWindow* histogramWnd = new HistogramWindow( _image, selection(), this->windowTitle());
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showHProjectionHistogram()
{
    bool ok;
    int value = QInputDialog::getInt(this, tr("Select value"), tr("Which value (0..255) ?"), 0, 0, 255, 1, &ok);

    if (ok)
    {
        ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_image, selection(), value, true, this->windowTitle());
        showGenericHistogram(histogramWnd);
    }
}

void StandardImageWindow::showVProjectionHistogram()
{
	bool ok;
	int value = QInputDialog::getInt(this, tr("Select value"), tr("Which value (0..255) ?"), 0, 0, 255, 1, &ok);
	
	if(ok)
	{
        ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_image, selection(), value, false,  this->windowTitle());
        showGenericHistogram(histogramWnd);
	} 
}

void StandardImageWindow::showLineProfile()
{
    imagein::Rectangle rect(0, _selectedPixel.y(), _image->getWidth(), 1);
    RowWindow* histogramWnd = new RowWindow(_image, rect, false, this->windowTitle() + QString(" - ") + tr("Line Profile"));
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showColumnProfile()
{
    imagein::Rectangle rect(_selectedPixel.x(), 0, 1, _image->getHeight());
    RowWindow* histogramWnd = new RowWindow(_image, rect, true, this->windowTitle() + QString(" - ")  + tr("Line Profile"));
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showPixelsGrid()
{
    GridWindow* grid = new GridWindow(_image);
    grid->setWindowTitle(this->windowTitle() + QString(" - ")  + tr("Pixels Grid"));
    emit addWidget(this, grid);
}

void StandardImageWindow::crop() {
    const Image* oldImg = _image;
    Image* newImg = oldImg->crop(_imageView->getRectangle());
    this->setDisplayImage(newImg);
    _image = newImg;
    delete oldImg;
    view()->update();
    this->adjustSize();
    this->updateGeometry();
}

void StandardImageWindow::copycrop() {
    const Image* oldImg = _image;
    Image* newImg = oldImg->crop(_imageView->getRectangle());
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, newImg);
    emit addImage(this, newImgWnd);
}

void StandardImageWindow::convertToGrayscale() {
    GrayscaleImage* newImg = RgbToGrayscale()(Converter<RgbImage>::convert(*_image));
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, newImg);

    emit addImage(this, newImgWnd);
}

void StandardImageWindow::convertToBinary() {
    GrayscaleImage* newImg = Otsu()(Converter<GrayscaleImage>::convert(*_image));
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, newImg);

    emit addImage(this, newImgWnd);
}

void StandardImageWindow::showSelectedPixelInformations(int x, int y) const
{
    _lSelectedPixelPosition->setText(QString("%1x%2").arg(x).arg(y));
    _lSelectedPixelColor->setText(tr("Color") + " : ");
    for(unsigned int i = 0; i < _image->getNbChannels(); i++)
    {
        try {
            _lSelectedPixelColor->setText(_lSelectedPixelColor->text() + QString(" %1").arg(_image->getPixel(x, y, i)) );
        }
        catch(std::out_of_range&) {
        }
    }
}

void StandardImageWindow::showHoveredPixelInformations(int x, int y) const
{
    _lHoveredPixelPosition->setText(QString("%1x%2").arg(x).arg(y));

    _lHoveredPixelColor->setText(tr("Color") + " :");
    for(unsigned int i = 0; i < _image->getNbChannels(); i++)
    {
        try {
            _lHoveredPixelColor->setText(_lHoveredPixelColor->text() + QString(" %1").arg(_image->getPixel(x, y, i)) );
        }
        catch(std::out_of_range&) {
        }
    }
}

void StandardImageWindow::updateSrc(GenericHistogramView* histo, imagein::Rectangle rect) {
    histo->update(_image, rect);
}
