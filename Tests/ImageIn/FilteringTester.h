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

#ifndef FILTERINGTESTER_H
#define FILTERINGTESTER_H

#include "Tester.h"
#include "FilteringTest.h"
#include <Algorithm/Filtering.h>

using namespace imagein;
using namespace imagein::algorithm;


class FilteringTester : public Tester {
  public:
    typedef depth_default_t D;
    FilteringTester() : Tester("Filtering") {       
        
    }

    void init() {
        ImageDiff<D> nodiff(0, 0, 0);
        
        Filtering* gaussian = new Filtering(Filter::gaussian(3));
        addTest(new FilteringTest<D>("Gaussian blur", gaussian, "res/lena.png", "res/lena_gaussian_16.png", nodiff));
    }

    void clean() {
    }
};


#endif //!FILTERINGTESTER_H
