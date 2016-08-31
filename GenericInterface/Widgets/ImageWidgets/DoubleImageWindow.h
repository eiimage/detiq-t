/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of DETIQ-T.
 *
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DOUBLEIMAGEWINDOW_H
#define DOUBLEIMAGEWINDOW_H

#include "ImageWindow.h"

#include <Converter.h>
#include <QDoubleSpinBox>


namespace genericinterface
{
/*!
 * \brief Store and display a double precision floating point Image (ie Image_t<double>)
 *
 * Creates and display the ImageView, and update the status bar.
 * Store the double Image and make a displayable version as a standard Image.
 */
class DoubleImageWindow : public ImageWindow
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor based on an Image already openned
     *
     * Display DoubleImageWindow with ImageView from image.
     *
     * \param path The image path (to attach to the right set)
     * \param gi The interface associated with this
     * \param image The image which is used
     */
    DoubleImageWindow(Image_t<double>* image, const QString path = QString(), bool normalize = false, bool logScale = false, double logConstantScale = 1.0, bool abs = false);
    DoubleImageWindow(const DoubleImageWindow&, imagein::Image_t<double>* img = NULL);


    /*!
     * \brief DoubleImageWindow destructor.
     *
     * The ImageView is deleted too.
     */
    virtual ~DoubleImageWindow();


    /*!
     * \brief Returns the real Image contained in the window.
     *
     */
    inline const imagein::Image_t<double>* getImage() const { return _image; }

    imagein::Image* makeDisplayable(const imagein::Image_t<double>*);
    virtual bool isDouble() const { return true; }
    virtual bool isStandard() const { return false; }
    inline bool isNormalized() const { return _normalize; }
    inline bool isLogScaled() const { return _logScale; }
    inline double getLogScale() const  { return _logConstantScale; }
    inline bool isAbsolute() const { return _abs; }


public slots:

    virtual void crop();
    virtual void copycrop();
    void showPixelsGrid();

    virtual void showHoveredPixelInformations(int x, int y) const;
    virtual void showSelectedPixelInformations(int x, int y) const;

    virtual void updateSrc(GenericHistogramView*, imagein::Rectangle);

    virtual void setLogScale(int);

    virtual void showHistogram();

    void convertRgb();

signals:

protected:
    const imagein::Image_t<double>* _image;

    bool _normalize;
    bool _logScale;
    double _logConstantScale;
    bool _abs;

    void init();
    void updateStatusBar();

    QLabel* _lHoveredPixelInfo;
    QLabel* _lHoveredPixelPosition;
    QLabel* _lHoveredPixelColor;
    QLabel* _lSelectedPixelInfo;
    QLabel* _lSelectedPixelPosition;
    QLabel* _lSelectedPixelColor;
    QDoubleSpinBox* _logBox;
};
}


#endif // DOUBLEIMAGEWINDOW_H
