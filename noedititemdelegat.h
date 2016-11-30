#ifndef NOEDITITEMDELEGAT_H
#define NOEDITITEMDELEGAT_H

#include <QAbstractItemModel>
#include <QItemDelegate>

class NoEditItemDelegat : public QItemDelegate
{
public:
    NoEditItemDelegat(QObject *parent=0) : QItemDelegate(parent) {}

    virtual QWidget * createEditor ( QWidget *, const QStyleOptionViewItem &,
                                        const QModelIndex &) const
       {
           return 0;
       }

};

#endif // NOEDITITEMDELEGAT_H
