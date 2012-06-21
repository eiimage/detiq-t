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

#ifndef ROWWINDOW_H
#define ROWWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

#include <sstream>

#include "GenericHistogramWindow.h"
#include "RowView.h"

#include "../../GenericInterface.h"

#include <Image.h>

namespace genericinterface
{
	/*!
   * \brief Contains the RowView
   *
   * Creates and display the RowView, and update the status bar.
   */
  class RowWindow : public GenericHistogramWindow
  {
  Q_OBJECT
  private:
        
  public:
		/*!
		 * \brief Default constructor
		 * 
		 * Initializes and display the RowView from the parameters. 
		 * 
		 * \param image The image concerned by the histogram
		 * \param rect The part of the image where the histogram is applied
		 * \param vertical False : row profile ; True : column profile
		 */
    RowWindow(const imagein::Image* image, imagein::Rectangle rect, bool vertical, QString name = "");
    
		
		/*!
		 * \brief RowWindow destructor.
		 *
		 * The RowView is deleted too
		 */
		virtual ~RowWindow();
  };
}

#endif // ROWWINDOW_H
