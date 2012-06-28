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
#include "../GenericInterface.h"
#include "FileService.h"
#include "WindowService.h"

#include <UnknownFormatException.h>

#include <QMessageBox>
#include <QDebug>
#include <QSettings>

using namespace genericinterface;

void FileService::display (GenericInterface* gi)
{

    _gi = gi;
    gi->toolBar("Tools")->setIconSize(QSize(16,16));
    
    _open = gi->menu(tr("&File"))->addAction(tr("&Open"));
    _open->setIcon(gi->style()->standardIcon(QStyle::SP_DialogOpenButton));
    _open->setShortcut(QKeySequence::Open);
    gi->toolBar(tr("Tools"))->addAction(_open);
    _saveAs = gi->menu(tr("&File"))->addAction(tr("Save &As"));
    _saveAs->setIcon(gi->style()->standardIcon(QStyle::SP_DialogSaveButton));
    _saveAs->setShortcut(QKeySequence::Save);
    _saveAs->setEnabled(false);
    gi->toolBar(tr("Tools"))->addAction(_saveAs);

    separatorAct = gi->menu(tr("&File"))->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        recentFileActs[i]->setShortcut(QKeySequence(QString("Ctrl+%1").arg(i+1)));
        gi->menu(tr("&File"))->addAction(recentFileActs[i]);
    }
   
    //gi->menu(tr("&File"))->addSeparator();
    //_open = gi->menu(tr("&File"))->addAction(tr("E&xit"));
    //_open->setIcon(gi->style()->standardIcon(QStyle::SP_DialogCloseButton));
    //_open->setShortcut(QKeySequence::Quit);

    updateRecentFileActions();
}

void FileService::connect (GenericInterface* gi)
{
	_gi = gi;
	QObject::connect(_open, SIGNAL(triggered()), this, SLOT(chooseFile()));
	//QObject::connect(_save, SIGNAL(triggered()), this, SLOT(save()));
	QObject::connect(_saveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    for (int i = 0; i < MaxRecentFiles; ++i) {
        QObject::connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
  
	QObject::connect(this, SIGNAL(fileChosen(const QString&)), dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE)), SLOT(addFile(const QString&)));
	
    //connexion des changements d'images
	WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
	QObject::connect(ws, SIGNAL(activeWidgetChanged(const QWidget*)), this, SLOT(checkActionsValid(const QWidget*)));
}

void FileService::save(const QString& path, const QString& ext)
{
	if(path == "") {
		this->saveAs();
	}
	else {
        try {
            WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
            if(ws != NULL) {
                StandardImageWindow* imw = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
                if(imw != NULL) {
					try {
						imw->getImage()->save(path.toStdString()); 
					}
					catch(const UnknownFormatException& e) {
						if(ext == "")
							throw e;
						
						imw->getImage()->save((path+ext).toStdString()); 
					}
                }
                else {
                    QMessageBox::critical(_gi, tr("Bad object type"), tr("Only images can be saved to a file."));
                }
            }
        }
        catch(const char* s) {
                QMessageBox::information(_gi, tr("Unknown exception"), s);
        }
	}
}

void FileService::saveAs()
{
    QString path;
	WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ImageWindow* currentWindow = ws->getCurrentImageWindow();
    if(currentWindow != NULL) {
        path = currentWindow->getPath();
    }
    QString selectedFilter;
	QString file = QFileDialog::getSaveFileName(_gi, tr("Save a file"), path, tr("PNG image (*.png);;BMP image (*.bmp);; JPEG image(*.jpg)"), &selectedFilter);

	QString ext = selectedFilter.right(5).left(4);

	if(file != "") {
	    if(!file.contains('.')) file += ext;
		this->save(path, ext);
	}
}

void FileService::loadFiles(const QStringList &filenames) {
    
    QStringList recentFiles = _gi->settings()->value("recentFileList").toStringList();

    foreach(QString filename, filenames) {
        if(filename != "") {
            recentFiles.removeAll(filename);
            recentFiles.prepend(filename);
            while (recentFiles.size() > MaxRecentFiles)
                recentFiles.removeLast();
            emit fileChosen(filename);
        }
    }
    
    _gi->settings()->setValue("recentFileList", recentFiles);
    updateRecentFileActions();
}

void FileService::updateRecentFileActions() {
    QStringList recentFiles = _gi->settings()->value("recentFileList").toStringList();

    int numRecentFiles = qMin(recentFiles.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(recentFiles[i]).fileName());
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(recentFiles[i]);
        recentFileActs[i]->setVisible(true);
    }
    
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

void FileService::chooseFile()
{
    QString path;
	WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    ImageWindow* currentWindow = ws->getCurrentImageWindow();
    if(currentWindow != NULL) {
        path = currentWindow->getPath();
    }
    QStringList filenames = QFileDialog::getOpenFileNames(_gi, tr("Open a file"), path, tr("Images (*.png *.bmp *.jpg *.jpeg)"));
    loadFiles(filenames);
}

void FileService::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        loadFiles(QStringList(action->data().toString()));
    }
}


void FileService::checkActionsValid(const QWidget* activeWidget)
{
    std::cout << "WindowService::activeWidgetChanged(" << reinterpret_cast<intptr_t>(activeWidget) << ")" << std::endl;
	const StandardImageWindow* window = dynamic_cast<const StandardImageWindow*>(activeWidget);
	if(window) {
		_saveAs->setEnabled(true);
	}
	else {
		_saveAs->setEnabled(false);
	}
}
