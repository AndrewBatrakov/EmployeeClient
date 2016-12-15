#include <QtWidgets>
#include <QtSql>

#include "subdivision.h"
#include "organization.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"

SubDivisionForm::SubDivisionForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;

    labelSubdivision = new QLabel(trUtf8("Наименование:"));
    editSubdivision = new LineEdit;
    editSubdivision->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x4010-\\x044f 0-9 \\-  ]{150}");
    editSubdivision->setValidator(new QRegExpValidator(regExp,this));
    labelSubdivision->setBuddy(editSubdivision);

    labelOrganization = new QLabel(trUtf8("Организация:"));
    editOrganization = new LineEdit;
    editOrganization->setReadOnly(onlyForRead);
    labelOrganization->setBuddy(editOrganization);

    QSqlQueryModel *orgModel = new QSqlQueryModel;
    orgModel->setQuery("SELECT organizationname FROM organization");
    QCompleter *orgCompleter = new QCompleter(orgModel);
    orgCompleter->setCompletionMode(QCompleter::PopupCompletion);
    orgCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editOrganization->setCompleter(orgCompleter);

    QHBoxLayout *editSubdivisionLayout = new QHBoxLayout;
    editSubdivisionLayout->addWidget(labelSubdivision);
    editSubdivisionLayout->addWidget(editSubdivision);

    QToolButton *addButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addButton->setIcon(addPix);
    addButton->setToolTip(trUtf8("Добавить новую запись"));
    connect(addButton,SIGNAL(clicked()),this,SLOT(addOrgRecord()));

    QToolButton *seeButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeButton->setIcon(seePix);
    seeButton->setToolTip(trUtf8("Просмотреть выбранную запись"));
    connect(seeButton,SIGNAL(clicked()),this,SLOT(seeOrgRecord()));

    QToolButton *listButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listButton->setIcon(listPix);
    listButton->setToolTip(trUtf8("Просмотр списка записей"));
    connect(listButton,SIGNAL(clicked()),this,SLOT(listOrgRecord()));

    QHBoxLayout *editOrganizationLayout = new QHBoxLayout;
    editOrganizationLayout->addWidget(labelOrganization);
    editOrganizationLayout->addWidget(editOrganization);
    if(!onlyForRead){
        editOrganizationLayout->addWidget(addButton);
        editOrganizationLayout->addWidget(seeButton);
        editOrganizationLayout->addWidget(listButton);
    }

    QGroupBox *orgGroupBox = new QGroupBox;
    orgGroupBox->setLayout(editOrganizationLayout);

    saveButton = new QPushButton(trUtf8("Записать"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(trUtf8("Отмена"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    newRecord = false;
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT sub.subdivisionname, (SELECT org.organizationname "
                      "FROM organization AS org WHERE org.organizationid = sub.organizationid) "
                      "FROM subdivision AS sub WHERE sub.subdivisionid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editSubdivision->setText(query.value(0).toString());
        editOrganization->setText(query.value(1).toString());
    }else{
        editSubdivision->clear();
        editSubdivision->setText(QObject::trUtf8("Наименование подразделения"));
        editSubdivision->selectAll();
        editOrganization->clear();
        editOrganization->setText(QObject::trUtf8("Наименование организации"));
        editOrganization->selectAll();
        newRecord = true;
    }
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(editSubdivisionLayout);
    mainLayout->addWidget(orgGroupBox);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox);
        editSubdivision->selectAll();
        editOrganization->selectAll();
    }
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Подразделение"));
    readSettings();
}

void SubDivisionForm::editRecord()
{
    if(checkFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        if(newRecord){
            NumPrefix numPref;
            indexTemp = numPref.getPrefix("subdivision");
            QSqlQuery query;
            query.prepare("INSERT INTO subdivision (subdivisionid, subdivisionname, "
                          "organizationid) VALUES(:subid, :subname, (SELECT organizationid "
                          "FROM organization WHERE organizationname = :orgname))");
            query.bindValue(":subid",indexTemp);
            query.bindValue(":subname",editSubdivision->text());
            query.bindValue(":orgname",editOrganization->text());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR!"),query.lastError().text());
                return;
            }

            line += "INSERT INTO subdivision (subdivisionid, subdivisionname, "
                    "organizationid) VALUES('";
            line += indexTemp.toUtf8();
            line += "', '";
            line += editSubdivision->text().toUtf8();
            line += "', '";
            QSqlQuery queryTemp;
            queryTemp.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
            queryTemp.bindValue(":name",editOrganization->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "')";
            line += "\r\n";
            stream<<line;
        }else{
            QSqlQuery query;
            query.prepare("UPDATE subdivision SET subdivisionname = :subname, "
                          "organizationid = (SELECT organizationid FROM organization "
                          "WHERE organizationname = :orgname) WHERE subdivisionid = :subid");
            query.bindValue(":subid",indexTemp);
            query.bindValue(":subname",editSubdivision->text());
            query.bindValue(":orgname",editOrganization->text());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("Ошибка базы данных!"),query.lastError().text());
                return;
            }
            line += "UPDATE subdivision SET subdivisionname = '";
            line += editSubdivision->text().toUtf8();
            line += "', organizationid ='";
            QSqlQuery queryTemp;
            queryTemp.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
            queryTemp.bindValue(":name",editOrganization->text());
            queryTemp.exec();
            queryTemp.next();
            line += query.value(0).toString().toUtf8();
            line += "' WHERE subdivisionid = '";
            line += indexTemp;
            line += "'";
            line += "\r\n";
            stream<<line;
        }
        emit accept();
        close();
    }
}

void SubDivisionForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"subdivision",this);
    //if(forDelete.result() == QDialog::Accepted){
    forDelete.exec();
    //int k = QMessageBox::warning(this,trUtf8("Attention!!!"),trUtf8("Delete item with the replacement for default value?"),
    //                     QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    //if(k == QMessageBox::Yes){
    forDelete.deleteOnDefault();
    if(indexTemp != "OWN000000001"){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        QSqlQuery query;
        query.prepare("DELETE FROM subdivision WHERE subdivisionid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        line += "DELETE FROM subdivision WHERE subdivisionid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QMessageBox::warning(this,QObject::trUtf8("Attention"),QObject::trUtf8("You dont may delete default value!"));
    }
}

void SubDivisionForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void SubDivisionForm::addOrgRecord()
{
    OrganizationForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT organizationname From organization WHERE organizationid = :orgid");
        query.bindValue(":orgid",orgForm.returnValue());
        query.exec();
        query.next();
        editOrganization->setText(query.value(0).toString());
    }
}

void SubDivisionForm::seeOrgRecord()
{
    QSqlQuery query;
    query.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
    query.bindValue(":name",editOrganization->text());
    query.exec();
    while(query.next()){
        OrganizationForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void SubDivisionForm::listOrgRecord()
{
    QSqlQuery query;
    query.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
    query.bindValue(":name",editOrganization->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"organization",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT organizationname FROM organization WHERE organizationid = :orgid");
        query.bindValue(":orgid",listTemp.returnValue());
        query.exec();
        query.next();
        editOrganization->setText(query.value(0).toString());
    }
}

void SubDivisionForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("SubDivisionForm").toByteArray());
}

void SubDivisionForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("SubDivisionForm", saveGeometry());
}

bool SubDivisionForm::checkFill()
{
    bool fill = true;
    if(editSubdivision->text() == ""){
        editSubdivision->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editSubdivision->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editOrganization->text() == ""){
        editOrganization->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editOrganization->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}
