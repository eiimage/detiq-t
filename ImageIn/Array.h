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

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>

namespace imagein
{
    /*!
     * \brief Fixed-size array with easy to use access method. Used as a base for Histogram and ProjectionHistogram_t.
     *
     * This class represents a fixed-size array of integers. It can be seen as lightweight vector with fixed size, but is easier to use than C arrays, and provides stl-like iterators.
     */
    template <typename T>
    class Array
    {
        public:
            typedef T* iterator; //!< Random access iterator
            typedef const T* const_iterator; //!< Random access const iterator

            /*!
             * \brief Creates an array of the given size.
             *
             * Values are not initialized.
             */
            inline Array(int width = 0) : _width(width), _min(0), _max(width - 1) { _array = new T[width]; }
            inline Array(int min, int max) : _min(min), _max(max), _width(max - min + 1 ) { _array = new T[ 1 + max - min]; }
            inline virtual ~Array() { delete _array; }

            //! Returns the size of the array.
            inline unsigned int getWidth() const { return _width; }
            inline unsigned int size() const { return _width; }
            inline int getMin() const {return _min; }
            inline int getMax() const {return _max; }
            /*Different from getMin() and getMin() which return the number of occurrences after counting,
             *getvMin() and getvMax() return the maximum and minimum values of the processing image,
             *in order to prepare for limiting the range of bin size input*/
            inline double getvMin() const {return _vmin; }
            inline double getvMax() const {return _vmax; }
            //! Access to the element at the given index.
            inline T operator[](int index) const { return (index >= _min && index <= _max ) ? _array[index - _min] : 0;}
            //! Constant access to the element at the given index.
            //inline T& operator[](int index) {return _array[index - _min]; }

            //! Returns an iterator to the first element of the array
            inline iterator begin() { return _array; }
            //! Returns a const_iterator to the first element of the array
            inline const_iterator begin() const { return _array; }
            //! Returns an iterator past then end of the array
            inline iterator end() { return _array + _width; }
            //! Returns a const_iterator past then end of the array
            inline const_iterator end() const { return _array + _width; }

        protected:
            T* _array;
            unsigned int _width;
            int _min;
            int _max;
            double _vmin;
            double _vmax;
    };
}

#endif // ARRAY_H
