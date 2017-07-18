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

#ifndef COLUMNPROFILE_H
#define COLUMNPROFILE_H


#include "Array.h"
#include "Image.h"
#include <iostream>

namespace imagein
{
    /*!
     * \brief ColumnProfile_t creates the column profile histogram of an Image from a given a value
     *
     * This class is inherited from Array, therefore it's a fixed-size array of integers, the element with rank K of the array is the number of pixels of the given value on the Kth row/column of the Image (cropped from the Rectangle).
     */
	template <typename D>
    class ColumnProfile_t : public Array<unsigned int>
    {
        public:
			/*!
                         * \brief Constructs a ColumnProfile_t from an image.
			 *
			 * \param img The image from which to compute the projection histogram.
			 * \param value The value that will be counted.
			 * \param horizontal The projection histogram will count the pixels of the selected value for each row if true, for each column if false.
			 * \param rect A rectangle used to crop the image before computing the Histogram.
			 * \param channel The channel to consider for the values. Default value is 0 in case of a one channel image.
			 */
            ColumnProfile_t(const Image_t<D>& img, D value, bool horizontal, const Rectangle& rect, unsigned int channel = 0);
			
			/*!
                         * \brief Constructs a ColumnProfile_t from an image.
			 *
			 * \param img The image from which to compute the projection histogram.
			 * \param value The value that will be counted.
			 * \param horizontal The projection histogram will count the pixels of the selected value for each row if true, for each column if false.
			 * \param channel The channel to consider for the values. Default value is 0 in case of a one channel image.
			 */
            ColumnProfile_t(const Image_t<D>& img, D value, bool horizontal, unsigned int channel = 0);
			
			/*!
                         * \brief Classical destructor for ColumnProfile_t.
			 */
            inline virtual ~ColumnProfile_t() {}
    };

    typedef ColumnProfile_t<depth8_t> ColumnProfile_8; //!< 8 bits depth Projection Histogram. Provided for convenience.
    typedef ColumnProfile_t<depth16_t> ColumnProfile_16; //!< 16 bits depth Projection Histogram. Provided for convenience.
    typedef ColumnProfile_t<depth32_t> ColumnProfile_32; //!< 32 bits depth Projection Histogram. Provided for convenience.
    typedef ColumnProfile_t<depth_default_t> ColumnProfile; //!< Standard Projection Histogram is depth_t bits depth.
}

#include "ColumnProfile.tpp"

#endif // COLUMNPROFILE_H
