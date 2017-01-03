#include "coloritemdelegate.h"

ColorItemDelegate::ColorItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

ColorItemDelegate::~ColorItemDelegate()
{

}

void ColorItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    const QAbstractItemModel* model = index.model();
    QStyleOptionViewItem myOption = option;

    bool propusk = model->data(model->index(index.row(), 18)).toBool();
    if(propusk){
        myOption.palette.setColor(QPalette::Text,Qt::red);
    }else{
        myOption.palette.setColor(QPalette::Text,Qt::black);
    }
    drawBackground(painter, myOption, index);
    drawDisplay(painter,myOption,myOption.rect,text);
    drawFocus(painter,myOption,myOption.rect);
}
