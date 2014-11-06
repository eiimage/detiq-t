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

#ifndef GENERICHISTOGRAMVIEW_H
#define GENERICHISTOGRAMVIEW_H

#include <QMouseEvent>

#include <stdlib.h>
#include <vector>

#include <Image.h>
#include <Rectangle.h>
#include <Histogram.h>
#include <ProjectionHistogram.h>

#include "AlternativeImageView.h"

#include <qwt_legend_data.h>

class QwtPlot;
class QwtPlotItem;

namespace genericinterface
{
class GraphicalHistogram;
class HistogramPicker;
/*!
 * \brief Create and display a graphical histogram
 *
 * Creates and display a graphical histogram from an image and a rectangle.
 * This class manages via HistogramPicker the mouse events.
 */
class GenericHistogramView : public QWidget
{
    Q_OBJECT


public:
    /*!
     * \brief Default constructor
     *
     * Initializes and display the histogram from the parameters.
     *
     * \param image The image concerned by the histogram
     * \param rect The part of the image where the histogram is applied
     */
    GenericHistogramView(const imagein::Image* image, imagein::Rectangle rect, bool horizontal=false, int value=0, bool projection=false, bool cumulated = false);

    /*!
     * \brief GenericHistogramView destructor.
     *
     * HistogramPicker are deleted
     */
    virtual ~GenericHistogramView();

    void update(const imagein::Image* image, imagein::Rectangle rect);
    
    //! Returns the image's histogram on the param channel
    //inline const imagein::Histogram* getHistogram(int channel) const { return new imagein::Histogram(*_image, channel, _rectangle); }

    //! Returns the graphical histogram
    inline QwtPlot* getGraphicalHistogram() const { return _qwtPlot; }
    inline imagein::Rectangle getApplicationArea() const { return _rectangle; }

signals:
    /*!
     * \brief Signal emits when the mouse left button is clicked
     *
     * \param value Value selected
     */
    void leftClickedValue(unsigned int index, std::vector<int> values) const;

    /*!
     * \brief Signal emits when the mouse right button is clicked
     *
     * \param value Value selected
     */
    void rightClickedValue(unsigned int index, std::vector<int> values) const;

    /*!
     * \brief Signal emits when the mouse move over the histogram
     *
     * \param value Value hovered
     */
    void hoveredValue(unsigned int index, std::vector<int> values) const;
    
    void updateApplicationArea(imagein::Rectangle rect) const;

    void leftMoved(const QPointF&) const;
    void rightMoved(const QPointF&) const;
    void leftSelected(const QPointF&) const;
    void rightSelected(const QPointF&) const;

private slots:
    void showItem(QVariant, bool, int) const;
    void move(const QPointF&) const;
    void leftClick(const QPointF&) const;
    void rightClick(const QPointF&) const;

protected:
    imagein::Rectangle _rectangle;
    bool _horizontal;
    int _value;
    QwtPlot* _qwtPlot;
    HistogramPicker* _principalPicker;
    HistogramPicker* _leftPicker;
    HistogramPicker* _rightPicker;
    std::vector<GraphicalHistogram*> _graphicalHistos;
    unsigned int nChannel;
    std::vector<int> getValues(unsigned int index) const;

private:
    bool _projection;
    bool _cumulated;
    void init();
    void populate(const imagein::Image*);
};
}

#endif // GENERICHISTOGRAMVIEW_H
