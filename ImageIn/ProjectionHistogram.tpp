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

//#include "ProjectionHistogram.h"

template <typename D>
imagein::ProjectionHistogram_t<D>::ProjectionHistogram_t(const Image_t<D>& img, D value, bool horizontal, const Rectangle& rect, unsigned int channel) {
    // We recreate the Array depending on the orientation of the projection and the size of the cropped Image
    if(horizontal) _width = rect.h;
    else _width = rect.w;
    delete [] _array;
    _array = new unsigned int[_width];
    _min = 0;
    _max = _width - 1;
    //_array initialization
    for(int i = 0; i<_width; i++){
        _array[i] = 0;
    }
    // We crop the Image to the Rectangle given in parameter
    Image_t<D>* workImg = img.crop(rect);
    // We prepare to iterate through the Image, we get the max width and height now to avoid to calculate it every iteration
    unsigned int wmax, hmax, iterw, iterh;
    wmax = workImg->getWidth();
    hmax = workImg->getHeight();
    if (horizontal){
        for(iterh=0;iterh<hmax;iterh++) {
            int numberPixel = 0;
            for(iterw=0;iterw<wmax;iterw++) {
                if(workImg->getPixel(iterw, iterh, channel)>=value) {
                    numberPixel++;

                }
            }
            _array[iterh] = numberPixel;
        }
    }else{

        for(iterw=0;iterw<wmax;iterw++) {
            int numberPixel = 0;
            for(iterh=0;iterh<hmax;iterh++) {
                if(workImg->getPixel(iterw, iterh, channel)>=value) {
                    numberPixel++;
                }
            }
            _array[iterw]=numberPixel;
        }
    }
	delete workImg;
}

template <typename D>
imagein::ProjectionHistogram_t<D>::ProjectionHistogram_t(const Image_t<D>& img, D value, bool horizontal, unsigned int channel) {
	// We recreate the Array depending on the orientation of the projection and the size of the cropped Image
    if(horizontal) _width = img.getHeight();
    else _width = img.getWidth();
    delete [] _array;
    _min = 0;
    _max = _width - 1;
    _array = new unsigned int[_width];
    //_array initialization
    for(int i = 0; i<_width; i++){
        _array[i] = 0;
    }
    // We prepare to iterate through the Image, we get the max width and height now to avoid to calculate it every iteration
    unsigned int wmax, hmax, iterw, iterh;
    wmax = img.getWidth();
    hmax = img.getHeight();
    if (!horizontal){
        for(iterh=0; iterh<hmax; iterh++) {
            int numberPixel = 0;
            for(iterw=0; iterw<wmax; iterw++){
                if(img.getPixel(iterh, iterw, channel)>=value) {
                    numberPixel++;
                }
            }
            _array[iterh] = numberPixel;
        }
    }else {
       for(iterw=0;iterw<wmax;iterw++) {
           int numberPixel = 0;
           for(iterh=0;iterh<hmax;iterh++) {
               if(img.getPixel(iterh, iterw, channel)>=value) {
                    numberPixel++;
               }
           }
           _array[iterw]=numberPixel;
        }
    }
}
