#include "indsecprogform.h"
#include <QtWidgets>
#include <QtSql>

#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"

IndSecProgForm::IndSecProgForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    indexTemp = id;

    labelName = new QLabel(trUtf8("Наименование:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
//    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 \" -]{150}");
//    editName->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelName->setBuddy(editName);

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Кнопка \"Записать и закрыть\""));

    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Кнопка Отмена"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT pbprogrammaname FROM pbprogramma WHERE pbprogrammaid = ?");
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

    setWindowTitle(trUtf8("Программа обучения Промбезопасности"));
    readSettings();
}

void IndSecProgForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE pbprogramma SET pbprogrammaname = :name WHERE  pbprogrammaid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        line += "UPDATE pbprogramma SET pbprogrammaname = '";
        line += editName->text().toUtf8();
        line += "' WHERE  otprogrammaid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM pbprogramma WHERE pbprogrammaname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("pbprogramma");
            QSqlQuery query;
            query.prepare("INSERT INTO pbprogramma (pbprogrammaid, pbprogrammaname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            line += "INSERT INTO pbprogramma (pbprogrammaid, pbprogrammaname) VALUES('";
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

void IndSecProgForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    //QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    //if(settings.value("CurrentUser") == "root" || settings.value("CurrentUser") == "Nataly"){
    ForDelete forDelete(indexTemp,"pbprogramma",this);

    forDelete.exec();
    forDelete.deleteOnDefault();

    QSqlQuery query;
    query.prepare("DELETE FROM pbprogramma WHERE pbprogrammaid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();

    line += "DELETE FROM pbprogramma WHERE pbprogrammaid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void IndSecProgForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void IndSecProgForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("IndSecProgForm").toByteArray());
}

void IndSecProgForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("IndSecProgForm", saveGeometry());
}
