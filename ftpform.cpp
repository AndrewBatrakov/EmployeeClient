#include <QtWidgets>

#include "ftpform.h"
#include "lineedit.h"

FtpForm::FtpForm(QWidget *parent) :
    QDialog(parent)
{
    labelAddress = new QLabel(trUtf8("Адрес:"));
    editAddress = new LineEdit;
    QRegExp regExp("[A-Za-z:/0-9-_.]{150}");
    editAddress->setValidator(new QRegExpValidator(regExp,this));
    labelAddress->setBuddy(editAddress);

    labelLogin = new QLabel(trUtf8("Логин:"));
    editLogin = new LineEdit;
    editLogin->setValidator(new QRegExpValidator(regExp,this));
    labelLogin->setBuddy(editLogin);

    labelPassword = new QLabel(trUtf8("Пароль:"));
    editPassword = new LineEdit;
    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setValidator(new QRegExpValidator(regExp,this));
    labelPassword->setBuddy(editPassword);

    labelCatalog = new QLabel(trUtf8("Каталог:"));
    editCatalog = new LineEdit;
    editCatalog->setValidator(new QRegExpValidator(regExp,this));
    labelCatalog->setBuddy(editCatalog);

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Записать и Закрыть"));

    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Отмена"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    editAddress->setText(settings.value("FtpForm/Address", "").toString());
    if(editAddress->text() == ""){
        editAddress->setText("ftp://90.150.87.95");
    }
    editLogin->setText(settings.value("FtpForm/Login", "").toString());
    if(editLogin->text() == ""){
        editLogin->setText("ftpsites");
    }
    editPassword->setText(settings.value("FtpForm/Password", "").toString());
    if(editPassword->text() == ""){
        editPassword->setText("Andrew123");
    }
    editCatalog->setText(settings.value("FtpForm/Catalog", "").toString());
    if(editCatalog->text() == ""){
        editCatalog->setText("/EmployeeClient");
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelAddress,0,0);
    mainLayout->addWidget(editAddress,0,1);
    mainLayout->addWidget(labelLogin,1,0);
    mainLayout->addWidget(editLogin,1,1);
    mainLayout->addWidget(labelPassword,2,0);
    mainLayout->addWidget(editPassword,2,1);
    mainLayout->addWidget(labelCatalog,3,0);
    mainLayout->addWidget(editCatalog,3,1);
    mainLayout->addWidget(buttonBox,4,1);

    setLayout(mainLayout);

    setWindowTitle(trUtf8("FTP"));
    readSettings();
}

void FtpForm::editRecord()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("FtpForm/Address", editAddress->text().simplified());
    settings.setValue("FtpForm/Login", editLogin->text().simplified());
    settings.setValue("FtpForm/Password", editPassword->text().simplified());
    settings.setValue("FtpForm/Catalog", editCatalog->text().simplified());
    emit accept();
    close();
}

void FtpForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void FtpForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("FtpForm").toByteArray());
}

void FtpForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("FtpForm", saveGeometry());
}
