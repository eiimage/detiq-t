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

//#include "ColumnProfile.h"

template <typename D>
imagein::ColumnProfile_t<D>::ColumnProfile_t(const Image_t<D>& img, int value, const Rectangle& rect, unsigned int channel) {
    // We recreate the Array depending on the orientation of the projection and the size of the cropped Image
    _width = rect.h;
    delete [] _array;
    _array = new unsigned int[_width];
    // We crop the Image to the Rectangle given in parameter
    Image_t<D>* workImg = img.crop(rect);
    // We prepare to iterate through the Image, we get the max width and height now to avoid to calculate it every iteration
    for(unsigned int i = 0; i < workImg->getHeight(); ++i) {

        int pixValue = workImg->getPixel(value, i, channel);
        _array[i]=pixValue;

    }
    delete workImg;
}

template <typename D>
imagein::ColumnProfile_t<D>::ColumnProfile_t(const Image_t<D>& img, D value, unsigned int channel) {
	// We recreate the Array depending on the orientation of the projection and the size of the cropped Image
    _width = img.getHeight();
    delete [] _array;
    _array = new unsigned int[_width];
    // We prepare to iterate through the Image, we get the max width and height now to avoid to calculate it every iteration
   for(unsigned int i = 0; i < img->getHeight(); ++i)
   {
       int pixValue = img->getPixel(value, i, channel);
       _array[i]=pixValue;
   }

}
