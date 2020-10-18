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
#include <QDebug>
#include <QObject>
#include <QTranslator>
#include <QPushButton>
#include <QApplication>
#include <Converter.h>
#include "../../../../../app/Operations/HoughDialog.h"
#include "../../Utilities/Log.h"
#include <UnknownFormatException.h>

using namespace genericinterface;
using namespace imagein;
using namespace std;

DoubleImageWindow::DoubleImageWindow(Image_t<double>* image, const QString path, bool normalize, bool logScale, double logConstantScale, bool abs)
    : ImageWindow(path), _image(image), _normalize(normalize), _logScale(logScale), _logConstantScale(logConstantScale), _abs(abs)
{
    init();
    _image = image;

    this->setDisplayImage(this->makeDisplayable(image));

    this->setWindowTitle(ImageWindow::getTitleFromPath(path));
}

DoubleImageWindow::DoubleImageWindow(const DoubleImageWindow& siw, imagein::Image_t<double>* image)
    : ImageWindow(siw.getPath()), _normalize(siw._normalize), _logScale(siw._logScale), _logConstantScale(siw._logConstantScale)
{
    init();
    if(image == NULL) {
        image = new Image_t<double>(*siw._image);
    }
    _image = image;
    this->setDisplayImage(this->makeDisplayable(image));

    this->setWindowTitle(siw.windowTitle());
}

DoubleImageWindow::~DoubleImageWindow()
{
    delete _image;
    delete _displayImg;
}


void DoubleImageWindow::init()
{
    QObject::connect(this->view(), SIGNAL(updateSrc(GenericHistogramView*,imagein::Rectangle)), this, SLOT(updateSrc(GenericHistogramView*,imagein::Rectangle)));
    /*If there exist negatif value in the image, offset and scaling will be enabled by default*/
    _boxLayout= new QHBoxLayout();
    _offsetBox = new QCheckBox(QObject::tr("Offset (127)"));
    _scalingBox = new QCheckBox(QObject::tr("Scaling"));
    _boxLayout->addWidget(_offsetBox);
    _boxLayout->addWidget(_scalingBox);
    _infoLayout->addLayout(_boxLayout);
    if(this->isOffsetNeeded()){
        _offsetBox->setChecked(true);
        _scalingBox->setChecked(true);
    }
    QObject::connect(_offsetBox,SIGNAL(stateChanged(int)), this, SLOT(offset_scaling(int)));
    QObject::connect(_scalingBox,SIGNAL(stateChanged(int)), this, SLOT(offset_scaling(int)));
    menu()->addAction(QObject::tr("Crop"), this, SLOT(crop()));
    menu()->addAction(QObject::tr("Copy & crop"), this, SLOT(copycrop()));
  //  menu()->addAction(tr("Convert to Rgb Image"), this, SLOT(convertRgb()));
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
    _lSelectedPixelInfo = new QLabel(QObject::tr("Selected") + " : ");
    font = _lSelectedPixelInfo->font();
    font.setPointSize(8);
    font.setBold(true);
    _lSelectedPixelInfo->setFont(font);
    _lSelectedPixelPosition = new QLabel(QString(""));
    font = _lSelectedPixelPosition->font();
    font.setPointSize(8);
    _lSelectedPixelPosition->setFont(font);
    _lSelectedPixelColor = new QLabel(QObject::tr("Color") + " : ");
    font = _lSelectedPixelColor->font();
    font.setPointSize(8);
    _lSelectedPixelColor->setFont(font);

    //Hovered pixel informations
    _lHoveredPixelInfo = new QLabel(QObject::tr("Hovered") + " : ");
    font = _lHoveredPixelInfo->font();
    font.setBold(true);
    font.setPointSize(8);
    _lHoveredPixelInfo->setFont(font);
    _lHoveredPixelPosition = new QLabel(QString(""));
    font = _lHoveredPixelPosition->font();
    font.setPointSize(8);
    _lHoveredPixelPosition->setFont(font);
    _lHoveredPixelColor = new QLabel(QObject::tr("Color") + " : ");
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
    ImageDouble* fakeImg = Converter<ImageDouble>::convert(*_displayImg);
    GridView* grid = new GridView(fakeImg, _displayImg);
//    GridView* grid = new GridView(_image, _displayImg);
    grid->setWindowTitle(this->windowTitle() + QString(" - ")  + QObject::tr("Pixels Grid"));
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
    //teste si l'operation est une transformee de Hough et gere l'affichage de la valeur selectionnee en consequence
    if (_hough)
    {
        double rho= ((double)y - ((double)(_image->getHeight()-1)/2))*distanceStep;
        double theta= ((double)x - ((double)(_image->getWidth()-1)/2))*angleStep;
        _lSelectedPixelPosition->setText(QString("Rho : %1 | Theta : %2").arg(rho).arg(theta));
    }
    else
    {
        _lSelectedPixelPosition->setText(QString("%1x%2").arg(x).arg(y));
    }
    _lSelectedPixelColor->setText(QObject::tr("Color") + " : ");
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
    //teste si l'operation est une transformee de Hough et gere l'affichage de la valeur survolee en consequence
    if (_hough)
    {
        double rho= ((double)y - ((double)(_image->getHeight()-1)/2))*distanceStep;
        double theta= ((double)x - ((double)(_image->getWidth()-1)/2))*angleStep;
        _lHoveredPixelPosition->setText(QString("Rho : %1 | Theta : %2").arg(rho).arg(theta));
    }
    else
    {
        _lHoveredPixelPosition->setText(QString("%1x%2").arg(x).arg(y));
    }

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

    /*Keep using double image during processing steps and cast it to standard image at the end*/
    ImageDouble* tmpImg = new ImageDouble(*image);
    Image* resImg = new Image(tmpImg->getWidth(), tmpImg->getHeight(), tmpImg->getNbChannels());

    /*This truncation message becomes annoying when user slide the log scale bar, better not to emit it*/
    if(!_logScale && (tmpImg->min()<0 || tmpImg->max()>255)){
        outInfo(QObject::tr("The display image is truncated, values outside the range of [0 , 255] have been eliminated\n\n-------------------------------------------"));
    }

    if(_abs) {
        for(Image_t<double>::iterator it = tmpImg->begin(); it < tmpImg->end(); ++it) {
            *it = std::abs(*it);
        }
    }

    /*Normalize option takes no effect when output a double image, since Offset and Scaling are applied by default, they can be managed after by user*/
    if(_normalize) {
//        std::cout << "Before normalize : " << tmpImg->min() << ":" << tmpImg->max() << std::endl;
//        tmpImg->normalize(0.0, 255.0);
//        std::cout << "After normalize : " << tmpImg->min() << ":" << tmpImg->max() << std::endl;
    }

    if(_logScale){
        /*The log scale will not work if mean value too small,
        In case of negatif values, we take use mean of absolute values instead*/
        //    double mean = tmpImg->mean();
        double mean = tmpImg->meanOfAbs();
        double logConstant = exp(-log2(mean)) / 8.;
        double denom = log(255.0 * logConstant * _logConstantScale + 1.0);
        double factor = 255.0 / denom;

        std::cout << "Log constant scale = " << _logConstantScale << std::endl;
        std::cout << "Log constant = " << logConstant << std::endl;
        for(unsigned int c = 0; c < tmpImg->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < tmpImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < tmpImg->getWidth(); ++i) {
                    double mag = tmpImg->getPixel(i, j, c);
                    double newValue = log(mag * logConstant * _logConstantScale + 1.0) * factor;
                    tmpImg->setPixel(i, j, c, newValue);
                }
            }
        }
    }

    /*Offset and Scaling are applied by default if exist value < 0, the result of log scale is inherited*/
