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

#include "../../GenericInterface.h"
#include "../../Services/FileService.h"
#include <UnknownFormatException.h>
#include "ImageWindow.h"
#include <QSpinBox>
#include <QDrag>
#include <QMimeData>
#include <QImage>
#include <QClipboard>
#include <QRect>
#include <QAction>
#include <string>
using namespace std;
using namespace genericinterface;
using namespace imagein;

SelectionWidget::SelectionWidget(QWidget* parent, int width, int height) : QWidget(parent) {
    this->setFont(QFont("arial", 8));
    _Xspinbox = new QSpinBox(this);
    _Yspinbox = new QSpinBox(this);
    _Wspinbox = new QSpinBox(this);
    _Hspinbox = new QSpinBox(this);
    this->setRange(width, height);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel(tr("Selection : ")));
    layout->addWidget(new QLabel(tr("x:")));
    layout->addWidget(_Xspinbox);
    layout->addWidget(new QLabel(tr("y:")));
    layout->addWidget(_Yspinbox);
    layout->addWidget(new QLabel(tr("width:")));
    layout->addWidget(_Wspinbox);
    layout->addWidget(new QLabel(tr("height:")));
    layout->addWidget(_Hspinbox);
    connect(_Xspinbox, SIGNAL(valueChanged(int)), this, SLOT(selectionMoved(int)));
    connect(_Yspinbox, SIGNAL(valueChanged(int)), this, SLOT(selectionMoved(int)));
    connect(_Wspinbox, SIGNAL(valueChanged(int)), this, SLOT(selectionMoved(int)));
    connect(_Hspinbox, SIGNAL(valueChanged(int)), this, SLOT(selectionMoved(int)));
}
void SelectionWidget::setRange(int width, int height) {
    _Xspinbox->setRange(0, width);
    _Yspinbox->setRange(0, height);
    _Wspinbox->setRange(0, width);
    _Hspinbox->setRange(0, height);
}
void SelectionWidget::updateSelection(QRect select) {
    this->blockSignals(true);
    _Xspinbox->setValue(select.x());
    _Yspinbox->setValue(select.y());
    _Wspinbox->setValue(select.width());
    _Hspinbox->setValue(select.height());
    this->blockSignals(false);
}
void SelectionWidget::selectionMoved(int) {
    emit selectionMoved(QRect(_Xspinbox->value(), _Yspinbox->value(), _Wspinbox->value(), _Hspinbox->value()));
}

ImageWindow::ImageWindow(QString path, const Image* displayImg, Rectangle rect)
    : _path(path), _displayImg(displayImg)
{
    _applicationArea = rect;
    _zoomFactor = 1;
    _imageView = new ImageView(this, _displayImg);
    _menu = new ImageContextMenu(_imageView);
    _imageView->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(_imageView, SIGNAL(customContextMenuRequested(const QPoint&)), _menu, SLOT(showContextMenu(const QPoint&)));
    _statusBar = new QWidget();
    initStatusBar();
    menu()->addAction(tr("Save As"), this, SLOT(saveAs()),QKeySequence::Save);
    menu()->addSeparator();

    menu()->addAction(tr("Zoom + (Ctrl+Scroll Wheel)"), this, SLOT(zoom_in()),QKeySequence::ZoomIn);
    menu()->addAction(tr("Zoom - (Ctrl+Scroll Wheel)"), this, SLOT(zoom_out()),QKeySequence::ZoomOut);
    menu()->addSeparator();
    menu()->addAction(tr("Rename"), this, SLOT(rename()));
    menu()->addSeparator();
    menu()->addAction(tr("Apply mask"), this, SLOT(applyBinaryMask()));
    menu()->addSeparator();
    menu()->addAction(tr("Copy image"), this, SLOT(copyImage()),QKeySequence::Copy);
    menu()->addSeparator();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_imageView);
    layout->addWidget(_statusBar);

    QObject::connect(_imageView, SIGNAL(pixelClicked(int, int)), this, SLOT(pixelClicked(int, int)));
    QObject::connect(_imageView, SIGNAL(pixelHovered(int, int)), this, SLOT(pixelHovered(int, int)));
    QObject::connect(_imageView, SIGNAL(startDrag()), this, SLOT(startDrag()));
}

void ImageWindow::setDisplayImage(const Image* displayImg) {
    _displayImg = displayImg;
    _imageView->setImage(displayImg);
    QString width = QString("%1").arg(_imageView->pixmap().width());
    QString height = QString("%1").arg(_imageView->pixmap().height());

    _lImageSize->setText(QString("(%1x%2)").arg(width, height));
    _selectWidget->setRange(displayImg->getWidth(), displayImg->getHeight());
}

