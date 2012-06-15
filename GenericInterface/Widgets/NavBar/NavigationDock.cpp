#include "NavigationDock.h"
#include "Image.h"
#include "../../Services/Node.h"

#include <QAbstractItemModel>
#include <QDragMoveEvent>
using namespace genericinterface;
using namespace imagein;
using namespace std;

NavigationDock::NavigationDock(const QString & title, QWidget * parent, Qt::WindowFlags flags) : QDockWidget(title, parent, flags)
{
    /* Creation of the attributs */
    //_model->setSupportedDragActions(Qt::MoveAction);
    //_model = new QStringListModel;

    //_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    
    //_view->setMovement(QListView::Free);

    QSize itemSize(96,96);

    _contextMenu = new QMenu(this);

    /* Context Menu */
    _contextMenu->addAction("Close all image from the selection", this, SLOT(closeSelection()));

    setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    QObject::connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(changeOrientation(Qt::DockWidgetArea)));
    

    /* layout */
    /*QHBoxLayout* layout = new QHBoxLayout;

    setLayout(layout);
    layout->addWidget(_view);*/
    
    
    _view = new NavigationBar(itemSize, Qt::Horizontal);
    QObject::connect(_view, SIGNAL(itemClicked(const QModelIndex&, QPoint, QPoint)), this, SLOT(itemClicked(const QModelIndex&, QPoint, QPoint)));
    //QObject::connect(_view, SIGNAL(removeNode(NodeId)), this, SIGNAL(removeId(NodeId)));
    QObject::connect(_view, SIGNAL(resized()), this, SLOT(listResized));
    
    //this->setFixedWidth(100);
    //_view->setFixedWidth(96);
    //_view->setGridSize(itemSize);
    //_view->setMovement(QListView::Snap);
    
    _model = new NodeListModel(NULL);
    QObject::connect(_model, SIGNAL(windowDropped(StandardImageWindow*)), this, SIGNAL(windowDropped(StandardImageWindow*)));
    _view->setModel(_model);
    
    _itemDelegate = new ImageDelegate(itemSize);
    _view->setItemDelegate(_itemDelegate);
    
    this->setWidget (_view);

}

QList<NodeId> NavigationDock::getSelection()
{
    QItemSelectionModel *selection = _view->selectionModel();
    QModelIndexList listSelect = selection->selectedIndexes();

    QList<NodeId> res;

    for(int i = 0; i < listSelect.size(); i++)
    {
        const Node* node = _model->data(listSelect[i], Qt::DisplayRole).value<const Node*>();
        res << node->getId();
    }

    return res;
}

void NavigationDock::addNode(const Node* node)
{
    /*_data << node;
    _model->setList(_data);*/
    int i = _model->rowCount();
    _model->insertRow(i);
    _model->setData(_model->index(i), QVariant::fromValue(node));
}

void NavigationDock::removeNode(NodeId id)
{
    /*for(int i = 0; i < _data.size(); ++i) {
        if(_data[i]->getId() == id) {
            _data.removeAt(i);
            --i;
        }
    }
    _model->setList(_data);*/
    for(int i = 0; i < _model->rowCount(); ++i) {
        QVariant data = _model->data(_model->index(i));
        if(data.canConvert<const Node*>()) {
            const Node* node = data.value<const Node*>();
            if(node && node->getId() == id) {
                _model->removeRow(i);
                --i;
            }
        }
    }
}

void NavigationDock::showContextMenu(const QPoint& pos)
{
    _contextMenu->popup(mapToGlobal(pos));
}

void NavigationDock::itemClicked(const QModelIndex& index, QPoint downPos, QPoint upPos)
{
    QRect closeRect = _itemDelegate->getCloseRect(index, _view->visualRect(index));
    if(closeRect.contains(downPos) && closeRect.contains(upPos)) {
        QVariant data = index.data();
        if(data.canConvert<const Node*>()) {
            const Node* node = data.value<const Node*>();
            emit removeId(node->getId());
        }
    }
    else {
        emit actionDone();
    }
}

void NavigationDock::closeSelection()
{
    int answer = QMessageBox::question(this, "Attention", "You're going to close all the relative windows, are you sure you want to continue?", QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::Yes)
    {
        QList<NodeId> selection = this->getSelection();
        
        for (QList<NodeId>::iterator it = selection.begin(); it != selection.end(); ++it)
        {
            emit removeId(*it);
        }
    }
}
void NavigationDock::listResized() {
    this->adjustSize();
}

void NavigationDock::changeOrientation(Qt::DockWidgetArea area) {
    Qt::Orientation orientation = Qt::Vertical;
    if(area == Qt::TopDockWidgetArea || area == Qt::BottomDockWidgetArea) {
        orientation = Qt::Horizontal;
    }
    _view->setOrientation(orientation);
}