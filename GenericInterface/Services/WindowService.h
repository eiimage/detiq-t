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

#ifndef QTINTERFACE_WINDOWSERVICE_H
#define QTINTERFACE_WINDOWSERVICE_H

#include <vector>

#include <QDockWidget>
#include <QMdiSubWindow>
#include <QMutex>
#include <QMutexLocker>

#include "Node.h"
#include "../Widgets/ImageWidgets/StandardImageWindow.h"
#include "../Widgets/ImageWidgets/ImageWindow.h"
#include "../Widgets/NavBar/NavigationDock.h"
#include "../GenericInterface.h"


namespace genericinterface
{
  class StandardImageWindow;
  
  class WindowService : public QObject, public Service
  {
  Q_OBJECT
  public:
  
    WindowService(Qt::DockWidgetArea navPos);
  
    void display(GenericInterface* gi);
    void connect(GenericInterface* gi);

   /**
    * @brief Returns the current subwindow if it is an ImageWindow.
    */
    ImageWindow* getCurrentImageWindow();
    std::vector<ImageWindow*> getImageWindows();

	NodeId getNodeId(QWidget* widget) const;
	const Node* getNode(NodeId id) const;
	const Node* getNode(QWidget* widget) const;
    
  protected:
	Node* findNode(NodeId id);
    Node* addNodeIfNeeded(NodeId id, const Image* img, QString path, int pos = -1);
    bool validWindow(QMdiSubWindow* sw) const;
	NodeId findNodeId(QMdiSubWindow* sw) const;
    void signalMdiChange() const;
    bool windowTitleExists(const QString& title);
    void checkWindowTitle(QWidget* widget);

  public slots:
    void addFile(const QString& path);
    void addImage(NodeId id, ImageWindow* image, int pos = -1);
    void addImage(ImageWindow* srcImg, ImageWindow* dstImg);
    bool addWidget(NodeId id, QWidget* widget);
    bool addWidget(ImageWindow* srcImg, QWidget* widget);
    void updateDisplay();
    void removeId(NodeId id);
    void moveToNode(StandardImageWindow* siw, int pos = -1);

  signals:
    void activeWidgetChanged(const QWidget*) const;
    

  private slots:
    void removeSubWindow(QMdiSubWindow*);
    void swActivated(QMdiSubWindow*);
	
  private:
    GenericInterface* _gi;
    QMdiArea* _mdi;
    NavigationDock* _nav;
    std::map<NodeId, Node*> _widgets;
    Qt::DockWidgetArea _navPos;
    mutable QMutex _mutex;
  };


  class SubWindowController : public QObject
  {
      Q_OBJECT
  public:
      SubWindowController(QMdiSubWindow* sw);

  public slots:
      void closeSubWindow();

  signals:
      void removeFromWindowsMap(QMdiSubWindow* sw);

  private:
      QMdiSubWindow* _sw;
  };
}

#endif
