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

#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <limits>

#include "mystdint.h"

#include "Rectangle.h"
#include "Histogram.h"

namespace imagein
{
    /*!
     * \brief Main class to manipulate images.
     *
     * The Image_t class is used to manipulate matrix images. It can handle any type of 2D image, and the
     * subclasses are only simplified interfaces for this class. If you don't know which type of image
     * you are dealing with, you can always use this class.
     *
     * The depth of the image is set using the template parameter. The most common depth
     * is 8 bits (1 byte) so you can use unsigned char as the template parameter. If you want
     * to use a 32-bit depth grayscale image, you can use uint32_t. These are only examples
     * and you can use whatever numeric type you wish as long as they are basic c++ types (with operator sizeof working).
     * Note that if you use  non-standard types, some algorithms may not work with this type of image.
     *
     * \tparam D the type of pixel values.
     */
    template <typename D>
    class Image_t
    {
        public:
            typedef D depth_t;
            typedef D* iterator; //!< Random access iterator
            typedef const D* const_iterator; //!< Random access const-iterator

            class Line {
              public:
                inline D& operator[](int i) {
                    return _ptr[i * _jmp];
                }
                int size() { return _size; }
//              protected:
                Line(D* ptr, int jmp, int size) : _ptr(ptr), _jmp(jmp), _size(size) {}
              private:
                D* _ptr;
                int _jmp;
                int _size;
            };
            class ConstLine {
              public:
                inline D operator[](int i) const {
                    return _ptr[i * _jmp];
                }
                int size() { return _size; }
//              protected:
                ConstLine(const D* ptr, int jmp, int size) : _ptr(ptr), _jmp(jmp), _size(size) {}
              private:
                const D* _ptr;
                int _jmp;
                int _size;
            };

            class Row : public Line {
              public:
                Row(D* ptr, int size) : Line(ptr, 1, size) {}
            };
            class Column : public Line {
              public:
                Column(D* ptr, int size, int jmp) : Line(ptr, jmp, size) {}
            };
            class ConstRow : public ConstLine {
              public:
                ConstRow(const D* ptr, int size) : ConstLine(ptr, 1, size) {}
            };
            class ConstColumn : public ConstLine {
              public:
                ConstColumn(const D* ptr, int size, int jmp) : ConstLine(ptr, jmp, size) {}
            };
            /*!
             * \brief Default constructor. Constructs an image from the parameters given.
             *
             * As the dimensions of an image are constant, creating an empty image is only useful when you plan to use the affectation constructor later.
             * It is only provided so that you can use Images C arrays.
             *
             * \param width The image width
             * \param height The image height
             * \param nChannels The number of channels of the image.
             * \param data The actual data matrix containing the pixels of the image.  No verification of the size of the array is performed.
             */
            Image_t(unsigned int width = 0, unsigned int height = 0, unsigned int nChannels = 0, const D* data = NULL);
            Image_t(unsigned int width, unsigned int height, unsigned int nChannels, D value);
            Image_t(std::vector<const Image_t<D>* >);

			/*!
             * \brief Constructs an image from the given file.
             *
             * The file format currently supported are jpg, png, bmp. Other formats will raise an exception.
             *
             * If you want to use other file formats, see the class ImageFile and ImageFileFactory for instructions.
             *
             * \param filename The relative or absolute filename to the image file.
             * \throw ImageFileException if the file format isn't supported or if there is an error while reading the file.
             */
            Image_t(std::string filename);

            /*!
             * \brief Image destructor.
             *
             * The data matrix is deleted when the image is deleted. You should be careful if the data
             * is still accessible from outside the class.
             */
            virtual ~Image_t();

            /*!
             * \brief Copy constructor
             *
             * \param other The image to be copied.
             */
            Image_t(const Image_t<D>& other);

            /*!
             * \brief Affect operator.
             *
             * \param other The image to be affected.
             * \sa Image(const Image_t<D>& other)
             */
            Image_t<D>& operator=(const Image_t<D>& other);

            //! Returns the width of the image
            inline unsigned int getWidth() const { return _width; }
            //! Returns the height of the image
            inline unsigned int getHeight() const { return _height; }
            //! Returns the number of channels of the image
            inline unsigned int getNbChannels() const { return _nChannels; }

            /*!
             * \brief Returns the value of a pixel.
             *
             * The origin of the coordinates system is in the top-left corner of the image.
             *
             * \param x The abscissa of the pixel
             * \param y The ordinate of the pixel
             * \param channel The channel of the value that will be returned.
             * \return The value contained in the matrix at the given coordinates.
             * \throw out_of_range if the parameters are not a valid pixel.
             */
            inline D getPixel(unsigned int x, unsigned int y, unsigned int channel) const;

            /*!
             * \brief Set the value of a channel for the given pixel.
             *
             * \param x The abscissa of the pixel
             * \param y The ordinate of the pixel
             * \param channel The channel to change.
             * \param cPixel The new value.
             * \throw out_of_range if the parameters are not a valid pixel.
             */
            inline void setPixel(unsigned int x, unsigned int y, unsigned int channel, D cPixel);

