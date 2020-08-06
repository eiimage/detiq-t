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

#include "../../GenericInterface.h"
#include "../../Services/FileService.h"

#include <qpen.h>
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_histogram.h>
#include <qwt_legend.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include "GenericHistogramWindow.h"
#include <UnknownFormatException.h>
using namespace genericinterface;
using namespace imagein;

GenericHistogramWindow::GenericHistogramWindow(GenericHistogramView* view) : _view(view)
{
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(_view);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setAlignment(Qt::AlignCenter);

    /*Create a zoom widget that is activated by clicking the magnifier icon*/
    _zoomer = new QwtPlotZoomer(_view->getGraphicalHistogram()->canvas());
    _zoomer->setRubberBandPen( QColor( Qt::black ) );
    _zoomer->setTrackerPen( QColor( Qt::black ) );
    /*Left-click the selected part to zoom in, right-click to zoom out one step, CTRL & right-click to return to the original size*/
    _zoomer->setMousePattern( QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    _zoomer->setMousePattern( QwtEventPattern::MouseSelect3,Qt::RightButton );
    /*Initialize the zoom stack to 0 and set the original rectangle as base*/
    _zoomer->setZoomBase(true);
    /*Every execution of zoom action call resetZoomer function*/
    QObject::connect(_zoomer,SIGNAL(zoomed(const QRectF&)),this, SLOT(resetZoomer(const QRectF&)));
    _zoomer->setEnabled(false);

    initStatusBar();
    /*Redesign the layout of buttons, pack the zoom button and save button on the right side*/
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(_view->getGraphicalHistogram());

    _zoomButton = new QToolButton(this);
    _zoomButton->setToolTip(tr("Zoom +/-"));
    _zoomButton->setIcon(QIcon(":/images/zoom-in.svg.png"));
    _zoomButton->setCheckable(true);
    _zoomButton->setAutoRaise(true);
    _zoomButton->setIconSize (QSize(24, 24));
    QObject::connect(_zoomButton, SIGNAL(clicked()), this, SLOT(zoom()));

    _saveAsButton = new QToolButton(this);
    _saveAsButton->setToolTip(tr("Save As Image"));
    _saveAsButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
    _saveAsButton->setCheckable(false);
    _saveAsButton->setAutoRaise(true);
    _saveAsButton->setIconSize (QSize(24, 24));
    QObject::connect(_saveAsButton, SIGNAL(clicked()), this, SLOT(saveAs()));

    QHBoxLayout* layoutH = new QHBoxLayout();
    layoutH->addWidget(_statusBar);
    layoutH->addWidget(_zoomButton);
    layoutH->addWidget(_saveAsButton);
    QWidget* widgetH = new QWidget();
    widgetH->setLayout(layoutH);
    layout->addWidget(widgetH);
    this->setLayout(layout);

    connect(_view, SIGNAL(leftClickedValue(int, std::vector<int>)), this, SLOT(showLeftClickedValue(int, std::vector<int>)));
    connect(_view, SIGNAL(rightClickedValue(int, std::vector<int>)), this, SLOT(showRightClickedValue(int, std::vector<int>)));
    connect(_view, SIGNAL(hoveredValue(int, std::vector<int>)), this, SLOT(showHoveredValue(int, std::vector<int>)));
}

GenericHistogramWindow::~GenericHistogramWindow()
{
    delete _view;
}

void GenericHistogramWindow::activated()
{
    emit(selectRectChange(_view->getApplicationArea(), _view));
}

void GenericHistogramWindow::initStatusBar()
{
	QFont font;
    _statusBar = new QStatusBar();
    
    /*_lImageName = new QLabel(QString::fromStdString("Image: ") + ImageWindow::getTitleFromPath(_path));
    font = _lImageName->font();
    font.setPointSize(8);
    font.setBold(true);
    _lImageName->setFont(font);*/
    
    _lHoveredValue = new QLabel(tr("Hovered") + " : ");
    font = _lHoveredValue->font();
    font.setPointSize(8);
    _lHoveredValue->setFont(font);
    
//    _lSelectedValue1 = new QLabel(tr("Value 1") + " : ");
    _lSelectedValue1 = new QLabel(tr("L click value") + " : ");
    font = _lSelectedValue1->font();
    font.setPointSize(8);
    _lSelectedValue1->setFont(font);
    
//    _lSelectedValue2 = new QLabel(tr("Value 2") + " : ");
    _lSelectedValue2 = new QLabel(tr("R click value") + " : ");
    font = _lSelectedValue2->font();
    font.setPointSize(8);
    _lSelectedValue2->setFont(font);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
    QWidget* widget = new QWidget();
	
	QHBoxLayout* layout1 = new QHBoxLayout();
	layout1->setContentsMargins(0, 0, 0, 0);
    QWidget* widget1 = new QWidget();
    layout1->addSpacing(8);
	layout1->addWidget(_lHoveredValue);
	
	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->setContentsMargins(0, 0, 0, 0);
    QWidget* widget2 = new QWidget();
    layout2->addSpacing(8);
	layout2->addWidget(_lSelectedValue1);

    QHBoxLayout* layout3 = new QHBoxLayout();
    layout3->setContentsMargins(0, 0, 0, 0);
    QWidget* widget3 = new QWidget();
    layout3->addSpacing(8);
    layout3->addWidget(_lSelectedValue2);

    widget1->setLayout(layout1);
    widget2->setLayout(layout2);
    widget3->setLayout(layout3);
    layout->addWidget(widget1);
    layout->addWidget(widget2);
    layout->addWidget(widget3);
    
    if(_view->isDouble()){
        _lBinSize = new QLabel(tr("Bin size") + " : ");
        font = _lBinSize->font();
        font.setPointSize(8);
        _lBinSize->setFont(font);

        _binSizeInput = new QLineEdit();
         /*The default value of bin size is 1*/
        _binSizeInput->setText("1.0");
        _inputValidator = new MyDoubleValidator(_view->getMaxBinSize()/1000.0, ceil(_view->getMaxBinSize()), 5, _binSizeInput);
        _inputValidator->setNotation(QDoubleValidator::StandardNotation);
        _binSizeInput->setValidator(_inputValidator);
        _button = new QPushButton(this);
        _button->setText(tr("Enter"));
        QObject::connect(_button, SIGNAL(clicked()), this, SLOT(customizeBinsize()));

        QHBoxLayout* layout4 = new QHBoxLayout();
        layout4->setContentsMargins(0, 0, 0, 0);
        QWidget* widget4 = new QWidget();
        layout4->addSpacing(8);
        layout4->addWidget(_lBinSize);
        layout4->addWidget(_binSizeInput);
        layout4->addWidget(_button);
        widget4->setLayout(layout4);
        layout->addWidget(widget4);
    }

    widget->setLayout(layout);
    _statusBar->addWidget(widget);
}


void GenericHistogramWindow::save(const QString& path, const QString& ext)
{
    if(path == "") {
        this->saveAs();
    }
    else {
        try {
           // WindowService* ws = _gi->windowService();
           // if(ws != NULL) {

                if(this != NULL) {
                    try {
                      //  QString name= path.toStdString();
                        QPixmap pixmap = QPixmap::grabWidget(this);
                        pixmap.save(path, "PNG");



                        //_view->getGraphicalHistogram()->save(path.toStdString());
                    }
                    catch(const UnknownFormatException& e) {
                        if(ext == "")
                            throw e;
                        QPixmap pix = QPixmap::grabWidget(_view);
                        pix.save((path+ext/*.toStdString()*/));
                    }
                }
                else {
                    QMessageBox::critical(this, tr("Bad object type"), tr("Only images can be saved to a file."));
                }
           // }
        }
        catch(const char* s) {
            QMessageBox::information(this, tr("Unknown exception"), s);
        }
    }
}

void GenericHistogramWindow::saveAs()
{
    QString path;
    //WindowService* ws = _gi->windowService();
    //ImageWindow* currentWindow = ws->getCurrentImageWindow();
    if(this != NULL) {

        path="histogram";
    }
    QString selectedFilter;
    QString file = QFileDialog::getSaveFileName(this, tr("Save a file"), path, tr("PNG image (*.png);;BMP image (*.bmp);; JPEG image(*.jpg *.jpeg);; VFF image (*.vff)"), &selectedFilter);

    QString ext = selectedFilter.right(5).left(4);

    if(file != "") {
        if(!file.contains('.')) file += ext;
        this->save(file, ext);
    }
}
/*
void GenericHistogramWindow::saveData()
{
    int i;
    for (i =0 ; i<_view->getApplicationArea()->right;i++){
        std::vector<int> data=_view->getValues(i);
        FILE *fichier;


        fichier = fopen ("tableau.txt", "wb");
        fwrite (data, sizeof data, sizeof *data, fichier);
        fclose (fichier);

    }
}


void GenericHistogramWindow::saveData()
{
    QString chemin, texte;

        while((chemin = QInputDialog::getText(NULL,"Fichier","Quel est le chemin du fichier ?")).isEmpty())
            QMessageBox::critical(NULL,"Erreur","Aucun chemin n'a été spécifié !");

        while((texte = QInputDialog::getText(NULL, "Texte", "Que voulez-vous écrire dans "+chemin.toLatin1())).isEmpty())
            QMessageBox::critical(NULL,"Erreur","Aucun texte n'a été spécifié !");

        QFile fichier(chemin);
        fichier.open(QIODevice::WriteOnly | QIODevice::Text);

        QTextStream flux(&fichier);
        flux << _view->getGraphicalHistogram().dat;

        fichier.close();
}
*/
void GenericHistogramWindow::showHoveredValue(int index, std::vector<int> values) const
{
	_lHoveredValue->setText(tr("Hovered") + QString(" : %1\t").arg(index) + formatValues(values));
}

void GenericHistogramWindow::showLeftClickedValue(int index, std::vector<int> values) const
{
//	_lSelectedValue1->setText(tr("Value 1") + QString(" : %1\t").arg(index) + formatValues(values));
    _lSelectedValue1->setText(tr("L click value") + QString(" : %1\t").arg(index) + formatValues(values));
}

void GenericHistogramWindow::showRightClickedValue(int index, std::vector<int> values) const
{
//	_lSelectedValue2->setText(tr("Value 2") + QString(" : %1\t").arg(index) + formatValues(values));
    _lSelectedValue2->setText(tr("R click value") + QString(" : %1\t").arg(index) + formatValues(values));
}

QString GenericHistogramWindow::formatValues(std::vector<int> values) const
{
	QString res;

    switch(values.size()) {
        case 1 : res = tr("C: %1"); break;
        case 2 : res = tr("C: %1, A: %2"); break;
        case 3 : res = tr("R: %1, G: %2, B: %3"); break;
        default: res = tr("R: %1, G: %2, B: %3, A: %4");
    }

    for(unsigned int i = 0; i<values.size() && i<4; ++i) {
        res = res.arg(values[i]);
    }

	return res;
}

/*Enable / disable the zoom widget according to the state of zoom button*/
void GenericHistogramWindow::zoom() {
    if(_zoomButton->isChecked()){
        _zoomer->setEnabled(true);
    }else{
        _zoomer->setEnabled(false);
    }
}

void GenericHistogramWindow::resetZoomer(const QRectF &rect)
{
    /*Check the depth of zoomer, if index back to 0, need to redraw the axis to prevent incorrect scaling*/
    if (_zoomer->zoomRectIndex()==0){
        _view->getGraphicalHistogram()->setAxisAutoScale(QwtPlot::xBottom);
        _view->getGraphicalHistogram()->setAxisAutoScale(QwtPlot::yLeft);
    }
    _view->getGraphicalHistogram()->replot();
}

void GenericHistogramWindow::customizeBinsize()
{
    if(_binSizeInput->text().length()!=0){
        emit sendBinSize(_binSizeInput->text().toDouble());
    }
}

void GenericHistogramWindow::updateViewByBinSize(const imagein::ImageDouble* image, double binSize)
{
    _view->updateByBinSize(image, binSize);
}

void GenericHistogramWindow::updateCumulativeViewByBinSize(const imagein::ImageDouble* image, double binSize)
{
    _view->updateCumulativeByBinSize(image, binSize);
}
