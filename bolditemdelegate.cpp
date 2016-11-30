#include "bolditemdelegate.h"

BoldItemDelegate::BoldItemDelegate(Qt::GlobalColor color, QObject *parent) :
    QItemDelegate(parent)
{
    colorSet = color;
}

BoldItemDelegate::~BoldItemDelegate()
{

}

void BoldItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();

    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
    myOption.font = QFont("Times New Roman", 10, QFont::Bold,true);
    //if(!colorSet == ""){

        myOption.palette.setColor(QPalette::Text,QColor(colorSet));
//    }else{
//        myOption.palette.setColor(QPalette::Text,QColor(Qt::darkBlue));
//    }
    drawBackground(painter, myOption, index);
    //painter->fillRect(option.rect, QColor(Qt::red));
    drawDisplay(painter,myOption,myOption.rect,text);
    drawFocus(painter,myOption,myOption.rect);
}
