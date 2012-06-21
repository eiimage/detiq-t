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

#include "GenericHistogramView.h"

using namespace genericinterface;
using namespace imagein;

GenericHistogramView::GenericHistogramView(const Image* image, imagein::Rectangle rect, bool horizontal, int value, bool projection): _rectangle(rect), _horizontal(horizontal), _value(value), _projection(projection)
{
	_qwtPlot = new QwtPlot();
	init(image);
}

GenericHistogramView::~GenericHistogramView()
{
	delete _principalPicker;
	delete _leftPicker;
	delete _rightPicker;
}

void GenericHistogramView::init(const imagein::Image* image)
{
  this->setMouseTracking(true); //Switch on mouse tracking (no need to press button)

  _qwtPlot->setTitle("Histogram");
  
  _qwtPlot->setCanvasBackground(QColor(255,255,255));
  _qwtPlot->plotLayout()->setAlignCanvasToScales(true);

  _qwtPlot->setAxisTitle(QwtPlot::yLeft, "Number of specimen");
  _qwtPlot->setAxisTitle(QwtPlot::xBottom, "Pixel value");
  _qwtPlot->setAxisScale(QwtPlot::xBottom, 0.0, 256);

	QwtLegend* legend = new QwtLegend();
  legend->setItemMode(QwtLegend::CheckableItem);
  _qwtPlot->insertLegend(legend, QwtPlot::RightLegend);

  populate(image);
  
  _qwtPlot->canvas()->setMouseTracking(true);
  
  if(_horizontal)
  _principalPicker = new HistogramPicker(QwtPlotPicker::HLineRubberBand, QwtPicker::AlwaysOn, _qwtPlot->canvas());
  else
  _principalPicker = new HistogramPicker(QwtPlotPicker::VLineRubberBand, QwtPicker::AlwaysOn, _qwtPlot->canvas());
  _principalPicker->setStateMachine(new QwtPickerDragPointMachine());
  _principalPicker->setTrackerPen(QColor(Qt::white));
  _principalPicker->setRubberBandPen(QColor(Qt::yellow));
  
  _leftPicker = new HistogramPicker(_qwtPlot->canvas());
  _leftPicker->setStateMachine(new QwtPickerDragPointMachine());
  
  _rightPicker = new HistogramPicker(_qwtPlot->canvas());
  _rightPicker->setStateMachine(new QwtPickerDragPointMachine());
  _rightPicker->setRubberBand(QwtPlotPicker::VLineRubberBand);
  _rightPicker->setRubberBandPen(QColor(Qt::yellow));
	_rightPicker->setMousePattern(QwtPicker::MouseSelect1, Qt::RightButton);

  connect(_qwtPlot, SIGNAL(legendChecked(QwtPlotItem*, bool)), this, SLOT(showItem(QwtPlotItem*, bool)));
  connect(_rightPicker, SIGNAL(selected(const QPointF&)), this, SLOT(rightClick(const QPointF&)));
  connect(_leftPicker, SIGNAL(selected(const QPointF&)), this, SLOT(leftClick(const QPointF&)));
  connect(_principalPicker, SIGNAL(moved(const QPointF&)), this, SLOT(move(const QPointF&)));

  _qwtPlot->replot(); // creating the legend items

  QwtPlotItemList items = _qwtPlot->itemList(QwtPlotItem::Rtti_PlotHistogram);
  for(int i = 0; i < items.size(); i++)
  {
      QwtLegendItem* legendItem = qobject_cast<QwtLegendItem*>(legend->find(items[i]));
      if(legendItem)
    legendItem->setChecked(true);

  items[i]->setVisible(true);
  }

  _qwtPlot->setAutoReplot(true);
}

void GenericHistogramView::populate(const imagein::Image* image)
{
  QwtPlotGrid* grid = new QwtPlotGrid();
  grid->enableX(false);
  grid->enableY(true);
  grid->enableXMin(false);
  grid->enableYMin(false);
  grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
  grid->attach(_qwtPlot);

	for(unsigned int i = 0; i < image->getNbChannels(); ++i)
	{
		/*imagein::Array<unsigned int>* histogram;
		if(_projection)
			histogram = new imagein::ProjectionHistogram(*image, _value, _horizontal, _rectangle, i);
		else
			histogram = new imagein::Histogram(*image, i, _rectangle);*/
		
		/*int values[histogram->getWidth()];

		for(unsigned int j = 0; j < histogram->getWidth(); ++j)
			values[j] = (*histogram)[j];*/
		
		GraphicalHistogram* graphicalHisto;
		switch(i)
		{
			case 0:
				if(image->getNbChannels() == 1 || image->getNbChannels() == 2)
					graphicalHisto = new GraphicalHistogram("Black", Qt::black);
				else
					graphicalHisto = new GraphicalHistogram("Red", Qt::red);
			break;
			case 1:
				if(image->getNbChannels() == 1 || image->getNbChannels() == 2)
					graphicalHisto = new GraphicalHistogram("Alpha", Qt::white);
				else
					graphicalHisto = new GraphicalHistogram("Green", Qt::green);
			break;
			case 2:
				graphicalHisto = new GraphicalHistogram("Blue", Qt::blue);
			break;
			case 3:
				graphicalHisto = new GraphicalHistogram("Alpha", Qt::black);
			break;
			default:
				graphicalHisto = new GraphicalHistogram("Default", Qt::black);
		}
		//graphicalHisto->setValues(sizeof(values) / sizeof(int), values);
		//graphicalHisto->setValues(histogram);
        
		if(_projection)
            graphicalHisto->setValues(imagein::ProjectionHistogram(*image, _value, _horizontal, _rectangle, i));
		else
            graphicalHisto->setValues(imagein::Histogram(*image, i, _rectangle));
		if(_horizontal)
			graphicalHisto->setOrientation(Qt::Horizontal);
		graphicalHisto->attach(_qwtPlot);
    _graphicalHistos.push_back(graphicalHisto);
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

void GenericHistogramView::showItem(QwtPlotItem *item, bool on) const
{
  item->setVisible(on);
}


std::vector<int> GenericHistogramView::getValues(unsigned int index) const {
    std::vector<int> values;
    for(unsigned int i =0; i < _graphicalHistos.size(); ++i) {
        const QwtSeriesData<QwtIntervalSample> *data = _graphicalHistos[i]->data();
        if(index < data->size()) {
            values.push_back(static_cast<int>(data->sample(index).value));
        }
    }
    return values;
}

void GenericHistogramView::leftClick(const QPointF& pos) const
{
    if(pos.x() < 0) return;
    unsigned int index = static_cast<unsigned int>(pos.toPoint().x());
    std::vector<int> values = getValues(index);
    if(values.size() == _graphicalHistos.size()) {
        emit(leftClickedValue(index, values));
    }
}

void GenericHistogramView::rightClick(const QPointF& pos) const
{
    if(pos.x() < 0) return;
    unsigned int index = static_cast<unsigned int>(pos.toPoint().x());
    std::vector<int> values = getValues(index);
    if(values.size() == _graphicalHistos.size()) {
        emit(rightClickedValue(index, values));
    }
}

void GenericHistogramView::move(const QPointF& pos) const
{
    if(pos.x() < 0) return;
    unsigned int index = static_cast<unsigned int>(pos.toPoint().x());
    std::vector<int> values = getValues(index);
    if(values.size() == _graphicalHistos.size()) {
        emit(hoveredValue(index, values));
    }
}
