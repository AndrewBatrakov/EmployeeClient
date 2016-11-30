#include "sqlform.h"

SQLForm::SQLForm(QWidget *parent) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    labelSQL = new QLabel(trUtf8("Запрос:"));
    editSQL = new QTextEdit;
//    QRegExp regExp("[A-Za-z ./\;:-0-9]{200}");
//    editSQL->setValidator(new QRegExpValidator(regExp,this));
    labelSQL->setBuddy(editSQL);

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Записать и Закрыть"));

    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(tr("Отмена"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelSQL,0,0);
    mainLayout->addWidget(editSQL,0,1);
    mainLayout->addWidget(buttonBox,1,1);

    setLayout(mainLayout);

    setWindowTitle(trUtf8("SQL Запрос"));
    readSettings();
}

void SQLForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    line += editSQL->toPlainText().toUtf8();
    line += "\r\n";
    stream<<line;
    emit accept();
}

void SQLForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void SQLForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("SQLForm").toByteArray());
}

void SQLForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("SQLForm", saveGeometry());
}
