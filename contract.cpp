#include <QtGui>
#include <QtSql>

#include "contract.h"
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"
#include "employee.h"
#include "viewlisttable.h"

ContractForm::ContractForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;
    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());

    labelNumber = new QLabel(tr("Number:"));
    editNumber = new LineEdit;
    editNumber->setReadOnly(onlyForRead);
    labelNumber->setBuddy(editNumber);

    labelDate = new QLabel(tr("Date:"));
    editDate = new QDateEdit;
    editDate->setCalendarPopup(true);
    editDate->setReadOnly(onlyForRead);
    editDate->setDate(QDate::currentDate());

    labelEmployee = new QLabel(tr("FIO:"));
    editEmployee = new LineEdit;
    editEmployee->setReadOnly(onlyForRead);
    labelEmployee->setBuddy(editEmployee);

    QSqlQueryModel *employeeModel = new QSqlQueryModel;
    employeeModel->setQuery("SELECT employeename FROM employee");
    QCompleter *employeeCompleter = new QCompleter(employeeModel);
    employeeCompleter->setCompletionMode(QCompleter::PopupCompletion);
    employeeCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editEmployee->setCompleter(employeeCompleter);

    QToolButton *addButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addButton->setIcon(addPix);
    addButton->setToolTip(tr("Add new record"));
    connect(addButton,SIGNAL(clicked()),this,SLOT(addRecord()));
    addButton->setStyleSheet(styleSheetString);

    QToolButton *seeButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeButton->setIcon(seePix);
    seeButton->setToolTip(tr("See select item"));
    connect(seeButton,SIGNAL(clicked()),this,SLOT(seeRecord()));
    seeButton->setStyleSheet(styleSheetString);

    QToolButton *listButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listButton->setIcon(listPix);
    listButton->setToolTip(tr("See list of item"));
    connect(listButton,SIGNAL(clicked()),this,SLOT(listRecord()));
    listButton->setStyleSheet(styleSheetString);

    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->addWidget(labelEmployee);
    editLayout->addWidget(editEmployee);
    if(!onlyForRead){
        editLayout->addWidget(addButton);
        editLayout->addWidget(seeButton);
        editLayout->addWidget(listButton);
    }

    savePushButton = new QPushButton(tr("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(tr("Save And Close Button"));

    cancelPushButton = new QPushButton(tr("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(tr("Cancel Button"));

    printPushButton = new QPushButton(tr("Print"));
    //connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    printPushButton->setToolTip(tr("Print Contract Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(printPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT organizationname FROM organization WHERE organizationid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            //editOrganization->setText(query.value(0).toString());
        }
    }else{
        //editOrganization->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelNumber,0,0);
    mainLayout->addWidget(editNumber,0,1);
    mainLayout->addWidget(labelDate,0,2);
    mainLayout->addWidget(editDate,0,3);
    mainLayout->addLayout(editLayout,1,0,1,3);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,2,3);
        //editOrganization->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Organization"));
}

void ContractForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE organization SET organizationname = :name WHERE  organizationid = :id");
        //query.bindValue(":name",editOrganization->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        //nameTemp = editOrganization->text();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM organization WHERE organizationname = :orgname");
        //query.bindValue(":orgname",editOrganization->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("organization");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO organization (organizationid, organizationname) VALUES(:idorg, :nameorg)");
            query.bindValue(":idorg",indexTemp);
            //query.bindValue(":nameorg",editOrganization->text().simplified());
            query.exec();
            //nameTemp = editOrganization->text();
        }else{
            //QString tempString = editOrganization->text();
            //tempString += QObject::tr(" is availble!");
            //QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void ContractForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"organization",this);

    //if(forDelete.result() == QDialog::Accepted){
    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        if(indexTemp != "OWN000000001"){
            QSqlQuery query;
            query.prepare("DELETE FROM organization WHERE organizationid = :id");
            query.bindValue(":id",indexTemp);
            query.exec();
            query.next();
        }else{
            QMessageBox::warning(this,QObject::tr("Attention"),QObject::tr("You dont may delete default value!"));
        }
    }
}

void ContractForm::addRecord()
{
    EmployeeForm form("",this,false);
    form.exec();
    if(form.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename FROM employee WHERE employeeid = :id");
        query.bindValue(":id",form.returnValue());
        query.exec();
        query.next();
        editEmployee->setText(query.value(0).toString());
    }
}

void ContractForm::seeRecord()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmployee->text());
    query.exec();
    while(query.next()){
        EmployeeForm form(query.value(0).toString(),this,true);
        form.exec();
    }
}

void ContractForm::listRecord()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmployee->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"employee",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename FROM employee WHERE employeeid = :id");
        query.bindValue(":id",listTemp.returnValue());
        query.exec();
        query.next();
        editEmployee->setText(query.value(0).toString());
    }
}
void ContractForm::done(int result)
{
    QDialog::done(result);
}
