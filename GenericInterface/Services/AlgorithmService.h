#ifndef QTINTERFACE_ALGORITHMSERVICE_H
#define QTINTERFACE_ALGORITHMSERVICE_H

#include <QObject>
#include <QToolBar>

#include <Image.h>
#include <Algorithm.h>
#include <GrayscaleImage.h>

#include "../Service.h"
#include "Node.h"



namespace genericinterface
{
    class StandardImageWindow;
    class WindowService;
    class GenericInterface;
    class AlgorithmService : public QObject, public Service
    {
    Q_OBJECT
    public:
        AlgorithmService();

        virtual void display(GenericInterface* gi);
        virtual void connect(GenericInterface* gi);

        virtual void applyAlgorithm(imagein::GenericAlgorithm_t<imagein::Image::depth_t>* algo);

    signals:
        void newImageWindowCreated(NodeId id, StandardImageWindow* widget);

    protected:
        GenericInterface* _gi;
        WindowService* _ws;
        QToolBar* _toolBar;
    };
}

#endif
