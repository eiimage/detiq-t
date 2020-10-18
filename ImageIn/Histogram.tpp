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

template <typename D>
imagein::Histogram::Histogram(const imagein::Image_t<D>& img, unsigned int channel, double binSize, const imagein::Rectangle& rect)
    : imagein::Array<unsigned int>((int)ceil((img.max(channel)-img.min(channel))/binSize))
{
    this->computeDoubleHistogram(img, channel, binSize, rect);
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

template<typename D>
void imagein::Histogram::computeDoubleHistogram(const Image_t<D>& img, unsigned int channel, double binSize, const Rectangle& rect)
{
    /*Traversing the entire image to find the extreme values*/
    _vmin=img.min(channel);
    _vmax=img.max(channel);
    /*The length of array is determined by the extreme values and interval length*/
    int size = (int)ceil((_vmax-_vmin)/binSize);
    for(int i=0; i < size; i++) {
        _array[i] = 0;
    }
    unsigned int maxw = rect.w > 0 ? rect.x+rect.w : img.getWidth();
    unsigned int maxh = rect.h > 0 ? rect.y+rect.h : img.getHeight();
    /*Traversing the image again to find the corresponding position of each pixel value in the array*/
    for(unsigned int i=rect.y; i<maxh; i++) {
        for(unsigned int j=rect.x; j<maxw; j++) {
            double pixel = img.getPixel(j, i, channel);
            int pos = (int)floor((pixel - _vmin)/binSize);
            if (pos >= size){
                pos = size-1;
            }
            _array[pos]++;
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

template <typename D>
imagein::CumulatedHistogram::CumulatedHistogram(const imagein::Image_t<D>& img, unsigned int channel, double binSize, const imagein::Rectangle& rect)
    : imagein::Array<double>((int)ceil((img.max(channel)-img.min(channel))/binSize))
{
    this->computeDoubleHistogram(img, channel, binSize, rect);
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

template<typename D>
void imagein::CumulatedHistogram::computeDoubleHistogram(const Image_t<D>& img, unsigned int channel, double binSize, const Rectangle& rect)
{
    Histogram histo = imagein::Histogram(img, channel, binSize, rect);
    _vmin = histo.getvMin();
    _vmax = histo.getvMax();

    double cumul = 0.0;
    double imgSize = img.getWidth() * img.getHeight();
    for(unsigned int i=0; i<histo.size(); ++i) {
        _array[i] = cumul;
    }
    /*The cumulative histogram is no longer a number of occupation but a percentage of the full image*/
    for(unsigned int i=0; i<histo.size(); ++i) {
        cumul += histo[i] / imgSize;
        _array[i] = cumul;
    }
}

