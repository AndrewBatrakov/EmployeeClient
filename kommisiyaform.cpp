#include <QtWidgets>
#include <QtSql>
#include "kommisiyaform.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"

KommisiyaForm::KommisiyaForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;

    labelEmp1 = new QLabel(trUtf8("Председатель:"));
    editEmp1 = new LineEdit;
    editEmp1->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x4010-\\x044f 0-9 \\-  ]{150}");
    editEmp1->setValidator(new QRegExpValidator(regExp,this));
    labelEmp1->setBuddy(editEmp1);

    QSqlQueryModel *empModel = new QSqlQueryModel;
    empModel->setQuery("SELECT employeename FROM employee");
    QCompleter *emp1Completer = new QCompleter(empModel);
    emp1Completer->setCompletionMode(QCompleter::PopupCompletion);
    emp1Completer->setCaseSensitivity(Qt::CaseSensitive);
    editEmp1->setCompleter(emp1Completer);

    QToolButton *add1Button = new QToolButton;
    QPixmap addPix(":/add.png");
    add1Button->setIcon(addPix);
    add1Button->setToolTip(trUtf8("Добавить новую запись"));
    connect(add1Button,SIGNAL(clicked(bool)),this,SLOT(addEmp1Record()));

    QToolButton *see1Button = new QToolButton;
    QPixmap seePix(":/see.png");
    see1Button->setIcon(seePix);
    see1Button->setToolTip(trUtf8("Просмотреть выбранную запись"));
    connect(see1Button,SIGNAL(clicked()),this,SLOT(seeEmp1Record()));

    QToolButton *list1Button = new QToolButton;
    QPixmap listPix(":/list.png");
    list1Button->setIcon(listPix);
    list1Button->setToolTip(tr("Просмотр списка записей"));
    connect(list1Button,SIGNAL(clicked()),this,SLOT(listEmp1Record()));

    QHBoxLayout *editEmp1Layout = new QHBoxLayout;
    editEmp1Layout->addWidget(editEmp1);
    if(!onlyForRead){
        editEmp1Layout->addWidget(add1Button);
        editEmp1Layout->addWidget(see1Button);
        editEmp1Layout->addWidget(list1Button);
    }

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
        query.prepare("SELECT "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.emponeid), "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.emptwoid), "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.empthreeid) "
                      "FROM komissiya WHERE komissiya.komissiyaid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editEmp1->setText(query.value(0).toString());
        editEmp2->setText(query.value(1).toString());
        editEmp3->setText(query.value(2).toString());
    }else{
        editEmp1->clear();
        editEmp1->setText(QObject::tr("Председатель комиссии"));
        editEmp1->selectAll();
//        editEmp2->clear();
//        editEmp2->setText(QObject::tr("Член комиссии"));
//        editEmp2->selectAll();
//        editEmp3->clear();
//        editEmp3->setText(QObject::tr("Член комиссии"));
//        editEmp3->selectAll();
        newRecord = true;
    }
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelEmp1,0,0);
    mainLayout->addLayout(editEmp1Layout,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
    }
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Комиссия"));
    readSettings();
}

