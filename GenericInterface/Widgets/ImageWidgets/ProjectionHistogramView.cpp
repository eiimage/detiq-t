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

#include "ProjectionHistogramView.h"
#include <qwt_plot.h>

using namespace genericinterface;
using namespace imagein;

ProjectionHistogramView::ProjectionHistogramView(const Image* image, Rectangle rect, int value, bool horizontal): GenericHistogramView(image, rect, horizontal, value, true)
{
    if (horizontal){
        _qwtPlot->setTitle(tr("Horizontal Projection Histogram"));
        _qwtPlot->setAxisTitle(QwtPlot::xBottom, tr("Line number"));
        _qwtPlot->setAxisScale(QwtPlot::xBottom, 0.0, image->getHeight());
    }else{
        _qwtPlot->setTitle(tr("Vertical Projection Histogram"));
        _qwtPlot->setAxisTitle(QwtPlot::xBottom, tr("Column number"));
        _qwtPlot->setAxisScale(QwtPlot::xBottom, 0.0, image->getWidth());
    }
    _qwtPlot->setAxisTitle(QwtPlot::yLeft, tr("Number of pixels >= %1").arg(value));

}

