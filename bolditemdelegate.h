#ifndef BOLDITEMDELEGATE_H
#define BOLDITEMDELEGATE_H

#include <QAbstractItemModel>
#include <QItemDelegate>

class BoldItemDelegate : public QItemDelegate
{
public:
    BoldItemDelegate(Qt::GlobalColor, QObject *parent=0);
    ~BoldItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QString colorSet;
};

#endif // BOLDITEMDELEGATE_H