void KommisiyaForm::editRecord()
{
    if(checkFill()){
//        QTextStream stream(&exchangeFile);
//        QString line;
//        while(!stream.atEnd()){
//            stream.readLine();
//        }
//        if(newRecord){
//            NumPrefix numPref;
//            indexTemp = numPref.getPrefix("subdivision");
//            QSqlQuery query;
//            query.prepare("INSERT INTO subdivision (subdivisionid, subdivisionname, "
//                          "organizationid) VALUES(:subid, :subname, (SELECT organizationid "
//                          "FROM organization WHERE organizationname = :orgname))");
//            query.bindValue(":subid",indexTemp);
//            query.bindValue(":subname",editSubdivision->text());
//            query.bindValue(":orgname",editOrganization->text());
//            query.exec();
//            if(!query.isActive()){
//                QMessageBox::warning(this,QObject::tr("DataBase ERROR!"),query.lastError().text());
//                return;
//            }

//            line += "INSERT INTO subdivision (subdivisionid, subdivisionname, "
//                    "organizationid) VALUES('";
//            line += indexTemp.toUtf8();
//            line += "', '";
//            line += editSubdivision->text().toUtf8();
//            line += "', '";
//            QSqlQuery queryTemp;
//            queryTemp.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
//            queryTemp.bindValue(":name",editOrganization->text());
//            queryTemp.exec();
//            queryTemp.next();
//            line += queryTemp.value(0).toString().toUtf8();
//            line += "')";
//            line += "\r\n";
//            stream<<line;
//        }else{
//            QSqlQuery query;
//            query.prepare("UPDATE subdivision SET subdivisionname = :subname, "
//                          "organizationid = (SELECT organizationid FROM organization "
//                          "WHERE organizationname = :orgname) WHERE subdivisionid = :subid");
//            query.bindValue(":subid",indexTemp);
//            query.bindValue(":subname",editSubdivision->text());
//            query.bindValue(":orgname",editOrganization->text());
//            query.exec();
//            if(!query.isActive()){
//                QMessageBox::warning(this,QObject::trUtf8("Ошибка базы данных!"),query.lastError().text());
//                return;
//            }
//            line += "UPDATE subdivision SET subdivisionname = '";
//            line += editSubdivision->text().toUtf8();
//            line += "', organizationid ='";
//            QSqlQuery queryTemp;
//            queryTemp.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
//            queryTemp.bindValue(":name",editOrganization->text());
//            queryTemp.exec();
//            queryTemp.next();
//            line += query.value(0).toString().toUtf8();
//            line += "' WHERE subdivisionid = '";
//            line += indexTemp;
//            line += "'";
//            line += "\r\n";
//            stream<<line;
//        }
//        emit accept();
//        close();
    }
}

void KommisiyaForm::deleteRecord()
{
//    ForDelete forDelete(indexTemp,"subdivision",this);
//    //if(forDelete.result() == QDialog::Accepted){
//    forDelete.exec();
//    //int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
//    //                     QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
//    //if(k == QMessageBox::Yes){
//    forDelete.deleteOnDefault();
//    if(indexTemp != "OWN000000001"){
//        QTextStream stream(&exchangeFile);
//        QString line;
//        while(!stream.atEnd()){
//            stream.readLine();
//        }
//        QSqlQuery query;
//        query.prepare("DELETE FROM subdivision WHERE subdivisionid = :id");
//        query.bindValue(":id",indexTemp);
//        query.exec();
//        query.next();
//        line += "DELETE FROM subdivision WHERE subdivisionid = '";
//        line += indexTemp;
//        line += "'";
//        line += "\r\n";
//        stream<<line;
//    }else{
//        QMessageBox::warning(this,QObject::tr("Attention"),QObject::tr("You dont may delete default value!"));
//    }
}

void KommisiyaForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void KommisiyaForm::addEmp1Record()
{
//    OrganizationForm orgForm("",this,false);
//    orgForm.exec();
//    if(orgForm.returnValue() != ""){
//        QSqlQuery query;
//        query.prepare("SELECT organizationname From organization WHERE organizationid = :orgid");
//        query.bindValue(":orgid",orgForm.returnValue());
//        query.exec();
//        query.next();
//        editOrganization->setText(query.value(0).toString());
//    }
}

void KommisiyaForm::seeEmp1Record()
{
//    QSqlQuery query;
//    query.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
//    query.bindValue(":name",editOrganization->text());
//    query.exec();
//    while(query.next()){
//        OrganizationForm orgForm(query.value(0).toString(),this,true);
//        orgForm.exec();
//    }
}

void KommisiyaForm::listEmp1Record()
{
//    QSqlQuery query;
//    query.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
//    query.bindValue(":name",editOrganization->text());
//    query.exec();
//    query.next();
//    ViewListTable listTemp(query.value(0).toString(),"organization",this);
//    listTemp.exec();
//    if(listTemp.returnValue() != ""){
//        QSqlQuery query;
//        query.prepare("SELECT organizationname FROM organization WHERE organizationid = :orgid");
//        query.bindValue(":orgid",listTemp.returnValue());
//        query.exec();
//        query.next();
//        editOrganization->setText(query.value(0).toString());
//    }
}

void KommisiyaForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("KommisiyaForm").toByteArray());
}

void KommisiyaForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("KommisiyaForm", saveGeometry());
}

bool KommisiyaForm::checkFill()
{
    bool fill = true;
    if(editEmp1->text() == ""){
        editEmp1->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editEmp1->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editEmp2->text() == ""){
        editEmp2->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editEmp2->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editEmp3->text() == ""){
        editEmp3->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editEmp3->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}
