#include <QtWidgets>
#include <QtSql>
#include "numprefix.h"
#include "photoload.h"

PhotoLoad::PhotoLoad(QWidget *parent) :
    QDialog(parent)
{
}

void PhotoLoad::importPhotoProcedure()
{
    QString dirName = QFileDialog::getExistingDirectory(this,trUtf8("Open Files"),"/home",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);

    QDir dir(dirName);
    //dir.setPath(dirName);
    QStringList fileNames = dir.entryList();

    QProgressDialog progressR(this);
    progressR.setWindowModality(Qt::WindowModal);
    progressR.setWindowTitle(trUtf8("Load photo"));
    int countValue = 1;
    progressR.setMinimum(countValue);
    progressR.setMaximum(fileNames.count());
    progressR.show();

    qApp->processEvents();
    QString title;
    foreach (QString fileN, fileNames) {
        title = QString::number(countValue);
        title += " from ";
        title += QString::number(fileNames.count());
        progressR.setWindowTitle(title);
        progressR.setValue(countValue);
        progressR.activateWindow();
        progressR.show();
        QString fileLoad = dirName;
        fileLoad += "/";
        fileLoad += fileN;
        QFile file(fileLoad);
        if(file.open(QIODevice::ReadOnly)){
            QFileInfo infoFile;
            infoFile.setFile(file.fileName());
            QByteArray imageByte = file.readAll();
            file.close();
            if(infoFile.size() < 300000){

                QString tempFileName = QFileInfo(file).baseName();
                QString tabNumber = tempFileName.mid(0,10);
                while(tabNumber.mid(0,1) == "0"){
                    tabNumber.remove(0,1);
                }
                //QString nameEmp = tempFileName.mid(11);
                QSqlQuery query;
                query.prepare("SELECT employeeid FROM employee WHERE tabnumber = :tabnum;");
                query.bindValue(":tabnum",tabNumber);
                if (!query.exec()) {
                    //errorMessage(query.lastError().text() + "\nGetViewsByUrl:1");
                    while (query.exec() == false) {}
                }
                query.next();
                if(!query.isActive()){
                    QMessageBox::warning(this,QObject::trUtf8("Employee, SELECT Table ERROR!"),query.lastError().text());
                }

                if(query.isValid()){
                    //QPixmap pMap;
                    //QImage re = pMap.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation);

                    QSqlQuery queryEx;
                    QString indexTemp = query.value(0).toString();
                    queryEx.prepare("SELECT photoid FROM photo WHERE photoid = :id");
                    queryEx.bindValue(":id",indexTemp);
                    queryEx.exec();
                    queryEx.next();
                    if(!queryEx.isActive()){
                        QMessageBox::warning(this,QObject::trUtf8("Photo id, SELECT Table ERROR!"),queryEx.lastError().text());
                    }

                    progressR.show();
                    qApp->processEvents();
                    if(queryEx.value(0).toString() == ""){
                        QSqlQuery query;
                        query.prepare("INSERT INTO photo ("
                                      "photoid, "
                                      "photoname) VALUES("
                                      ":photid, :photoname)"
                                      );
                        query.bindValue(":photid",indexTemp);
                        query.bindValue(":photoname",imageByte);
                        query.exec();
                        if(!query.isActive()){
                            QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR!"),query.lastError().text());
                        }
                        progressR.show();
                        qApp->processEvents();
                    }
                }
            }else{
                QFile badFile;
                badFile.setFileName("./BadFiles.txt");
                badFile.open(QIODevice::Append);
                QTextStream stream(&badFile);
                stream<<fileLoad;
                stream<<"\r\n";

                badFile.close();
            }
            progressR.setValue(countValue);
            //file.close();
        }
        ++countValue;
        progressR.show();
        qApp->processEvents();
    }
}

