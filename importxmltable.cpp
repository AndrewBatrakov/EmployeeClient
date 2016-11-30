#include "importxmltable.h"
#include <QtSql>
#include "numprefix.h"

ImportXmlTable::ImportXmlTable(QWidget *parent) : QDialog(parent)
{
    QDir::setCurrent("d:/apache/QtProject/ServerEmployee/");
}
void ImportXmlTable::importXMLTable()
{
    QDomDocument domDoc;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"),".","*.xml");
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)){
        if(domDoc.setContent(&file)){
            QDomElement domElement = domDoc.documentElement();
            traverse(domElement);
        }
        file.close();
    }
}

void ImportXmlTable::traverse(const QDomNode &node)
{
    QTextStream outC(stdout);
    outC.setCodec("IBM866");
    NumPrefix numPrefix;

    QDomNode domNode = node.firstChild();

    while(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "Element"){
                    QString personName;
                    personName = domElement.attribute("Employee","").simplified();
                    QString upperName;
                    upperName = personName.toUpper();
                    QString dateName;
                    dateName = domElement.attribute("DataRozdeniya","");

                    QString sizName[30];
                    for(int i = 1; i < 30; ++i){
                        QString domString = "sizName";
                        domString += QString::number(i);
                        sizName[i] = domElement.attribute(domString,"").simplified();
                        if(sizName[i].isEmpty())
                            break;
                    }

                    QString sizOstatok[30];
                    for(int i = 1; i < 30; ++i){
                        QString domString = "sizOstatok";
                        domString += QString::number(i);
                        sizOstatok[i] = domElement.attribute(domString,"").simplified();
                        if(sizOstatok[i].isEmpty())
                            break;
                    }

                    QString sizData[30];
                    for(int i = 1; i < 30; ++i){
                        QString domString = "sizData";
                        domString += QString::number(i);
                        sizData[i] = domElement.attribute(domString,"").simplified();
                        if(sizData[i].isEmpty())
                            break;
                    }

                    QString sizSrok[30];
                    for(int i = 1; i < 30; ++i){
                        QString domString = "sizSrok";
                        domString += QString::number(i);
                        sizSrok[i] = domElement.attribute(domString,"").simplified();
                        if(sizData[i].isEmpty())
                            break;
                    }

                    QSqlQuery queryPer;
                    queryPer.prepare("SELECT employeeid FROM employee "
                                     "WHERE (employeename = :employeename AND "
                                     "datebirthday = :datebirthday);");
                    queryPer.bindValue(":employeename",personName);
                    queryPer.bindValue(":datebirthday",dateName);
                    queryPer.exec();
                    queryPer.next();
                    if(queryPer.isValid()){
                        QString personId = queryPer.value(0).toString();
                        //***************************************************
                        for(int i = 1; i < 30; ++i){
                            if(sizName[i] != ""){
                                QSqlQuery querySIZ;
                                querySIZ.prepare("SELECT siznaimid FROM siznaim WHERE siznaimname = :siznaimname");
                                querySIZ.bindValue(":siznaimname",sizName[i]);
                                querySIZ.exec();
                                querySIZ.next();
                                QString idSizNaim;
                                if(!querySIZ.isValid()){
                                    QSqlQuery query;
                                    idSizNaim = numPrefix.getPrefix("siznaim");
                                    query.prepare("INSERT INTO siznaim ("
                                                  "siznaimid, "
                                                  "siznaimname"
                                                  ") VALUES(:siznaimid, :siznaimname);");
                                    query.bindValue(":siznaimid",idSizNaim);
                                    query.bindValue(":siznaimname",sizName[i]);
                                    query.exec();
                                }else{
                                    idSizNaim = querySIZ.value(0).toString();
                                }
                                QSqlQuery queryOrg;
                                queryOrg.prepare("SELECT * FROM siz WHERE (employeeid = :employeeid AND siznaimid = :siznaimid);");
                                queryOrg.bindValue(":employeeid",personId);
                                queryOrg.bindValue(":siznaimid",idSizNaim);
                                queryOrg.exec();
                                queryOrg.next();

                                int mon = sizSrok[i].toInt();
                                QDateTime dat = QDateTime::fromString(sizData[i],"yyyy-MM-ddTHH:mm:ss");
                                QDateTime sr = dat.addMonths(mon);
                                QString ww = sr.toString("yyyy-MM-ddTHH:mm:ss");
                                QString idSiz;

                                if(!queryOrg.isValid()){
                                    if(sr > QDateTime::currentDateTime()){
                                        QSqlQuery query;
                                        idSiz = numPrefix.getPrefix("siz");
                                        query.prepare("INSERT INTO siz ("
                                                      "sizid, "
                                                      "employeeid, "
                                                      "siznaimid, "
                                                      "ostatok, "
                                                      "dataperedachi, "
                                                      "srokisp, "
                                                      "dataokon"
                                                      ") VALUES(:sizid, :employeeid, :siznaimid, :ostatok, "
                                                      ":dataperedachi, :srokisp, :dataokon);");
                                        query.bindValue(":employeeid",personId);
                                        query.bindValue(":sizid",idSiz);
                                        query.bindValue(":siznaimid",idSizNaim);
                                        query.bindValue(":ostatok",sizOstatok[i]);
                                        query.bindValue(":dataperedachi",sizData[i]);
                                        query.bindValue(":srokisp",sizSrok[i]);
                                        query.bindValue(":dataokon",ww);
                                        query.exec();
                                        query.next();
                                        if(!query.isActive()){
                                            qDebug()<<"SIZ Insert ERROR!"<<query.lastError().text();
                                        }
                                    }
//                                }else{
//                                    QSqlQuery query;
//                                    query.prepare("UPDATE siz SET "
//                                                  "employeeid = :employeeid, "
//                                                  "siznaimid = :siznaimid, "
//                                                  "ostatok = :ostatok, "
//                                                  "dataperedachi = :dataperedachi, "
//                                                  "srokisp = :srokisp, "
//                                                  "dataokon = :dataokon "
//                                                  "WHERE sizid = :sizid;");
//                                    query.bindValue(":employeeid",personId);
//                                    query.bindValue(":sizid",queryOrg.value(0).toString());
//                                    query.bindValue(":siznaimid",idSizNaim);
//                                    query.bindValue(":ostatok",sizOstatok[i]);
//                                    query.bindValue(":dataperedachi",sizData[i]);
//                                    query.bindValue(":srokisp",sizSrok[i]);
//                                    query.bindValue(":dataokon",ww);
//                                    query.exec();
//                                    query.next();
//                                    if(!query.isActive()){
//                                        qDebug()<<"SIZ Update ERROR!"<<query.lastError().text();
//                                    }
                                }
                            }
                        }

                    }
                }
            }
        }
        traverse(domNode);
        domNode = domNode.nextSibling();
    }
}

