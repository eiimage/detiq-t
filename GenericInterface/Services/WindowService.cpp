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

#include "WindowService.h"

#include <QListIterator>
#include <QErrorMessage>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include "../Widgets/ImageListBox.h"
#include "../Widgets/ImageWidgets/StandardImageWindow.h"
#include "../Widgets/ImageWidgets/DoubleImageWindow.h"

using namespace std;
using namespace genericinterface;
using namespace imagein;
WindowService::WindowService(Qt::DockWidgetArea navPos) : _navPos(navPos),  _mutex(QMutex::Recursive) {

}

void WindowService::display(GenericInterface* gi)
{
    _mdi = gi->initCentralWidget();
    _mdi->setActivationOrder(QMdiArea::CreationOrder);
    _nav = new NavigationDock(tr("Images"), gi);
    gi->addDockWidget(_navPos, _nav);
    
    QAction* tile = gi->menu(tr("&Window"))->addAction(tr("&Tile windows"));
    tile->setIcon(QIcon(":/images/application-view-tile.png"));
    //tile->setShortcut(QKeySequence::Open);
    gi->toolBar(GenericInterface::MAIN_TOOLBAR)->addAction(tile);
	QObject::connect(tile, SIGNAL(triggered()), _mdi, SLOT(tileSubWindows()));
    
    QAction* cascade = gi->menu(tr("&Window"))->addAction(tr("&Cascade windows"));
    cascade->setIcon(QIcon(":/images/application-cascade.png"));
    //tile->setShortcut(QKeySequence::Open);
    gi->toolBar(GenericInterface::MAIN_TOOLBAR)->addAction(cascade);
	QObject::connect(cascade, SIGNAL(triggered()), _mdi, SLOT(cascadeSubWindows()));

    QAction* dockimage = gi->menu(tr("&Window"))->addAction(tr("Hide/Show Image dock"));
    dockimage->setIcon(QIcon(":/images/application-dock.png"));
    gi->toolBar(GenericInterface::MAIN_TOOLBAR)->addAction(dockimage);
        QObject::connect(dockimage, SIGNAL(triggered()), _nav, SLOT(hideshow()));
}

void WindowService::connect(GenericInterface* gi)
{
    _gi = gi;
    QObject::connect(_nav, SIGNAL(actionDone()), this, SLOT(updateDisplay()));
    QObject::connect(_nav, SIGNAL(removeId(NodeId)), this, SLOT(removeId(NodeId)));
    QObject::connect(_nav, SIGNAL(windowDropped(StandardImageWindow*, int)), 
                        this, SLOT(moveToNode(StandardImageWindow*, int)));
    QObject::connect(_mdi, SIGNAL(subWindowActivated(QMdiSubWindow*)), 
                        this, SLOT(swActivated(QMdiSubWindow*)));
}

ImageWindow* WindowService::getCurrentImageWindow()
{
    QMdiSubWindow* sw = _mdi->currentSubWindow();

    if(sw != NULL) {
        ImageWindow* imw = dynamic_cast<ImageWindow*>(sw->widget());
        return imw;
    }
    else {
        qDebug ("No current window !");
        return NULL;
    }
}

GenericHistogramWindow* WindowService::getCurrentHistogramWindow()
{
    QMdiSubWindow* sw = _mdi->currentSubWindow();

    if(sw != NULL) {
        GenericHistogramWindow* imw = dynamic_cast<GenericHistogramWindow*>(sw->widget());
        return imw;
    }
    else {
        qDebug ("No current window !");
        return NULL;
    }
}

std::vector<const ImageWindow*> WindowService::getImageWindows() const {
    vector<const ImageWindow*> result;
    QList<QMdiSubWindow*> windows = _mdi->subWindowList();
    foreach(QMdiSubWindow* sw, windows) {
        const ImageWindow* siw = dynamic_cast<const ImageWindow*>(sw->widget());
        if(siw != NULL) {
            result.push_back(siw);
        }
    }
    return result;
}
NodeId WindowService::findNodeId(QMdiSubWindow* sw) const {
    QMutexLocker locker(&_mutex);
    for(std::map<NodeId, Node*>::const_iterator it = _widgets.begin() ; it != _widgets.end() ; ++it)
    {
        if(it->second->windows.contains(sw)) {
            return it->first;
        }
    }
    return NodeId();
}
    