            inline D getPixelAt(unsigned int x, unsigned int y, unsigned int channel = 0) const
            {
                return _mat[channel*_width*_height + y*_width + x];
            }
            inline D& pixelAt(unsigned int x, unsigned int y, unsigned int channel = 0)
            {
                return _mat[channel*_width*_height + y*_width + x];
            }

            inline void setPixelAt(unsigned int x, unsigned int y, unsigned int channel, D cPixel)
            {
                _mat[channel*_width*_height + y*_width + x] = cPixel;
            }
            inline void setPixelAt(unsigned int x, unsigned int y, D cPixel)
            {
                _mat[y*_width + x] = cPixel;
            }
            inline Row getRow(unsigned int j, unsigned int c = 0) {
                return Row(_mat + c*_width*_height + j*_width, _width);
            }
            inline Line getColumn(unsigned int i, unsigned int c = 0) {
                return Line(_mat + c*_width*_height + i, _width, _height);
            }
            inline ConstRow getConstRow(unsigned int j, unsigned int c = 0) const {
                return ConstRow(_mat + c*_width*_height + j*_width, _width);
            }
            inline ConstLine getConstColumn(unsigned int i, unsigned int c = 0) const {
                return ConstLine(_mat + c*_width*_height + i, _width, _height);
            }

//             /*!
//             * \brief Set the values of the given pixel.
//             *
//             * \param x The abscissa of the pixel
//             * \param y The ordinate of the pixel
//             * \param pixel An array containing the values of each channels.
//             * \throw out_of_range if the parameters are not a valid pixel.
//             */
//            void setPixel(unsigned int x, unsigned int y, const D* pixel);

            //! Returns an iterator to the first channel on the top-left corner of the image
            inline iterator begin() { return _mat; }
            //! Returns a const iterator to the first channel on the top-left corner of the image
            inline const_iterator begin() const { return _mat; }
            //! Returns an iterator past then end of the image
            inline iterator end() { return _mat + size(); }
            //! returns a const iterator past the end of the image
            inline const_iterator end() const { return _mat + size(); }
            inline unsigned int size() const { return _width*_height*_nChannels; }
            bool operator==(const imagein::Image_t<D>& img) const;

            inline Image_t<D>* operator-(const Image_t<D>& img) const {
                Image_t<D>* res = new Image_t<D>(std::min(_width, img._width), std::min(_height, img._height), std::min(_nChannels, img._nChannels));
                for(unsigned int c = 0; c < res->_nChannels; ++c) {
                    for(unsigned int j = 0; j < res->_height; ++j) {
                        for(unsigned int i = 0; i < res->_width; ++i) {
                            int value = getPixelAt(i, j, c) - img.getPixelAt(i, j, c);
                            value = std::max(static_cast<int>(std::numeric_limits<D>::min()), value);
                            value = std::min(static_cast<int>(std::numeric_limits<D>::max()), value);
                            res->setPixel(i, j, c, static_cast<D>(value));
                        }
                    }
                }
                return res;
            }

            /*!
             * \brief Saves the image into a file.
             *
             * The format of the image will be based on the filename extension.
             *
             * \param filename The filename to save the image to. If it exists, the content of the file will be replaced.
             */
            void save(const std::string& filename) const;

            /*!
             * \brief Returns the histogram of the image.
             *
             * \param channel The channel to take into account for the histogram.
             * \param rect The image area on which to calculate the Histogram.
             */
            inline Histogram getHistogram(unsigned int channel=0, const Rectangle& rect = Rectangle()) const { return Histogram(*this, channel, rect); }


            /*!
             * \brief Crops the image to the boundaries defined by a Rectangle.
             *
             * \param rect the rectangle used to define the boundaries of the new image.
             * \return A new image of the same type.
             */
            virtual Image_t<D>* crop(const Rectangle& rect) const;

            depth_t min(unsigned int channel) const;
            depth_t max(unsigned int channel) const;
            double mean(unsigned int channel) const;
            double deviation(unsigned int channel, double mean) const;
            inline double deviation(unsigned int channel) const { return deviation(channel, mean()); }
            depth_t min() const;
            depth_t max() const;
            double mean() const;
            double deviation(double mean) const;
            double deviation() const { return deviation(mean()); }
            void normalize(double min = static_cast<double>(std::numeric_limits<D>::min()),
                           double max = static_cast<double>(std::numeric_limits<D>::max()));

        protected:
            void crop(const Rectangle& rect, D* mat) const;

            unsigned int _width;
            unsigned int _height;
            unsigned int _nChannels;
            D* _mat;
    };
    
    typedef uint8_t depth8_t;
    typedef uint16_t depth16_t;
    typedef uint32_t depth32_t;
    typedef depth8_t depth_default_t;

    typedef Image_t<depth8_t> Image_8; //!< 8 bits depth Image. Provided for convenience.
    typedef Image_t<depth16_t> Image_16; //!< 8 bits depth Image. Provided for convenience.
    typedef Image_t<depth32_t> Image_32; //!< 8 bits depth Image. Provided for convenience.
    typedef Image_t<depth_default_t> Image; //!< Standard Image is 8 bits depth.

    typedef Image_t<double> ImageDouble; //!< Standard Image with double values for each pixel
}

#include "Image.tpp"

#endif // IMAGE_H
