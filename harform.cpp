#include <QtSql>
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"
#include "harform.h"

HarForm::HarForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    indexTemp = id;

    labelName = new QLabel(trUtf8("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelName->setBuddy(editName);

    savePushButton = new QPushButton(trUtf8("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Save And Close Button"));

    cancelPushButton = new QPushButton(trUtf8("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT harname FROM har WHERE harid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());
        }
    }else{
        editName->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Harakteristika"));
    readSettings();
}

void HarForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE har SET harname = :name WHERE  harid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        line += "UPDATE har SET harname = '";
        line += editName->text().toUtf8();
        line += "' WHERE harid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM har WHERE harname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("har");

            QSqlQuery query;
            query.prepare("INSERT INTO har (harid, harname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            line += "INSERT INTO har (harid, harname) VALUES('";
            line += indexTemp.toUtf8();
            line += "', '";
            line += editName->text().simplified().toUtf8();
            line += "')";
            line += "\r\n";
            stream<<line;
        }else{
            QString tempString = editName->text();
            tempString += QObject::trUtf8(" is availble!");
            QMessageBox::warning(this,QObject::trUtf8("Atention!!!"),tempString);
            return;
        }
    }
    emit accept();
    close();
}

void HarForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    //QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    //if(settings.value("CurrentUser") == "root" || settings.value("CurrentUser") == "Nataly"){
    ForDelete forDelete(indexTemp,"har",this);

    forDelete.exec();
    forDelete.deleteOnDefault();

    QSqlQuery query;
    query.prepare("DELETE FROM har WHERE harid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM har WHERE harid = '";
    line += indexTemp.toUtf8();
    line += "'";
    line += "\r\n";
    stream<<line;
}

void HarForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void HarForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "HarForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void HarForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "HarForm");
    settings.setValue("Geometry", saveGeometry());
}
