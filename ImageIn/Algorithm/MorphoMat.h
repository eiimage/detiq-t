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

#ifndef IMAGEIN_MORPHOMAT_H
#define IMAGEIN_MORPHOMAT_H

#include <vector>
#include <algorithm>

#include "../GenericAlgorithm.h"
#include "../GrayscaleImage.h"
#include "Difference.h"
#include "Otsu.h"
#include "../Converter.h"

namespace imagein {

  namespace MorphoMat {

    class StructElem  : public GrayscaleImage_t<bool> {
      public:
        enum Dir {Left, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft};
        StructElem(Dir);
        StructElem(std::string filename);
        StructElem(GrayscaleImage_t<bool> elem, unsigned int centerX, unsigned int centerY);
        inline unsigned char getScale() const { return _scale; }
        inline void setScale(unsigned char scale) { if(scale>0) { _scale = scale; } }
        inline unsigned int getCenterX() { return _centerX; }
        inline unsigned int getCenterY() { return _centerY; }
        inline void setCenterX(unsigned int centerX) { _centerX = centerX; }
        inline void setCenterY(unsigned int centerY) { _centerY = centerY; }
        inline void setCenter(unsigned int centerX, unsigned int centerY) { _centerX = centerX; _centerY = centerY; }

        template <typename D>
        std::vector<D> intersect(const Image_t<D>& image, unsigned int offsetX, unsigned int offsetY, unsigned int channel);
        void save(const std::string& filename) const;

        void dilate(const StructElem& elem);

        inline StructElem transpose() {
            //Code before dev2018
           /* GrayscaleImage_t<bool> t(_height*_scale,_width*_scale);
            
            for(unsigned int i = 0 ; i < _width*_scale ; i++){
                for(unsigned int j=0 ; j <_height*_scale ; j++)
                   t.setPixel(j,i,this->getPixel(i,j));
            }

            StructElem res(t, (_width-this->_centerX)*_scale,(_height-this->_centerY)*_scale);

            return res;*/
            GrayscaleImage_t<bool> t(_width*_scale,_height*_scale);
            for(unsigned int i = 0 ; i < _width*_scale ; i++){
                for(unsigned int j = 0 ; j <_height*_scale ; j++){
                   t.setPixel(i, j, this->getPixel(_width*_scale-1-i, _height*_scale-1-j));
                }
            }

            StructElem res(t, _width - this->_centerX - 1, _height - this->_centerY - 1);
            res.setScale(this->_scale);

            return res;
        }

      private:
        unsigned char _scale;
        unsigned int _centerX, _centerY;
    };

    template <typename D>
    class Operator : public GenericAlgorithm_t<D> {
      public:
        Operator(const StructElem& elem);
        inline void setElem(const StructElem& elem) { _elem = elem; }
      protected:
        StructElem _elem;
    };

    template <typename D>
    class Dilatation : public Operator<D> {
      public:
        Dilatation(const StructElem& elem);
      protected:
        Image_t<D>* algorithm(const std::vector<const Image_t<D>*>& imgs);
    };

    template <typename D>
    std::vector<D> StructElem::intersect(const Image_t<D>& image, unsigned int offsetX, unsigned int offsetY, unsigned int channel) {
        std::vector<D> pixels;
        for(unsigned int j = 0; j < getHeight()*_scale; ++j) {
            for(unsigned int i = 0; i < getWidth()*_scale; ++i) {
                if(getPixel(i/_scale,j/_scale)) {
                    int x = i - _centerX*_scale + offsetX;
                    int y = j - _centerY*_scale + offsetY;
                    if(x>=0 && y>=0 && static_cast<unsigned int>(x)<image.getWidth() && static_cast<unsigned int>(y)<image.getHeight()) {
                        pixels.push_back(image.getPixel(static_cast<unsigned int>(x),static_cast<unsigned int>(y),channel));
                    }
                }
            }
        }
        return pixels;
    }


    template <typename D>
    Operator<D>::Operator(const StructElem& elem) : _elem(elem) {}

