#include "NavigationDock.h"
#include "Image.h"
#include "../../Services/Node.h"

#include <QAbstractItemModel>
using namespace genericinterface;
using namespace imagein;
using namespace std;

NavigationDock::NavigationDock() : QWidget()
{
    /* Creation of the attributs */
    _model = new NodeListModel(NULL);
    _view = new NavigationBar;

    _view->setModel(_model);
    _view->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QObject::connect(_view, SIGNAL(clicked(const QModelIndex&)), this, SLOT(emitAction(const QModelIndex&)));
    QObject::connect(_view, SIGNAL(removeNode(NodeId)), this, SIGNAL(removeId(NodeId)));

    _contextMenu = new QMenu(this);

    /* Context Menu */
    _contextMenu->addAction("Close all image from the selection", this, SLOT(closeSelection()));

    setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    /* layout */
    QHBoxLayout* layout = new QHBoxLayout;

    setLayout(layout);
    layout->addWidget(_view);

    setFixedWidth(110);
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

void NavigationDock::emitAction(const QModelIndex& index)
{
    emit actionDone();
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