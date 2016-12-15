#include <QtGui>
#include "databasedirection.h"

DataBaseDirection::DataBaseDirection()
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
}


bool DataBaseDirection::connectDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("CE_SQLite.dat");
    if (!db.open()) {
        QMessageBox::warning(0, QObject::trUtf8("SQLite Database Error"),
                             db.lastError().text());
        return false;
    }else{
        return true;
    }
}

void DataBaseDirection::closeDataBase()
{
    db.close();
    db.removeDatabase("QSQLITE");
    //QSqlDatabase::removeDatabase("QSQLITE");
    //if(QFile::remove("CE_SQLite.dat"));
}
