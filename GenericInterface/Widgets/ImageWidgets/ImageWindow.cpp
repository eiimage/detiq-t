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

#include "ImageWindow.h"

using namespace genericinterface;
using namespace imagein;

ImageWindow::ImageWindow(QString path, Rectangle rect): QWidget(), _path(path)
{
    _applicationArea = rect;
    _statusBar = new QStatusBar();
}

void ImageWindow::activated()
{
    emit(selectRectChange(_applicationArea, this));
    std::cout << "selectRectChange" << std::endl;
}

void ImageWindow::setApplicationArea(imagein::Rectangle rect)
{
    _applicationArea = rect;
}

QString ImageWindow::getTitleFromPath(QString path)
{
    std::string p = path.toStdString();
    size_t pos = p.rfind("\\");

    if(pos != std::string::npos)
        p = p.substr(pos + 1);
    else if((pos = p.rfind("/")) != std::string::npos)
        p = p.substr(pos + 1);

    return QString::fromStdString(p);
}

