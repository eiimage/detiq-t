#ifndef GENERICINTERFACE_H_02022012
#define GENERICINTERFACE_H_02022012

/* Stl includes */
#include <map>

/* Qt includes */
#include <QMainWindow>
#include <QMessageBox>
#include <QMdiArea>
#include <QMenuBar>
#include <QMenu>
#include <QString>

class GenericInterface; /* Pre-declaration for the Service include */

#include "Service.h"
#include "InterfaceExceptions.h"
#include "Log.h"

/**
* @brief The Generic Interface provides to the ImageIn developper a way to built easily
* a demonstration of his algorithm
*
* It is built with the philosophy of services : each functionnality of the application is
* a service running by an instance of Service interface.
*/
class GenericInterface : public QMainWindow
{
public:
  /**
  * @brief Add a new service to the generic interface
  *
  * <p>The integer using for the identification of one service is very important : it's thanks to
  * its you will be able to connect your services with other, including the default ones. The GenericInterface
  * gives a set of minimalist services, identify by an integer :</p>
  *
  * <ul>
  *   <li> WindowService : 0
  *   <li> IOService : 1
  *   <li> UtilityService : 2
  * </ul>
  *
  * <p>
  *
  * @param id the identifier of the service
  * @param s the service to add
  */
  void addService (int id, Service* s) throw (BadIdException);
  /**
  * @brief Change the id service to a custom one.
  *
  * Be careful : the new service has to implemant the signals and slots of the
  * former one. If not, the connect method will throw an ServiceConnectionException
  * and your application won't be able to assume the basics tasks.
  *
  * @param id
  * @param s
  */
  void changeService (int id, Service* s);

  /**
  * @brief Provides a direct access to the wanted service, mostly use by the Service::connect (GenericInterface*)
  * function.
  *
  * @param id its id
  *
  * @return 
  */
  Service* service (int id);

  /**
  * @brief This function has to be prefered to the Qt function void QMainWindow::show (), it connects the
  * services to each other (simply calling the function void Service::connect (GenericInterfac*) of each 
  * service adding to the GenericInterface.
  *
  * @param shw Request to QMainWindow::show (), the default value is true. 
  */
  virtual void run (bool shw = true);

  /**
  * @brief Create the Mdi Widget of the main window
  *
  * This function will throw an exception if it was already called
  *
  * @return a pointer to the created widget
  */
  virtual QMdiArea* initCentralWidget();

  /**
  * @brief Get a pointer to the QMenu labelled name
  *
  * @param name the label of the wanted menu
  *
  * @return the pointer
  */
  virtual QMenu* menu(QString name);

protected:
  /**
  * @brief The services the interface can uses.
  */
  std::map<int, Service*> _services;
  std::map<QString, QMenu*> _menus;
};

#endif