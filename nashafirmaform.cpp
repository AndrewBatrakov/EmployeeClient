#include <QtWidgets>
#include <QtSql>

#include "nashafirmaform.h"
#include "lineedit.h"

NashaFirmaForm::NashaFirmaForm(QWidget *parent) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    labelName = new QLabel(trUtf8("Наименование:"));
    editName = new LineEdit;
    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelName->setBuddy(editName);

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Кнопка \"Записать и закрыть\""));

    cancelPushButton = new QPushButton(trUtf8("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Кнопка Отмены"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);


    QSqlQuery query;
    query.prepare("SELECT nashafirmaid FROM nashafirma");
    query.exec();
    while(query.next()){
        editName->setText(query.value(0).toString());
    }
    if(editName->text() != ""){
        newRecord = false;
    }else{
        newRecord = true;
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addWidget(buttonBox,1,1);

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Наша фирма"));
    readSettings();
}

void NashaFirmaForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(!newRecord){
        QSqlQuery query;
        query.prepare("UPDATE nashafirma SET nashafirmaid = :name");
        query.bindValue(":name",editName->text());
        query.exec();
        line += "UPDATE nashafirma SET nashafirmaid = '";
        line += editName->text().simplified().toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("INSERT INTO nashafirma (nashafirmaid) VALUES(:name)");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        line += "INSERT INTO nashafirma (nashafirmaid) VALUES('";
        line += editName->text().simplified().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
    }
    emit accept();
    close();
}

void NashaFirmaForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }


    QSqlQuery query;
    query.prepare("DELETE FROM nashafirma");
    query.exec();
    query.next();

    line += "DELETE FROM nashafirma";
    line += "\r\n";
    stream<<line;
}

void NashaFirmaForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void NashaFirmaForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("NashaFirmaForm").toByteArray());
}

void NashaFirmaForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("NashaFirmaForm", saveGeometry());
}