ImageWindow::~ImageWindow() {
    delete _menu;
}

void ImageWindow::initStatusBar()
{
    QString width = QString("%1").arg(_imageView->pixmap().width());
    QString height = QString("%1").arg(_imageView->pixmap().height());

    QFont font;
    QVBoxLayout* layout = new QVBoxLayout(_statusBar);
    layout->setContentsMargins(0, 0, 0, 0);

    _lImageName = new QLabel(tr("Image") + " : " + ImageWindow::getTitleFromPath(_path));
    font = _lImageName->font();
    font.setPointSize(8);
    font.setBold(true);
    _lImageName->setFont(font);

    _lImageSize = new QLabel(QString("(%1x%2)").arg(width, height));
    font = _lImageSize->font();
    font.setPointSize(8);
    _lImageSize->setFont(font);

    _lZoom = new QLabel(tr("Zoom") + " : 100%");
    font = _lZoom->font();
    font.setPointSize(8);
    _lZoom->setFont(font);

    _selectButton = new QToolButton(this);
    _selectButton->setToolTip(tr("Selection mode"));
    _selectButton->setIcon(QIcon(":/images/tool-rect-select.png"));
    _selectButton->setCheckable(true);
    _selectButton->setAutoRaise(true);
    _selectButton->setIconSize (QSize(24, 24));

    _mouseButton = new QToolButton(this);
    _mouseButton->setToolTip(tr("Hand mode"));
    _mouseButton->setIcon(QIcon(":/images/tool-smudge.png"));
    _mouseButton->setCheckable(true);
    _mouseButton->setAutoRaise(true);
    _mouseButton->setIconSize (QSize(24, 24));
    _mouseButton->setChecked(true);

    _selectAllButton = new QToolButton(this);
    _selectAllButton->setToolTip(tr("Select all"));
    _selectAllButton->setIcon(QIcon(":/images/tool-align.png"));
    _selectAllButton->setCheckable(false);
    _selectAllButton->setAutoRaise(true);
    _selectAllButton->setIconSize (QSize(24, 24));
    _selectAllButton->setEnabled(false);

    _zoomInButton = new QToolButton(this);
    _zoomInButton->setToolTip(tr("Zoom +"));
    _zoomInButton->setIcon(QIcon(":/images/zoom-in.svg.png"));
    _zoomInButton->setCheckable(false);
    _zoomInButton->setAutoRaise(true);
    _zoomInButton->setIconSize (QSize(24, 24));


    _zoomOutButton = new QToolButton(this);
    _zoomOutButton->setToolTip(tr("Zoom -"));
    _zoomOutButton->setIcon(QIcon(":/images/zoom-out.svg.png"));
    _zoomOutButton->setCheckable(false);
    _zoomOutButton->setAutoRaise(true);
    _zoomOutButton->setIconSize (QSize(24, 24));

    _saveAsButton = new QToolButton(this);
    _saveAsButton->setToolTip(tr("Save As"));
    _saveAsButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
    _saveAsButton->setCheckable(false);
    _saveAsButton->setAutoRaise(true);
    _saveAsButton->setIconSize (QSize(24, 24));

    _selectWidget = new SelectionWidget(this, _imageView->pixmap().width(), _imageView->pixmap().height());
    connect(_imageView, SIGNAL(selectionMoved(QRect)), _selectWidget, SLOT(updateSelection(QRect)));
    connect(_selectWidget, SIGNAL(selectionMoved(QRect)), _imageView, SLOT(moveSelection(QRect)));
    _selectWidget->hide();

    _infoWidget = new QWidget(this);
    _infoLayout = new QVBoxLayout(_infoWidget);

    QObject::connect(_mouseButton, SIGNAL(toggled(bool)), this, SLOT(toggleMouseMode(bool)));
    QObject::connect(_selectButton, SIGNAL(toggled(bool)), this, SLOT(toggleSelectMode(bool)));
    QObject::connect(_selectAllButton, SIGNAL(clicked()), _imageView, SLOT(selectAll()));
    QObject::connect(_zoomInButton, SIGNAL(clicked()), this, SLOT(zoom_in()));
    QObject::connect(_zoomOutButton, SIGNAL(clicked()), this, SLOT(zoom_out()));
    QObject::connect(_saveAsButton, SIGNAL(clicked()), this, SLOT(saveAs()));

    QWidget* widgetImage = new QWidget();
    QHBoxLayout* layoutImage = new QHBoxLayout(widgetImage);
    layoutImage->setContentsMargins(0, 0, 0, 0);
    layoutImage->addWidget(_lImageName);
    layoutImage->addWidget(_lImageSize);
    layoutImage->addSpacing(30);
    layoutImage->addWidget(_lZoom);
    layoutImage->addSpacing(30);
    layoutImage->setSpacing(0);
    layoutImage->addWidget(_mouseButton);
    layoutImage->addWidget(_selectButton);
    layoutImage->addWidget(_selectAllButton);
    layoutImage->addWidget(_zoomInButton);
    layoutImage->addWidget(_zoomOutButton);
    layoutImage->addWidget(_saveAsButton);
    layoutImage->addSpacing(8);

    layout->addWidget(widgetImage);
    layout->addWidget(_selectWidget);
    layout->addWidget(_infoWidget);

}


