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

#ifndef LINEPROFILE_H
#define LINEPROFILE_H


#include "Array.h"
#include "Image.h"
#include <iostream>

namespace imagein
{
    /*!
     * \brief LineProfile_t creates the line profile histogram of an Image from a given a value
     *
     * This class is inherited from Array, therefore it's a fixed-size array of integers, the element with rank K of the array is the number of pixels of the given value on the Kth row/column of the Image (cropped from the Rectangle).
     */
	template <typename D>
    class LineProfile_t : public Array<unsigned int>
    {
        public:
			/*!
                         * \brief Constructs a LineProfile_t from an image.
			 *
			 * \param img The image from which to compute the projection histogram.
			 * \param value The value that will be counted.
			 * \param horizontal The projection histogram will count the pixels of the selected value for each row if true, for each column if false.
			 * \param rect A rectangle used to crop the image before computing the Histogram.
			 * \param channel The channel to consider for the values. Default value is 0 in case of a one channel image.
			 */
            LineProfile_t(const Image_t<D>& img, D value, bool horizontal, const Rectangle& rect, unsigned int channel = 0);
			
			/*!
                         * \brief Constructs a LineProfile_t from an image.
			 *
			 * \param img The image from which to compute the projection histogram.
			 * \param value The value that will be counted.
			 * \param horizontal The projection histogram will count the pixels of the selected value for each row if true, for each column if false.
			 * \param channel The channel to consider for the values. Default value is 0 in case of a one channel image.
			 */
            LineProfile_t(const Image_t<D>& img, D value, bool horizontal, unsigned int channel = 0);
			
			/*!
                         * \brief Classical destructor for LineProfile_t.
			 */
            inline virtual ~LineProfile_t() {}
    };

    typedef LineProfile_t<depth8_t> LineProfile_8; //!< 8 bits depth Projection Histogram. Provided for convenience.
    typedef LineProfile_t<depth16_t> LineProfile_16; //!< 16 bits depth Projection Histogram. Provided for convenience.
    typedef LineProfile_t<depth32_t> LineProfile_32; //!< 32 bits depth Projection Histogram. Provided for convenience.
    typedef LineProfile_t<depth_default_t> LineProfile; //!< Standard Projection Histogram is depth_t bits depth.
}

#include "LineProfile.tpp"

#endif // LINEPROFILE_H
