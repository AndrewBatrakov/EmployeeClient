#ifndef UPLOADBASE_H
#define UPLOADBASE_H

#include <QtNetwork>
#include <QtWidgets>

class UpLoadBase : public QDialog
{
    Q_OBJECT
public:
    UpLoadBase(QWidget *parent=0);

public slots:
    void exeVersion();

private slots:
    void httpDoneExe();
    void closeConnection();
    void updateDataReadProgressExe(qint64, qint64);
    void uncompressFile();

private:
    QUrl url;
    QNetworkAccessManager httpExe;
    QNetworkReply *replyExe;
    QFile *fileHttpExe;
    QProgressDialog *progressDialogExe;
};
#endif // UPLOADBASE_H
