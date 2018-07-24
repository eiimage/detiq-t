#ifndef SQUARE_SUM_H
#define SQUARE_SUM_H

#include <algorithm>
#include <functional>
#include <cmath>

#include "../PixelAlgorithm.h"

namespace imagein {
    namespace algorithm {

    /*!
     * \brief Difference<I, A> is a PixelAlgorithm which perform the difference of 2 images
     * \tparam I A templated class representing an Image type such as RgbImage_t. This classe will take the first template parameter D as its template parameter.
     */
        template <class I, unsigned int A>
        class SquareSum : public PixelAlgorithm_t<I, A> {
        private:
            typedef typename I::depth_t D;
        
        protected:

            /*!
             * \brief When a PixelAlgorithm is applied, the pixelOp method is called to determine the value of each pixel of the resulting image
             */
            D pixelOp(D pixel[A]) const
            {
                D result = pow(pixel[0], 2 );
                for(unsigned int i=1; i < A; i++)
                {
                    result += pow(pixel[i], 2 );
                }
                return sqrt(result);
            }
        };
    }
}
#endif //!SQUARE_SUM_H
