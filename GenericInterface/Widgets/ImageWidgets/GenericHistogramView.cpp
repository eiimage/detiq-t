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

#include <qpen.h>
#include <QtMath>

#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_histogram.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>

#include "GenericHistogramView.h"
#include "HistogramPicker.h"
#include "GraphicalHistogram.h"

using namespace genericinterface;
using namespace imagein;

GenericHistogramView::GenericHistogramView(const Image* image, imagein::Rectangle rect, bool horizontal, int value, bool projection, bool cumulated, bool lineProfile, bool columnProfile)
    : _rectangle(rect), _horizontal(horizontal), _value(value), _projection(projection), _cumulated(cumulated), _lineProfile(lineProfile), _columnProfile(columnProfile)
{
    _qwtPlot = new QwtPlot();
    init(image->getNbChannels());

    for(unsigned int i = 0; i < _graphicalHistos.size(); ++i) {
        GraphicalHistogram* graphicalHisto = _graphicalHistos[i];

        if(_projection) {
            graphicalHisto->setValues(imagein::ProjectionHistogram(*image, _value, _horizontal, _rectangle, i));

        } else if(_cumulated) {
            graphicalHisto->setValues(imagein::CumulatedHistogram(*image, i, _rectangle));

        } else if(_lineProfile){

            graphicalHisto->setValues(imagein::LineProfile(*image, _value, _rectangle, i));

        } else if(_columnProfile){

            graphicalHisto->setValues(imagein::ColumnProfile(*image, _value, _rectangle, i));

        }else {
            graphicalHisto->setValues(imagein::Histogram(*image, i, _rectangle));

        }
    }

    // The origin of the histogram will be 0 on the x axis
    _originValue = qFloor(image->min());
}

GenericHistogramView::GenericHistogramView(const ImageDouble *image, Rectangle rect, bool horizontal, int value, bool projection, bool cumulated)
    : _rectangle(rect), _horizontal(horizontal), _value(value), _projection(projection), _cumulated(cumulated)
{
    _qwtPlot = new QwtPlot();

    init(image->getNbChannels());
    // Here we have to fix min and max for the current diagram
    _qwtPlot->setAxisAutoScale(QwtPlot::xBottom);

    for(unsigned int channel = 0; channel < _graphicalHistos.size(); ++channel) {
        GraphicalHistogram* graphicalHisto = _graphicalHistos[channel];

        if(_projection) {
            graphicalHisto->setValues(imagein::ProjectionHistogram_t<double>(*image, _value, _horizontal, _rectangle, channel));
        } else if(_cumulated) {
            qDebug() << "Cumulated Histogram for ImageDouble is unsupported for now";
        } else {
          graphicalHisto->setValues(imagein::Histogram(*image, channel, _rectangle));

        }
    }

    // Store the origin, to apply on indexes passed when cursor moves on histogram or click on it
    _originValue = qFloor(image->min());
}

GenericHistogramView::~GenericHistogramView()
{
    delete _principalPicker;
    delete _leftPicker;
    delete _rightPicker;
}

