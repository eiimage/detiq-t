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

//#include "Filtering.h"

#include "Filtering.h"

#include <algorithm>
#include "Average.h"
#include "SquareSum.h"

namespace imagein
{
namespace algorithm
{
Filtering::Filtering(Filter* filter)
{
    _filters.push_back(filter);
    _policy = POLICY_BLACK;
    _normalisation = false;
}

Filtering::Filtering(std::vector<Filter*> filters) : _filters(filters)
{
    _policy = POLICY_BLACK;
    _normalisation = false;
}


double Filtering::filterPixel(const Image_t<double>* img, int x, int y, int c, Filter* filter, int hwf, int hhf){
    double newPixel = 0;
    for(unsigned int i = 0; i < filter->getWidth(); i++){
        for(unsigned int j = 0; j < filter->getHeight(); j++){
            int imgX = x + i - hwf;
            int imgY = y + j - hhf;
            switch(_policy) {
                case POLICY_TOR:
                    {
                    newPixel += filter->getPixelAt(i,j) * sphericalPolicy(img, imgX, imgY, c);
                    break;
                    }
                case POLICY_NEAREST:
                    {
                    newPixel += filter->getPixelAt(i,j) * nearestPolicy(img, imgX, imgY, c);
                    break;
                    }
                case POLICY_MIRROR:
                    {
                    newPixel += filter->getPixelAt(i,j) * mirrorPolicy(img, imgX, imgY, c);
                    break;
                    }
                default:
                    {
                    newPixel += filter->getPixelAt(i,j) * blackPolicy(img, imgX, imgY, c);
                    break;
                    }
	        }
        }
    }
    return newPixel;
}

double Filtering::blackPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel)
{
    double newPixel;
	try
	{
	newPixel = img->getPixel(x, y, channel);
	}
	catch(std::out_of_range)
	{
		newPixel = 0;
	}
	return newPixel;
}

double Filtering::mirrorPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel)
{
    double newPixel;
	try
	{
	newPixel = img->getPixel(x, y, channel);
	}
	catch(std::out_of_range)
    {
		int nx = x, ny = y;
		if(x < 0)
			nx = -x;
		else if(x >= (int)img->getWidth())
			nx = img->getWidth() - (x - img->getWidth());
		if(y < 0)
			ny = -y;
		else if(y >= (int)img->getHeight())
			ny = img->getHeight() - (y - img->getHeight());
					
		try
		{
			newPixel = img->getPixel(nx, ny, channel);
		}
		catch(std::out_of_range)
		{
			newPixel = 0;
		}
	}
    return newPixel;
}
			
double Filtering::nearestPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel)
{
    double newPixel;
	try
	{
		newPixel = img->getPixel(x, y, channel);
	}
	catch(std::out_of_range)
	{
		int nx = x, ny = y;
		if(x < 0)
			nx = 0;
		else if(x >= (int)img->getWidth())
			nx = img->getWidth() - 1;
		if(y < 0)
			ny = 0;
		else if(y >= (int)img->getHeight())
			ny = img->getHeight() - 1;
					
		try
		{
			newPixel = img->getPixel(nx, ny, channel);
		}
		catch(std::out_of_range)
		{
			newPixel = 0;
    	}
	}
	return newPixel;
}
			
double Filtering::sphericalPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel)
{
    double newPixel;
	try
	{
		newPixel = img->getPixel(x, y, channel);
	}
   	catch(std::out_of_range)
	{
		int nx = x, ny = y;
		if(x < 0)
	    	nx = img->getWidth() - x;
		else if(x >= (int)img->getWidth())
    		nx = x - img->getWidth();
		if(y < 0)
			ny = img->getHeight() - y;
		else if(y >= (int)img->getHeight())
			ny = y - img->getHeight();
					
		try
		{
		newPixel = img->getPixel(nx, ny, channel);
    	}
		catch(std::out_of_range)
		{
			newPixel = 0;
		}
	}
	return newPixel;
}



Image_t<double>* Filtering::algorithm(const std::vector<const Image_t<double>*>& imgs)
{
    const Image_t<double>* img = imgs.at(0);

    if(img == NULL) {
        throw ImageTypeException(__LINE__, __FILE__);
    }

    int width = img->getWidth();
    int height = img->getHeight();
    int nChannels = img->getNbChannels();
    bool odd = width % 2 != 1 || height % 2 != 1;

    std::vector<Filter*>::iterator filter;
    std::vector<Image_t<double>*> images;


// attention Normalisation du filtre 
    
    for(filter = _filters.begin(); filter != _filters.end(); ++filter)
    {
        Filter::iterator iter = (*filter)->begin();
        
        if(_normalisation != 0){
            for(Filter::iterator it = (*filter)->begin(); it < (*filter)->end(); ++it) {
                    *it /= abs(_normalisation);
                }
            }
        }
        Image_t<double>* result = new Image_t<double>(width, height, nChannels);
        
        int halfHeightFilter = (*filter)->getHeight() / 2;
        int halfWidthFilter = (*filter)->getWidth() / 2;

        for(int c = 0; c < nChannels; c++) {
	        for(int y = 0; y < height; y++) {
	            for(int x = 0; x < width; x++) {
                    result->pixelAt(x, y, c) = filterPixel(img, x, y, c, *filter,halfWidthFilter, halfHeightFilter);
                }
            }
        }

        images.push_back(result);
    }
    Image_t<double>* result = NULL;

    if(images.size() == 1)
        result = images[0];
    else
    {
        unsigned int size = images.size();
        
        SquareSum<Image_t<double>, 2> av;

        for(unsigned int i = 0; i < size; ++i)
        {
            if(result == NULL)
            {
                result = av(images[0], images[1]);
            }
            else
            {
                result = av(result, images[i]);
            }
        }
    }
    return result;
}

Filtering Filtering::uniformBlur(int numPixels = 3)
{
    return Filtering(Filter::uniform(numPixels));
}

Filtering Filtering::gaussianBlur(int size, double sigma)
{
    return Filtering(Filter::gaussian(size, sigma));
}

Filtering Filtering::gaussianBlur(double alpha)
{
    return Filtering(Filter::gaussian(alpha));
}

Filtering Filtering::prewitt(int numPixels = 3)
{
    return Filtering(Filter::prewitt(numPixels));
}

Filtering Filtering::roberts()
{
    return Filtering(Filter::roberts());
}

Filtering Filtering::sobel()
{
    return Filtering(Filter::sobel());
}

Filtering Filtering::squareLaplacien()
{
    return Filtering(Filter::squareLaplacien());
}




}
}
