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

#ifndef HISTOGRAMTEST_H
#define HISTOGRAMTEST_H

#include <string>
#include <sstream>

#include <Histogram.h>
#include <Image.h>
#include "Test.h"
#include <QDebug>

class HistogramTest : public Test {

  public:
    typedef unsigned char D;

    HistogramTest()
        : Test("Histogram"), _refImg(NULL), _fail(true) {}

    virtual bool init() {
        //Reference Image
        int width = 768;
        int height = 512;
        int nbChannels = 3;
        unsigned char* dataRgb = new unsigned char[width * height * nbChannels];

        for(int i = 0 ; i < width ; ++i) {
            short red = std::max(255 - i, 255-(768-i));
            if(red < 0) red = 0;
            short green = 255 - std::abs(256-i);
            if(green < 0) green = 0;
            short blue = 255 - std::abs(512-i);
            if(blue < 0) blue = 0;

            double offset = std::max(std::max((double)red/255.0, (double)green/255.0), (double)blue/255.0);
            if(offset<1) red /= offset; green /=offset; blue/=offset;

            for(int j = 0 ; j < height ; ++j) {
            short r = red, g = green, b = blue;
                if(j<256) {
                    r = std::min(r+256-j, 255);
                    g = std::min(g+256-j, 255);
                    b = std::min(b+256-j,255);
                }
                else {
                        r = std::max(r+256-j, 0);
                        g = std::max(g+256-j, 0);
                        b = std::max(b+256-j,0);
                }

                dataRgb[j*width*nbChannels + i*nbChannels] = r;
                dataRgb[j*width*nbChannels + i*nbChannels + 1] = g;
                dataRgb[j*width*nbChannels + i*nbChannels + 2] = b;
            }
        }
		_refImg = new imagein::Image_t<D>(width, height, nbChannels, dataRgb);
        return true;
    }

    virtual bool test() {
        // testing horizontal histogram
		imagein::Histogram hist(*_refImg);

        if(hist.size() < 1) {
            return false;
        }

        _fail = false;
        return true;
    }

    virtual bool cleanup() {
        delete _refImg;
        return true;
    }

    virtual std::string info() {
        return "";
    }

  protected:
    imagein::Image_t<D>* _refImg;
    bool _fail;

    
};

#endif //!PROJHISTTEST_H
