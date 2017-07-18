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

#include "ColumnView.h"
#include <qwt_plot.h>

using namespace genericinterface;
using namespace imagein;

ColumnView::ColumnView(const Image* image, Rectangle rect, int value): GenericHistogramView(image, rect, false, value, false, false, false, true)
{
        _qwtPlot->setTitle(tr("Column Profile Histogram for column number %1").arg(value));
        _qwtPlot->setAxisTitle(QwtPlot::yLeft, tr("Pixel Value"));
        _qwtPlot->setAxisTitle(QwtPlot::xBottom, tr("Line"));
        _qwtPlot->setAxisScale(QwtPlot::xBottom, 0.0, image->getHeight());
        _qwtPlot->setAxisScale(QwtPlot::yLeft, 0.0, 257.0);
}
