#ifndef INTITEMDELEGATE_H
#define INTITEMDELEGATE_H

//#include <QAbstractItemModel>
//#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets>

class IntItemDelegate : public QStyledItemDelegate
{
    //Q_OBJECT

public:
    //IntItemDelegate(QObject *parent=0);
    //~IntItemDelegate();

    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QWidget *w = QStyledItemDelegate::createEditor(parent,option,index);
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(w);
        if(lineEdit){
            //QRegExp regExp("[0-9]{10}");
            lineEdit->setInputMask("99");//->setValidator(new QRegExpValidator(regExp));
        }
        return w;
    }
};

#endif // INTITEMDELEGATE_H