    template <typename D>
    class Erosion : public Operator<D> {
      public:
        Erosion(const StructElem& elem) : Operator<D>(elem) {}
      protected:
        Image_t<D>* algorithm(const std::vector<const Image_t<D>*>& imgs) {
            const Image_t<D>& img = *imgs[0];
            Image_t<D> *result = new Image_t<D>(img.getWidth(), img.getHeight(), img.getNbChannels());

            if(this->_elem.getScale()>1) {
                for(unsigned int channel = 0; channel < img.getNbChannels(); ++channel) {
                    for(unsigned int offsetY = 0; offsetY < img.getHeight(); ++offsetY) {
                        for(unsigned int offsetX = 0; offsetX < img.getWidth(); ++offsetX) {
                           
                            D value = std::numeric_limits<D>::max();
                            
                            for(unsigned int j = 0; j < this->_elem.getHeight()*this->_elem.getScale(); ++j) {
                                for(unsigned int i = 0; i < this->_elem.getWidth()*this->_elem.getScale(); ++i) {
                                    if(this->_elem.getPixel(i/this->_elem.getScale(),j/this->_elem.getScale())) {
                                        const int px = i - this->_elem.getCenterX()*this->_elem.getScale() + offsetX;        
                                        const int py = j - this->_elem.getCenterY()*this->_elem.getScale() + offsetY;
                                        if(px>=0 && py>=0 && static_cast<unsigned int>(px)<img.getWidth() && static_cast<unsigned int>(py)<img.getHeight()) {
                                            if(img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel) < value) {
                                                value = img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel);
                                            }
                                        }
                                    }
                                }
                            }
                            result->setPixel(offsetX, offsetY, channel, value);
                        }
                    }
                }
            }
            else {
                for(unsigned int channel = 0; channel < img.getNbChannels(); ++channel) {
                    for(unsigned int offsetY = 0; offsetY < img.getHeight(); ++offsetY) {
                        for(unsigned int offsetX = 0; offsetX < img.getWidth(); ++offsetX) {
                           
                            D value = std::numeric_limits<D>::max();
                            
                            for(unsigned int j = 0; j < this->_elem.getHeight(); ++j) {
                                for(unsigned int i = 0; i < this->_elem.getWidth(); ++i) {
                                    if(this->_elem.getPixel(i,j)) {
                                        const int px = i - this->_elem.getCenterX() + offsetX;        
                                        const int py = j - this->_elem.getCenterY() + offsetY;
                                        if(px>=0 && py>=0 && static_cast<unsigned int>(px)<img.getWidth() && static_cast<unsigned int>(py)<img.getHeight()) {
                                            if(img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel) < value) {
                                                value = img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel);
                                            }
                                        }
                                    }
                                }
                            }
                            result->setPixel(offsetX, offsetY, channel, value);
                        }
                    }
                }
            }

            return result;
        }
    };
    
    template <typename D>
    Dilatation<D>::Dilatation(const StructElem& elem) : Operator<D>(elem) {}

    template <typename D>
    Image_t<D>* Dilatation<D>::algorithm(const std::vector<const Image_t<D>*>& imgs) {
        const Image_t<D>& img = *imgs[0];
        Image_t<D> *result = new Image_t<D>(img.getWidth(), img.getHeight(), img.getNbChannels());

        if(this->_elem.getScale()>1) {
            for(unsigned int channel = 0; channel < img.getNbChannels(); ++channel) {
                for(unsigned int offsetY = 0; offsetY < img.getHeight(); ++offsetY) {
                    for(unsigned int offsetX = 0; offsetX < img.getWidth(); ++offsetX) {

                        D value = std::numeric_limits<D>::min();

                        for(unsigned int j = 0; j < this->_elem.getHeight()*this->_elem.getScale(); ++j) {
                            for(unsigned int i = 0; i < this->_elem.getWidth()*this->_elem.getScale(); ++i) {
                                if(this->_elem.getPixel(i/this->_elem.getScale(),j/this->_elem.getScale())) {
                                    const int px = i - this->_elem.getCenterX()*this->_elem.getScale() + offsetX;
                                    const int py = j - this->_elem.getCenterY()*this->_elem.getScale() + offsetY;
                                    if(px>=0 && py>=0 && static_cast<unsigned int>(px)<img.getWidth() && static_cast<unsigned int>(py)<img.getHeight()) {
                                        if(img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel) > value) {
                                            value = img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel);
                                        }
                                    }
                                }
                            }
                        }
                        result->setPixel(offsetX, offsetY, channel, value);
                    }
                }
            }
        }
        else {
            for(unsigned int channel = 0; channel < img.getNbChannels(); ++channel) {
                for(unsigned int offsetY = 0; offsetY < img.getHeight(); ++offsetY) {
                    for(unsigned int offsetX = 0; offsetX < img.getWidth(); ++offsetX) {

                        D value = std::numeric_limits<D>::min();

                        for(unsigned int j = 0; j < this->_elem.getHeight(); ++j) {
                            for(unsigned int i = 0; i < this->_elem.getWidth(); ++i) {
                                if(this->_elem.getPixel(i,j)) {
                                    const int px = i - this->_elem.getCenterX() + offsetX;
                                    const int py = j - this->_elem.getCenterY() + offsetY;
                                    if(px>=0 && py>=0 && static_cast<unsigned int>(px)<img.getWidth() && static_cast<unsigned int>(py)<img.getHeight()) {
                                        if(img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel) > value) {
                                            value = img.getPixel(static_cast<unsigned int>(px),static_cast<unsigned int>(py),channel);
                                        }
                                    }
                                }
                            }
                        }
                        result->setPixel(offsetX, offsetY, channel, value);
                    }
                }
            }
        }

        //for(unsigned int k = 0; k < img.getNbChannels(); ++k) {
            //for(unsigned int j = 0; j < img.getHeight(); ++j) {
                //for(unsigned int i = 0; i < img.getWidth(); ++i) {
                    //std::vector<D> inter = this->_elem.intersect(img, i, j, k);
                    //D value = *max_element(inter.begin(), inter.end());
                    //result->setPixel(i, j, k, value);
                //}
            //}
        //}

        return result;
    }
    
    template <typename D>
    class Opening : public Operator<D> {
      public:
        Opening(const StructElem& elem) : Operator<D>(elem) {}
      protected:
        Image_t<D>* algorithm(const std::vector<const Image_t<D>*>& imgs) {
            
            const Image_t<D>& img = *imgs[0];

            Erosion<D> erosion(this->_elem);
            Image_t<D>* buffer = erosion(&img);
            Dilatation<D> dilatation(this->_elem.transpose());
            Image_t<D>* result = dilatation(buffer);
            delete buffer;

            return result;
        }
    };
    
    template <typename D>
    class Closing : public Operator<D> {
      public:
        Closing(const StructElem& elem) : Operator<D>(elem) {}
      protected:
        Image_t<D>* algorithm(const std::vector<const Image_t<D>*>& imgs) {
            
            const Image_t<D>& img = *imgs[0];

            Dilatation<D> dilatation(this->_elem);
            Image_t<D>* buffer = dilatation(&img);
            Erosion<D> erosion(this->_elem.transpose());
            Image_t<D>* result = erosion(buffer);
            delete buffer;

            return result;
        }
    };

    template <typename D>
    class Gradient : public Operator<D> {
      public:
        Gradient(const StructElem& elem) : Operator<D>(elem) {}
      protected:
        Image_t<D>* algorithm(const std::vector<const Image_t<D>*>& imgs) {
            
            const Image_t<D>& img = *imgs[0];

            Dilatation<D> dilatation(this->_elem);
            Image_t<D>* bufferd = dilatation(&img);
            Erosion<D> erosion(this->_elem);
            Image_t<D>* buffere = erosion(&img);

            algorithm::Difference<Image_t<D> > difference;

            Image_t<D>* result = difference(bufferd, buffere);
            delete bufferd;
            delete buffere;
            return result;
        }
    };
    
    template <typename D>
    class WhiteTopHat : public Operator<D> {
      public:
        WhiteTopHat(const StructElem& elem) : Operator<D>(elem) {}
      protected:
        Image_t<D>* algorithm(const std::vector<const Image_t<D>*>& imgs) {
            
            const Image_t<D>& img = *imgs[0];

            Opening<D> op(this->_elem);
            Image_t<D>* buffer = op(&img);

            algorithm::Difference<Image_t<D> > difference;

            Image_t<D>* result = difference(imgs[0], buffer);
            delete buffer;
            return result;
        }
    };
    
    template <typename D>
    class BlackTopHat : public Operator<D> {
      public:
        BlackTopHat(const StructElem& elem) : Operator<D>(elem) {}
      protected:
        Image_t<D>* algorithm(const std::vector<const Image_t<D>*>& imgs) {
            
            const Image_t<D>& img = *imgs[0];

            Closing<D> op(this->_elem);
            Image_t<D>* buffer = op(&img);

            algorithm::Difference<Image_t<D> > difference;

            //Image_t<D>* result = difference(buffer, imgs[0]);
            Image_t<D>* result = new Image_t<D>(img.getWidth(), img.getHeight(), img.getNbChannels());
            typename Image_t<D>::const_iterator it1 = img.begin();
            typename Image_t<D>::const_iterator it2 = buffer->begin();
            typename Image_t<D>::iterator it3 = result->begin();
            unsigned int i =0;
            while(it3 < result->end()) {

                *it3 = *it2 - *it1;
                if(*it1 > *it2) {
                    std::cout << (int)*it2 << ">"  << (int)*it1 << std::endl;
                    ++i;
                }
                ++it1;
                ++it2;
                ++it3;
            }
            delete buffer;
            return result;
        }
    };
  
  }

}

#endif //!IMAGIN_MORPHOMAT_H