void WindowService::swActivated(QMdiSubWindow* sw) {
    
    QWidget* widget = sw ? sw->widget() : NULL;
    emit activeWidgetChanged(widget);
    
    NodeId id = findNodeId(sw);
    if(id != NodeId()) {
        //_nav->setActiveNode(id);
    }
}

NodeId WindowService::getNodeId(QWidget* widget) const
{
    QMutexLocker locker(&_mutex);
    for(std::map<NodeId, Node*>::const_iterator it = _widgets.begin() ; it != _widgets.end() ; ++it)
    {
        const NodeId& id = it->first;
        const Node* node = it->second;
        for(QList<QMdiSubWindow*>::const_iterator it = node->windows.begin(); it != node->windows.end(); ++it) {
            if((*it)->widget() == widget) {
                return id;
            }
        }
    }
    return NodeId();
}

Node* WindowService::findNode(NodeId id) {
    //std::cout << "findNode(id)::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    std::map<NodeId, Node*>::iterator it = _widgets.find(id);
    if(it == _widgets.end()) {
        return NULL;
    }
    return it->second;
}

const Node* WindowService::getNode(NodeId id) const {
    //std::cout << "getNode(id)::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    std::map<NodeId, Node*>::const_iterator it = _widgets.find(id);
    if(it == _widgets.end()) {
        return NULL;
    }
    return it->second;
}

const Node* WindowService::getNode(QWidget* widget) const {
    //std::cout << "getNode(widget)::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    for(std::map<NodeId, Node*>::const_iterator it = _widgets.begin() ; it != _widgets.end() ; ++it)
    {
        const Node* node = it->second;
        for(QList<QMdiSubWindow*>::const_iterator it = node->windows.begin(); it != node->windows.end(); ++it) {
            if((*it)->widget() == widget) {
                return node;
            }
        }
    }
    return NULL;
}

Node* WindowService::addNodeIfNeeded(NodeId id, const Image* img, QString path, int pos) {
    //std::cout << "addNodeIfNeeded::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    Node* node = findNode(id);
    if(node == NULL) {
        node = new Node(img, path);
        _widgets[id] = node;
        _nav->addNode(node, pos);
    }
    return node;
}

void WindowService::signalMdiChange() const {
    const QMdiSubWindow* sw = _mdi->currentSubWindow();
    const QWidget* widget = sw ? sw->widget() : NULL;
    emit activeWidgetChanged(widget);
}

void WindowService::addFile(const QString& path)
{
    QMutexLocker locker(&_mutex);
    StandardImageWindow* siw = new StandardImageWindow(path);
    const Image* img = siw->getImage();
    if(img->size() == 0) {
        delete siw;
    }
    else {
        this->addImage(img, siw);
    }
}

bool WindowService::windowTitleExists(const QString& title) {
    foreach(QMdiSubWindow* sw, _mdi->subWindowList()) {
        if(sw->windowTitle() == title) {
            return true;
        }
    }
    return false;
}

void WindowService::checkWindowTitle(QWidget *widget) {
    QString title = widget->windowTitle();
    if(!windowTitleExists(title)) return;
    title = title + " - %1";
    int i;
    for(i = 0; windowTitleExists(title.arg(i)); ++i);
    widget->setWindowTitle(title.arg(i));
}

void WindowService::addImage(NodeId id, ImageWindow* imgWnd, int pos)
{
    //std::cout << "addImage::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    Node* node = addNodeIfNeeded(id, imgWnd->getDisplayImage(), imgWnd->getPath(), pos);

    checkWindowTitle(imgWnd);
    QMdiSubWindow* sw = _mdi->addSubWindow(imgWnd);
    node->windows << sw;
    sw->setWindowIcon(QIcon(":/images/applications-graphics-5.png"));

    SubWindowController* swc = new SubWindowController(sw);

    QObject::connect(imgWnd, SIGNAL(addImage(ImageWindow*,ImageWindow*)), this, SLOT(addImage(ImageWindow*,ImageWindow*)));
    QObject::connect(imgWnd, SIGNAL(addWidget(ImageWindow*,QWidget*)), this, SLOT(addWidget(ImageWindow*,QWidget*)));
    QObject::connect(imgWnd, SIGNAL(applyBinaryMask(ImageWindow*)), this, SLOT(applyBinaryMask(ImageWindow*)));
    QObject::connect(sw, SIGNAL(aboutToActivate()), imgWnd, SLOT(activated()));

    QObject::connect(sw, SIGNAL(destroyed()), swc, SLOT(closeSubWindow()));
    QObject::connect(swc, SIGNAL(removeFromWindowsMap(QMdiSubWindow*)), 
                    this, SLOT(removeSubWindow(QMdiSubWindow*)));

    sw->show();
    _mdi->setActiveSubWindow(sw);
    sw->activateWindow();
    _nav->setActiveNode(id);
    //signalMdiChange();

}