void GenericHistogramView::init(uint nbChannels)
{

    this->setMouseTracking(true); //Switch on mouse tracking (no need to press button)

    _qwtPlot->setTitle(tr("Histogram"));

    _qwtPlot->setCanvasBackground(QColor(255,255,255));
    _qwtPlot->plotLayout()->setAlignCanvasToScales(true);

    _qwtPlot->setAxisTitle(QwtPlot::yLeft, tr("Number of specimen"));
    _qwtPlot->setAxisTitle(QwtPlot::xBottom, tr("Pixel value"));
   if((!_columnProfile) || (!_lineProfile)){
        _qwtPlot->setAxisScale(QwtPlot::xBottom, 0.0, 256);
    }


    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode(QwtLegendData::Checkable);
    _qwtPlot->insertLegend(legend, QwtPlot::RightLegend);

    _qwtPlot->canvas()->setMouseTracking(true);

    if(_horizontal)
        _principalPicker = new HistogramPicker(QwtPicker::HLineRubberBand, QwtPicker::AlwaysOn, _qwtPlot->canvas());
    else
        _principalPicker = new HistogramPicker(QwtPicker::VLineRubberBand, QwtPicker::AlwaysOn, _qwtPlot->canvas());
    _principalPicker->setStateMachine(new QwtPickerDragPointMachine());
    _principalPicker->setTrackerPen(QColor(Qt::black));
    _principalPicker->setRubberBandPen(QColor(Qt::yellow));

    _leftPicker = new HistogramPicker(QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly, _qwtPlot->canvas());
    _leftPicker->setStateMachine(new QwtPickerDragPointMachine());
    _leftPicker->setRubberBand(QwtPlotPicker::CrossRubberBand);
    _leftPicker->setRubberBandPen(QColor(Qt::yellow));

    _rightPicker = new HistogramPicker(QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly, _qwtPlot->canvas());
    _rightPicker->setStateMachine(new QwtPickerDragPointMachine());
    _rightPicker->setRubberBand(QwtPlotPicker::CrossRubberBand);
    _rightPicker->setRubberBandPen(QColor(Qt::yellow));
    _rightPicker->setMousePattern(QwtPicker::MouseSelect1, Qt::RightButton);

    connect(legend, SIGNAL(checked(QVariant,bool,int)), this, SLOT(showItem(QVariant,bool,int)));
    connect(_rightPicker, SIGNAL(selected(const QPointF&)), this, SLOT(rightClick(const QPointF&)));
    connect(_leftPicker, SIGNAL(selected(const QPointF&)), this, SLOT(leftClick(const QPointF&)));
    connect(_principalPicker, SIGNAL(moved(const QPointF&)), this, SLOT(move(const QPointF&)));

    connect(_leftPicker, SIGNAL(selected(const QPointF&)), this, SIGNAL(leftSelected(const QPointF&)));
    connect(_rightPicker, SIGNAL(selected(const QPointF&)), this, SIGNAL(rightSelected(const QPointF&)));
    connect(_leftPicker, SIGNAL(moved(const QPointF&)), this, SIGNAL(leftMoved(const QPointF&)));
    connect(_rightPicker, SIGNAL(moved(const QPointF&)), this, SIGNAL(rightMoved(const QPointF&)));

    _qwtPlot->replot(); // creating the legend items

    _qwtPlot->setAutoReplot(true);

    QwtPlotGrid* grid = new QwtPlotGrid();
    grid->enableX(false);
    grid->enableY(true);
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->attach(_qwtPlot);

    for(uint i = 0; i < nbChannels; ++i)
    {
        GraphicalHistogram* graphicalHisto;
        switch(i)
        {
        case 0:
            if(nbChannels == 1 || nbChannels == 2)
                graphicalHisto = new GraphicalHistogram(tr("Black"), Qt::black);
            else
                graphicalHisto = new GraphicalHistogram(tr("Red"), Qt::red);
            break;
        case 1:
            if(nbChannels == 1 || nbChannels == 2)
                graphicalHisto = new GraphicalHistogram(tr("Alpha"), Qt::white);
            else
                graphicalHisto = new GraphicalHistogram(tr("Green"), Qt::green);
            break;
        case 2:
            graphicalHisto = new GraphicalHistogram(tr("Blue"), Qt::blue);
            break;
        case 3:
            graphicalHisto = new GraphicalHistogram(tr("Alpha"), Qt::black);
            break;
        default:
            graphicalHisto = new GraphicalHistogram(tr("Channel"), Qt::black);
        }

        /*if(_horizontal) {
            graphicalHisto->setOrientation(Qt::Horizontal);
        }*/
        graphicalHisto->attach(_qwtPlot);
        _graphicalHistos.push_back(graphicalHisto);

        // By default, check each legend entry
        QWidget *legendWidget = legend->legendWidget(_qwtPlot->itemToInfo(graphicalHisto));
        QwtLegendLabel *label = qobject_cast<QwtLegendLabel *>(legendWidget);
        if(label) {
            label->setChecked(true);
        }

    }
}

void GenericHistogramView::update(const imagein::Image* image, imagein::Rectangle rect)
{
    _rectangle = rect;

    emit(updateApplicationArea(rect));

    for(unsigned int i = 0; i < image->getNbChannels(); ++i)
    {
        /*const imagein::Array<unsigned int>& histogram;
        if(_projection)
            histogram = imagein::ProjectionHistogram(*image, _value, _horizontal, _rectangle, i);
        else
            histogram = imagein::Histogram(*image, i, _rectangle);
            */

        /*int values[histogram->getWidth()];

        for(unsigned int j = 0; j < histogram->getWidth(); ++j)
            values[j] = (*histogram)[j];

        _graphicalHistos[i]->setValues(sizeof(values) / sizeof(int), values);*/
        if(_projection)
            _graphicalHistos[i]->setValues(imagein::ProjectionHistogram(*image, _value, _horizontal, _rectangle, i));
        else
            _graphicalHistos[i]->setValues(imagein::Histogram(*image, i, _rectangle));
    }
}

void GenericHistogramView::showItem(QVariant itemInfo, bool visible, int) const
{
    QwtPlotItem* item = _qwtPlot->infoToItem(itemInfo);
    if(item) {
        item->setVisible(visible);
    }
}

std::vector<int> GenericHistogramView::getValues(int index) const {
    std::vector<int> values;

    uint dataIndex = index - _originValue;
    for(unsigned int i = 0; i < _graphicalHistos.size(); ++i) {
        const QwtSeriesData<QwtIntervalSample> *data = _graphicalHistos[i]->data();
        if(dataIndex < data->size()) {
            values.push_back(static_cast<int>(data->sample(dataIndex).value));
        }
    }
    return values;
}

void GenericHistogramView::leftClick(const QPointF& pos) const
{
    int index = qFloor(pos.x());
    std::vector<int> values = getValues(index);
    if(values.size() == _graphicalHistos.size()) {
        emit(leftClickedValue(index, values));
    }
}

void GenericHistogramView::rightClick(const QPointF& pos) const
{
    int index = qFloor(pos.x());
    std::vector<int> values = getValues(index);
    if(values.size() == _graphicalHistos.size()) {
        emit(rightClickedValue(index, values));
    }
}

void GenericHistogramView::move(const QPointF& pos) const
{
    int index = qFloor(pos.x());
    std::vector<int> values = getValues(index);

    if(values.size() == _graphicalHistos.size()) {
        emit(hoveredValue(index, values));
    }
}
