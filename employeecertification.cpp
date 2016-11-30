#include <QtGui>
#include <QtSql>

#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"
#include "viewlisttable.h"
#include "employee.h"
#include "employeecertification.h"
#include "testtask.h"
#include "empcertdate.h"

EmployeeCertification::EmployeeCertification(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    addIntoTable = false;
    indexTemp = id;

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString toolButtonStyle = QLatin1String(file.readAll());

    labelEmployee = new QLabel(tr("FIO:"));
    editEmployee = new LineEdit;
    editEmployee->setReadOnly(onlyForRead);
    connect(editEmployee,SIGNAL(textChanged(QString)),this,SLOT(setEnter(QString)));

    QSqlQueryModel *employeeModel = new QSqlQueryModel;
    employeeModel->setQuery("SELECT employeename FROM employee");
    QCompleter *employeeCompleter = new QCompleter(employeeModel);//employeeModel);
    employeeCompleter->setCompletionMode(QCompleter::PopupCompletion);
    employeeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editEmployee->setCompleter(employeeCompleter);

    QToolButton *addEmployeeButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addEmployeeButton->setIcon(addPix);
    addEmployeeButton->setToolTip(tr("Add new record"));
    addEmployeeButton->setStyleSheet(toolButtonStyle);
    connect(addEmployeeButton,SIGNAL(clicked()),this,SLOT(addEmployeeRecord()));

    QToolButton *seeEmployeeButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeEmployeeButton->setIcon(seePix);
    seeEmployeeButton->setToolTip(tr("See select item"));
    seeEmployeeButton->setStyleSheet(toolButtonStyle);
    connect(seeEmployeeButton,SIGNAL(clicked()),this,SLOT(seeEmployeeRecord()));

    QToolButton *listEmployeeButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listEmployeeButton->setIcon(listPix);
    listEmployeeButton->setToolTip(tr("See list of item"));
    listEmployeeButton->setStyleSheet(toolButtonStyle);
    connect(listEmployeeButton,SIGNAL(clicked()),this,SLOT(listEmployeeRecord()));

    labelPeriodicity = new QLabel(tr("Periodicity"));
    editPeriodicity = new LineEdit;

    QSqlQueryModel *queryPer = new QSqlQueryModel;
    queryPer->setQuery("SELECT periodicityname FROM periodicity");
    QCompleter *perCompleter = new QCompleter(queryPer);
    perCompleter->setCompletionMode(QCompleter::PopupCompletion);
    perCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editPeriodicity->setCompleter(perCompleter);

    QPushButton *addTableLine = new QPushButton(tr("Add record"));
    addTableLine->setIcon(addPix);
    connect(addTableLine,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPushButton *deleteTableLine = new QPushButton(tr("Delete record"));
    QPixmap pixDelete(":/delete.png");
    deleteTableLine->setIcon(pixDelete);
    deleteTableLine->setStyleSheet("QPushButton:hover {color: red}");
    connect(deleteTableLine,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QHBoxLayout *tableButtonBox = new QHBoxLayout;
    tableButtonBox->addWidget(addTableLine);
    tableButtonBox->addWidget(deleteTableLine);
    tableButtonBox->addStretch();

    tableWidget = new QTableWidget(0,5);

    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Cipher")<<tr("Protocol Date")<<tr("Protocol Number")<<tr("Test task"));
    QHeaderView *head = tableWidget->horizontalHeader();
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM empcert WHERE empcertid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            QSqlQuery employeeQuery;
            employeeQuery.prepare("SELECT employeename FROM employee WHERE employeeid = :id;");
            employeeQuery.bindValue(":id",query.value(1).toString());
            employeeQuery.exec();
            employeeQuery.next();
            QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
            settings.setValue("Employeeid", query.value(1).toString());
            settings.setValue("Empcertid",indexTemp);
            editEmployee->setText(employeeQuery.value(0).toString());
            idEmployee = query.value(1).toString();

            editPeriodicity->setText(query.value(2).toString());

            QSqlQuery queryTable;
            queryTable.prepare("SELECT empcertdateid FROM empcerttable WHERE empcertid = :id");
            queryTable.bindValue(":id",indexTemp);
            queryTable.exec();
            if(!queryTable.isActive()){
                QMessageBox::warning(this,QObject::tr("EmpCert Table, SELECT Table ERROR!"),queryTable.lastError().text());
            }
            int rowCount = 0;
            while(queryTable.next()){
                QSqlQuery queryName;
                queryName.prepare("SELECT * FROM empcertdate WHERE empcertdateid = :id");
                empcertdateId = queryTable.value(0).toString();
                queryName.bindValue(":id",empcertdateId);
                queryName.exec();
                if(!queryName.isActive()){
                    QMessageBox::warning(this,QObject::tr("EmpCert Table, SELECT Testtask ERROR!"),queryName.lastError().text());
                }
                while(queryName.next()){
                    QSqlQuery queryId;
                    queryId.prepare("SELECT cipher, testtaskname FROM testtask WHERE testtaskid = :id");
                    queryId.bindValue(":id",queryName.value(3).toString());
                    queryId.exec();
                    queryId.next();


                    tableWidget->insertRow(rowCount);
                    QTableWidgetItem *itemName = new QTableWidgetItem;
                    tableWidget->setItem(rowCount,0,itemName);
                    tableWidget->item(rowCount,0)->setText(queryId.value(0).toString());

                    QTableWidgetItem *itemPD = new QTableWidgetItem;
                    itemPD->setTextAlignment(Qt::AlignCenter);
                    tableWidget->setItem(rowCount,1,itemPD);
                    tableWidget->item(rowCount,1)->setText(queryName.value(1).toString());

                    QTableWidgetItem *itemPN = new QTableWidgetItem;
                    itemPN->setTextAlignment(Qt::AlignCenter);
                    tableWidget->setItem(rowCount,2,itemPN);
                    tableWidget->item(rowCount,2)->setText(queryName.value(2).toString());

                    QTableWidgetItem *itemTT = new QTableWidgetItem;
                    itemTT->setTextAlignment(Qt::AlignLeft);
                    tableWidget->setItem(rowCount,3,itemTT);
                    tableWidget->item(rowCount,3)->setText(queryId.value(1).toString());

                    QTableWidgetItem *itemId = new QTableWidgetItem;
                    tableWidget->setItem(rowCount,4,itemId);
                    tableWidget->item(rowCount,4)->setText(queryTable.value(0).toString());
                    ++rowCount;
                }
            }
        }
        QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
        int kk = settings.value("CopyEmpcert").toInt();
        if(kk == 1){
            indexTemp = settings.value("EmpcertID").toString();
        }
    }else{
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("empcert");
        if(indexTemp == ""){
            close();
        }
    }

    tableWidget->setAlternatingRowColors(true);
    tableWidget->resizeColumnsToContents();
    tableWidget->setColumnHidden(4,true);
    //tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    head->setStretchLastSection(true);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelEmployee,0,0);
    mainLayout->addWidget(editEmployee,0,1);
    mainLayout->addWidget(addEmployeeButton,0,2);
    mainLayout->addWidget(seeEmployeeButton,0,3);
    mainLayout->addWidget(listEmployeeButton,0,4);
    mainLayout->addWidget(labelPeriodicity,1,0);
    mainLayout->addWidget(editPeriodicity,1,1);
    if(!onlyForRead){
        mainLayout->addLayout(tableButtonBox,2,0,1,5);
    }
    mainLayout->addWidget(tableWidget,3,0,1,5);
    if(!onlyForRead){
        QPushButton *saveButton = new QPushButton(tr("Save"));
        connect(saveButton,SIGNAL(clicked()),this,SLOT(saveRecord()));

        QPushButton *cancelButton = new QPushButton(tr("Cancel"));
        cancelButton->setStyleSheet("QPushButton:hover {color: red}");
        connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelRecord()));

        QHBoxLayout *scButtonBox = new QHBoxLayout;
        scButtonBox->addWidget(cancelButton);
        scButtonBox->addWidget(saveButton);
        mainLayout->addLayout(scButtonBox,6,0,1,5,Qt::AlignRight);
    }
    setLayout(mainLayout);

    setWindowTitle(tr("Employee Certification"));
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(seeRecordOfTable()));
}

