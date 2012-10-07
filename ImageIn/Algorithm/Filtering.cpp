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
#ifdef __linux__
#include <pthread.h>
#include <sys/sysinfo.h>
#endif
#include "Average.h"
#include "Distance.h"

namespace imagein
{
namespace algorithm
{
Filtering::Filtering(Filter* filter)
{
    _filters.push_back(filter);
//    _policy = blackPolicy;
    _policy = POLICY_BLACK;
}

Filtering::Filtering(std::vector<Filter*> filters) : _filters(filters)
{
//    _policy = blackPolicy;
    _policy = POLICY_BLACK;
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

    double posFactor = 0.;
    double negFactor = 0.;
    for(filter = _filters.begin(); filter != _filters.end(); ++filter)
    {
        Filter::iterator iter = (*filter)->begin();
        for(; iter != (*filter)->end(); ++iter)
        {
            if((*iter) < 0)
                negFactor -= (*iter);
            else
                posFactor += (*iter);
        }

        double factor = std::max(posFactor, negFactor);
        std::cout << "facteur = " << factor << std::endl;
        for(Filter::iterator it = (*filter)->begin(); it < (*filter)->end(); ++it) {
            *it /= factor;
            std::cout << *it << std::endl;
        }

        Image_t<double>* result = new Image_t<double>(width, height, nChannels);

#ifdef __linux__

        int numCPU;
#ifdef _SC_NPROCESSORS_ONLN
        numCPU = sysconf( _SC_NPROCESSORS_ONLN );
#else
        numCPU = 1;
#endif
        pthread_t threads[numCPU];

        for(int i = 0; i < numCPU; i++)
        {
            pthread_t thread;
            pthread_attr_t attr;
            pthread_attr_init(&attr);

            struct ParallelArgs* args = new struct ParallelArgs;
            args->img = img;
            args->result = result;
            args->filter = *filter;
            args->policy = _policy;
            args->infl = (i * img->getHeight() * img->getNbChannels()) / numCPU;
            args->supl = ( (i + 1) * img->getHeight() * img->getNbChannels()) / numCPU;
            args->factor = factor;
            args->odd = odd;

            pthread_create(&thread, &attr, parallelAlgorithm, (void*)args);

            threads[i] = thread;
        }

        for(int i = 0; i < numCPU; i++)
            pthread_join(threads[i], NULL);

#else
        
        int halfHeightFilter = (*filter)->getHeight() / 2;
        int halfWidthFilter = (*filter)->getWidth() / 2;

        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                for(int channel = 0; channel < nChannels; channel++)
                {
                    double newPixel = 0;

                    for(int i = 0; i < (*filter)->getWidth(); i++)
                    {
                        for(int j = 0; j < (*filter)->getHeight(); j++)
                        {
                            if(odd)
                            {
                                newPixel += (*filter)->getPixelAt(i,j) * ((*_policy)(img, x + i - halfWidthFilter, y + j - halfHeightFilter, channel));
                            }
                            else
                            {
                                newPixel += (*filter)->getPixelAt(i,j) * ((*_policy)(img, x + i - halfWidthFilter - 1, y + j - halfHeightFilter - 1, channel));
                            }
                        }
                    }
                    if(factor > 1)
                        newPixel /= factor;
                    result->setPixel(x, y, channel, newPixel);
                }
            }
        }
#endif
        images.push_back(result);
    }
    Image_t<double>* result = NULL;

