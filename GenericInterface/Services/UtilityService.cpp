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

#include "UtilityService.h"

using namespace genericinterface;
using namespace imagein;

void UtilityService::display(GenericInterface* gi)
{
    _currentWindowService = gi->windowService();

    //ajout des actions dans le menu show
    _showMenu = gi->menu(tr("&Show"));
    _showHistogram = _showMenu->addAction(tr("Histogram"));
    _showCumulHistogram = _showMenu->addAction(tr("Cumulated Histogram"));
    _showMenu->addSeparator();
    _showHProjectionHistogram = _showMenu->addAction(tr("Horizontal projection histogram"));
    _showVProjectionHistogram = _showMenu->addAction(tr("Vertical projection histogram"));
    _showMenu->addSeparator();
    _showLineProfile = _showMenu->addAction(tr("Line Profile"));
    _showColumnProfile = _showMenu->addAction(tr("Column Profile"));
    _showMenu->addSeparator();
    _showPixelsGrid = _showMenu->addAction(tr("Pixels grid"));

    _colorsMenu = gi->menu(tr("Colors"));
    _showConvertToBin = _colorsMenu->addAction(tr("Convert to Binary"));
    _showConvertToGrayscale = _colorsMenu->addAction(tr("Convert to Grayscale"));

    _showLineProfile->setEnabled(false);
    _showColumnProfile->setEnabled(false);
    _showCumulHistogram->setEnabled(false);
    _showHistogram->setEnabled(false);
    _showHProjectionHistogram->setEnabled(false);
    _showVProjectionHistogram->setEnabled(false);
    _showPixelsGrid->setEnabled(false);
    _showConvertToBin->setEnabled(false);
    _showConvertToGrayscale->setEnabled(false);



}

void UtilityService::connect(GenericInterface*)
{
    // If, for any reason, _currentWindowService is NULL, do not connect anything
    if(_currentWindowService) {
        //Connexion des actions
        QObject::connect(_showHistogram, SIGNAL(triggered()), this, SLOT(showHistogram()));
        QObject::connect(_showHProjectionHistogram, SIGNAL(triggered()), this, SLOT(showHProjectionHistogram()));
        QObject::connect(_showVProjectionHistogram, SIGNAL(triggered()), this, SLOT(showVProjectionHistogram()));
        QObject::connect(_showPixelsGrid, SIGNAL(triggered()), this, SLOT(showPixelsGrid()));
        QObject::connect(_showCumulHistogram, SIGNAL(triggered()), this, SLOT(showCumulHistogram()));
        QObject::connect(_showLineProfile, SIGNAL(triggered()), this, SLOT(showLineProfile()));
        QObject::connect(_showColumnProfile, SIGNAL(triggered()), this, SLOT(showColumnProfile()));
        QObject::connect(_showConvertToBin, SIGNAL(triggered()), this, SLOT(convertToBinary()));
        QObject::connect(_showConvertToGrayscale, SIGNAL(triggered()), this, SLOT(convertToGrayscale()));

        //connexion des changements d'images
        QObject::connect(_currentWindowService, SIGNAL(activeWidgetChanged(const QWidget*)), this, SLOT(checkActionsValid(const QWidget*)));
    }
}

void UtilityService::convertToBinary()
{
    StandardImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<StandardImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->convertToBinary();
    }

}

void UtilityService::convertToGrayscale()
{
    StandardImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<StandardImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->convertToGrayscale();
    }

}

void UtilityService::showLineProfile()
{
    StandardImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<StandardImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->showLineProfile();
    }

}

void UtilityService::showColumnProfile()
{
    StandardImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<StandardImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->showColumnProfile();
    }

}

void UtilityService::showCumulHistogram()
{
    StandardImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<StandardImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->showCumulatedHistogram();
    }

}

void UtilityService::showHistogram()
{
    ImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<ImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->showHistogram();
    }
}

void UtilityService::showHProjectionHistogram()
{
    StandardImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<StandardImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->showHProjectionHistogram();
    }
}

void UtilityService::showVProjectionHistogram()
{
    StandardImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<StandardImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->showVProjectionHistogram();
    }
}

void UtilityService::showPixelsGrid()
{
    ImageWindow* curWindow = NULL;
    if(curWindow = dynamic_cast<ImageWindow*>(_currentWindowService->getCurrentImageWindow())) {
        curWindow->showPixelsGrid();
    }
}

void UtilityService::checkActionsValid(const QWidget* activeWidget)
{
    const ImageWindow* window = dynamic_cast<const ImageWindow*>(activeWidget);
    if(window) {
        window->menu()->addAction(_showHistogram);
        window->menu()->addSeparator();
        window->menu()->addAction(_showPixelsGrid);

        // Histogram & Pixel grids can be displayed for DoubleImage AND
        // StandardImage windows
        _showHistogram->setEnabled(true);
        _showPixelsGrid->setEnabled(true);

        // Projections can only be displayed for standard images
        if(dynamic_cast<const StandardImageWindow*>(window)) {
            _showHProjectionHistogram->setEnabled(true);
            _showVProjectionHistogram->setEnabled(true);
            _showCumulHistogram->setEnabled(true);
            _showLineProfile->setEnabled(true);
            _showColumnProfile->setEnabled(true);
            _showConvertToBin->setEnabled(true);
            _showConvertToGrayscale->setEnabled(true);
        } else {
            _showHProjectionHistogram->setEnabled(false);
            _showVProjectionHistogram->setEnabled(false);
            _showCumulHistogram->setEnabled(false);
            _showLineProfile->setEnabled(false);
            _showColumnProfile->setEnabled(false);
            _showConvertToBin->setEnabled(false);
            _showConvertToGrayscale->setEnabled(false);
        }
    }
    else {
        _showHistogram->setEnabled(false);
        _showHProjectionHistogram->setEnabled(false);
        _showVProjectionHistogram->setEnabled(false);
        _showPixelsGrid->setEnabled(false);
        _showCumulHistogram->setEnabled(false);
        _showLineProfile->setEnabled(false);
        _showColumnProfile->setEnabled(false);
        _showConvertToBin->setEnabled(false);
        _showConvertToGrayscale->setEnabled(false);
    }
}
