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

#ifndef QTINTERFACE_UTILITYSERVICE_H
#define QTINTERFACE_UTILITYSERVICE_H

#include "../GenericInterface.h"
#include "Widgets/ImageWidgets/DoubleImageWindow.h"
#include <QMenu>

namespace genericinterface
{
    class UtilityService : public QObject, public Service
    {
        Q_OBJECT
        public:
            void display(GenericInterface*);
            void connect(GenericInterface*);

        public slots:

            //These are the slots for actions on StandardImageWindow
            void showHistogram();
            void showHProjectionHistogram();
            void showVProjectionHistogram();
            void showPixelsGrid();
            void showCumulHistogram();
            void showLineProfile();
            void showColumnProfile();
            void convertToBinary();
            void convertToGrayscale();

            void checkActionsValid(const QWidget* activeWidget);
        signals:

        private:
            WindowService *_currentWindowService;
            QMenu* _showMenu;
            QMenu* _colorsMenu;

            QAction* _showHistogram;
            QAction* _showHProjectionHistogram;
            QAction* _showVProjectionHistogram;
            QAction* _showPixelsGrid;
            QAction* _showCumulHistogram;
            QAction* _showLineProfile;
            QAction* _showColumnProfile;
            QAction* _showConvertToBin;
            QAction* _showConvertToGrayscale;
    };
}

#endif