    if(images.size() == 1)
        result = images[0];
    else
    {
        unsigned int size = images.size();
        
        MaxDistance<Image_t<double>, 2> av;

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

template<Filtering::Policy>
inline double filtering(const Image_t<double>* img, int x, int y, int c, Filter* filter, int hwf, int hhf);

template<>
inline double filtering<Filtering::POLICY_BLACK>(const Image_t<double>* img, int x, int y, int c, Filter* filter, int hwf, int hhf) {
    double newPixel = 0.;
    for(unsigned int i = 0; i < filter->getWidth(); i++)
    {
        for(unsigned int j = 0; j < filter->getHeight(); j++)
        {
            const int imgX = x + i - hwf;
            const int imgY = y + j - hhf;
            if(imgX > 0 && imgX < img->getWidth() && imgY > 0 && imgY < img->getHeight()) {
                newPixel += filter->getPixelAt(i,j) * img->getPixelAt(imgX, imgY, c);
            }
        }
    }
    return newPixel;
}

template<>
inline double filtering<Filtering::POLICY_MIRROR>(const Image_t<double>* img, int x, int y, int c, Filter* filter, int hwf, int hhf) {
    double newPixel = 0.;
    for(unsigned int i = 0; i < filter->getWidth(); i++)
    {
        for(unsigned int j = 0; j < filter->getHeight(); j++)
        {
            int imgX = x + i - hwf;
            int imgY = y + j - hhf;
            if(imgX < 0) imgX = -imgX;
            if(imgY < 0) imgY = -imgY;
            if(imgX >= img->getWidth()) imgX = 2*img->getWidth() - imgX - 1;
            if(imgY >= img->getHeight()) imgY = 2*img->getHeight() - imgY - 1;
            newPixel += filter->getPixelAt(i,j) * img->getPixelAt(imgX, imgY, c);
        }
    }
    return newPixel;
}

template<>
inline double filtering<Filtering::POLICY_NEAREST>(const Image_t<double>* img, int x, int y, int c, Filter* filter, int hwf, int hhf) {
    double newPixel = 0.;
    for(unsigned int i = 0; i < filter->getWidth(); i++)
    {
        for(unsigned int j = 0; j < filter->getHeight(); j++)
        {
            int imgX = x + i - hwf;
            int imgY = y + j - hhf;
            if(imgX < 0) imgX = 0;
            if(imgY < 0) imgY = 0;
            if(imgX >= img->getWidth()) imgX = img->getWidth() - 1;
            if(imgY >= img->getHeight()) imgY = img->getHeight() - 1;
            newPixel += filter->getPixelAt(i,j) * img->getPixelAt(imgX, imgY, c);
        }
    }
    return newPixel;
}

template<>
inline double filtering<Filtering::POLICY_TOR>(const Image_t<double>* img, int x, int y, int c, Filter* filter, int hwf, int hhf) {
    double newPixel = 0.;
    for(unsigned int i = 0; i < filter->getWidth(); i++)
    {
        for(unsigned int j = 0; j < filter->getHeight(); j++)
        {
            int imgX = x + i - hwf;
            int imgY = y + j - hhf;
            if(imgX < 0) imgX = img->getWidth() - imgX;
            if(imgY < 0) imgY = img->getHeight() - imgY;
            if(imgX >= img->getWidth()) imgX = imgX - img->getWidth();
            if(imgY >= img->getHeight()) imgY = imgY - img->getHeight();
            newPixel += filter->getPixelAt(i,j) * img->getPixelAt(imgX, imgY, c);
        }
    }
    return newPixel;
}


#ifdef __linux__
void* Filtering::parallelAlgorithm(void* data)
{
    struct ParallelArgs args = *((ParallelArgs*) data);

    const Image_t<double>* img = args.img;
    Image_t<double>* result = args.result;
    Filter* filter = args.filter;
    Policy policy = args.policy;
    int factor = args.factor;
    bool odd = args.odd;

    int halfHeightFilter = (filter->getHeight()-1) / 2;
    int halfWidthFilter = (filter->getWidth()-1) / 2;
    bool check = true;
    switch(policy) {
        case POLICY_TOR:
        {
            for(unsigned int l = args.infl; l < args.supl; ++l) {
                const unsigned int c = l / img->getHeight();
                const unsigned int y = l % img->getHeight();
                for(unsigned int x = 0; x < img->getWidth(); ++x) {
                    result->pixelAt(x, y, c) = filtering<POLICY_TOR>(img, x, y, c, filter, halfWidthFilter, halfHeightFilter);
                }
            }
            break;
        }
        case POLICY_NEAREST:
        {
            for(unsigned int l = args.infl; l < args.supl; ++l) {
                const unsigned int c = l / img->getHeight();
                const unsigned int y = l % img->getHeight();
                for(unsigned int x = 0; x < img->getWidth(); ++x) {
                    result->pixelAt(x, y, c) = filtering<POLICY_NEAREST>(img, x, y, c, filter, halfWidthFilter, halfHeightFilter);
                }
            }
            break;
        }
        case POLICY_MIRROR:
        {
            for(unsigned int l = args.infl; l < args.supl; ++l) {
                const unsigned int c = l / img->getHeight();
                const unsigned int y = l % img->getHeight();
                for(unsigned int x = 0; x < img->getWidth(); ++x) {
                    result->pixelAt(x, y, c) = filtering<POLICY_MIRROR>(img, x, y, c, filter, halfWidthFilter, halfHeightFilter);
                }
            }
            break;
        }
        default:
        {
            for(unsigned int l = args.infl; l < args.supl; ++l) {
                const unsigned int c = l / img->getHeight();
                const unsigned int y = l % img->getHeight();
                for(unsigned int x = 0; x < img->getWidth(); ++x) {
                    result->pixelAt(x, y, c) = filtering<POLICY_BLACK>(img, x, y, c, filter, halfWidthFilter, halfHeightFilter);
                }
            }
        }
    }

//            double newPixel = 0;
//            for(unsigned int i = 0; i < filter->getWidth(); i++)
//            {
//                for(unsigned int j = 0; j < filter->getHeight(); j++)
//                {
//                    const int imgX = x + i - halfWidthFilter;
//                    const int imgY = y + j - halfHeightFilter;
//                    if(imgX > 0 && imgX < img->getWidth() && imgY > 0 && imgY < img->getHeight()) {
////                        newPixel += filter->getPixelAt(i,j) * ((*policy)(img, imgX, imgY, c));
//                        newPixel += filter->getPixelAt(i,j) * img->getPixelAt(imgX, imgY, c);
//                    }
//                    else {
////                        newPixel += filter->getPixelAt(i,j) * ((*policy)(img, imgX, imgY, c));
//                    }
////                    else {
////                        newPixel += filter->getPixelAt(i,j) * img->getPixelAt(x, y, channel);
////                    }
//                }
//            }
////            if(factor > 1) newPixel /= factor;
//            result->setPixel(x, y, c, newPixel);
//        }

//    }
//    for(unsigned int x = infx; x < supx; x++)
//    {
//        bool check = x < halfWidthFilter || x > (img->getWidth() - halfWidthFilter);
//        for(unsigned int y = 0; y < img->getHeight(); y++)
//        {
//            check = check || ( y < halfHeightFilter || y > (img->getHeight() - halfHeightFilter) );
//            for(unsigned int channel = 0; channel < img->getNbChannels(); channel++)
//            {
//                double newPixel = 0;

//                for(unsigned int i = 0; i < filter->getWidth(); i++)
//                {
//                    for(unsigned int j = 0; j < filter->getHeight(); j++)
//                    {
//                        const int imgX = x + i - halfWidthFilter;
//                        const int imgY = y + j - halfHeightFilter;
//                        if(imgX < 0 || imgX >= img->getWidth() || imgY <0 || imgY >= img->getHeight()) {
//                            newPixel += filter->getPixelAt(i,j) * ((*policy)(img, imgX, imgY, channel));
//                        }
//                        else {
//                            newPixel += filter->getPixelAt(i,j) * img->getPixelAt(x, y, channel);
//                        }
//                    }
//                }
//                if(factor > 1)
//                    newPixel /= factor;
//                result->setPixel(x, y, channel, newPixel);
//            }
//        }
//    }
    delete (ParallelArgs*) data;
    if(args.supl != img->getHeight() * img->getNbChannels())
        pthread_exit(NULL);
    return NULL;
}
#endif

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
