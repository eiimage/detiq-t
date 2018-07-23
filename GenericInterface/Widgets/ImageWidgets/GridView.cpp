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

#include <QHBoxLayout>
#include <QGraphicsView>
#include <vector>

#include "GridView.h"
#include "ImageViewer.h"
#include "ZoomViewer.h"
#include "ImageView.h"
#include "PixelGrid.h"
#include "ThumbnailView.h"

using namespace imagein;
using namespace genericinterface;

ChannelSelector::ChannelSelector(const QString& name, int nb)
  : QRadioButton(name), _nb(nb), _name(name)
{
  if(nb == 0){
    setChecked(true);
  }
  if(nb == 0 && _name.toStdString()!="R"){
    setVisible(false);
  }
    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(send(bool)));
}

void ChannelSelector::send(bool b)
{
  if(b)
    emit activated(_nb);
}

RadioPanel::RadioPanel(int nb)
{
  std::vector<QString> names;

  names.resize(nb);

  if(nb == 3)
  {
    names[0] = tr("R");
    names[1] = tr("G");
    names[2] = tr("B");
  }

  QHBoxLayout* l = new QHBoxLayout();

  for(int i = 0; i < nb; i++)
  {
    ChannelSelector* b = new ChannelSelector(names[i], i);
    QObject::connect(b, SIGNAL(activated(int)), this, SLOT(rcvActivated(int)));
    l->addWidget(b);
  }

  setLayout(l);
}

void RadioPanel::rcvActivated(int a)
{
  emit activated(a);
}

GridView::GridView(const imagein::Image* im)
{
    _layout = new QHBoxLayout(this);

    QWidget* leftWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(leftWidget);

    RadioPanel* panel = new RadioPanel(im->getNbChannels());

    ThumbnailView* view = new ThumbnailView(this, im);

    if(view->pixmap().height()<view->pixmap().width())
        view->setFixedSize(256, 256*view->pixmap().height()/view->pixmap().width());
    else
        view->setFixedSize(256*view->pixmap().width()/view->pixmap().height(), 256);

    layout->addWidget(view);
    layout->addWidget(panel);
    _layout->addWidget(leftWidget);

    PixelGrid* pixelGrid = new PixelGrid(im);
    pixelGrid->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _layout->addWidget(pixelGrid);

    QObject::connect(panel, SIGNAL(activated(int)), pixelGrid, SLOT(setChannel(int)));

    QObject::connect(view, SIGNAL(positionChanged(QPoint)), pixelGrid, SLOT(setOffset(QPoint)));
    QObject::connect(pixelGrid, SIGNAL(resized(QSize)), view, SLOT(setRectSize(QSize)));
    QObject::connect(pixelGrid, SIGNAL(originMoved(QPoint)), view, SLOT(setRectPos(QPoint)));

}

GridView::GridView(const imagein::Image_t<double>* dataImg, const imagein::Image* displayImg)
{
    _layout = new QHBoxLayout(this);

    QWidget* leftWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(leftWidget);

    RadioPanel* panel = new RadioPanel(dataImg->getNbChannels());

    ThumbnailView* view = new ThumbnailView(this, displayImg);
    if(view->pixmap().height()<view->pixmap().width())
        view->setFixedSize(256, 256*view->pixmap().height()/view->pixmap().width());
    else
        view->setFixedSize(256*view->pixmap().width()/view->pixmap().height(), 256);
    layout->addWidget(view);
    layout->addWidget(panel);
    _layout->addWidget(leftWidget);

    PixelGrid* pixelGrid = new DoublePixelGrid(dataImg, displayImg);
    pixelGrid->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _layout->addWidget(pixelGrid);

    QObject::connect(panel, SIGNAL(activated(int)), pixelGrid, SLOT(setChannel(int)));

    QObject::connect(view, SIGNAL(positionChanged(QPoint)), pixelGrid, SLOT(setOffset(QPoint)));
    QObject::connect(pixelGrid, SIGNAL(resized(QSize)), view, SLOT(setRectSize(QSize)));
    QObject::connect(pixelGrid, SIGNAL(originMoved(QPoint)), view, SLOT(setRectPos(QPoint)));

}
