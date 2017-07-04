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

#ifndef QTINTERFACE_FILESERVICE_H
#define QTINTERFACE_FILESERVICE_H

#include <QMessageBox>
#include <QFileDialog>

#include "../Service.h"

#define QSETTINGS_LANGUAGE_PREFERENCE "default_language"

namespace genericinterface {
    class GenericInterface;
}

namespace genericinterface
{
    class FileService : public QObject, public Service
    {
        Q_OBJECT
        public:
            void connect (GenericInterface* gi);
            void display (GenericInterface* gi);

            /**
             * @brief Check if the given fileName path can be opened.
             *
             * This perform a simple check on file existency and suffix support.
             * The file is not read by this method.
             *
             * @param fileName The path of the file
             * @return true if the file has a supported type and exists
             */
            static bool canOpen(const QString &fileName);

        public slots:
            void save(const QString& path = QString(), const QString& ext = QString());
            void saveAs();
           // void copyImage();
            void checkActionsValid(const QWidget* activeWidget);
            void loadFiles(const QStringList &fileNames);

        private slots:
            void chooseFile();
            void openRecentFile();

            void changeLanguage();

        signals:
            void fileChosen(const QString& path);

        private:
            void updateRecentFileActions();

            QAction* _open;
            QAction* _save;
            QAction* _saveAs;

            //QAction* _copy;

            GenericInterface* _gi;
            //ImageWindow* _iw = _gi->windowService()->getCurrentImageWindow();
            enum { MaxRecentFiles = 5 };
            QAction *recentFileActs[MaxRecentFiles];
            QAction* separatorAct;
    };
}

#endif
