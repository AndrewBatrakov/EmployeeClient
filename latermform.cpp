#include <QtGui>
#include <QtSql>

#include "latermform.h"
#include "numprefix.h"
#include "fordelete.h"

LaTermForm::LaTermForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 \" - , .]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    labelDescription = new QLabel(tr("Description:"));
    editDescription = new LineEdit;
    editDescription->setReadOnly(onlyForRead);
    editDescription->setValidator(new QRegExpValidator(regExp,this));
    labelDescription->setBuddy(editDescription);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM laterm WHERE latermid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editName->setText(query.value(1).toString());
        editDescription->setText(query.value(2).toString());
    }else{
        editName->clear();
//        editName->setText(QObject::tr("Name of Term"));
//        editName->selectAll();
//        editDescription->setText(QObject::tr("Description of Term"));
//        editDescription->selectAll();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addWidget(labelDescription,1,0);
    mainLayout->addWidget(editDescription,1,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,2,1);
        editName->selectAll();
    }
    setLayout(mainLayout);

    setWindowTitle(tr("Term"));
}

void LaTermForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE laterm SET latermname = :name, latermdescription = :des WHERE latermid = :id;");
        query.bindValue(":name",editName->text());
        query.bindValue(":des",editDescription->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("UPDATE laterm, DataBase ERROR!"),query.lastError().text());
            return;
        }
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM laterm WHERE latermname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("laterm");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO laterm (latermid, latermname, latermdescription) VALUES(:id, :name, :des)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.bindValue(":des",editDescription->text().simplified());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::tr("INSERT laterm, DataBase ERROR!"),query.lastError().text());
                return;
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

void LaTermForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"laterm",this);
    forDelete.exec();
    forDelete.deleteOnDefault();
    if(indexTemp != "OWN000000001"){
        QSqlQuery query;
        query.prepare("DELETE FROM laterm WHERE latermid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }else{
        QMessageBox::warning(this,QObject::tr("Attention"),QObject::tr("You dont may delete default value!"));
    }
}

void LaTermForm::done(int result)
{
    QDialog::done(result);
}
