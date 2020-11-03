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

#ifndef PROJHISTTEST_H
#define PROJHISTTEST_H

#include <string>
#include <sstream>

#include <ProjectionHistogram.h>
#include <Image.h>
#include "Test.h"


class ProjHistTest : public Test {

  public:
    typedef unsigned char D;

    ProjHistTest()
        : Test("Projection histogram"), _refImg(NULL), _hfail(true), _vfail(true) {}

    virtual bool init() {

        int width = 512;
        int height = 512;
        int nbChannels = 3;
        unsigned char* dataProj = new unsigned char[width * height * nbChannels];

        for(int j = 0; j < height ; ++j) {
            for(int i = 0 ; i < width ; ++i) {
                if(j>i) {
                    dataProj[j*width*nbChannels + i*nbChannels] = 0;
                    dataProj[j*width*nbChannels + i*nbChannels +1] = 0;
                    dataProj[j*width*nbChannels + i*nbChannels +2] = 0;
                }
                else {
                    dataProj[j*width*nbChannels + i*nbChannels] = 255;
                    dataProj[j*width*nbChannels + i*nbChannels +1] = 255;
                    dataProj[j*width*nbChannels + i*nbChannels +2] = 255;
                }
            }
        }
        _refImg = new imagein::Image_t<D>(width, height, nbChannels, dataProj);
        return true;
    }

    virtual bool test() {

        _refImg->save("refImg_projhist.png");

        // testing horizontal histogram
        imagein::ProjectionHistogram_t<uint8_t> ph(*_refImg,255,true);

        if(ph.size() < 1){
            return false;
        }

        _hfail = false;
        _vfail = true;

        // testing vertical histogram
        imagein::ProjectionHistogram_t<uint8_t> pv(*_refImg,255,false);

        if(pv.size() < 1){
            return false;
        }

        _vfail = false;
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
    bool _hfail;
    bool _vfail;

};

#endif //!PROJHISTTEST_H
