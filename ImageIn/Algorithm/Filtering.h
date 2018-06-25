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

#ifndef FILTRAGE_H
#define FILTRAGE_H

#include <vector>
#include <pthread.h>

#include "../Image.h"
#include "../Algorithm.h"
#include "Filter.h"

namespace imagein
{
	namespace algorithm
	{		
        class Filtering : public Algorithm_t<Image_t<double>, 1>
        {
            public:
            enum Policy { POLICY_BLACK, POLICY_MIRROR, POLICY_NEAREST, POLICY_TOR};
//            typedef double (*Policy)(const Image_t<double>*, const int&, const int&, const int&);
			
		public:
			Filtering(Filter* filter);
			Filtering(std::vector<Filter*> filters);
			Filtering(const Filtering& f) : _filters(f._filters), _policy(f._policy) {}
  
			inline void setPolicy(Policy policy) { _policy = policy; }
			
			static Filtering uniformBlur(int numPixels);
			static Filtering gaussianBlur(double alpha);
            static Filtering gaussianBlur(int size, double sigma);
            static Filtering prewitt(int numPixels);
			static Filtering roberts();
			static Filtering sobel();
			static Filtering squareLaplacien();

			double filterPixel(const Image_t<double>* img, int x, int y, int c, Filter* filter, int hwf, int hhf);

            double blackPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel);
            double mirrorPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel);
            double nearestPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel);
			double sphericalPolicy(const Image_t<double>* img, const int& x, const int& y, const int& channel);
			
		protected:

            Image_t<double>* algorithm(const std::vector<const Image_t<double>*>& imgs);
		
		private:
			std::vector<Filter*> _filters;
			Policy _policy;
		};
	}
}

#endif //FILTRAGE_H

