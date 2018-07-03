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

#include "GraphicalHistogram.h"
#include <iostream>


using namespace genericinterface;

GraphicalHistogram::GraphicalHistogram(const QString& title, const QColor& color): QwtPlotHistogram(title)
{
    setStyle(QwtPlotHistogram::Outline);
    
    QColor c = color;
    c.setAlpha(48);
    setBrush(QBrush(c));
    setPen(QPen(color));

    QwtColumnSymbol* symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
    symbol->setFrameStyle(QwtColumnSymbol::Raised);
    symbol->setLineWidth(2);
    symbol->setPalette(QPalette(color));
    setSymbol(symbol);
}

void GraphicalHistogram::setValues(const imagein::Array<unsigned int>& values)
{   
    QVector<QwtIntervalSample> samples(values.size());
    for (int i = values.getMin(); i <= values.getMax(); ++i)
    {
        unsigned int j = i  - values.getMin();
        QwtInterval interval(double(i), double(i + 1.0));
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);

        samples[j] = QwtIntervalSample(values[i], interval);
    }
    setData(new QwtIntervalSeriesData(samples));
}

void GraphicalHistogram::setValues(const imagein::Array<double>& values)
{
    QVector<QwtIntervalSample> samples(values.size());
    for (int i = values.getMin(); i <= values.getMax(); ++i)
    {
        unsigned int j = i  - values.getMin();
        QwtInterval interval(double(i), double(i + 1.0));
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);

        samples[j] = QwtIntervalSample(values[i], interval);
    }
    setData(new QwtIntervalSeriesData(samples));
}

QwtIntervalSeriesData GraphicalHistogram::getValues(const imagein::Array<double>& values)
{
    QVector<QwtIntervalSample> samples(values.size());
    for (int i = values.getMin(); i <= values.getMax(); ++i)
    {
        unsigned int j = i  - values.getMin();
        QwtInterval interval(double(i), double(i + 1.0));
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);

        samples[j] = QwtIntervalSample(values[i], interval);
    }
   return QwtIntervalSeriesData(samples);
}