void EmployeeCertification::editRecord()
{
    if(checkingFill()){
        QSqlQuery query;
        query.prepare("SELECT * FROM empcert WHERE empcertid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        if(query.isValid()){

            QSqlQuery queryU;
            queryU.prepare("UPDATE empcert SET "
                           "employeeid = :employeeid, "
                           "periodicity = :periodicity "
                           "WHERE empcertid = :id");
            queryU.bindValue(":employeeid",idEmployee);
            queryU.bindValue(":id",indexTemp);
            queryU.bindValue(":periodicity",editPeriodicity->text().simplified());
            queryU.exec();
            if(!queryU.isActive()){
                QMessageBox::warning(this,QObject::tr("Employee Cert, UPDATE ERROR!"),queryU.lastError().text());
            }
        }else{

            QSqlQuery queryInsert;
            queryInsert.prepare("INSERT INTO empcert ("
                                "empcertid, "
                                "employeeid, "
                                "periodicity"
                                ") VALUES(:id, "
                                ":employeeid, "
                                ":periodicity"
                                ");");
            queryInsert.bindValue(":id",indexTemp);
            queryInsert.bindValue(":employeeid",idEmployee);
            queryInsert.bindValue(":periodicity",editPeriodicity->text().simplified());
            queryInsert.exec();
            if(!queryInsert.isActive()){
                QMessageBox::warning(this,QObject::tr("Employee Cert, INSERT ERROR!"),queryInsert.lastError().text());
            }

        }
    }
}

