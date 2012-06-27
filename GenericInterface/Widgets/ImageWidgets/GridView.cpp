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
#include "StandardImageView.h"
#include "PixelGrid.h"
#include "ThumbnailView.h"

using namespace imagein;
using namespace genericinterface;

ChannelSelector::ChannelSelector(const QString& name, int nb)
  : QRadioButton(name), _nb(nb)
{
  if(nb == 0)
    setChecked(true);
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

GridView::GridView(const imagein::Image* im, int dx, int dy)
  : AlternativeImageView(im)
{
    _layout = new QHBoxLayout;
    //_viewer = new ImageViewer(im, dx, dy);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);
    //QGraphicsView* view = new QGraphicsView;
    //view->setScene(_viewer);

    RadioPanel* panel = new RadioPanel(im->getNbChannels());

    //layout->addWidget(view);
    ThumbnailView* view = new ThumbnailView(this, im);
    view->setFixedSize(256, 256*view->pixmap().height()/view->pixmap().width());
    layout->addWidget(view);
    layout->addWidget(panel);
    _layout->addLayout(layout);

    //ZoomViewer* zv = new ZoomViewer(im, ImageViewer::RECT_W, ImageViewer::RECT_H);
    //QGraphicsView* viewz = new QGraphicsView;
    //viewz->setScene(zv);
    //_layout->addWidget(viewz);
    PixelGrid* pixelGrid = new PixelGrid(im);
    pixelGrid->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _layout->addWidget(pixelGrid);

    QObject::connect(panel, SIGNAL(activated(int)), pixelGrid, SLOT(setChannel(int)));

    QObject::connect(view, SIGNAL(positionChanged(QPoint)), pixelGrid, SLOT(setOffset(QPoint)));
    QObject::connect(pixelGrid, SIGNAL(resized(QSize)), view, SLOT(setRectSize(QSize)));

    setLayout(_layout);
}
