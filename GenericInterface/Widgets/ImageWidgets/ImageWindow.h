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

#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H


#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFont>
#include <QKeyEvent>
#include <QPoint>
#include <QInputDialog>
#include <QToolButton>
#include <QStatusBar>
#include <QImage>
#include <QClipboard>
#include <QRect>
#include <QAction>

#include <sstream>
#include <iostream>
#include <list>

#include <Rectangle.h>

#include <Image.h>
#include "ImageView.h"

class QSpinBox;

namespace genericinterface
{

class GenericInterface;

class SelectionWidget : public QWidget {
  Q_OBJECT
  public:
    SelectionWidget(QWidget* parent, int width, int height);
    void setRange(int width, int height);
    QSpinBox* getX() const {return _Xspinbox;}
    QSpinBox* getY() const {return _Yspinbox;}
    QSpinBox* getWidth() const {return _Wspinbox;}
    QSpinBox* getHeight() const {return _Hspinbox;}
  public slots:
    void updateSelection(QRect select);
    void selectionMoved(int);
  signals:
    void selectionMoved(QRect);
  protected:
    QSpinBox *_Xspinbox, *_Yspinbox, *_Wspinbox, *_Hspinbox;
};
/*!
   * \brief Inherited class by all Windows
   *
   * The ImageWindow class contains the status bar and inherit QWidget.
   */
class ImageWindow : public QWidget
{
    Q_OBJECT
    public:
        /*!
         * \brief Default constructor.
         *
         * Constructs an empty QWidget and initialized the source window and the application area from parameters.
         *
         * \param source The source window
         * \param rect The application area (Rectangle used for Histogram, Algorithm...)
         */
        ImageWindow(QString path, const imagein::Image* image = NULL, imagein::Rectangle rect = imagein::Rectangle());

        ~ImageWindow();

        /*!
         * \brief Returns the title from a path.
         *
         * \param path The path of a file.
         */
        static QString getTitleFromPath(QString path);

        inline QString getPath() const { return _path; }
        inline void setPath(QString path) { _path = path; }

        inline QString getName() const { return getTitleFromPath(_path); }

        /*!
         * \brief Returns the displayable Image contained in the window.
         *
         */
        inline const imagein::Image* getDisplayImage() const { return _displayImg; }

        /*!
         * \brief Returns the Rectangle which is the current image's selection on the window.
         *
         */
        const imagein::Rectangle selection() const { return _imageView->getRectangle(); }

        inline QMenu* menu() const { return _menu; }
        inline QPoint selectedPixel() const { return _selectedPixel; }

        inline ImageView* view() { return _imageView; }

        void zoom(int delta);
        virtual bool isDouble() const = 0;
        virtual bool isStandard() const = 0;

         SelectionWidget* getSelectionWidget() const {return _selectWidget;}
        QToolButton* getSelectionButton() const {return _selectButton;}

    public slots:


        void activated();
        void setApplicationArea(const imagein::Rectangle rect);
        void startDrag();
        void updateZoom(double z) const;
        void toggleMouseMode(bool);
        void toggleSelectMode(bool);

        virtual void crop() = 0;
        virtual void copycrop() = 0;

        virtual void pixelClicked(int x, int y);
        virtual void pixelHovered(int x, int y);

        void rename();
        void applyBinaryMask();

        virtual void showHistogram();
        virtual void showPixelsGrid() = 0;

        void zoom_in();
        void zoom_out();

        void save(const QString& path = QString(), const QString& ext = QString());
        void saveAs();

        void copyImage();

    signals:
        /*!
         * \brief Signal emits when this is activated
         *
         * \param rect Rectangle to display on the source window
         */
        void selectRectChange(imagein::Rectangle rect, ImageWindow* source);

        /*!
         * \brief Signal emits when the highlight rect have to changes
         *
         * \param rect The rectangle to display
         */
        void highlightRectChange(imagein::Rectangle rect, ImageWindow* source);

        void addWidget(ImageWindow*, QWidget*);
        void addImage(ImageWindow*, ImageWindow*);
        void applyBinaryMask(ImageWindow*);

    protected:
        void setDisplayImage(const Image* displayImg);

        imagein::Rectangle _applicationArea;
        QString _path;
        const imagein::Image* _displayImg;
        ImageView* _imageView;

        ImageContextMenu* _menu;

        QWidget* _statusBar;
        QLabel* _lImageSize;
        QLabel* _lImageName;
        QLabel* _lZoom;
        QPoint _selectedPixel;
        QToolButton* _selectButton;
        QToolButton* _mouseButton;
        QToolButton* _selectAllButton;
        SelectionWidget* _selectWidget;
        QWidget* _infoWidget;
        QVBoxLayout* _infoLayout;

        QToolButton* _zoomInButton;
        QToolButton* _zoomOutButton;
        QToolButton* _saveAsButton;


        void initStatusBar();

        double _zoomFactor;

        void wheelEvent (QWheelEvent * event);
        void keyPressEvent ( QKeyEvent * event );

        virtual void showHoveredPixelInformations(int x, int y) const = 0;
        virtual void showSelectedPixelInformations(int x, int y) const = 0;
        void showGenericHistogram(GenericHistogramWindow* histogramWnd);


        double _logConstantScale;

       // QAction* copy;


};

}

#endif // IMAGEWINDOW_H
