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

#include <qpen.h>
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_histogram.h>
#include <qwt_legend.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>

#include "GenericHistogramWindow.h"

using namespace genericinterface;
using namespace imagein;

GenericHistogramWindow::GenericHistogramWindow(GenericHistogramView* view) : _view(view)
{
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(_view);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setAlignment(Qt::AlignCenter);

    initStatusBar();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(_view->getGraphicalHistogram());
    layout->addWidget(_statusBar);
    this->setLayout(layout);

    connect(_view, SIGNAL(leftClickedValue(unsigned int, std::vector<int>)), this, SLOT(showLeftClickedValue(unsigned int, std::vector<int>)));
    connect(_view, SIGNAL(rightClickedValue(unsigned int, std::vector<int>)), this, SLOT(showRightClickedValue(unsigned int, std::vector<int>)));
    connect(_view, SIGNAL(hoveredValue(unsigned int, std::vector<int>)), this, SLOT(showHoveredValue(unsigned int, std::vector<int>)));
}

GenericHistogramWindow::~GenericHistogramWindow()
{
    delete _view;
}

void GenericHistogramWindow::activated()
{
    emit(selectRectChange(_view->getApplicationArea(), _view));
}

void GenericHistogramWindow::initStatusBar()
{
	QFont font;
    _statusBar = new QStatusBar();
    
    /*_lImageName = new QLabel(QString::fromStdString("Image: ") + ImageWindow::getTitleFromPath(_path));
    font = _lImageName->font();
    font.setPointSize(8);
    font.setBold(true);
    _lImageName->setFont(font);*/
    
    _lHoveredValue = new QLabel(tr("Hovered") + " : ");
    font = _lHoveredValue->font();
    font.setPointSize(8);
    _lHoveredValue->setFont(font);
    
    _lSelectedValue1 = new QLabel(tr("Value 1") + " : ");
    font = _lSelectedValue1->font();
    font.setPointSize(8);
    _lSelectedValue1->setFont(font);
    
    _lSelectedValue2 = new QLabel(tr("Value 2") + " : ");
    font = _lSelectedValue2->font();
    font.setPointSize(8);
    _lSelectedValue2->setFont(font);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
    QWidget* widget = new QWidget();
	
	QHBoxLayout* layout1 = new QHBoxLayout();
	layout1->setContentsMargins(0, 0, 0, 0);
    QWidget* widget1 = new QWidget();
	/*layout1->addWidget(_lImageName);*/
	layout1->addSpacing(15);
	layout1->addWidget(_lHoveredValue);
    widget1->setLayout(layout1);
    layout->addWidget(widget1);
	
	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->setContentsMargins(0, 0, 0, 0);
    QWidget* widget2 = new QWidget();
	layout2->addWidget(_lSelectedValue1);
	layout2->addSpacing(15);
	layout2->addWidget(_lSelectedValue2);
    widget2->setLayout(layout2);
    layout->addWidget(widget2);
    
    widget->setLayout(layout);
	
    _statusBar->addWidget(widget);
}

void GenericHistogramWindow::showHoveredValue(unsigned int index, std::vector<int> values) const
{
	_lHoveredValue->setText(tr("Hovered") + QString(" : %1\t").arg(index) + formatValues(values));
}

void GenericHistogramWindow::showLeftClickedValue(unsigned int index, std::vector<int> values) const
{
	std::ostringstream oss;
    oss << index;
    std::string xs = oss.str();
	_lSelectedValue1->setText(tr("Value 1") + QString(" : %1\t").arg(index) + formatValues(values));
}

void GenericHistogramWindow::showRightClickedValue(unsigned int index, std::vector<int> values) const
{
	_lSelectedValue2->setText(tr("Value 2") + QString(" : %1\t").arg(index) + formatValues(values));
}

QString GenericHistogramWindow::formatValues(std::vector<int> values) const
{
	QString res;

    switch(values.size()) {
        case 1 : res = tr("C: %1"); break;
        case 2 : res = tr("C: %1, A: %2"); break;
        case 3 : res = tr("R: %1, G: %2, B: %3"); break;
        default: res = tr("R: %1, G: %2, B: %3, A: %4");
    }
    
    for(unsigned int i = 0; i<values.size() && i<4; ++i) {
        res = res.arg(values[i]);
    }
  
	return res;
}
