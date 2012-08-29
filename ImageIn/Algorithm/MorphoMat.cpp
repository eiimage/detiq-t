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

#include "MorphoMat.h"

using namespace imagein::MorphoMat;
StructElem::StructElem(Dir dir) {
    _width = (dir == Top || dir == Bottom) ? 1 : 2;
    _height = (dir == Left || dir == Right) ? 1 : 2;
    _mat = new bool[_width * _height];
    _mat[0] = !(dir == TopRight || dir == BottomLeft);
    _mat[1] = !(dir == TopLeft || dir == BottomRight);
    if(_width > 1 && _height > 1) {
        _mat[2] = (dir == TopRight || dir == BottomLeft);
        _mat[3] = (dir== TopLeft || dir == BottomRight);
    }
    _scale = 1;
    _centerX = (dir == Left || dir == TopLeft || dir == BottomLeft) ? 1 : 0;
    _centerY = (dir == TopLeft || dir == Top || dir == TopRight) ? 1 : 0;
}
StructElem::StructElem(std::string filename) {
    Image image(filename);
    algorithm::Otsu algo;
    GrayscaleImage* im_tmp = Converter<GrayscaleImage>::convert(image);
    GrayscaleImage* im_res = algo(im_tmp);

    _width = im_res->getWidth();
    _height = im_res->getHeight();
    _mat = new bool[_width * _height * _nChannels];

    for(unsigned int j = 0; j < getHeight(); ++j) {
        for(unsigned int i = 0; i < getWidth(); ++i) {
            setPixel(i, j, (im_res->getPixel(i, j) <= 0));
        }
    }

    _scale = 1;
    _centerX = getWidth()/2;
    _centerY = getHeight()/2;
}
StructElem::StructElem(GrayscaleImage_t<bool> elem, unsigned int centerX, unsigned int centerY) : GrayscaleImage_t<bool>(elem), _scale(1), _centerX(centerX), _centerY(centerY) {}

void StructElem::save(const std::string& filename) const {
    GrayscaleImage img(getWidth(), getHeight());
    for(unsigned int j = 0; j < img.getHeight(); ++j) {
        for(unsigned int i = 0; i < img.getWidth(); ++i) {
            img.setPixel(i, j, getPixel(i, j) ? 0 : 255);
        }
    }
    img.save(filename);
}

void StructElem::dilate(const StructElem& elem) {
    unsigned int newWidth = _width + elem._width - 1;
    unsigned int newHeight = _height + elem._height - 1;
    GrayscaleImage_t<bool>* bufImg = new GrayscaleImage_t<bool>(newWidth, newHeight);
    for(GrayscaleImage_t<bool>::iterator it = bufImg->begin(); it < bufImg->end(); ++it) {
        *it = false;
    }
    for(unsigned int j = 0; j < _height; ++j) {
        for(unsigned int i = 0; i < _width; ++i) {
            bufImg->setPixel(i + elem._width - elem._centerX - 1, j + elem._height - elem._centerY - 1, getPixel(i, j));
        }
    }
    Dilatation<bool> op(elem);
    Image_t<bool>* resImg = op(bufImg);
    delete bufImg;
    bool* data = _mat;
    _width = resImg->getWidth();
    _height = resImg->getHeight();
    _mat = new bool[_width * _height];
    delete data;
    std::copy(resImg->begin(), resImg->end(), this->begin());
    delete resImg;
}
