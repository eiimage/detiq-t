#include <QVBoxLayout>

#include "GridWindow.h"
#include "GridView.h"
#include "ZoomViewer.h"
#include "../ImageWidgets/StandardImageWindow.h"

using namespace imagein;
using namespace genericinterface;

GridWindow::GridWindow(const imagein::Image* img, ImageWindow* source) : ImageWindow("", source)
{
  QVBoxLayout* box = new QVBoxLayout;

  _view = new GridView(img, 0, 0);

  box->addWidget(_view);
  
  this->setWindowTitle(source->windowTitle() + QString::fromStdString(" - Pixels Grid"));

  setLayout(box);
}