void WindowService::addImage(ImageWindow* srcWnd, ImageWindow* imgWnd) {
    this->addImage(this->getNodeId(srcWnd), imgWnd);
}

bool WindowService::addWidget(NodeId id, QWidget* widget)
{
    QMutexLocker locker(&_mutex);
    Node* node = findNode(id);
    if(node == NULL) {
        return false;
    }
    
    checkWindowTitle(widget);
    QMdiSubWindow* sw = _mdi->addSubWindow(widget);
    node->windows << sw;
    sw->setWindowIcon(QIcon(":/images/applications-utilities-2.png"));

    SubWindowController* swc = new SubWindowController(sw);

    if(qobject_cast<GenericHistogramWindow*>(widget)) {
        QObject::connect(sw, SIGNAL(aboutToActivate()), widget, SLOT(activated()));
    }
    QObject::connect(sw, SIGNAL(destroyed()), swc, SLOT(closeSubWindow()));
    QObject::connect(swc, SIGNAL(removeFromWindowsMap(QMdiSubWindow*)), 
                    this, SLOT(removeSubWindow(QMdiSubWindow*)));

    sw->show();
    _mdi->setActiveSubWindow(sw);
    sw->activateWindow();
    _nav->setActiveNode(id);
    //signalMdiChange();
    return true;
}

bool WindowService::addWidget(ImageWindow* srcWnd, QWidget* widget) {
   return this->addWidget(this->getNodeId(srcWnd), widget);
}

void WindowService::addText(std::string s) {
    QMdiSubWindow* sw = _mdi->addSubWindow(new QLabel(QString::fromStdString(s)));
    sw->show();
}

void WindowService::removeSubWindow(QMdiSubWindow* sw)
{
    //std::cout << "removeSubWindow::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    NodeId id = findNodeId(sw);
    Node* node = findNode(id);
    if(node == NULL) return;
    
    //std::cout << "node->windows.removeAll(" << sw << ")" << std::endl;
    node->windows.removeAll(sw);

    if (node->windows.empty()) {
        //std::cout << "remove node" << std::endl;
        _nav->removeNode(id);
        //std::cout << "erase node from widget map" << std::endl;
        _widgets.erase(id);
        //std::cout << "delete node" << std::endl;
        delete node;
        QList<QMdiSubWindow*> swList = _mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
        if(!swList.isEmpty()) {
            QMdiSubWindow* sw = swList.last();
            sw->showNormal();
            _nav->setActiveNode(findNodeId(sw));
            _mdi->setActiveSubWindow(sw);
        }
    }
    else {
        _mdi->setActiveSubWindow(node->windows.last());
    }
    //signalMdiChange();
    //std::cout << "removeSubWindow::end" << std::endl;
    //this->updateDisplay();
    if(getCurrentImageWindow() == NULL) emit activeWidgetChanged(NULL);
}

void WindowService::removeId(NodeId id)
{
    //std::cout << "removeId::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    //std::cout << "removeId->" << std::endl;
    Node* node = findNode(id);
    if(node == NULL) return;
    
    if(node->windows.size() > 1) {
        int answer = QMessageBox::question(_gi, tr("Attention"), tr("You're going to close all the relative windows, are you sure you want to continue ?"), QMessageBox::Yes | QMessageBox::No);
        if(answer != QMessageBox::Yes) return;
    }

    //std::cout << "closing " << node->windows.size() << " windows..." << std::endl;
    
    QList<QMdiSubWindow*> windows = node->windows;
    
    for (QList<QMdiSubWindow*>::iterator it = windows.begin(); it != windows.end(); ++it)
    {
        QMdiSubWindow* sw = *it;
        if(validWindow(sw)) {
            //std::cout << "close(" << sw << ")" << std::endl;
            //std::cout << "_mdi->removeSubWindow(" << sw << ")" << std::endl;
            _mdi->removeSubWindow(sw);
            sw->close();
        }
    }
    //delete node;
    //this->updateDisplay();
    //std::cout << "<-removeId" << std::endl;
}

