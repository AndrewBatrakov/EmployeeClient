#ifndef DATEITEMDELEGATE_H
#define DATEITEMDELEGATE_H

#include <QAbstractItemModel>
#include <QItemDelegate>

class DateItemDelegate : public QItemDelegate
{
public:
    DateItemDelegate(QObject *parent=0);
    ~DateItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DATEITEMDELEGATE_H
