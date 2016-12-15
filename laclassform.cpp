#include <QtGui>
#include <QtSql>

#include "laclassform.h"
#include "numprefix.h"
#include "fordelete.h"

LaClassForm::LaClassForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;

    labelName = new QLabel(trUtf8("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x4010-\\x044f 0-9 \\-  ]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    saveButton = new QPushButton(trUtf8("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(trUtf8("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT laclassname FROM laclass WHERE laclassid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editName->setText(query.value(0).toString());
    }else{
        editName->clear();
        editName->setText(QObject::trUtf8("Name of Class"));
        editName->selectAll();
    }
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
        editName->selectAll();
    }
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Class"));
}

void LaClassForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE laclass SET laclassname = :name WHERE  laclassid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        //indexTemp = editName->text();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM laclass WHERE laclassname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("laclass");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO laclass (laclassid, laclassname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            //ind = editName->text();
        }else{
            QString tempString = editName->text();
            tempString += QObject::trUtf8(" is availble!");
            QMessageBox::warning(this,QObject::trUtf8("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void LaClassForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"class",this);
    forDelete.exec();
    forDelete.deleteOnDefault();
    if(indexTemp != "OWN000000001"){
        QSqlQuery query;
        query.prepare("DELETE FROM laclass WHERE laclassid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }else{
        QMessageBox::warning(this,QObject::trUtf8("Attention"),QObject::trUtf8("You dont may delete default value!"));
    }
}

void LaClassForm::done(int result)
{
    QDialog::done(result);
}
