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

#include "VffImage.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "mystdint.h"
#include "ImageFileException.h"
#include <iostream>

using namespace imagein;
using namespace std;

VffImage::VffImage(std::string filename) : ImageFile(filename)
{
}

unsigned int VffImage::readHeight() {
    FILE *fp;
    unsigned int width, height;
    fp = fopen(this->_filename.c_str(),"rb");
    if(fp == NULL) {
        throw ImageFileException("Cannot open vff file "+this->_filename, __LINE__, __FILE__);
    }
    fscanf(fp,"ncaa\nrank=2;\nsize=%i %i;\nbands=1;\n", &width, &height);
    fclose(fp);
    return height;
}
unsigned int VffImage::readWidth() {
    FILE *fp;
    unsigned int width, height;
    fp = fopen(this->_filename.c_str(),"rb");
    if(fp == NULL) {
        throw ImageFileException("Cannot open vff file "+this->_filename, __LINE__, __FILE__);
    }
    fscanf(fp,"ncaa\nrank=2;\nsize=%i %i;\nbands=1;\n", &width, &height);
    fclose(fp);
    return width;
}
unsigned int VffImage::readNbChannels() {
    return 1;
}
unsigned int VffImage::readDepth() {
    return sizeof(uint8_t)*8;
}

void* VffImage::readData()
{
    FILE *fp ;

    unsigned int width, height;
    fp = fopen(this->_filename.c_str(),"rb");
    if(fp == NULL) {
        throw ImageFileException("Cannot open vff file "+this->_filename, __LINE__, __FILE__);
    }
    fscanf(fp,"ncaa\nrank=2;\nsize=%i %i;\nbands=1;\n", &width, &height);

    uint8_t* img = new uint8_t[width * height];

    while( fgetc(fp) != 12 );
    fseek(fp, 1, SEEK_CUR);
    for(unsigned int i = 0; i < height; ++i) {
        fread(&img[i*width], 1, width, fp);
    }
    fclose(fp);
    return reinterpret_cast<void*>(img);
}

void VffImage::writeData(const void* const data_, unsigned int width, unsigned int height, unsigned int /*nChannels*/, unsigned int depth)
{
    const uint8_t* data = reinterpret_cast<const uint8_t*>(data_);
    FILE *fp ;

    fp = fopen(this->_filename.c_str(), "wb");
    if(fp == NULL) {
        throw ImageFileException("Cannot open vff file "+this->_filename, __LINE__, __FILE__);
    }
    fprintf(fp, "ncaa\nrank=2;\nsize=%i %i;\nbands=1;\n", width, height);
    fprintf(fp, "bits=8;\nformat=base;\n");
    fprintf(fp, "type=raster;\n\n\n\n\n\n\n\n\n\f\n");
    for(unsigned int j = 0; j < height; ++j) {
        fwrite(&data[j * width], width, 1, fp);
    }
    fclose(fp);
    return;
}
