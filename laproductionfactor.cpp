#include "laproductionfactor.h"
#include "numprefix.h"
#include <QtSql>

LaProductionFactor::LaProductionFactor(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;

    labelSanatoryNorms = new QLabel(tr("Sanatory Norms:"));
    editSanatoryNorms = new LineEdit;
    labelApproved = new QLabel(tr("Approved:"));
    editApproved = new LineEdit;
    labelApprovedDate = new QLabel(tr("Approved Date:"));
    editApprovedDate = new LineEdit;

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
        query.prepare("SELECT * FROM laproductionfactor WHERE laproductionfactorid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editName->setText(query.value(1).toString());
        editSanatoryNorms->setText(query.value(2).toString());
        editApproved->setText(query.value(3).toString());
        editApprovedDate->setText(query.value(4).toString());
    }else{
        editName->clear();
        editSanatoryNorms->clear();
        editApproved->clear();
        editApprovedDate->clear();
        editName->setText(QObject::tr("Name of Production Factor"));
        editName->selectAll();
        editSanatoryNorms->setText(QObject::tr("Sanatory Norms of Production Factor"));
        editSanatoryNorms->selectAll();
        editApproved->setText(QObject::tr("Approved of Production Factor"));
        editApproved->selectAll();
        editApprovedDate->setText(QObject::tr("Approved Date of Production Factor"));
        editApprovedDate->selectAll();
    }

    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addWidget(labelSanatoryNorms,1,0);
    mainLayout->addWidget(editSanatoryNorms,1,1);
    mainLayout->addWidget(labelApproved,2,0);
    mainLayout->addWidget(editApproved,2,1);
    mainLayout->addWidget(labelApprovedDate,3,0);
    mainLayout->addWidget(editApprovedDate,3,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,4,1);
        editName->selectAll();
    }

    setLayout(mainLayout);
    setWindowTitle(tr("Production Factor"));
}

void LaProductionFactor::deleteRecord()
{

}

void LaProductionFactor::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE laproductionfactor SET laproductionfactorname = :name, "
                      "sanatorynorms = :sanatorynorms, "
                      "approved = :approved, "
                      "approveddate = :approveddate "
                      "WHERE  laproductionfactorid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":approved",editApproved->text());
        query.bindValue(":sanatorynorms",editSanatoryNorms->text());
        query.bindValue(":approveddate",editApprovedDate->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        //indexTemp = editName->text();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM laproductionfactor WHERE laproductionfactorname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("laproductionfactor");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO laproductionfactor (laproductionfactorid, "
                          "sanatorynorms, "
                          "approved, "
                          "approveddate, "
                          "laproductionfactorname) VALUES(:id, :name, "
                          ":sanatorynorms, "
                          ":approved, "
                          ":approveddate"
                          ")");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.bindValue(":approved",editApproved->text().simplified());
            query.bindValue(":sanatorynorms",editSanatoryNorms->text().simplified());
            query.bindValue(":approveddate",editApprovedDate->text().simplified());
            query.exec();
            //ind = editName->text();
        }else{
            QString tempString = editName->text();
            tempString += QObject::tr(" is availble!");
            QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void LaProductionFactor::done(int result)
{
    QDialog::done(result);
}
