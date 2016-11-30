#include <QtSql>
#include "fordelete.h"
#include "numprefix.h"

ForDelete::ForDelete(QString id, QString tabNameAud, QWidget *parent) :
    QDialog(parent)
{
    tabNameAuditee = tabNameAud;
    indexTemp = id;
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    tabNameAuditee = settings.value("Table").toString();
    indexTemp = settings.value("Index").toString();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QSqlQuery query;
    query.exec("select tbl_name from sqlite_master where type = 'table'");//SQLite

    QLabel *newLabel = new QLabel;
    labelLabel = new QLabel;
    QString toOut;
    QPushButton *okButton = new QPushButton("Ok");
    connect(okButton,SIGNAL(clicked()),this,SLOT(close()));

    while(query.next()){
        if(tabNameAuditee != query.value(0).toString()){//имя таблицы
            QSqlQuery queryTable;

            QString table = query.value(0).toString();
            QString tableid = table;
            tableid += "id";
            QString tableName = table;
            tableName += "name";
            QString textQuery = "SELECT ";
            textQuery += tableid;
            textQuery += ", ";
            textQuery += tableName;
            textQuery += " FROM ";
            textQuery += table;
            textQuery += " WHERE ";
            textQuery += tabNameAuditee;
            textQuery += "id = '";
            textQuery += indexTemp;
            textQuery += "';";
            queryTable.exec(textQuery);
            queryTable.next();
            if(queryTable.isActive()){
                toOut = tr("Table ");
                toOut += table;
                toOut += tr(" contains - ");
                toOut += QString::number(queryTable.size());
                toOut += tr(" records.");
                newLabel->setStyleSheet("font: bold; color: red;");
            }else{
                toOut = "No related Tables! Deleting.";
                newLabel->setStyleSheet("font: bold; color: darkblue;");
            }
        }
    }
    newLabel->setText(toOut);
    mainLayout->addWidget(newLabel);
    mainLayout->addWidget(okButton);
    setLayout(mainLayout);
    setWindowTitle(tr("Conditions for removal..."));
}

void ForDelete::deleteOnDefault()
{
    QSqlQuery query;
    query.exec("select tbl_name from sqlite_master where type = 'table'");
    while(query.next()){
        if(tabNameAuditee != query.value(0).toString()){//имя таблицы
            QSqlQuery queryTable;
            QString table = query.value(0).toString();
            QString tableid = table;
            tableid += "id";
            QString tableName = table;
            tableName += "name";
            QString textQuery = "SELECT ";
            textQuery += tableid;
            textQuery += ", ";
            textQuery += tableName;
            textQuery += " FROM ";
            textQuery += table;
            textQuery += " WHERE ";
            textQuery += tabNameAuditee;
            textQuery += "id = %1";
            textQuery.arg(indexTemp);
            queryTable.exec(textQuery);
            while(queryTable.next()){
                QSqlQuery queryCheck;
                QString textCheck = "SELECT ";
                textCheck += tabNameAuditee;
                textCheck += "id FROM ";
                textCheck += tabNameAuditee;
                textCheck += " WHERE ";
                textCheck += tabNameAuditee;
                textCheck += "name = 'No Value'";
                queryCheck.exec(textCheck);
                queryCheck.next();
                QString indexToUpdate;
                if(!queryCheck.isValid()){
                    NumPrefix numPrefix;
                    indexToUpdate = numPrefix.getPrefix(tabNameAuditee);
                    QSqlQuery queriInto;
                    QString textInto = "INSERT INTO ";
                    textInto += tabNameAuditee;
                    textInto += " (";
                    textInto += tabNameAuditee;
                    textInto += "id, ";
                    textInto += tabNameAuditee;
                    textInto += "name) VALUES(%1, %2)";
                    textInto.arg(indexToUpdate).arg("No Value");
                    queriInto.exec(textInto);
                }else{
                    QSqlQuery queryReplace;
                    //QString tableid = table;
                    //tableid += "id";
                    //QString tableName = table;
                    //tableName += "name";
                    QString textUpdate = "UPDATE ";
                    textUpdate += table;
                    textUpdate += " SET ";
                    textUpdate += tabNameAuditee;
                    textUpdate += "id = %1";
                    textUpdate += " WHERE ";
                    textUpdate += tableid;
                    textUpdate += "id = %2";
                    textUpdate.arg(indexToUpdate).arg(queryTable.value(0).toString());
                    queryReplace.exec(textUpdate);
                }
            }
        }
    }
}
