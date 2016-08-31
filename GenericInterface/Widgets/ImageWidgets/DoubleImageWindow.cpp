/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of DETIQ-T.
 *
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GenericInterface.h"
#include "DoubleImageWindow.h"
#include "GridView.h"
#include <QDoubleSpinBox>
#include <QSlider>

using namespace genericinterface;
using namespace imagein;
using namespace std;

DoubleImageWindow::DoubleImageWindow(Image_t<double>* image, const QString path, bool normalize, bool logScale, double logConstantScale, bool abs)
    : ImageWindow(path), _image(image), _normalize(normalize), _logScale(logScale), _logConstantScale(logConstantScale), _abs(abs)
{
    _image = image;

    this->setDisplayImage(this->makeDisplayable(image));

    this->setWindowTitle(ImageWindow::getTitleFromPath(path));

    init();
}

DoubleImageWindow::DoubleImageWindow(const DoubleImageWindow& siw, imagein::Image_t<double>* image)
    : ImageWindow(siw.getPath()), _normalize(siw._normalize), _logScale(siw._logScale), _logConstantScale(siw._logConstantScale)
{
    if(image == NULL) {
        image = new Image_t<double>(*siw._image);
    }

    _image = image;
    this->setDisplayImage(this->makeDisplayable(image));

    this->setWindowTitle(siw.windowTitle());

    init();
}

DoubleImageWindow::~DoubleImageWindow()
{
    delete _image;
    delete _displayImg;
}


void DoubleImageWindow::init()
{
    QObject::connect(this->view(), SIGNAL(updateSrc(GenericHistogramView*,imagein::Rectangle)), this, SLOT(updateSrc(GenericHistogramView*,imagein::Rectangle)));

    menu()->addAction(tr("Crop"), this, SLOT(crop()));
    menu()->addAction(tr("Copy & crop"), this, SLOT(copycrop()));
    menu()->addAction(tr("Convert to Rgb Image"), this, SLOT(convertRgb()));

    updateStatusBar();

}

void DoubleImageWindow::updateStatusBar()
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
    _lSelectedPixelInfo->hide();
    _lSelectedPixelPosition->hide();
    _lSelectedPixelColor->hide();

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

    if(_logScale) {
        QWidget* logWidget = new QWidget();
        QHBoxLayout* logLayout = new QHBoxLayout(logWidget);
        QSlider* logSlider = new QSlider(Qt::Horizontal);
        logSlider->setRange(0, 12);
        logSlider->setValue( (log2(_logConstantScale) / 3. + 3.) * 2.);
    //    QDoubleSpinBox* logBox = new QDoubleSpinBox();
    //    logBox->setRange(0., 1000000000.);
    //    logBox->setValue(_logConstant);
        QObject::connect(logSlider, SIGNAL(valueChanged(int)), this, SLOT(setLogScale(int)));
        logLayout->addWidget(new QLabel("Log scale : "));

        logLayout->addWidget(logSlider);
        _infoLayout->addWidget(logWidget);
    }

}

void DoubleImageWindow::showPixelsGrid()
{
    GridView* grid = new GridView(_image, _displayImg);
    grid->setWindowTitle(this->windowTitle() + QString(" - ")  + tr("Pixels Grid"));
    emit addWidget(this, grid);
}

void DoubleImageWindow::crop() {
    const Image_t<double>* oldImg = _image;
    const Image* oldDisplayImg = _displayImg;
    Image_t<double>* newImg = oldImg->crop(_imageView->getRectangle());
    this->setDisplayImage(this->makeDisplayable(newImg));
    _image = newImg;
    delete oldImg;
    delete oldDisplayImg;

    view()->update();
    this->adjustSize();
    this->updateGeometry();
}

void DoubleImageWindow::copycrop() {
    Image_t<double>* newImg = _image->crop(_imageView->getRectangle());
    DoubleImageWindow* newImgWnd = new DoubleImageWindow(*this, newImg);
    emit addImage(this, newImgWnd);
}

void DoubleImageWindow::convertRgb() {
    Image* newImg = new Image();
    StandardImageWindow* newImgWnd = new StandardImageWindow(newImg);
    emit addImage(this, newImgWnd);
}

void DoubleImageWindow::showSelectedPixelInformations(int x, int y) const
{
    _lSelectedPixelPosition->setText(QString("%1x%2").arg(x).arg(y));
    _lSelectedPixelColor->setText(tr("Color") + " : ");
    for(unsigned int i = 0; i < _image->getNbChannels(); i++)
    {
        try {
            _lSelectedPixelColor->setText(_lSelectedPixelColor->text() + QString(" %1").arg(_image->getPixel(x, y, i), 0, 'f', 2) );
        }
        catch(std::out_of_range&) {
        }
    }
    _lSelectedPixelInfo->show();
    _lSelectedPixelPosition->show();
    _lSelectedPixelColor->show();
}

void DoubleImageWindow::showHoveredPixelInformations(int x, int y) const
{
    _lHoveredPixelPosition->setText(QString("%1x%2").arg(x).arg(y));

    _lHoveredPixelColor->setText(tr("Color") + " :");
    for(unsigned int i = 0; i < _image->getNbChannels(); i++)
    {
        try {
            _lHoveredPixelColor->setText(_lHoveredPixelColor->text() + QString(" %1").arg(_image->getPixel(x, y, i), 0, 'f', 2) );
        }
        catch(std::out_of_range&) {
        }
    }
}

void DoubleImageWindow::updateSrc(GenericHistogramView* /*histo*/, imagein::Rectangle /*rect*/) {
}

Image* DoubleImageWindow::makeDisplayable(const Image_t<double>* image) {

    Image_t<double>* tmpImg = new Image_t<double>(*image);
    Image* resImg = new Image(image->getWidth(), image->getHeight(), image->getNbChannels());

    if(_abs) {
        for(Image_t<double>::iterator it = tmpImg->begin(); it < tmpImg->end(); ++it) {
            *it = std::abs(*it);
        }
    }
    std::cout << "Before normalize : " << tmpImg->min() << ":" << tmpImg->max() << std::endl;
    if(_normalize) {
        tmpImg->normalize(0.0, 255.0);
    }

    std::cout << "After normalize : " << tmpImg->min() << ":" << tmpImg->max() << std::endl;
    double mean = tmpImg->mean();
    double logConstant = exp(-log2(mean)) / 8.;
    std::cout << "Log constant scale = " << _logConstantScale << std::endl;
    std::cout << "Log constant = " << logConstant << std::endl;
    for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
        const double denom = log(255.0 * logConstant * _logConstantScale + 1.0);
        const double factor = 255.0 / denom;
        for(unsigned int j = 0; j < image->getHeight(); ++j) {
            for(unsigned int i = 0; i < image->getWidth(); ++i) {
                double mag = tmpImg->getPixel(i, j, c);
                if(_logScale) {
                    mag = log(mag*logConstant*_logConstantScale + 1.0) * factor;
                }
                mag = std::min(255.0, std::max(0.0, mag));
                resImg->setPixel(i, j, c, mag);
            }
        }
    }
    return resImg;
}

void DoubleImageWindow::setLogScale(int logScale) {

//    _logConstant = logConstant;
    const Image* tmpImg = _displayImg;
    _logConstantScale = std::pow(8, logScale/2. - 3.);
    setDisplayImage(makeDisplayable(_image));
    delete tmpImg;
}

void DoubleImageWindow::showHistogram()
{
    HistogramWindow* histogramWnd = new HistogramWindow(_image, selection(), this->windowTitle());
    showGenericHistogram(histogramWnd);
}
