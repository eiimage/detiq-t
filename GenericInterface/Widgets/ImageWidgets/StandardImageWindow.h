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

#ifndef STANDARDIMAGEWINDOW_H
#define STANDARDIMAGEWINDOW_H

#include "ImageWindow.h"
#include "RowWindow.h"
#include "HistogramWindow.h"
#include "ProjectionHistogramWindow.h"
#include "GridWindow.h"

#include <Histogram.h>

#include <Converter.h>


namespace genericinterface
{
class GraphicalHistogram;
/*!
 * \brief Store and display a standard Image (ie Image_t<unsigned char>)
 *
 * Creates and display the ImageView, and update the status bar.
 */
class StandardImageWindow : public ImageWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Default constructor
     *
     * Display StandardImageWindow with StandardImageView from image path.
     *
     * \param path The image path
     * \param gi The interface associated with this
     */
    StandardImageWindow(const QString path);

    /*!
     * \brief Constructor based on an Image already openned
     *
     * Display StandardImageWindow with StandardImageView from image path.
     *
     * \param path The image path (to attach to the right set)
     * \param gi The interface associated with this
     * \param image The image which is used
     */
    StandardImageWindow(const QString path, Image* image);
    StandardImageWindow(const StandardImageWindow&, imagein::Image* img = NULL);


    /*!
     * \brief StandardImageWindow destructor.
     *
     * The StandardImageView is deleted too.
     */
    virtual ~StandardImageWindow();

//    /*!
//     * \brief Changes the Image contained in the window.
//     *
//     * \param image The image we want to put in the window.
//     */
//    void setImage(Image* image);

    /*!
     * \brief Returns the real Image contained in the window.
     *
     */
    inline const imagein::Image* getImage() const { return _image; }


public slots:
    void showHistogram();
    void showHProjectionHistogram();
    void showVProjectionHistogram();
    void showPixelsGrid();
    void showLineProfile();
    void showColumnProfile();

    void convertToGrayscale();
    void convertToBinary();

    virtual void crop();
    virtual void copycrop();

    virtual void showHoveredPixelInformations(int x, int y) const;
    virtual void showSelectedPixelInformations(int x, int y) const;

    virtual void updateSrc(GenericHistogramView*, imagein::Rectangle);

protected:
    const imagein::Image* _image;

    void init();
    void updateStatusBar();
    void showGenericHistogram(GenericHistogramWindow* histogramWnd);

    QLabel* _lHoveredPixelInfo;
    QLabel* _lHoveredPixelPosition;
    QLabel* _lHoveredPixelColor;
    QLabel* _lSelectedPixelInfo;
    QLabel* _lSelectedPixelPosition;
    QLabel* _lSelectedPixelColor;
};
}

#endif // STANDARDIMAGEWINDOW_H
