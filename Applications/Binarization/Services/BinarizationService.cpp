#include "BinarizationService.h"

#include <GenericInterface.h>

#include <QMessageBox>

using namespace genericinterface;


void BinarizationService::display(GenericInterface* gi)
{
    AlgorithmService::display(gi);

    _binarize = _toolBar->addAction("&Binarization");

    QAction* actionOtsu = gi->menu("?")->addAction("About Otsu");
    QObject::connect(actionOtsu, SIGNAL(triggered()), this, SLOT(aboutOtsu()));
}

void BinarizationService::connect(GenericInterface* gi)
{
    AlgorithmService::connect(gi);

    QObject::connect(_binarize, SIGNAL(triggered()), this, SLOT(applyBinarization()));
}


void BinarizationService::applyBinarization()
{
    StandardImageWindow* siw = dynamic_cast<StandardImageWindow*>(_ws->getCurrentImageWindow());
    if (siw != NULL)
    {
        if (siw->getImage()->getNbChannels() != 1)
        {
            QMessageBox::information(siw,
                                     "Attention",
                                     "You want to binarize a non grayscale image, it's impossible. You should first transform it.");
        }
        else
        {
            NodeId nodeId = _ws->getNodeId(siw);
            _binWidget = new BinarizationWidget(siw, nodeId);
            QObject::connect(_binWidget, SIGNAL(exportBinarizedImage(NodeId,Image*)),
                             this, SLOT(exportBinarizedImage(NodeId,Image*)));
            _ws->addWidget(nodeId, _binWidget);
        }
    }
}

void BinarizationService::exportBinarizedImage(NodeId nodeId, Image* im)
{
    const Node* node = _ws->getNode(nodeId);
    StandardImageWindow* new_siw = new StandardImageWindow(node->path, _gi, im);
    new_siw->setWindowTitle(ImageWindow::getTitleFromPath(node->path));
    emit newImageWindowCreated(new_siw->getImage(), new_siw);
}

void BinarizationService::aboutOtsu()
{
    QString text = "Otsu's method, named after its inventor Nobuyuki Otsu, is one of many binarization algorithms.<br />";
    text += "For more information, see <a href=\"http://en.wikipedia.org/wiki/Otsu's_method\"> Wikipedia </a>.";
    QMessageBox::information(_gi, "Otsu's method", text);
}
