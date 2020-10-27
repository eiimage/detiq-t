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

#ifndef KEYBOARDSHORTCUT_H
#define KEYBOARDSHORTCUT_H

#include <QMessageBox>

/**
 * @brief The KeyboardShortcut class represents the list of hot keys
 */
class KeyboardShortcut : public QMessageBox
{
    Q_OBJECT
public:
    explicit KeyboardShortcut(QWidget *parent = 0);

signals:

public slots:

};

#endif // KEYBOARDSHORTCUT_H
