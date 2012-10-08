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

#ifndef VFFIMAGE_H
#define VFFIMAGE_H

#include "ImageFile.h"

namespace imagein
{
    //! ImageFile subclass for VFF files. See ImageFile for details.
    class VffImage : public ImageFile
    {
        public:
            VffImage(std::string filename);

            inline unsigned int readHeight();
            inline unsigned int readWidth();
            inline unsigned int readNbChannels();
            inline unsigned int readDepth();
            void* readData();

            void writeData(const void* const data, unsigned int width, unsigned int height, unsigned int, unsigned int depth);

    };
}

#endif // VFFIMAGE_H
