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

using namespace genericinterface;
using namespace imagein;
WindowService::WindowService(Qt::DockWidgetArea navPos) : _navPos(navPos),  _mutex(new QMutex(QMutex::Recursive)) {

}

void WindowService::display(GenericInterface* gi)
{
    _mdi = gi->initCentralWidget();
    _nav = new NavigationDock("Images", gi);
    gi->addDockWidget(_navPos, _nav);
}

void WindowService::connect(GenericInterface* gi)
{
    _gi = gi;
    QObject::connect(_nav, SIGNAL(actionDone()), this, SLOT(updateDisplay()));
    QObject::connect(_nav, SIGNAL(removeId(NodeId)), this, SLOT(removeId(NodeId)));
    QObject::connect(_nav, SIGNAL(windowDropped(StandardImageWindow*, int)), 
                        this, SLOT(moveToNode(StandardImageWindow*, int)));
    QObject::connect(_mdi, SIGNAL(subWindowActivated(QMdiSubWindow*)), 
                        this, SIGNAL(subWindowActivated(QMdiSubWindow*)));
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

NodeId WindowService::getNodeId(QWidget* widget) const
{
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
    _mutex->lock();
    std::map<NodeId, Node*>::iterator it = _widgets.find(id);
    if(it == _widgets.end()) {
        return NULL;
    }
    _mutex->unlock();
    return it->second;
}

const Node* WindowService::getNode(NodeId id) const {
    _mutex->lock();
    std::map<NodeId, Node*>::const_iterator it = _widgets.find(id);
    if(it == _widgets.end()) {
        return NULL;
    }
    _mutex->unlock();
    return it->second;
}

const Node* WindowService::getNode(QWidget* widget) const {
    _mutex->lock();
    for(std::map<NodeId, Node*>::const_iterator it = _widgets.begin() ; it != _widgets.end() ; ++it)
    {
        const Node* node = it->second;
        for(QList<QMdiSubWindow*>::const_iterator it = node->windows.begin(); it != node->windows.end(); ++it) {
            if((*it)->widget() == widget) {
                _mutex->unlock();
                return node;
            }
        }
    }
    _mutex->unlock();
    return NULL;
}

Node* WindowService::addNodeIfNeeded(NodeId id, const Image* img, QString path, int pos) {
    _mutex->lock();
    Node* node = findNode(id);
    if(node == NULL) {
        node = new Node(img, path);
        _widgets[id] = node;
        _nav->addNode(node, pos);
    }
    _mutex->unlock();
    return node;
}

void WindowService::addFile(const QString& path)
{
    _mutex->lock();
    StandardImageWindow* siw = new StandardImageWindow(path, _gi);
    const Image* img = siw->getImage();
    if(img->size() == 0) {
        delete siw;
    }
    else {
        this->addImage(img, siw);
    }
    _mutex->unlock();
}

void WindowService::addImage(NodeId id, StandardImageWindow* imgWnd, int pos)
{
    _mutex->lock();
    Node* node = addNodeIfNeeded(id, imgWnd->getImage(), imgWnd->getPath(), pos);

    QMdiSubWindow* sw = _mdi->addSubWindow(imgWnd);
    node->windows << sw;

    SubWindowController* swc = new SubWindowController(id, sw);

    QObject::connect(sw, SIGNAL(aboutToActivate()), imgWnd, SLOT(activated()));

    QObject::connect(sw, SIGNAL(destroyed()), swc, SLOT(closeSubWindow()));
    QObject::connect(swc, SIGNAL(removeFromWindowsMap(NodeId, QMdiSubWindow*)), 
                    this, SLOT(removeSubWindow(NodeId,QMdiSubWindow*)));

    sw->show();
    _mdi->setActiveSubWindow(sw);
    sw->activateWindow();
    _mutex->unlock();
}

bool WindowService::addWidget(NodeId id, QWidget* widget)
{
    _mutex->lock();
    Node* node = findNode(id);
    if(node == NULL) {
        return false;
    }
    
    QMdiSubWindow* sw = _mdi->addSubWindow(widget);
    node->windows << sw;

    SubWindowController* swc = new SubWindowController(id, sw);

    QObject::connect(sw, SIGNAL(destroyed()), swc, SLOT(closeSubWindow()));
    QObject::connect(swc, SIGNAL(removeFromWindowsMap(NodeId, QMdiSubWindow*)), 
                    this, SLOT(removeSubWindow(NodeId,QMdiSubWindow*)));

    sw->show();
    _mdi->setActiveSubWindow(sw);
    sw->activateWindow();
    return true;
    _mutex->unlock();
}

void WindowService::removeSubWindow(NodeId id, QMdiSubWindow* sw)
{
    _mutex->lock();
    Node* node = findNode(id);
    if(node == NULL) return;
    
    std::cout << "node->windows.removeAll(" << sw << ")" << std::endl;
    node->windows.removeAll(sw);

    if (node->windows.empty()) {
        std::cout << "remove node" << std::endl;
        _nav->removeNode(id);
        _widgets.erase(id);
        delete node;
    }
    //this->updateDisplay();
    _mutex->unlock();
}

void WindowService::removeId(NodeId id)
{
    _mutex->lock();
    //std::cout << "removeId->" << std::endl;
    Node* node = findNode(id);
    if(node == NULL) return;
    
    if(node->windows.size() > 1) {
        int answer = QMessageBox::question(_gi, "Attention", "You're going to close all the relative windows, are you sure you want to continue?", QMessageBox::Yes | QMessageBox::No);
        if(answer != QMessageBox::Yes) return;
    }
    
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
    _mutex->unlock();
}

void WindowService::moveToNode(StandardImageWindow* siw, int pos) {
    _mutex->lock();
    StandardImageWindow* newSiw = new StandardImageWindow(*siw);
    foreach(QMdiSubWindow *sw, _mdi->subWindowList()) {
        if(sw->widget() == siw) {
            sw->close();
        }
    }
    addImage(newSiw->getImage(), newSiw, pos);
    _mutex->unlock();
}

bool WindowService::validWindow(QMdiSubWindow* sw) {
    return _mdi->subWindowList().contains(sw);
}

void WindowService::updateDisplay()
{
    _mutex->lock();
    const QList<NodeId>& selection = _nav->getSelection();
    
    QMdiSubWindow* firstValidWindow = NULL;
    
    for(std::map<NodeId, Node*>::iterator it = _widgets.begin(); it != _widgets.end(); ++it) {
    
        NodeId id = it->first;
        Node* node = it->second;
        QList<QMdiSubWindow*>& windows = node->windows;
        
        if(selection.indexOf(id) >= 0) {
            for (QList<QMdiSubWindow*>::iterator jt = windows.begin(); jt != windows.end(); ++jt) {
                if(validWindow((*jt))) {
                    if(firstValidWindow == NULL) firstValidWindow = *jt;
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
    QMdiSubWindow* currentWindow = _mdi->currentSubWindow();
    if(currentWindow != NULL) {
        currentId = getNodeId(currentWindow->widget());
        Node* currentNode = findNode(currentId);
        //std::cout << "Current node : " << currentNode->path.toStdString() << std::endl;
        //std::cout << "Current node " << (selection.indexOf(currentId) >= 0 ? "in selection" : "not in selection") << std::endl;
    }
    
    if(!selection.isEmpty() && selection.indexOf(currentId) < 0) {
        //std::cout << "Raising first window in selection : " << firstValidWindow->windowTitle().toStdString() << std::endl;
        _mdi->setActiveSubWindow(firstValidWindow);
    }
    _mutex->unlock();
    
}

SubWindowController::SubWindowController(NodeId id, QMdiSubWindow* sw) : _id(id), _sw(sw)
{
    
} 

void SubWindowController::closeSubWindow()
{
    emit removeFromWindowsMap(_id, _sw);
    delete this;
}