void EmployeeCertification::deleteRecord()
{
    QSqlQuery query;
    query.prepare("DELETE FROM empcert WHERE empcertid = :id;");
    query.bindValue(":id",indexTemp);
    query.exec();
}

void EmployeeCertification::done(int result)
{
    writeSettings();
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.remove("Employeeid");
    settings.remove("Empcertid");
    QDialog::done(result);
}

void EmployeeCertification::addRecordOfTable()
{
    if(checkingFill()){

        EmpCertDate viewListTable("",this,false);
        viewListTable.exec();
        QString rr = viewListTable.returnValue();
        empcertdateId = rr;

        QSqlQuery query;
        query.prepare("SELECT * FROM empcertdate WHERE empcertdateid = :id");
        query.bindValue(":id",empcertdateId);
        query.exec();
        while(query.next()){
            QSqlQuery queryCi;
            queryCi.prepare("SELECT cipher FROM testtask WHERE testtaskid = :id");
            queryCi.bindValue(":id",query.value(3).toString());
            queryCi.exec();
            queryCi.next();
            int rowCount = tableWidget->rowCount();
            bool insert = true;
            //Проверка на существование записи
            if (rowCount != 0){
                for(int kk = 0; kk < rowCount; ++kk){
                    QString yy = tableWidget->item(kk,0)->text();
                    QString pp = queryCi.value(0).toString();
                    if(yy == pp){
                        QString tempString = queryCi.value(0).toString();
                        tempString += QObject::tr(" is availble!");
                        QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
                        insert = false;
                        break;
                    }
                }
            }//
            if(insert){
                addIntoTable = true;
                //?
                QSqlQuery queryId;
                queryId.prepare("SELECT cipher, testtaskname FROM testtask WHERE testtaskid = :id");
                queryId.bindValue(":id",query.value(3).toString());
                queryId.exec();
                queryId.next();

                tableWidget->insertRow(rowCount);

                QTableWidgetItem *itemName = new QTableWidgetItem;
                tableWidget->setItem(rowCount,0,itemName);
                tableWidget->item(rowCount,0)->setText(queryId.value(0).toString());

                QTableWidgetItem *itemPD = new QTableWidgetItem;
                itemPD->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(rowCount,1,itemPD);
                tableWidget->item(rowCount,1)->setText(query.value(1).toString());

                QTableWidgetItem *itemPN = new QTableWidgetItem;
                itemPN->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(rowCount,2,itemPN);
                tableWidget->item(rowCount,2)->setText(query.value(2).toString());

                QTableWidgetItem *itemTT = new QTableWidgetItem;
                itemTT->setTextAlignment(Qt::AlignLeft);
                tableWidget->setItem(rowCount,3,itemTT);
                tableWidget->item(rowCount,3)->setText(queryId.value(1).toString());

                QTableWidgetItem *itemId = new QTableWidgetItem;
                tableWidget->setItem(rowCount,4,itemId);
                tableWidget->item(rowCount,4)->setText(empcertdateId);

                tableWidget->setColumnHidden(4,true);

                NumPrefix numPrefix;
                QString indexCost = numPrefix.getPrefix("empcerttable");
                if(indexCost == ""){
                    close();
                }

                QSqlQuery queryCT;
                queryCT.prepare("INSERT INTO empcerttable ("
                                "empcerttableid, empcertid, empcertdateid"
                                ") VALUES(:empcerttableid, :empcertid, :empcertdateid)");
                queryCT.bindValue(":empcerttableid",indexCost);
                queryCT.bindValue(":empcertid",indexTemp);
                queryCT.bindValue(":empcertdateid",empcertdateId);
                queryCT.exec();
                if(!queryCT.isActive()){
                    QMessageBox::warning(this,QObject::tr("Employee Cert Table, INSERT ERROR!"),queryCT.lastError().text());
                }
            }
        }
    }
}

