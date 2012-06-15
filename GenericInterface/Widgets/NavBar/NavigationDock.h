#ifndef QTINTERFACE_WIDGET_NAVIGATIONDOCK
#define QTINTERFACE_WIDGET_NAVIGATIONDOCK

#include <QHBoxLayout>
#include <QMessageBox>
#include <QStringListModel>
#include <QMenu>
#include <QModelIndex>
#include <QAction>
#include <QDockWidget>
#include "Image.h"

#include "NavigationBar.h"
#include "NodeListModel.h"
#include "../../Services/Node.h"


namespace genericinterface
{
class NavigationDock : public QDockWidget
{
    Q_OBJECT
public:
    /**
    * @brief Default constructor, layout the elements
    */
    NavigationDock(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);

    QList<NodeId> getSelection();    

public slots:
    /**
    * @brief
    *
    * @param node
    */
    void addNode(const Node*);
    void removeNode(NodeId);
    void changeOrientation(Qt::DockWidgetArea);

protected slots:
    /**
    * @brief
    *
    * @param pos
    */
    void showContextMenu(const QPoint& pos);
    void itemClicked(const QModelIndex& index, QPoint downPos, QPoint upPos);
    void closeSelection();
    void listResized();

signals:
    void actionDone();
    void removeId(NodeId);
    void windowDropped(StandardImageWindow *siw);

private:
    //QList<const Node*> _data;
    NodeListModel* _model;
    NavigationBar* _view;
    ImageDelegate* _itemDelegate;
    QMenu* _contextMenu;

};
}

#endif