//    if(this->isOffsetNeeded()){

//    }else{
//        resImg = disableOffset_Scaling(tmpImg);
//    }
    if(_offsetBox->isChecked() && !_scalingBox->isChecked()){
        resImg = enableOffset(tmpImg);
    }else if(!_offsetBox->isChecked() && _scalingBox->isChecked()){
        resImg = enableScaling(tmpImg);
    }else if(_offsetBox->isChecked() && _scalingBox->isChecked()){
        resImg = enableOffset_Scaling(tmpImg);
    }else{
        resImg = disableOffset_Scaling(tmpImg);
    }

    delete tmpImg;
    return resImg;
}

void DoubleImageWindow::setLogScale(int logScale) {

    const Image* tmpImg = _displayImg;
    _logConstantScale = std::pow(8, logScale/2. - 3.);
    setDisplayImage(makeDisplayable(_image));
    delete tmpImg;
}

void DoubleImageWindow::showHistogram()
{
//    HistogramWindow* histogramWnd = new HistogramWindow(_image, selection(), this->windowTitle());
//    showGenericHistogram(histogramWnd);
    HistogramWindow* histogramWnd = new HistogramWindow(_image, selection(), this->windowTitle());
    /*Modify _binSize after receiving the new value*/
    QObject::connect(histogramWnd, SIGNAL(sendBinSize(double)), this, SLOT(setBinSize(double)));
    showGenericHistogram(histogramWnd);
    /*Created an infinite event loop to wait for the incoming bin size value, the loop breaks when the window is closed*/
    QEventLoop loop;
    while(1){
        QObject::connect(histogramWnd, SIGNAL(sendBinSize(double)), &loop, SLOT(quit()));
        histogramWnd->updateViewByBinSize(_image, _binSize);
        /*Re-initialize after _binSize each execution to ensure that reopening the histogram window will not retain the previous result*/
        _binSize = 1.0;
        loop.exec();
        if(!histogramWnd->isVisible()){
            break;
        }
    }
    histogramWnd->close();
}

