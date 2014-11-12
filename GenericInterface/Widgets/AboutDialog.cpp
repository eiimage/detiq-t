#include "AboutDialog.h"

#include <QLabel>
#include <QLayout>

AboutDialog::AboutDialog(QWidget *parent) :
    QMessageBox(parent)
{
    setWindowTitle(tr("About"));

    const char* NEWLINE = "<br/>";

    QString theText = tr("This software has been developed at IETR/Insa.");
    theText.append(NEWLINE).append(NEWLINE);
    theText.append(tr("<strong>Authors:</strong>")).append(NEWLINE);
    theText.append("Benoit Averty, Samuel Babin, "
                   "Matthieu Bergère, Thomas Letan, "
                   "Sacha Percot-Tétu, Florian Teyssier");
    setText(theText);
}
