#include <QtWidgets>
#include <QtSql>
#include "kommisiyaform.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"
#include "employee.h"

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
    list1Button->setToolTip(trUtf8("Просмотр списка записей"));
    connect(list1Button,SIGNAL(clicked()),this,SLOT(listEmp1Record()));

    QHBoxLayout *editEmp1Layout = new QHBoxLayout;
    editEmp1Layout->addWidget(editEmp1);
    if(!onlyForRead){
        editEmp1Layout->addWidget(add1Button);
        editEmp1Layout->addWidget(see1Button);
        editEmp1Layout->addWidget(list1Button);
    }

    labelEmp2 = new QLabel(trUtf8("Член комиссии:"));
    editEmp2 = new LineEdit;
    editEmp2->setReadOnly(onlyForRead);
    editEmp2->setValidator(new QRegExpValidator(regExp,this));
    labelEmp2->setBuddy(editEmp2);

    QCompleter *emp2Completer = new QCompleter(empModel);
    emp2Completer->setCompletionMode(QCompleter::PopupCompletion);
    emp2Completer->setCaseSensitivity(Qt::CaseSensitive);
    editEmp1->setCompleter(emp2Completer);

    QToolButton *add2Button = new QToolButton;
    add2Button->setIcon(addPix);
    add2Button->setToolTip(trUtf8("Добавить новую запись"));
    connect(add2Button,SIGNAL(clicked(bool)),this,SLOT(addEmp2Record()));

    QToolButton *see2Button = new QToolButton;
    see2Button->setIcon(seePix);
    see2Button->setToolTip(trUtf8("Просмотреть выбранную запись"));
    connect(see2Button,SIGNAL(clicked()),this,SLOT(seeEmp2Record()));

    QToolButton *list2Button = new QToolButton;
    list2Button->setIcon(listPix);
    list2Button->setToolTip(trUtf8("Просмотр списка записей"));
    connect(list2Button,SIGNAL(clicked()),this,SLOT(listEmp2Record()));

    QHBoxLayout *editEmp2Layout = new QHBoxLayout;
    editEmp2Layout->addWidget(editEmp2);
    if(!onlyForRead){
        editEmp2Layout->addWidget(add2Button);
        editEmp2Layout->addWidget(see2Button);
        editEmp2Layout->addWidget(list2Button);
    }

    labelEmp3 = new QLabel(trUtf8("Член комиссии:"));
    editEmp3 = new LineEdit;
    editEmp3->setReadOnly(onlyForRead);
    editEmp3->setValidator(new QRegExpValidator(regExp,this));
    labelEmp3->setBuddy(editEmp3);

    QCompleter *emp3Completer = new QCompleter(empModel);
    emp3Completer->setCompletionMode(QCompleter::PopupCompletion);
    emp3Completer->setCaseSensitivity(Qt::CaseSensitive);
    editEmp3->setCompleter(emp3Completer);

    QToolButton *add3Button = new QToolButton;
    add3Button->setIcon(addPix);
    add3Button->setToolTip(trUtf8("Добавить новую запись"));
    connect(add3Button,SIGNAL(clicked(bool)),this,SLOT(addEmp3Record()));

    QToolButton *see3Button = new QToolButton;
    see3Button->setIcon(seePix);
    see3Button->setToolTip(trUtf8("Просмотреть выбранную запись"));
    connect(see3Button,SIGNAL(clicked()),this,SLOT(seeEmp3Record()));

    QToolButton *list3Button = new QToolButton;
    list3Button->setIcon(listPix);
    list3Button->setToolTip(trUtf8("Просмотр списка записей"));
    connect(list3Button,SIGNAL(clicked()),this,SLOT(listEmp3Record()));

    QHBoxLayout *editEmp3Layout = new QHBoxLayout;
    editEmp3Layout->addWidget(editEmp3);
    if(!onlyForRead){
        editEmp3Layout->addWidget(add3Button);
        editEmp3Layout->addWidget(see3Button);
        editEmp3Layout->addWidget(list3Button);
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
        editEmp1->setText(QObject::trUtf8("Председатель комиссии"));
        editEmp1->selectAll();
        editEmp2->clear();
        editEmp2->setText(QObject::trUtf8("Член комиссии"));
        editEmp2->selectAll();
        editEmp3->clear();
        editEmp3->setText(QObject::trUtf8("Член комиссии"));
        editEmp3->selectAll();
        newRecord = true;
    }
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelEmp1,0,0);
    mainLayout->addLayout(editEmp1Layout,0,1);
    mainLayout->addWidget(labelEmp2,1,0);
    mainLayout->addLayout(editEmp2Layout,1,1);
    mainLayout->addWidget(labelEmp3,2,0);
    mainLayout->addLayout(editEmp3Layout,2,1);
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
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        if(newRecord){
            NumPrefix numPref;
            indexTemp = numPref.getPrefix("komissiya");
            QSqlQuery query;
            query.prepare("INSERT INTO komissiya (komissiyaid, emponeid, "
                          "emptwoid, empthreeid) VALUES(:komissiyaid, "
                          "(SELECT employeeid FROM employee WHERE employeename = :nameone), "
                          "(SELECT employeeid FROM employee WHERE employeename = :nametwo), "
                          "(SELECT employeeid FROM employee WHERE employeename = :namethree))");
            query.bindValue(":komissiyaid",indexTemp);
            query.bindValue(":nameone",editEmp1->text());
            query.bindValue(":nametwo",editEmp2->text());
            query.bindValue(":namethree",editEmp3->text());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR! INSERT INTO komissiya"),query.lastError().text());
                return;
            }

            line += "INSERT INTO komissiya (komissiyaid, emponeid, "
                    "emptwoid, empthreeid) VALUES('";
            line += indexTemp.toUtf8();
            line += "', '";
            QSqlQuery queryTemp;
            queryTemp.prepare("SELECT employeeid FROM employee WHERE employeename = :nameone");
            queryTemp.bindValue(":nameone",editEmp1->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', '";
            queryTemp.clear();
            queryTemp.prepare("SELECT employeeid FROM employee WHERE employeename = :nameone");
            queryTemp.bindValue(":nameone",editEmp2->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', '";
            queryTemp.clear();
            queryTemp.prepare("SELECT employeeid FROM employee WHERE employeename = :nameone");
            queryTemp.bindValue(":nameone",editEmp3->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "')";
            line += "\r\n";
            stream<<line;
        }else{
            QSqlQuery query;
            query.prepare("UPDATE komissiya SET emponeid = (SELECT employeeid FROM employee WHERE employeename = :nameone), "
                          "emptwoid = (SELECT employeeid FROM employee WHERE employeename = :nametwo), "
                          "empthreeid = (SELECT employeeid FROM employee WHERE employeename = :namethree) "
                          "WHERE komissiyaid = :komissiyaid");
            query.bindValue(":komissiyaid",indexTemp);
            query.bindValue(":nameone",editEmp1->text());
            query.bindValue(":nametwo",editEmp2->text());
            query.bindValue(":namethree",editEmp3->text());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("Ошибка базы данных! UPDATE komissiya"),query.lastError().text());
                return;
            }
            line += "UPDATE komissiya SET emponeid = '";
            QSqlQuery queryTemp;
            queryTemp.prepare("SELECT employeeid FROM employee WHERE employeename = :nameone");
            queryTemp.bindValue(":nameone",editEmp1->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', emptwoid = '";
            queryTemp.clear();
            queryTemp.prepare("SELECT employeeid FROM employee WHERE employeename = :nameone");
            queryTemp.bindValue(":nameone",editEmp2->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', empthreeid = '";
            queryTemp.clear();
            queryTemp.prepare("SELECT employeeid FROM employee WHERE employeename = :nameone");
            queryTemp.bindValue(":nameone",editEmp3->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "' WHERE komissiyaid =  '";
            line += indexTemp;
            line += "'";
            line += "\r\n";
            stream<<line;
        }
        emit accept();
        close();
    }
}

void KommisiyaForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"komissiya",this);
    forDelete.exec();
    forDelete.deleteOnDefault();
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM komissiya WHERE komissiyaid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM komissiya WHERE komissiyaid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void KommisiyaForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void KommisiyaForm::addEmp1Record()
{
    EmployeeForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename From employee WHERE employeeid = :id");
        query.bindValue(":id",orgForm.returnValue());
        query.exec();
        query.next();
        editEmp1->setText(query.value(0).toString());
    }
}

