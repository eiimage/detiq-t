#ifndef QTINTERFACE_WINDOWSERVICE_H
#define QTINTERFACE_WINDOWSERVICE_H

#include <QDockWidget>
#include <QMdiSubWindow>

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

	NodeId getNodeId(QWidget* widget) const;
	const Node* getNode(NodeId id) const;
	const Node* getNode(QWidget* widget) const;
    
  protected:
	Node* findNode(NodeId id);
    Node* addNodeIfNeeded(NodeId id, const Image* img, QString path);

  public slots:
    void addFile(const QString& path);
    void addImage(NodeId id, StandardImageWindow* image);
    bool addWidget(NodeId id, QWidget* widget);
    void updateDisplay();
    void removeId(NodeId id);
    void moveToNode(StandardImageWindow* siw);

  signals:
    void subWindowActivated(QMdiSubWindow*);

  private slots:
    void removeSubWindow(NodeId id, QMdiSubWindow* sw);
	
  private:
    GenericInterface* _gi;
    QMdiArea* _mdi;
    NavigationDock* _nav;
    std::map<NodeId, Node*> _widgets;
  };


  class SubWindowController : public QObject
  {
      Q_OBJECT
  public:
      SubWindowController(NodeId id, QMdiSubWindow* sw);

  public slots:
      void closeSubWindow();

  signals:
      void removeFromWindowsMap(NodeId id, QMdiSubWindow* sw);

  private:
      NodeId _id;
      QMdiSubWindow* _sw;
  };
}

#endif
