#ifndef QTINTERFACE_WIDGET_NAVIGATIONDOCK
#define QTINTERFACE_WIDGET_NAVIGATIONDOCK

#include <QHBoxLayout>
#include <QMessageBox>
#include <QStringListModel>
#include <QMenu>
#include <QModelIndex>
#include <QAction>
#include "Image.h"

#include "NavigationBar.h"
#include "../../Services/Node.h"


namespace genericinterface
{
class NavigationDock : public QWidget
{
    Q_OBJECT
public:
    /**
    * @brief Default constructor, layout the elements
    */
    NavigationDock();

    QList<NodeId> getSelection();    

public slots:
    /**
    * @brief
    *
    * @param node
    */
    void addNode(const Node*);
    void removeNode(NodeId);

protected slots:
    /**
    * @brief
    *
    * @param pos
    */
    void showContextMenu(const QPoint& pos);
    void emitAction(const QModelIndex& index);
    void closeSelection();

signals:
    void actionDone();
    void removeId(NodeId);
    void windowDropped(StandardImageWindow *siw);

private:
    //QList<const Node*> _data;
    NodeListModel* _model;
    NavigationBar* _view;
    QMenu* _contextMenu;

};
}

#endif
