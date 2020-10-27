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
#include "KeyboardShortcut.h"

#include <QLabel>
#include <QLayout>

KeyboardShortcut::KeyboardShortcut(QWidget *parent) :
    QMessageBox(parent)
{
    setWindowTitle(tr("Hot keys"));

    const char* NEWLINE = "<br/>";

    QString theText = tr("Keyboard Shortcut List");

    theText.append(NEWLINE).append(NEWLINE);
    theText.append("<strong>");
    theText.append(tr("Image Window:"));
    theText.append("</strong>").append("<ul>");
    theText.append("<li>").append(tr("Zoom In")).append(" : ").append(tr("Ctrl & + / Ctrl & Wheel Forward")).append("</li>");
    theText.append("<li>").append(tr("Zoom Out")).append(" : ").append(tr("Ctrl & - / Ctrl & Wheel Backward")).append("</li>");
    theText.append("<li>").append(tr("Save As")).append(" : ").append("Ctrl & S").append("</li>");
    theText.append("<li>").append(tr("Copy Image to Clipboard")).append(" : ").append("Ctrl & C").append("</li>");
    theText.append("</ul>");

    theText.append("<strong>");
    theText.append(tr("Histogram Window (magnifier icon pressed):"));
    theText.append("</strong>").append("<ul>");
    theText.append("<li>").append(tr("Zoom In")).append(" : ").append(tr("Left Area Selection")).append("</li>");
    theText.append("<li>").append(tr("Zoom Out One Step")).append(" : ").append(tr("Right Click")).append("</li>");
    theText.append("<li>").append(tr("Zoom Out to Origin")).append(" : ").append(tr("Ctrl & Right Click")).append("</li>");
    theText.append("<li>").append(tr("Save As")).append(" : ").append("Ctrl & S").append("</li>");
    theText.append("</ul>");

    setText(theText);
}
