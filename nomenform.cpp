#include <QtWidgets>
#include <QtSql>

#include "nomenform.h"
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"

NomenForm::NomenForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelName->setBuddy(editName);

    savePushButton = new QPushButton(tr("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(tr("Save And Close Button"));

    cancelPushButton = new QPushButton(tr("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT nomenname FROM nomen WHERE nomenid = ?");
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

    setWindowTitle(trUtf8("Номенклатура СИЗ"));
    readSettings();
}

void NomenForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE nomen SET nomenname = :name WHERE  nomenid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        line += "UPDATE nomen SET nomenname = '";
        line += editName->text().toUtf8();
        line += "' WHERE  nomenid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM nomen WHERE nomenname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("nomen");

            QSqlQuery query;
            query.prepare("INSERT INTO nomen (nomenid, nomenname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            line += "INSERT INTO nomen (nomenid, nomenname) VALUES('";
            line += indexTemp;
            line += "', '";
            line += editName->text().simplified().toUtf8();
            line += "')";
            line += "\r\n";
            stream<<line;
        }else{
            QString tempString = editName->text();
            tempString += QObject::trUtf8(" существует!");
            QMessageBox::warning(this,QObject::trUtf8("Внимание!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void NomenForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    //QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    //if(settings.value("CurrentUser") == "root" || settings.value("CurrentUser") == "Nataly"){
    ForDelete forDelete(indexTemp,"nomen",this);

    forDelete.exec();
    forDelete.deleteOnDefault();

    QSqlQuery query;
    query.prepare("DELETE FROM nomen WHERE nomenid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();

    line += "DELETE FROM nomen WHERE nomenid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void NomenForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void NomenForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "NomenForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void NomenForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "NomenForm");
    settings.setValue("Geometry", saveGeometry());
}
