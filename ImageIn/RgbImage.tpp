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

//#include "RgbImage.h"

#include "BadImageException.h"

template <typename D>
imagein::RgbImage_t<D>::RgbImage_t(std::string filename)
 : imagein::Image_t<D>(filename)
{
    if(this->_nChannels != 3) 
        throw BadImageException("Image with 3 channels expected", __LINE__, __FILE__);
}

template <typename D>
imagein::RgbImage_t<D>* imagein::RgbImage_t<D>::crop(const Rectangle& rect) const
{
    RgbImage_t<D>* ret = new RgbImage_t<D>(rect.w, rect.h);

    Image_t<D>::crop(rect, ret->_mat);

    return ret;
}
