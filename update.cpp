#include "update.h"

Update::Update(QWidget *parent) :
    QDialog(parent)
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
}

void Update::iniVersion()
{
    QString localFileName = "EmployeeClientFromSite.ini";
    fileHttpIni = new QFile;
    fileHttpIni->setFileName(localFileName);
    if(!fileHttpIni->open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Не могу открыть файл %1").arg(localFileName));
    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString name = settings.value("FtpForm/Address", "").toString();
    name += settings.value("FtpForm/Catalog", "").toString();
    name += "/EmployeeClient.ini";

    url = name;
    url.setUserName(settings.value("FtpForm/Login", "").toString());
    url.setPassword(settings.value("FtpForm/Password", "").toString());
    url.setPort(21);

    replyIni = httpIni.get(QNetworkRequest(url));
    connect(replyIni,SIGNAL(finished()),this,SLOT(httpDoneIni()));
    connect(replyIni,SIGNAL(readyRead()),this,SLOT(httpReadyReadIni()));
}

bool Update::newVersion()
{
    bool resultUpdates = false;
    QSettings settings("EmployeeClientFromSite.ini",QSettings::IniFormat);
    QString iniVersion = settings.value("Version").toString();

    QSettings iniSettings("EmployeeClient.ini",QSettings::IniFormat);
    nowVersion = iniSettings.value("Version").toString();

    if(iniVersion.toFloat() > nowVersion.toFloat()){
        int k = QMessageBox::question(this,trUtf8("Внимание!!!"),tr("Есть обновления! Загрузить?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if(k == QMessageBox::Yes){
            QSettings iniSettings("EmployeeClient.ini",QSettings::IniFormat);
            iniSettings.setValue("Version",iniVersion);
            iniSettings.sync();

            resultUpdates = true;
            exeVersion();
        }
    }
    QFile fileRe;
    fileRe.setFileName("EmployeeClientFromSite.ini");
    if(fileRe.exists()){
        fileRe.remove();
    }
    fileRe.setFileName("EmployeeClient.exe.bak");
    if(fileRe.exists()){
        fileRe.remove();
    }
    return resultUpdates;
}

void Update::exeVersion()
{
    progressDialogExe = new QProgressDialog(this);
    QString localFileName1 = "./EmployeeClient.tmp";
    if(localFileName1.isEmpty())
        localFileName1 = "null.out";
    fileHttpExe = new QFile(localFileName1);
    if(!fileHttpExe->open(QIODevice::ReadWrite)){

    }

    QFile renFile;
    renFile.setFileName("./EmployeeClient.exe.bak");
    if(renFile.exists()){
        renFile.remove();
    }
    QFile fileR;
    fileR.setFileName("./EmployeeClient.exe");
    fileR.rename("./EmployeeClient.exe.bak");
    fileR.close();

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString name = settings.value("FtpForm/Address", "").toString();
    name += settings.value("FtpForm/Catalog", "").toString();
    name += "/EmployeeClient";

    url = name;
    url.setUserName(settings.value("FtpForm/Login", "").toString());
    url.setPassword(settings.value("FtpForm/Password", "").toString());
    url.setPort(21);

    replyExe = httpExe.get(QNetworkRequest(url));
    connect(replyExe,SIGNAL(finished()),this,SLOT(httpDoneExe()));
    connect(replyExe,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(updateDataReadProgressExe(qint64,qint64)));
    connect(replyExe,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDataReadProgressExe(qint64,qint64)));

    progressDialogExe->setLabelText(trUtf8("Загрузка %1 ...").arg(localFileName1));
    progressDialogExe->setEnabled(true);
    progressDialogExe->exec();
}

void Update::tranceVersion()
{
    progressDialogTrance = new QProgressDialog(this);
    QString localFileName1 = "./EmployeeClient_ru.qm";
    if(localFileName1.isEmpty())
        localFileName1 = "null.out";
    fileHttpTrance = new QFile(localFileName1);
    if(!fileHttpTrance->open(QIODevice::WriteOnly)){

    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString name = settings.value("FtpForm/Address", "").toString();
    name += settings.value("FtpForm/Catalog", "").toString();
    name += "/EmployeeClient_ru.qm";

    url = name;
    url.setUserName(settings.value("FtpForm/Login", "").toString());
    url.setPassword(settings.value("FtpForm/Password", "").toString());
    url.setPort(21);

    
    replyTrance = httpTrance.get(QNetworkRequest(url));
    connect(replyTrance,SIGNAL(finished()),this,SLOT(httpDoneTrance()));
    connect(replyTrance,SIGNAL(readyRead()),this,SLOT(httpReadyReadTrance()));
    connect(replyTrance,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(updateDataReadProgressTrance(qint64,qint64)));
    connect(replyTrance,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDataReadProgressTrance(qint64,qint64)));

    progressDialogTrance->setLabelText(trUtf8("Загрузка %1 ...").arg(localFileName1));
    progressDialogTrance->setEnabled(false);
    progressDialogTrance->exec();
}

void Update::httpDoneIni()
{
    fileHttpIni->close();
    newVersion();
}

void Update::httpDoneExe()
{
    if (fileHttpExe){
        fileHttpExe->write(replyExe->readAll());

    }
    progressDialogExe->hide();

    fileHttpExe->rename("./EmployeeClient.exe");
    fileHttpExe->close();
    tranceVersion();
}

void Update::httpDoneTrance()
{
    fileHttpTrance->close();
    closeConnection();
    restartProgramm();
}

void Update::closeConnection()
{
    httpIni.destroyed();
    httpExe.destroyed();
    httpTrance.destroyed();
}

void Update::restartProgramm()
{
    QProcess::startDetached("./EmployeeClient.exe");
    qApp->quit();
}


void Update::updateDataReadProgressExe(qint64 readBytes, qint64 totalBytes)
{
    progressDialogExe->setMaximum(totalBytes);
    progressDialogExe->setValue(readBytes);
}

void Update::updateDataReadProgressTrance(qint64 readBytes, qint64 totalBytes)
{
    progressDialogTrance->setMaximum(totalBytes);
    progressDialogTrance->setValue(readBytes);
}

void Update::httpReadyReadIni()
{
    if (fileHttpIni)
        fileHttpIni->write(replyIni->readAll());
}

void Update::httpReadyReadExe()
{
    if (fileHttpExe){
        fileHttpExe->write(replyExe->readAll());

    }
    progressDialogExe->hide();
}

void Update::httpReadyReadTrance()
{
    if (fileHttpTrance)
        fileHttpTrance->write(replyTrance->readAll());
    progressDialogTrance->hide();
}