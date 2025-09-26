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

#include "HistogramWindow.h"
#include "Converter.h"

using namespace genericinterface;
using namespace imagein;

HistogramWindow::HistogramWindow(const Image* image, imagein::Rectangle rect, QString name, bool cumul)
    : GenericHistogramWindow(new HistogramView(image, rect, cumul))
{
    _image = Converter<Image_t<double>>::convert(*image);
    if(!cumul) {
        this->setWindowTitle(name + QString(" - ") + tr("Histogram"));
    }
    else {
        this->setWindowTitle(name + QString(" - ") + tr("Cumulative histogram normalized"));
    }
}

HistogramWindow::HistogramWindow(const ImageDouble *image, Rectangle rect, QString name, bool cumul)
    : GenericHistogramWindow(new HistogramView(image, rect, cumul))
{
    _image = image;
    if(!cumul) {
        this->setWindowTitle(name + QString(" - ") + tr("Histogram"));
    }
    else {
        this->setWindowTitle(name + QString(" - ") + tr("Cumulative histogram normalized"));
    }
}

HistogramWindow::HistogramWindow(const ImageDouble *image, Rectangle rect, double binSize, QString name, bool cumul)
    : GenericHistogramWindow(new HistogramView(image, rect, binSize, cumul))
{
    _image = image;
    if(!cumul) {
        this->setWindowTitle(name + QString(" - ") + tr("Histogram"));
    }
    else {
        this->setWindowTitle(name + QString(" - ") + tr("Cumulative histogram normalized"));
    }
}

HistogramWindow::~HistogramWindow()
{
}