void EmployeeCertification::deleteRecordOfTable()
{
    int k = QMessageBox::question(this,tr("Attention!!"),
                              tr("Really delete?"),
                                  QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(k == QMessageBox::Yes){

        int i = tableWidget->currentRow();

        QSqlQuery queryET;
        queryET.prepare("DELETE FROM empcerttable WHERE empcertdateid = :id;");
        queryET.bindValue(":id",tableWidget->item(i,4)->text());
        queryET.exec();
        if(!queryET.isActive()){
            QMessageBox::warning(this,QObject::tr("Table Value, DELETE ERROR!"),queryET.lastError().text());
            return;
        }
        QSqlQuery queryETD;
        queryETD.prepare("DELETE FROM empcertdate WHERE empcertdateid = :id;");
        queryETD.bindValue(":id",tableWidget->item(i,4)->text());
        queryETD.exec();
        if(!queryETD.isActive()){
            QMessageBox::warning(this,QObject::tr("empcertdate Value, DELETE ERROR!"),queryETD.lastError().text());
            return;
        }
        tableWidget->removeRow(tableWidget->currentRow());
        tableWidget->repaint();
    }
}

void EmployeeCertification::seeRecordOfTable()
{
    int rowCount = tableWidget->currentRow();
    //    QSqlQuery query;
    //    query.prepare("SELECT empcertdateid FROM empcertdate WHERE testtaskid IN (SELECT testtaskid FROM testtask WHERE testtaskname = :name);");
    //    query.bindValue(":name",tableWidget->item(i,0)->text());
    //    query.exec();
    //    query.next();

    EmpCertDate showForm(tableWidget->item(rowCount,4)->text(),this,false);
    showForm.exec();
    empcertdateId = showForm.returnValue();

    QSqlQuery query;
    query.prepare("SELECT * FROM empcertdate WHERE empcertdateid = :id");
    query.bindValue(":id",empcertdateId);
    query.exec();
    while(query.next()){

        QSqlQuery queryId;
        queryId.prepare("SELECT cipher, testtaskname FROM testtask WHERE testtaskid = :id");
        queryId.bindValue(":id",query.value(3).toString());
        queryId.exec();
        queryId.next();

        QTableWidgetItem *itemName = new QTableWidgetItem;
        tableWidget->setItem(rowCount,0,itemName);
        tableWidget->item(rowCount,0)->setText(queryId.value(0).toString());

        QTableWidgetItem *itemPD = new QTableWidgetItem;
        itemPD->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(rowCount,1,itemPD);
        tableWidget->item(rowCount,1)->setText(query.value(1).toString());

        QTableWidgetItem *itemPN = new QTableWidgetItem;
        itemPN->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(rowCount,2,itemPN);
        tableWidget->item(rowCount,2)->setText(query.value(2).toString());

        QTableWidgetItem *itemTT = new QTableWidgetItem;
        itemTT->setTextAlignment(Qt::AlignLeft);
        tableWidget->setItem(rowCount,3,itemTT);
        tableWidget->item(rowCount,3)->setText(queryId.value(1).toString());

        QTableWidgetItem *itemId = new QTableWidgetItem;
        tableWidget->setItem(rowCount,4,itemId);
        tableWidget->item(rowCount,4)->setText(empcertdateId);

        tableWidget->setColumnHidden(4,true);
        tableWidget->repaint();
    }
}

void EmployeeCertification::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("EmpCert").toByteArray());
}

