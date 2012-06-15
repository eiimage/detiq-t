#include "WindowService.h"

#include <QListIterator>

using namespace genericinterface;
using namespace imagein;

void WindowService::display(GenericInterface* gi)
{
    _mdi = gi->initCentralWidget();

    QDockWidget* dock = new QDockWidget(gi);
    gi->addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock->setWidget(_nav = new NavigationDock);
}

void WindowService::connect(GenericInterface* gi)
{
    _gi = gi;
    QObject::connect(_nav, SIGNAL(actionDone()), this, SLOT(updateDisplay()));
    QObject::connect(_nav, SIGNAL(removeId(NodeId)), this, SLOT(removeId(NodeId)));
    QObject::connect(_nav, SIGNAL(windowDropped(StandardImageWindow*)), 
                        this, SLOT(moveToNode(StandardImageWindow*)));
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
    else return NULL;
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
    std::map<NodeId, Node*>::iterator it = _widgets.find(id);
    if(it == _widgets.end()) {
        return NULL;
    }
    return it->second;
}

const Node* WindowService::getNode(NodeId id) const {
    std::map<NodeId, Node*>::const_iterator it = _widgets.find(id);
    if(it == _widgets.end()) {
        return NULL;
    }
    return it->second;
}

const Node* WindowService::getNode(QWidget* widget) const {
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

Node* WindowService::addNodeIfNeeded(NodeId id, const Image* img, QString path) {
    Node* node = findNode(id);
    if(node == NULL) {
        node = new Node(img, path);
        _widgets[id] = node;
        _nav->addNode(node);
    }
    return node;
}

void WindowService::addFile(const QString& path)
{
    StandardImageWindow* siw = new StandardImageWindow(path, _gi);
    const Image* img = siw->getImage();
    if(img->size() == 0) {
        delete siw;
    }
    else {
        this->addImage(img, siw);
    }
}

void WindowService::addImage(NodeId id, StandardImageWindow* imgWnd)
{
    Node* node = addNodeIfNeeded(id, imgWnd->getImage(), imgWnd->getPath());

    QMdiSubWindow* sw = _mdi->addSubWindow(imgWnd);
    node->windows << sw;

    SubWindowController* swc = new SubWindowController(id, sw);

    QObject::connect(sw, SIGNAL(aboutToActivate()), imgWnd, SLOT(activated()));

    QObject::connect(sw, SIGNAL(destroyed()), swc, SLOT(closeSubWindow()));
    QObject::connect(swc, SIGNAL(removeFromWindowsMap(NodeId, QMdiSubWindow*)), 
                    this, SLOT(removeSubWindow(NodeId,QMdiSubWindow*)));

    sw->show();
}

bool WindowService::addWidget(NodeId id, QWidget* widget)
{
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
    return true;
}

void WindowService::removeSubWindow(NodeId id, QMdiSubWindow* sw)
{
    Node* node = findNode(id);
    if(node == NULL) return;
    
    node->windows.removeAll(sw);

    if (node->windows.empty()) {
        _nav->removeNode(id);
        _widgets.erase(id);
        //delete node;
    }
    //this->updateDisplay();
}

void WindowService::removeId(NodeId id)
{
    Node* node = findNode(id);
    if(node == NULL) return;
    
    
    for (QList<QMdiSubWindow*>::iterator it = node->windows.begin(); it != node->windows.end(); ++it)
    {
        QMdiSubWindow* sw = *it;
        sw->close();
        _mdi->removeSubWindow(sw);
    }
    //delete node;
    //this->updateDisplay();
}

void WindowService::moveToNode(StandardImageWindow* siw) {
    StandardImageWindow* newSiw = new StandardImageWindow(*siw);
    foreach(QMdiSubWindow *sw, _mdi->subWindowList()) {
        if(sw->widget() == siw) {
            sw->close();
        }
    }
    addImage(newSiw->getImage(), newSiw);
}

void WindowService::updateDisplay()
{
    const QList<NodeId>& selection = _nav->getSelection();
    
    for(std::map<NodeId, Node*>::iterator it = _widgets.begin(); it != _widgets.end(); ++it) {
    
        NodeId id = it->first;
        Node* node = it->second;
        QList<QMdiSubWindow*>& windows = node->windows;
        
        if(selection.indexOf(id) >= 0) {
            for (QList<QMdiSubWindow*>::iterator jt = windows.begin(); jt != windows.end(); ++jt) {
                (*jt)->showNormal();
            }
        }
        else {
            for (QList<QMdiSubWindow*>::iterator jt = windows.begin(); jt != windows.end(); ++jt) {
                (*jt)->showMinimized();
            }
        }
    }
}

SubWindowController::SubWindowController(NodeId id, QMdiSubWindow* sw) : _id(id), _sw(sw)
{
    
} 

void SubWindowController::closeSubWindow()
{
    emit removeFromWindowsMap(_id, _sw);
    delete this;
}