void WindowService::moveToNode(StandardImageWindow* siw, int pos) {
    
    //std::cout << "moveToNode::lock" << std::endl;
    QMutexLocker locker(&_mutex);

    Node* node = NULL;
    NodeId id;
    QMdiSubWindow* subWnd = NULL;
    for(std::map<NodeId, Node*>::iterator it = _widgets.begin() ; it != _widgets.end() ; ++it)
    {
        Node* currentNode = it->second;
        NodeId currentId = it->first;
        for(QList<QMdiSubWindow*>::iterator it = currentNode->windows.begin(); it != currentNode->windows.end(); ++it) {
            if((*it)->widget() == siw) {
                node = currentNode;
                id = currentId;
                subWnd = *it;
            }
        }
    }

    if(node == NULL) {
        return addImage(siw->getImage(), siw, pos);
    }
    
    node->windows.removeAll(subWnd);
    if(node->windows.empty()) {
        int index = _nav->removeNode(id);
        _widgets.erase(id);
        //std::cout << "erased node #" << index << " while droping on #" << pos << std::endl;
        if(index < pos) --pos;
        delete node;
    }


    NodeId newId(siw->getImage());
    Node* newNode = addNodeIfNeeded(newId, siw->getImage(), siw->getPath(), pos);
    newNode->windows << subWnd;
    _nav->setActiveNode(newId);
    _nav->update();

    //StandardImageWindow* newSiw = new StandardImageWindow(*siw);
    
    //QList<QMdiSubWindow*> windows = _mdi->subWindowList();
    
    //for (QList<QMdiSubWindow*>::iterator it = windows.begin(); it != windows.end(); ++it)
    //{
        //QMdiSubWindow* sw = *it;
        //if(validWindow(sw) && sw->widget() == siw) {
            //_mdi->removeSubWindow(sw);
            //sw->close();
        //}
    //}
    //addImage(newSiw->getImage(), newSiw, pos);
}

bool WindowService::validWindow(QMdiSubWindow* sw) const {
    return _mdi->subWindowList().contains(sw);
}

void WindowService::updateDisplay()
{
    //std::cout << "updateDisplay::lock" << std::endl;
    QMutexLocker locker(&_mutex);
    const QList<NodeId>& selection = _nav->getSelection();
    
    QMdiSubWindow* activeWindow = _mdi->currentSubWindow();
    QMdiSubWindow* firstValidWindow = NULL;
    
    for(std::map<NodeId, Node*>::iterator it = _widgets.begin(); it != _widgets.end(); ++it) {
    
        NodeId id = it->first;
        Node* node = it->second;
        QList<QMdiSubWindow*>& windows = node->windows;
        
        if(selection.indexOf(id) >= 0) {
            for (QList<QMdiSubWindow*>::iterator jt = windows.begin(); jt != windows.end(); ++jt) {
                if(validWindow((*jt))) {
                    if(firstValidWindow == NULL || *jt == activeWindow) firstValidWindow = *jt;
                    (*jt)->showNormal();
                }
            }
        }
        else {
            for (QList<QMdiSubWindow*>::iterator jt = windows.begin(); jt != windows.end(); ++jt) {
                if(validWindow((*jt))) {
                    (*jt)->showMinimized();
                }
            }
        }
    }
    NodeId currentId = NodeId();
    /*QMdiSubWindow* currentWindow = _mdi->currentSubWindow();
    if(currentWindow != NULL) {
        currentId = getNodeId(currentWindow->widget());
        Node* currentNode = findNode(currentId);
        //std::cout << "Current node : " << currentNode->path.toStdString() << std::endl;
        //std::cout << "Current node " << (selection.indexOf(currentId) >= 0 ? "in selection" : "not in selection") << std::endl;
    }*/
    
    if(!selection.isEmpty() && selection.indexOf(currentId) < 0) {
        //std::cout << "Raising first window in selection : " << firstValidWindow->windowTitle().toStdString() << std::endl;
        _mdi->setActiveSubWindow(firstValidWindow);
    }
    
}

