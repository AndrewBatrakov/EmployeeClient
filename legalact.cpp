#include <QtGui>
#include <QtSql>

#include "legalact.h"
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"

LegalAct::LegalAct(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    //QRegExp regExp("[\\x0410-\\x044f . , 0-9]{100}");
    //editName->setValidator(new QRegExpValidator(regExp,this));
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
        query.prepare("SELECT * FROM legalacts WHERE legalactsid = ?;");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(1).toString());
        }
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Legalacts Update ERROR!"),query.lastError().text());
        }
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0,Qt::AlignRight);
    mainLayout->addWidget(editName,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,0,1,2);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Legal Act"));
    LegalAct::setTabOrder(editName,cancelPushButton);
}

void LegalAct::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE legalacts SET legalactsname = :name"
                      " WHERE legalactsid = :id;");
        query.bindValue(":name",editName->text().simplified());
        query.bindValue(":id",indexTemp);
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Legalacts Update ERROR!"),query.lastError().text());
        }
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM legalacts WHERE legalactsname = :name;");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("legalacts");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO legalacts (legalactsid, legalactsname"
                          ") VALUES (:id, :name);");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::tr("Legalacts INSERT ERROR!"),query.lastError().text());
            }
        }else{
            QString tempString = editName->text();
            tempString += QObject::tr(" is availble!");
            QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void LegalAct::deleteRecord()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "legalacts");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"legalacts",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM legalacts WHERE legalactsid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }

    settings.remove("Table");
    settings.remove("Index");
}

void LegalAct::done(int result)
{
    QDialog::done(result);
}
