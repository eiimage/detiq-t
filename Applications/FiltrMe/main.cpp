#include <QApplication>

#include <GenericInterface.h>
#include "FilteringService.h"

using namespace genericinterface;

int main (int argc, char** argv)
{
  QApplication app (argc, argv);

  Log::configure (true, false, 0);

  GenericInterface m;
  int serviceId = m.addService(new FilteringService);

  m.run ();

  return app.exec ();
}
