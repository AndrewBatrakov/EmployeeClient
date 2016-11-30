#ifndef BACKUPFTP_H
#define BACKUPFTP_H

#include <QtWidgets>
#include <QtNetwork>

class BackUpFTP : public QDialog
{
    Q_OBJECT

public:
    BackUpFTP(QWidget *parent);
    void BackUpProcedure();

private slots:
    //void replyFinished(QNetworkReply*);
    //void cancelDownLoadDat();
    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);
    void httpDoneExe();
    //void closeConnection();

private:
    QString urlString;
    QFile *filePut;
    QProgressDialog *progressDialogDat;
    QNetworkAccessManager manager;
    QNetworkReply *reply;

    QString pref, localFileName;
};

#endif // BACKUPFTP_H
