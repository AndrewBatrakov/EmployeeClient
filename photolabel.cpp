#include <QtWidgets>
#include <QtSql>
#include "photolabel.h"
//#include "employee.h"

PhotoLabel::PhotoLabel(QWidget *parent) :
    QLabel(parent)
{
}

void PhotoLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        emit clicked();
    }
    if(ev->button() == Qt::RightButton){
        QMenu menu(this);
        QPixmap pixD(":/delete.png");
        menu.addAction(pixD,tr("Delete Photo"));
        menu.setContextMenuPolicy(Qt::ActionsContextMenu);
        QAction *m = menu.exec(ev->globalPos());
        if(m){
            QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
            QString in = settings.value("indexPhoto", "").toString();
            QSqlQuery query;
            query.prepare("DELETE FROM photo WHERE photoid = :id;");
            query.bindValue(":id",in);
            query.exec();
            query.next();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::tr("DataBase ERROR!"),query.lastError().text());
            }
            this->clear();
            this->setText(tr("No Photo"));
        }

    }
}
