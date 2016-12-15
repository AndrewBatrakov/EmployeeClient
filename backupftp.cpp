#include "backupftp.h"
//#include "databasedirection.h"
//#include <QtSql>

BackUpFTP::BackUpFTP(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("FTP Client");

    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
}

void BackUpFTP::BackUpProcedure()
{
    QEventLoop loop;
    localFileName = "CE_SQLite.dat";
    filePut = new QFile(localFileName,this);
    if(filePut->open(QIODevice::ReadOnly)){
        QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
        QString host = settings.value("Host").toString();
        QString pass = settings.value("Pass").toString();

        QString name = "ftp://";
        name += host;
        name += "/QtProject/ServerEmployee/Base/CE_SQLite.dat";
        QUrl url(name);
        url.setUserName("ftp");
        url.setPassword(pass);
        url.setPort(2210);

        QNetworkRequest request;
        request.setUrl(url);
        reply = manager.put(request,filePut);
        progressDialogDat = new QProgressDialog(this);
        connect(reply,SIGNAL(uploadProgress(qint64,qint64)),SLOT(updateDataTransferProgress(qint64,qint64)));
        connect(reply,SIGNAL(finished()),SLOT(httpDoneExe()));
        filePut->setParent(reply);

        progressDialogDat->setLabelText(trUtf8("Downloading %1 ...").arg(localFileName));
        progressDialogDat->setEnabled(true);
        if(reply->isRunning()){
            loop.exec();
        }
    }else{
        QMessageBox::warning(this,trUtf8("Attention"),trUtf8("Don't open %1").arg(localFileName));
    }
}

void BackUpFTP::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    progressDialogDat->setMaximum(totalBytes);
    progressDialogDat->setValue(readBytes);
}

void BackUpFTP::httpDoneExe()
{
    filePut->deleteLater();
    reply->deleteLater();
}