void KommisiyaForm::seeEmp1Record()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmp1->text());
    query.exec();
    while(query.next()){
        EmployeeForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void KommisiyaForm::listEmp1Record()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmp1->text());
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
        editEmp1->setText(query.value(0).toString());
    }
}

void KommisiyaForm::addEmp2Record()
{
    EmployeeForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename From employee WHERE employeeid = :id");
        query.bindValue(":gid",orgForm.returnValue());
        query.exec();
        query.next();
        editEmp2->setText(query.value(0).toString());
    }
}

void KommisiyaForm::seeEmp2Record()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmp2->text());
    query.exec();
    while(query.next()){
        EmployeeForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void KommisiyaForm::listEmp2Record()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmp2->text());
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
        editEmp2->setText(query.value(0).toString());
    }
}

void KommisiyaForm::addEmp3Record()
{
    EmployeeForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename From employee WHERE employeeid = :id");
        query.bindValue(":gid",orgForm.returnValue());
        query.exec();
        query.next();
        editEmp3->setText(query.value(0).toString());
    }
}

void KommisiyaForm::seeEmp3Record()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmp3->text());
    query.exec();
    while(query.next()){
        EmployeeForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void KommisiyaForm::listEmp3Record()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmp3->text());
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
        editEmp3->setText(query.value(0).toString());
    }
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
