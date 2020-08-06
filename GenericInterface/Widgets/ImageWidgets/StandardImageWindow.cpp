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
#include "GridView.h"

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
//        image = new Image(path.toStdString());
        /*Returns the local 8-bit representation of the file path as a QByteArray.*/
        image = new Image(path.toLocal8Bit().constData());
    }
    catch(const imagein::UnknownFormatException& e) {
        error = true;
        msg = tr("Unknown file format !");
    }
    catch(const imagein::ImageFileException& e) {
        error = true;
        msg = QString::fromStdString(e.getMsg());
    }
    catch(const char* e) {
        error = true;
        msg = e;
    }
    catch(...) {
        error = true;
        msg = tr("Unknown exception");
    }
    
    if(error) {
        QMessageBox::critical(this, tr("Error while opening file"), msg);
        image = new Image();
    }

    this->setDisplayImage(image);
    this->setWindowTitle(ImageWindow::getTitleFromPath(path));
    
    init();
}

StandardImageWindow::StandardImageWindow(Image* image, const QString path)
    : ImageWindow(path, image)
{
    this->setWindowTitle(ImageWindow::getTitleFromPath(path));

    init();
}

StandardImageWindow::StandardImageWindow(const StandardImageWindow& siw, imagein::Image* image)
    : ImageWindow(siw.getPath())
{
    if(image == NULL) {
        image = new Image(*siw._displayImg);
    }

    this->setDisplayImage(image);
    this->setWindowTitle(siw.windowTitle());

    init();
}

StandardImageWindow::~StandardImageWindow()
{
}

void StandardImageWindow::init()
{
    QObject::connect(this->view(), SIGNAL(updateSrc(GenericHistogramView*,imagein::Rectangle)), this, SLOT(updateSrc(GenericHistogramView*,imagein::Rectangle)));

    menu()->addAction(tr("Crop"), this, SLOT(crop()));
    menu()->addAction(tr("Copy and crop"), this, SLOT(copycrop()));
    menu()->addSeparator();

    updateStatusBar();
}

void StandardImageWindow::updateStatusBar()
{
    QFont font;

    //Statistics
    QString stats("min : %1\t max : %2\t mean : %3\t standard deviation : %4");
    QString min="", max="", mean="", dev="";
    for(unsigned int c = 0; c < _displayImg->getNbChannels(); ++c) {
        min += QString("%1").arg(_displayImg->min(c));
        max += QString("%1").arg(_displayImg->max(c));
        mean += QString("%1").arg(_displayImg->mean(c), 0, 'f', 1);
        dev += QString("%1").arg(_displayImg->deviation(c), 0, 'f', 1);
        if(c < _displayImg->getNbChannels()-1)  {
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
    _infoLayout->addWidget(infoWidget);

}

void StandardImageWindow::showCumulatedHistogram() {
    HistogramWindow* histogramWnd = new HistogramWindow( _displayImg, selection(), this->windowTitle(), true);
    showGenericHistogram(histogramWnd);
}

void StandardImageWindow::showHProjectionHistogram()
{
    bool ok;
    int value = QInputDialog::getInt(this, tr("Threshold selection"), tr("Enter the threshold value\n(only pixels superior or equal to this value will be counted)"), _displayImg->getPixel(_selectedPixel.x(),_selectedPixel.y(),0), 0, 255, 1, &ok);

    if (ok)
    {
        ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_displayImg, selection(), value, true, this->windowTitle());
        showGenericHistogram(histogramWnd);
    }
}

void StandardImageWindow::showVProjectionHistogram()
{
    bool ok;
    int value = QInputDialog::getInt(this, tr("Threshold selection"), tr("Enter the threshold value\n(only pixels superior or equal to this value will be counted)"), _displayImg->getPixel(_selectedPixel.x(),_selectedPixel.y(),0), 0, 255, 1, &ok);

    if(ok)
    {
    ProjectionHistogramWindow* histogramWnd = new ProjectionHistogramWindow(_displayImg, selection(), value, false,  this->windowTitle());
    showGenericHistogram(histogramWnd);
    }
}

void StandardImageWindow::showLineProfile()
{
    bool ok;
    int rowNumber = QInputDialog::getInt(this, tr("Line selection"), tr("Select the line to project"), _selectedPixel.y(), 0, selection().h /*_displayImg->getHeight()*/, 1, &ok);

    if(ok)
    {
        RowWindow* histogramWnd = new RowWindow(_displayImg, selection(), rowNumber, this->windowTitle());
        showGenericHistogram(histogramWnd);
    }

}

void StandardImageWindow::showColumnProfile()
{
    bool ok;
    int columnNumber = QInputDialog::getInt(this, tr("Column selection"), tr("Select the column to project"), _selectedPixel.x(), 0, selection().w /*_displayImg->getWidth()*/, 1, &ok);

    if(ok)
    {        
        ColumnWindow* histogramWnd = new ColumnWindow(_displayImg, selection(), columnNumber, this->windowTitle());
        showGenericHistogram(histogramWnd);
    }
}

void StandardImageWindow::showPixelsGrid()
{
    GridView* grid = new GridView(_displayImg);
    grid->setWindowTitle(this->windowTitle() + QString(" - ")  + tr("Pixels Grid"));
    emit addWidget(this, grid);
}
void StandardImageWindow::crop() {
    const Image* oldImg = _displayImg;
    Image* newImg = oldImg->crop(_imageView->getRectangle());
    this->setDisplayImage(newImg);
    delete oldImg;
    view()->update();
    this->adjustSize();
    this->updateGeometry();
}

void StandardImageWindow::copycrop() {
    const Image* oldImg = _displayImg;
    Image* newImg = oldImg->crop(_imageView->getRectangle());
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, newImg);
    emit addImage(this, newImgWnd);
}

void StandardImageWindow::convertToGrayscale() {
    GrayscaleImage* newImg = RgbToGrayscale()(Converter<RgbImage>::convert(*_displayImg));
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, newImg);

    emit addImage(this, newImgWnd);
}


void StandardImageWindow::convertToBinary() {
    GrayscaleImage* newImg = Otsu()(Converter<GrayscaleImage>::convert(*_displayImg));
    StandardImageWindow* newImgWnd = new StandardImageWindow(*this, newImg);

    emit addImage(this, newImgWnd);
}

void StandardImageWindow::showSelectedPixelInformations(int x, int y) const
{
    _lSelectedPixelPosition->setText(QString("%1x%2").arg(x).arg(y));
    _lSelectedPixelColor->setText(tr("Color") + " : ");
    for(unsigned int i = 0; i < _displayImg->getNbChannels(); i++)
    {
        try {
            _lSelectedPixelColor->setText(_lSelectedPixelColor->text() + QString(" %1").arg(_displayImg->getPixel(x, y, i)) );
        }
        catch(std::out_of_range&) {
        }
    }
}

void StandardImageWindow::showHoveredPixelInformations(int x, int y) const
{
    _lHoveredPixelPosition->setText(QString("%1x%2").arg(x).arg(y));

    _lHoveredPixelColor->setText(tr("Color") + " :");
    for(unsigned int i = 0; i < _displayImg->getNbChannels(); i++)
    {
        try {
            _lHoveredPixelColor->setText(_lHoveredPixelColor->text() + QString(" %1").arg(_displayImg->getPixel(x, y, i)) );
        }
        catch(std::out_of_range&) {
        }
    }
}

void StandardImageWindow::updateSrc(GenericHistogramView* histo, imagein::Rectangle rect) {
    histo->update(_displayImg, rect);
}