void DoubleImageWindow::showCumulativeHistogram()
{
    HistogramWindow* histogramWnd = new HistogramWindow(_image, selection(), _binSize, this->windowTitle(), true);
    /*It shares the same window as histogram, so the signal of bin size is also shared*/
    QObject::connect(histogramWnd, SIGNAL(sendBinSize(double)), this, SLOT(setBinSize(double)));
    showGenericHistogram(histogramWnd);
    QEventLoop loop;
    while(1){
        QObject::connect(histogramWnd, SIGNAL(sendBinSize(double)), &loop, SLOT(quit()));
        histogramWnd->updateCumulativeViewByBinSize(_image, _binSize);
        /*Re-initialize after _binSize each execution to ensure that reopening the histogram window will not retain the previous result*/
        _binSize = 1.0;
        loop.exec();
        if(!histogramWnd->isVisible()){
            break;
        }
    }
    histogramWnd->close();
}

void DoubleImageWindow::offset_scaling(int i){
    setDisplayImage(makeDisplayable(_image));
}

Image* DoubleImageWindow::enableOffset(ImageDouble* imgD){

    int height = imgD->getHeight();
    int width = imgD->getWidth();
    int nbChannels = imgD->getNbChannels();

    Image* offsetImg = new Image(width, height, nbChannels, 0.);
    for(unsigned int c = 0; c < nbChannels; ++c) {
        for(unsigned int i = 0; i < height; ++i){ // on parcourt l'image
            for(unsigned int j = 0; j < width; ++j){
                double oldPixel = imgD->getPixelAt(j,i,c);
                double newPixel = oldPixel + 127;
                newPixel  = std::min(255.0, std::max(0.0, newPixel));
                offsetImg->setPixelAt(j, i, c, newPixel);
            }
        }
    }
    if(!_logScale){
        outInfo(QObject::tr("Offset applied : val_display = val_image + 127\n\n-------------------------------------------"));
    }
    return offsetImg;
}

Image* DoubleImageWindow::enableScaling(ImageDouble* imgD){

    int height = imgD->getHeight();
    int width = imgD->getWidth();
    int nbChannels = imgD->getNbChannels();
    double maxValue = imgD->max();
    Image* scalingImg = new Image(width, height, nbChannels, 0.);

    for(unsigned int c = 0; c < nbChannels; ++c) {
        for(unsigned int i = 0; i < height; ++i){ // on parcourt l'image
            for(unsigned int j = 0; j < width; ++j){
                double oldPixel = imgD->getPixelAt(j,i,c);
                double newPixel = (oldPixel < 0 || maxValue == 0) ? 0 : oldPixel * 255 / maxValue;
                newPixel  = std::min(255.0, std::max(0.0, newPixel));
                scalingImg->setPixelAt(j, i, c, newPixel);
            }
        }
    }
    if(!_logScale){
        outInfo(QObject::tr("Scaling applied : val_display = (val_image < 0 || maxValue == 0) ? 0 : val_image * 255 / maxValue\n\n-------------------------------------------"));
    }
    return scalingImg;
}

Image* DoubleImageWindow::enableOffset_Scaling(ImageDouble* imgD){

    int height = imgD->getHeight();
    int width = imgD->getWidth();
    int nbChannels = imgD->getNbChannels();
    double maxValue = imgD->max();
    double minValue = imgD->min();
    Image* offset_scalingImg = new Image(width, height, nbChannels, 0.);
    for(unsigned int c = 0; c < nbChannels; ++c) {
        for(unsigned int i = 0; i < height; ++i){ // on parcourt l'image
            for(unsigned int j = 0; j < width; ++j){
                double oldPixel = imgD->getPixelAt(j,i,c);
                double newPixel = (127-minValue) > (maxValue-127) ? oldPixel * 127 / (- minValue) + 127 : oldPixel * 128 / maxValue + 127;
                newPixel  = std::min(255.0, std::max(0.0, newPixel));
                offset_scalingImg->setPixelAt(j, i, c, newPixel);
            }
        }
    }
    if(!_logScale){
        outInfo(QObject::tr("Both Offset and Scaling applied : val_display = (127-minValue) > (maxValue-127) ? val_image * 127 / (- minValue) + 127 : val_image * 128 / maxValue + 127\n\n-------------------------------------------"));
    }
    return offset_scalingImg;
}

Image* DoubleImageWindow::disableOffset_Scaling(ImageDouble* imgD){

    int height = imgD->getHeight();
    int width = imgD->getWidth();
    int nbChannels = imgD->getNbChannels();
    Image* resImg = new Image(width, height, nbChannels, 0.);

    for(unsigned int c = 0; c < nbChannels; ++c) {
        for(unsigned int i = 0; i < height; ++i){ // on parcourt l'image
            for(unsigned int j = 0; j < width; ++j){
                double oldPixel = imgD->getPixelAt(j,i,c);
                double newPixel  = std::min(255.0, std::max(0.0, oldPixel));
                resImg->setPixelAt(j, i, c, newPixel);
            }
        }
    }
    return resImg;
}

void DoubleImageWindow::setBinSize(double binSize)
{
    _binSize = binSize;
}

void DoubleImageWindow::outInfo(QString str){
    emit textToShow(str);
}
