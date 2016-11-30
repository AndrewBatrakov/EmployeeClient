#include "uploadbase.h"

UpLoadBase::UpLoadBase(QWidget *parent) :
    QDialog(parent)
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
    setGeometry(0,0,180,500);
}

void UpLoadBase::exeVersion()
{
    progressDialogExe = new QProgressDialog(this);
    QString localFileName = "CE_SQLite.arh";
    fileHttpExe = new QFile(localFileName);
    if(!fileHttpExe->open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Attention!!!"),tr("Don't open file..."));
    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString name = settings.value("FtpForm/Address", "").toString();
    name += settings.value("FtpForm/Catalog", "").toString();
    name += "/Base/CE_SQLite.arh";

    url = name;
    url.setUserName(settings.value("FtpForm/Login", "").toString());
    url.setPassword(settings.value("FtpForm/Password", "").toString());
    url.setPort(21);

    replyExe = httpExe.get(QNetworkRequest(url));
    connect(replyExe,SIGNAL(finished()),this,SLOT(httpDoneExe()));
    connect(replyExe,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(updateDataReadProgressExe(qint64,qint64)));
    connect(replyExe,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDataReadProgressExe(qint64,qint64)));
    progressDialogExe->setLabelText(trUtf8("Загрузка базы %1 ...").arg(localFileName));
    progressDialogExe->setEnabled(true);
    progressDialogExe->setMinimum(0);
    progressDialogExe->setWindowTitle(trUtf8("Сотрудники предприятия"));
    connect(progressDialogExe,SIGNAL(canceled()),this,SLOT(closeConnection()));
    progressDialogExe->exec();
}

void UpLoadBase::httpDoneExe()
{
    if (fileHttpExe){
        fileHttpExe->write(replyExe->readAll());
    }
    progressDialogExe->hide();
    fileHttpExe->close();
    uncompressFile();
    closeConnection();
}

void UpLoadBase::closeConnection()
{
    httpExe.destroyed();
}

void UpLoadBase::updateDataReadProgressExe(qint64 readBytes, qint64 totalBytes)
{
    progressDialogExe->setMaximum(totalBytes);
    progressDialogExe->setValue(readBytes);
}

void UpLoadBase::uncompressFile()
{
    QFile file;
    file.setFileName("CE_SQLite.arh");
    file.open(QIODevice::ReadOnly);
    QByteArray byte = file.readAll();
    file.close();
    QByteArray unCompress = qUncompress(byte);
    QFile unCompressFile("CE_SQLite.dat");
    unCompressFile.open(QIODevice::WriteOnly);
    unCompressFile.write(unCompress);
    unCompressFile.close();
}
