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

#include "Histogram.h"
#include "Image.h"


#include <cmath>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include <limits>
#include <assert.h>

template <typename D>
imagein::Histogram::Histogram(const imagein::Image_t<D>& img, unsigned int channel, const imagein::Rectangle& rect)
    : imagein::Array<unsigned int>(img.min(channel), img.max(channel))
{
   // assert(std::numeric_limits<D>::is_integer && "Initializing an Histogram from a Image_t<double> is a non-sense!");
    this->computeHistogram(img, channel, rect);
}

template <typename D>
imagein::Histogram::Histogram(const imagein::Image_t<D>& img, const imagein::Rectangle& rect)
    : imagein::Array<unsigned int>(img.min(0), img.max(0))
{
 //   assert(std::numeric_limits<D>::is_integer && "Initializing an Histogram from a Image_t<double> is a non-sense!");
    this->computeHistogram(img, 0, rect);
}

template<typename D>
void imagein::Histogram::computeHistogram(const Image_t<D>& img, unsigned int channel, const Rectangle& rect)
{
    for(int i=getMin(); i<=getMax(); i++) {
        _array[i - _min] = 0;
    }
    unsigned int maxw = rect.w > 0 ? rect.x+rect.w : img.getWidth();
    unsigned int maxh = rect.h > 0 ? rect.y+rect.h : img.getHeight();
    for(unsigned int i=rect.y; i<maxh; i++) {
        for(unsigned int j=rect.x; j<maxw; j++) {
            int pixel = img.getPixel(j, i, channel);
            _array[pixel - _min]++;
        }
    }
}

template <typename D>
imagein::CumulatedHistogram::CumulatedHistogram(const imagein::Image_t<D>& img, unsigned int channel, const imagein::Rectangle& rect)
    :imagein::Array<double>(std::min( (int) img.min(channel), 0), std::max( (int) img.max(channel), 255))
{
    this->computeHistogram(img, channel, rect);
}

template <typename D>
imagein::CumulatedHistogram::CumulatedHistogram(const imagein::Image_t<D>& img, const imagein::Rectangle& rect)
    : imagein::Array<double>(img.min(0), img.max(0))
{
    this->computeHistogram(img, 0, rect);
}

template<typename D>
void imagein::CumulatedHistogram::computeHistogram(const Image_t<D>& img, unsigned int channel, const Rectangle& rect)
{
    Histogram histo = img.getHistogram(channel, rect);
    double cumul = 0.;
    double imgSize = img.getWidth() * img.getHeight();
    for(int i=this->getMin(); i<histo.getMin(); i++) {
        _array[i - _min] = cumul;
    }
    for(int i=histo.getMin(); i<=histo.getMax(); i++) {
        cumul += histo[i] / imgSize;
        _array[i - _min] = cumul;
    }
    for(int i=histo.getMax() + 1; i<this->getMax(); i++) {
        _array[i - _min] = cumul;
    }
}
