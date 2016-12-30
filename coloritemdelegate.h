#ifndef COLORITEMDELEGATE_H
#define COLORITEMDELEGATE_H

#include <QAbstractItemModel>
#include <QItemDelegate>

class ColorItemDelegate : public QItemDelegate
{
public:
    ColorItemDelegate(QObject *parent=0);
    ~ColorItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // COLORITEMDELEGATE_H
