#include "dateitemdelegate.h"
#include <QDate>
#include <QtWidgets>

DateItemDelegate::DateItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

DateItemDelegate::~DateItemDelegate()
{

}

void DateItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    QTextStream outC(stdout);
//    outC.setCodec("IBM866");
    QString text = index.data(Qt::DisplayRole).toString();
    QString textOut;

    QDateTime timeTemp = QDateTime::fromString(text,"yyyy-MM-ddTHH:mm:ss");
    if(timeTemp.isValid()){
        textOut = timeTemp.toString("dd.MM.yyyy");
    }else{
        QDate timeTemp = QDate::fromString(text,"yyyy-MM-dd");
        textOut = timeTemp.toString("dd.MM.yyyy");
    }

    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
    //myOption.font = QFont("Times New Roman", 10, QFont::Normal,true);
    //myOption.palette.setColor(QPalette::Text,QColor(Qt::darkBlue));
    drawBackground(painter, myOption, index);
    //painter->fillRect(option.rect, QColor(Qt::red));
    drawDisplay(painter,myOption,myOption.rect,textOut);
    drawFocus(painter,myOption,myOption.rect);
}
