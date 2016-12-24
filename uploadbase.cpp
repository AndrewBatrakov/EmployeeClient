#include "uploadbase.h"

UpLoadBase::UpLoadBase(QWidget *parent) :
    QDialog(parent)
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
    //setGeometry(0,0,180,500);
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString name = "http://";
    name += settings.value("FtpForm/Address", "").toString();
    name += settings.value("FtpForm/Catalog", "").toString();
    name += "/Base/CE_SQLite.arh";

    url = name;
}

void UpLoadBase::exeVersion()
{
    progressDialogExe = new QProgressDialog(this);
    QString localFileName = "CE_SQLite.arh";
    fileHttpExe = new QFile(localFileName);
    if(!fileHttpExe->open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,trUtf8("Attention!!!"),trUtf8("Don't open file..."));
    }

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
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");

    if(replyExe->isOpen()){
        if (fileHttpExe){
            fileHttpExe->write(replyExe->readAll());
            fileHttpExe->close();
            uncompressFile();
            settings.setValue("OUT","false");
        }
    }else{
        QMessageBox::warning(this,trUtf8("Внимание!"),trUtf8("Проверьте соединение с интернетом!"));
        closeConnection();
        settings.setValue("OUT","true");
    }
    closeConnection();
}

void UpLoadBase::closeConnection()
{
    progressDialogExe->hide();
    fileHttpExe->close();
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