void WindowService::applyBinaryMask(ImageWindow* imgWnd) {

    vector<const ImageWindow*> wndList =this->getImageWindows();
    QString currentImgName = imgWnd->windowTitle();
    map<const Image*,string> stdImgList;
    map<const Image_t<double>*,string> dblImgList;
    for(vector<const ImageWindow*>::const_iterator it = wndList.begin(); it != wndList.end(); ++it) {
        if((*it)->isStandard()) {
            const StandardImageWindow* stdImgWnd = dynamic_cast<const StandardImageWindow*>(*it);
            stdImgList.insert(pair<const Image*, string>(stdImgWnd->getImage(), stdImgWnd->windowTitle().toStdString()));
        }
        else if((*it)->isDouble()) {
            const DoubleImageWindow* dblImgWnd = dynamic_cast<const DoubleImageWindow*>(*it);
            dblImgList.insert(pair<const Image_t<double>*, string>(dblImgWnd->getImage(), dblImgWnd->windowTitle().toStdString()));
        }
    }


    QDialog* dialog = new QDialog(imgWnd);
    dialog->setWindowTitle(currentImgName);
    QFormLayout* layout = new QFormLayout(dialog);
    MixImageListBox* imgBox = new MixImageListBox(dialog, currentImgName.toStdString(), stdImgList, dblImgList);
    layout->insertRow(0, tr("Mask to apply : "), imgBox);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(1, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    Image_t<double>* mask;
    if(imgBox->currentType() == MixImageListBox::STDIMG) {
        const Image* stdImg = imgBox->getStdImage(imgBox->currentText().toStdString());
        if(stdImg == NULL) return;
        mask = Converter<Image_t<double> >::convert(*stdImg);
    }
    else if(imgBox->currentType() == MixImageListBox::DBLIMG) {
        const Image_t<double>* dblImg = imgBox->getDblImage(imgBox->currentText().toStdString());
        if(dblImg == NULL) return;
        mask = new Image_t<double>(*dblImg);
    }
    else return;

    Image_t<double>* img;
    if(imgWnd->isStandard()) {
        StandardImageWindow* siw = dynamic_cast<StandardImageWindow*>(imgWnd);
        img = Converter<Image_t<double> >::convert(*siw->getImage());
    }
    else if(imgWnd->isDouble()) {
        DoubleImageWindow* diw = dynamic_cast<DoubleImageWindow*>(imgWnd);
        img = new Image_t<double>(*diw->getImage());
    }

//    const double mean = (mask->max() - mask->min()) / 2.;
    const double max = mask->max();
    for(Image_t<double>::iterator it = mask->begin(); it < mask->end(); ++it) {
//        *it = (*it < mean) ? 0. : 1.;
        *it = *it / max;
    }

    const unsigned int width = min(mask->getWidth(), img->getWidth());
    const unsigned int height = min(mask->getHeight(), img->getHeight());
    Image_t<double>* resImg = new Image_t<double>(width, height, img->getNbChannels());
    for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
        const unsigned int maskChannel = mask->getNbChannels() >= img->getNbChannels() ? c : 0;
        for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
            for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                resImg->pixelAt(i, j, c) = img->pixelAt(i, j, c) * mask->pixelAt(i, j, maskChannel);
            }
        }
    }
    delete img;
    delete mask;

    ImageWindow* newImgWnd;
    if(imgWnd->isStandard()) {
        Image* stdImg = Converter<Image>::convertAndRound(*resImg);
        delete resImg;
        newImgWnd = new StandardImageWindow(stdImg, imgWnd->getPath());
    }
    else if(imgWnd->isDouble()) {
        DoubleImageWindow* diw = dynamic_cast<DoubleImageWindow*>(imgWnd);
        newImgWnd = new DoubleImageWindow(resImg, diw->getPath(), diw->isNormalized(), diw->isLogScaled(), diw->getLogScale(), diw->isAbsolute());
    }
    newImgWnd->setWindowTitle(imgWnd->windowTitle() + tr(" - masked"));
    this->addImage(imgWnd, newImgWnd);

}

SubWindowController::SubWindowController(QMdiSubWindow* sw) : _sw(sw)
{
    
} 

void SubWindowController::closeSubWindow()
{
    emit removeFromWindowsMap(_sw);
    this->deleteLater();
}