void ImageWindow::activated()
{
    emit(selectRectChange(_applicationArea, this));
}

void ImageWindow::setApplicationArea(imagein::Rectangle rect)
{
    _applicationArea = rect;
}

QString ImageWindow::getTitleFromPath(QString path)
{
    std::string p = path.toStdString();
    size_t pos = p.rfind("\\");

    if(pos != std::string::npos)
        p = p.substr(pos + 1);
    else if((pos = p.rfind("/")) != std::string::npos)
        p = p.substr(pos + 1);

    return QString::fromStdString(p);
}


void ImageWindow::startDrag() {
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
    stream << QVariant::fromValue(ptr);
    if(_imageView->mode() == ImageView::MODE_MOUSE) {
        mimeData->setData("application/detiqt.genericinterface.stdimgwnd", encodedData);
        drag->setPixmap(_imageView->pixmap().scaled(QSize(76,76), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
    else {
        mimeData->setData("application/detiqt.genericinterface.stdimgwnd.copy", encodedData);
        drag->setPixmap(_imageView->pixmap().copy(_imageView->select()).scaled(QSize(76,76), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));
    drag->exec();
}

void ImageWindow::toggleMouseMode(bool checked) {
    if(checked) {
        _selectButton->setChecked(false);
        _imageView->switchMode(ImageView::MODE_MOUSE);
        _selectAllButton->setEnabled(false);
    }
    else {
        if(!_selectButton->isChecked()) {
            _mouseButton->setChecked(true);
        }
    }
}

void ImageWindow::toggleSelectMode(bool checked) {
    if(checked) {
        _mouseButton->setChecked(false);
        _imageView->switchMode(ImageView::MODE_SELECT);
        _selectAllButton->setEnabled(true);
        _infoWidget->hide();
        _selectWidget->show();
    }
    else {
        if(!_mouseButton->isChecked()) {
            _selectButton->setChecked(true);
        }
        _selectWidget->hide();
        _infoWidget->show();
    }
}


void ImageWindow::save(const QString& path, const QString& ext)
{
    if(path == "") {
        this->saveAs();
    }
    else {
        try {
           // WindowService* ws = _gi->windowService();
           // if(ws != NULL) {

                if(this != NULL) {
                    try{
                        _displayImg->save(path.toStdString());
                    }
                    catch(const UnknownFormatException& e) {
                        if(ext == "")
                            throw e;

                        _displayImg->save((path+ext).toStdString());
                    }
                }
                else {
                    QMessageBox::critical(this, tr("Bad object type"), tr("Only images can be saved to a file."));
                }
           // }
        }
        catch(const char* s) {
            QMessageBox::information(this, tr("Unknown exception"), s);
        }
    }
}

void ImageWindow::saveAs()
{
    QString path;
    //WindowService* ws = _gi->windowService();
    //ImageWindow* currentWindow = ws->getCurrentImageWindow();
    if(this != NULL) {
    /*Use the processed name as the default name to be proposed*/
//        path = this->getPath();
        path = this->windowTitle();
    }
    QString selectedFilter;
    QString file = QFileDialog::getSaveFileName(this, tr("Save a file"), path, tr("PNG image (*.png);;BMP image (*.bmp);; JPEG image (*.jpeg *.jpg);; VFF image (*.vff)"), &selectedFilter);

    QString ext = selectedFilter.right(5).left(4);
    if(file != "") {
//        if(!file.contains('.')) file += ext;
        /*Verify that the suffix of the original picture is the same as expected
         * if different, replace the suffix with the one requested by the user*/
        std::string existExt = file.toStdString().substr(file.toStdString().find_last_of('.'));
        if(strcmp(ext.toStdString().c_str(),existExt.c_str())!=0){
            std::string newFile = file.toStdString().substr(0,file.toStdString().find_last_of('.'));
            newFile.append(ext.toStdString());
            file = QString::fromLocal8Bit(newFile.c_str());
        }
        this->save(file, ext);
    }
}

void ImageWindow::zoom(int delta) {
    const double coef = 1.4142135623730950488016887242096980785696718753769480;
    if(delta < 0 && _imageView->widget()->width() > 16) //Zoom out
    {
        _zoomFactor /= coef;
    }
    else if(delta > 0)//Zoom in
    {
        _zoomFactor *= coef;
    }
    else if(delta == 0)
    {
        _zoomFactor = 1;
    }
    _imageView->scale(_zoomFactor, _zoomFactor);
    //if(delta < 0) {
        //_imageView->resize(_imageView->sizeHint());
        //QApplication::processEvents();
        //this->adjustSize();
        //QApplication::processEvents();
        //this->
    //}
    updateZoom(_zoomFactor*100);
}

void ImageWindow::zoom_in() {
    const double coef = 1.4142135623730950488016887242096980785696718753769480;
    _zoomFactor *= coef;
    _imageView->scale(_zoomFactor, _zoomFactor);
    //if(delta < 0) {
        //_imageView->resize(_imageView->sizeHint());
        //QApplication::processEvents();
        //this->adjustSize();
        //QApplication::processEvents();
        //this->
    //}
    updateZoom(_zoomFactor*100);
}

void ImageWindow::zoom_out() {
    const double coef = 1.4142135623730950488016887242096980785696718753769480;
    _zoomFactor /= coef;
    _imageView->scale(_zoomFactor, _zoomFactor);
    //if(delta < 0) {
        //_imageView->resize(_imageView->sizeHint());
        //QApplication::processEvents();
        //this->adjustSize();
        //QApplication::processEvents();
        //this->
    //}
    updateZoom(_zoomFactor*100);
}

void ImageWindow::updateZoom(double z) const
{
    _lZoom->setText(tr("Zoom") + QString(" : %1\%").arg(z));
}

void ImageWindow::wheelEvent (QWheelEvent * event) {
    bool ctrlPressed = event->modifiers().testFlag(Qt::ControlModifier);
    if (ctrlPressed && event->orientation() == Qt::Vertical)
    {
        this->zoom(event->delta());
    }
}

void ImageWindow::keyPressEvent(QKeyEvent *event){
    bool ctrlPressed = event->modifiers().testFlag(Qt::ControlModifier);
    if(ctrlPressed && (event->key() == Qt::Key_Plus)) this->zoom(100);
    if(ctrlPressed && (event->key() == Qt::Key_Minus)) this->zoom(-100);
    if(ctrlPressed && (event->key() == Qt::Key_0)) this->zoom(0);

}

void ImageWindow::pixelClicked(int x, int y) {
    _selectedPixel = QPoint(x, y);
    this->showSelectedPixelInformations(x, y);
}

void ImageWindow::pixelHovered(int x, int y) {
    this->showHoveredPixelInformations(x, y);
}

void ImageWindow::rename() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Rename image"),
                                         tr("New name:"), QLineEdit::Normal,
                                         windowTitle(), &ok);
    if (ok && !text.isEmpty())
        this->setWindowTitle(text);
}

void ImageWindow::applyBinaryMask() {
    emit applyBinaryMask(this);
}

void ImageWindow::showHistogram()
{
    HistogramWindow* histogramWnd = new HistogramWindow(_displayImg, selection(), this->windowTitle());
    showGenericHistogram(histogramWnd);
}

void ImageWindow::showGenericHistogram(GenericHistogramWindow* histogramWnd) {

    _imageView->setSelectSrc(histogramWnd->getView());
    QObject::connect(histogramWnd, SIGNAL(selectRectChange(imagein::Rectangle, GenericHistogramView*)), _imageView, SLOT(showSelectRect(imagein::Rectangle, GenericHistogramView*)));

    emit addWidget(this, histogramWnd);
}

void ImageWindow::copyImage(){
    QImage img = _imageView->widget()->convertImage(_displayImg);
    QClipboard *clipboard = QApplication::clipboard();
    if(_selectButton->isChecked()){
        QRect rect(_selectWidget->getX()->value(),_selectWidget->getY()->value(),_selectWidget->getWidth()->value(),_selectWidget->getHeight()->value());
        QImage image = img.copy(rect);
        clipboard->setImage(image);
    }
    else
    {
        clipboard->setImage(img);
    }
}



//void ImageWindow::keyPressEvent ( QKeyEvent * /*event*/ ) {
////    if(event->
//}
