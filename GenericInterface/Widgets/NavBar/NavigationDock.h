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
    virtual ~NavigationDock();

    QList<NodeId> getSelection();    

public slots:
    /**
    * @brief
    *
    * @param node
    */
    void addNode(const Node*, int pos = -1);
    int removeNode(NodeId);
    void changeOrientation(Qt::DockWidgetArea);
    void setActiveNode(NodeId id);

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
    void windowDropped(StandardImageWindow *siw, int pos);

private:
    //QList<const Node*> _data;
    NodeListModel* _model;
    NavigationBar* _view;
    ImageDelegate* _itemDelegate;
    QMenu* _contextMenu;

};
}

#endif
