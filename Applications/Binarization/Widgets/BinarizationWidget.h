#ifndef BINARIZATIONWIDGET_H
#define BINARIZATIONWIDGET_H

#include <GrayscaleImage.h>

#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <Widgets/ImageWidgets/HistogramWindow.h>

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QString>

using namespace genericinterface;

class BinarizationWidget : public QWidget
{
    Q_OBJECT
public:
    BinarizationWidget(StandardImageWindow* siw, NodeId id);

signals:
    void exportBinarizedImage(NodeId id, Image* im);

private slots:
    void displayThresholdSelection(int index);
    void applyBinarization(int i);
    void exportBinarizedImage();

private:
    NodeId _nodeId;
    Image* _originalImage;
    GrayscaleImage* _originalGrayscaleImage;
    QComboBox* _nbThreshold;
    QLabel* _thresholdValue;
    QSpinBox* _firstThreshold;
    QSpinBox* _secondThreshold;

    HistogramWindow* _histo;
    StandardImageView* _binarizedImage;
};

#endif // MANUALWIDGET_H
