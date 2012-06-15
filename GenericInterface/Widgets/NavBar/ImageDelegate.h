#ifndef QTINTERFACE_WIDGET_IMAGEDELEGATE_H
#define QTINTERFACE_WIDGET_IMAGEDELEGATE_H

#include <QItemDelegate>
#include <QPainter>
#include <QRectF>

#include "Image.h"

namespace genericinterface
{
  class ImageDelegate : public QItemDelegate
  {
    Q_OBJECT
    public:
    ImageDelegate(QSize itemSize);
    /**
	 * @brief Redefinition of paint() from QItemDelegate to change de display in the navigation bar.
	 //TODO parameters
	 * @param painter xxx
	 * @param option xxx
	 * @param index xxx
	 */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
    /**
	 * @brief Redefinition of sizeHint) from QItemDelegate.
	 //TODO parameters
	 * @param option xxx
	 * @param index xxx
	 */
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
    
    const QPixmap* getPixmap(const QModelIndex &index) const;
    QSize getItemSize(const QModelIndex &index) const;
    QRect getCloseRect(const QModelIndex &index, QRect visualRect) const;
    
    public slots:
    bool helpEvent ( QHelpEvent * event, QAbstractItemView * view, const QStyleOptionViewItem & option, const QModelIndex & index );
    private:
        QSize _itemSize;
  };
}

#endif