void EmployeeCertification::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("EmpCert", saveGeometry());
}

void EmployeeCertification::saveRecord()
{
    editRecord();
    emit accept();
}

void EmployeeCertification::cancelRecord()
{
    if(addIntoTable){
        //Удаление табличной части документа
        for(int row = 0; row < tableWidget->rowCount(); ++row){
            QSqlQuery queryCD;
            queryCD.prepare("DELETE FROM empcertdate WHERE empcertdateid = :id");
            queryCD.bindValue(":id",tableWidget->item(row,4)->text());
            queryCD.exec();
            if(!queryCD.isActive()){
                QMessageBox::warning(this,QObject::tr("EmpCert Table, DELETE ERROR!"),queryCD.lastError().text());
            }
        }
        QSqlQuery queryCT;
        queryCT.prepare("DELETE FROM empcerttable WHERE empcertid = :id");
        queryCT.bindValue(":id",indexTemp);
        queryCT.exec();
        if(!queryCT.isActive()){
            QMessageBox::warning(this,QObject::tr("EmpCert Table, DELETE ERROR!"),queryCT.lastError().text());
        }
    }
    indexTemp = "";//?????
    emit accept();
}

void EmployeeCertification::addEmployeeRecord()
{
    EmployeeForm openForm("",this,false);
    openForm.exec();
    idEmployee = openForm.returnValue();
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Employeeid",idEmployee);
    if(idEmployee != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename FROM employee WHERE employeeid = :id");
        query.bindValue(":id",idEmployee);
        query.exec();
        query.next();
        editEmployee->setText(query.value(0).toString());
    }
}

void EmployeeCertification::seeEmployeeRecord()
{
    //modelHead->
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmployee->text());
    query.exec();
    while(query.next()){
        EmployeeForm openForm(query.value(0).toString(),this,true);
        openForm.exec();
        idEmployee = openForm.returnValue();
    }
}

void EmployeeCertification::listEmployeeRecord()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmployee->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"employee",this);
    listTemp.exec();
    idEmployee = listTemp.returnValue();
    if(idEmployee != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename FROM employee WHERE employeeid = :id");
        query.bindValue(":id",idEmployee);
        query.exec();
        query.next();
        editEmployee->setText(query.value(0).toString());
        QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
        settings.setValue("Employeeid",idEmployee);
    }
}

bool EmployeeCertification::checkingFill()
{
    bool fill = true;
    if(editEmployee->text() == ""){
        editEmployee->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editEmployee->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editPeriodicity->text() == ""){
        editPeriodicity->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editPeriodicity->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}

void EmployeeCertification::setEnter(QString text)
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",text);
    query.exec();
    while(query.next()){
        QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
        settings.setValue("Employeeid", query.value(0).toString());
        idEmployee = query.value(0).toString();
    }
}
