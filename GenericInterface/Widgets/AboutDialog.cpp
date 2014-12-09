/*
 * Copyright 2011-2014 INSA Rennes
 *
 * This file is part of DETIQ-T.
 *
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "AboutDialog.h"

#include <QLabel>
#include <QLayout>

AboutDialog::AboutDialog(QWidget *parent) :
    QMessageBox(parent)
{
    setWindowTitle(tr("About"));

    const char* NEWLINE = "<br/>";

    QString theText = tr("This software has been developed at Insa.");

    theText.append(NEWLINE).append(NEWLINE);
    theText.append("<strong>");
    theText.append(tr("Main authors:"));
    theText.append("</strong>").append("<ul>");
    theText.append("<li>").append("Benoit Averty").append("</li>");
    theText.append("<li>").append("Samuel Babin").append("</li>");
    theText.append("<li>").append("Matthieu Bergère").append("</li>");
    theText.append("<li>").append("Thomas Letan").append("</li>");
    theText.append("<li>").append("Sacha Percot-Tétu").append("</li>");
    theText.append("<li>").append("Florian Teyssier").append("</li>");
    theText.append("</ul>");

    theText.append(tr("Contributors:")).append(NEWLINE);
    theText.append("Antoine Lorence");

    theText.append(NEWLINE).append(NEWLINE);
    theText.append(tr("Compiled on %1 at %2").arg(__DATE__, __TIME__));

    setText(theText);
}
