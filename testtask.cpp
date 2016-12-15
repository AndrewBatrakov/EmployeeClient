#include <QtGui>
#include <QtSql>

#include "testtask.h"
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"

TestTask::TestTask(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;

    labelName = new QLabel(trUtf8("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    //QRegExp regExp("[\\x0410-\\x044f . , 0-9]{100}");
    //editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    labelCipher = new QLabel(trUtf8("Cipher:"));
    editCipher = new LineEdit;
    editCipher->setReadOnly(onlyForRead);
    //editCipher->setValidator(new QRegExpValidator(regExp,this));
    labelCipher->setBuddy(editCipher);

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
        query.prepare("SELECT * FROM testtask WHERE testtaskid = ?;");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(2).toString());
            editCipher->setText(query.value(1).toString());
        }
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Testtask Update ERROR!"),query.lastError().text());
        }
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,1,0,Qt::AlignRight);
    mainLayout->addWidget(editName,1,1);
    mainLayout->addWidget(labelCipher,0,0);
    mainLayout->addWidget(editCipher,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,2,0,1,2);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Test task"));
    TestTask::setTabOrder(editName,cancelPushButton);
}

void TestTask::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE testtask SET testtaskname = :name, cipher = :cipher"
                      " WHERE testtaskid = :id;");
        query.bindValue(":name",editName->text().simplified());
        query.bindValue(":id",indexTemp);
        query.bindValue(":cipher",editCipher->text());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Test task Update ERROR!"),query.lastError().text());
        }
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM testtask WHERE testtaskname = :name AND cipher = :cipher;");
        query.bindValue(":name",editName->text().simplified());
        query.bindValue(":cipher",editCipher->text());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("testtask");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO testtask (testtaskid, testtaskname, cipher"
                          ") VALUES (:id, :name, :cipher);");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.bindValue(":cipher",editCipher->text().simplified());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("Testtask INSERT ERROR!"),query.lastError().text());
            }
        }else{
            QString tempString = editName->text();
            tempString += QObject::trUtf8(" is availble!");
            QMessageBox::warning(this,QObject::trUtf8("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void TestTask::deleteRecord()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "testtask");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"testtask",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,trUtf8("Attention!!!"),trUtf8("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM testtask WHERE testtaskid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }

    settings.remove("Table");
    settings.remove("Index");
}

void TestTask::done(int result)
{
    QDialog::done(result);
}
