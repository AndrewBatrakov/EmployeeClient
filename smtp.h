#ifndef SMTP_H
#define SMTP_H

#include <QDialog>
#include <QtGui>
#include <QtNetwork>

class Smtp : public QObject
{
    Q_OBJECT
public:
    Smtp(QString host, QString username, QString pass);
    ~Smtp();
    void SetHost(QString host,QString userName, QString pass);
    void Send(const QString &from, const QString &to,
              const QString &subject, const QString &body);
    int linesend;
    QStringList ErrorMSG;
    QStringList GetErrorlist();

signals:
    void status(const QString &);
    void ConnectorSuccess();
    void SendLine();
    void ErrorCloseAll();
    void SuccessQuit();

private slots:
    void disconnected();
    void connected();
    void ReadLiner();
    void PutSendLine();

private:
    bool isConneccted;
    QString smtphost;
    QString smtpusername;
    QString smtppass;
    QString message;
    QString output;
    QString RemoteServerName;
    QString mailstatus;
    QTextStream *t;
    QTcpSocket *smtpsocet;
    QString from;
    QString rcpt;
    QString responce;
    QString SendLineAndGrab(QString  senddata);
    int Timeout;
    QString encodeBase64(QString xml);
    QString decodeBase64(QString xml);
    int dateswap(QString from, uint unixtime);
    QString TimeStampMail();
};

#endif // SMTP_H
