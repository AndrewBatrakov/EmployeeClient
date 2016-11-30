#include "putfile.h"

PutFile::PutFile()
{
}

PutFile::~PutFile()
{
    filePut->close();
    filePut->remove();
}

void PutFile::putFile(QString nameOfFile)
{
    filePut = new QFile;
    filePut->setFileName(nameOfFile);
    filePut->open(QIODevice::ReadOnly);

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString name = settings.value("FtpForm/Address", "").toString();
    name += settings.value("FtpForm/Catalog", "").toString();
    name += "/Change/";
    name += nameOfFile;

    QUrl url(name);
    url.setUserName(settings.value("FtpForm/Login", "").toString());
    url.setPassword(settings.value("FtpForm/Password", "").toString());
    url.setPort(21);

    QEventLoop loop;
    manager = new QNetworkAccessManager(0);
    reply = manager->put(QNetworkRequest(url),filePut);
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
}
