#include <QtGui>
#include <QtSql>

#include "safetyreqprod.h"
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"
#include "viewlisttable.h"
#include "safetyrequirements.h"
#include "security.h"
#include "testtask.h"
#include "legalact.h"

SafetyReqProd::SafetyReqProd(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    addIntoTable = false;
    //Header Of Document
    indexTemp = id;

    labelSecurity = new QLabel(tr("Security:"));
    editSecurity = new LineEdit;
    editSecurity->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f . 0-9]{150}");
    editSecurity->setValidator(new QRegExpValidator(regExp,this));
    labelSecurity->setBuddy(editSecurity);

    QSqlQueryModel *securityModel = new QSqlQueryModel;
    securityModel->setQuery("SELECT securityname FROM security");
    QCompleter *securityCompleter = new QCompleter(securityModel);
    securityCompleter->setCompletionMode(QCompleter::PopupCompletion);
    securityCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editSecurity->setCompleter(securityCompleter);

    QToolButton *addSecurityButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addSecurityButton->setIcon(addPix);
    addSecurityButton->setToolTip(tr("Add new record"));
    connect(addSecurityButton,SIGNAL(clicked()),this,SLOT(addSecurityRecord()));

    QToolButton *seeSecurityButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeSecurityButton->setIcon(seePix);
    seeSecurityButton->setToolTip(tr("See select item"));
    connect(seeSecurityButton,SIGNAL(clicked()),this,SLOT(seeSecurityRecord()));

    QToolButton *listSecurityButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listSecurityButton->setIcon(listPix);
    listSecurityButton->setToolTip(tr("See list of item"));
    connect(listSecurityButton,SIGNAL(clicked()),this,SLOT(listSecurityRecord()));

    QHBoxLayout *securityLayout = new QHBoxLayout;
    securityLayout->addWidget(labelSecurity);
    securityLayout->addWidget(editSecurity);
    securityLayout->addWidget(addSecurityButton);
    securityLayout->addWidget(seeSecurityButton);
    securityLayout->addWidget(listSecurityButton);

    QVBoxLayout *nameLayout = new QVBoxLayout;
    nameLayout->addLayout(securityLayout);

    labelSafetyReq = new QLabel(tr("Safety Requirement:"));
    editSafetyReq = new LineEdit;
    editSafetyReq->setReadOnly(onlyForRead);
    editSafetyReq->setValidator(new QRegExpValidator(regExp,this));
    labelSafetyReq->setBuddy(editSafetyReq);

    QSqlQueryModel *safetyReqModel = new QSqlQueryModel;
    safetyReqModel->setQuery("SELECT safetyreqname FROM safetyreq");
    QCompleter *safetyReqCompleter = new QCompleter(safetyReqModel);
    safetyReqCompleter->setCompletionMode(QCompleter::PopupCompletion);
    safetyReqCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editSafetyReq->setCompleter(safetyReqCompleter);

    QToolButton *addSafetyReqButton = new QToolButton;
    addSafetyReqButton->setIcon(addPix);
    addSafetyReqButton->setToolTip(tr("Add new record"));
    connect(addSafetyReqButton,SIGNAL(clicked()),this,SLOT(addSafetyReqRecord()));

    QToolButton *seeSafetyReqButton = new QToolButton;
    seeSafetyReqButton->setIcon(seePix);
    seeSafetyReqButton->setToolTip(tr("See select item"));
    connect(seeSafetyReqButton,SIGNAL(clicked()),this,SLOT(seeSafetyReqRecord()));

    QToolButton *listSafetyReqButton = new QToolButton;
    listSafetyReqButton->setIcon(listPix);
    listSafetyReqButton->setToolTip(tr("See list of item"));
    connect(listSafetyReqButton,SIGNAL(clicked()),this,SLOT(listSafetyReqRecord()));

    QHBoxLayout *safetyReqLayout = new QHBoxLayout;
    safetyReqLayout->addWidget(labelSafetyReq);
    safetyReqLayout->addWidget(editSafetyReq);
    safetyReqLayout->addWidget(addSafetyReqButton);
    safetyReqLayout->addWidget(seeSafetyReqButton);
    safetyReqLayout->addWidget(listSafetyReqButton);

    nameLayout->addLayout(safetyReqLayout);

    labelTestTask = new QLabel(tr("Test task:"));
    editTestTask = new LineEdit;
    editTestTask->setReadOnly(onlyForRead);
    editTestTask->setValidator(new QRegExpValidator(regExp,this));
    labelTestTask->setBuddy(editTestTask);
    editCiphr = new LineEdit;

    QSqlQueryModel *testTaskModel = new QSqlQueryModel;
    testTaskModel->setQuery("SELECT securityname FROM security");
    QCompleter *testTaskCompleter = new QCompleter(testTaskModel);
    testTaskCompleter->setCompletionMode(QCompleter::PopupCompletion);
    testTaskCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editTestTask->setCompleter(testTaskCompleter);

    QToolButton *addTestTaskButton = new QToolButton;
    addTestTaskButton->setIcon(addPix);
    addTestTaskButton->setToolTip(tr("Add new record"));
    connect(addTestTaskButton,SIGNAL(clicked()),this,SLOT(addTestTaskRecord()));

    QToolButton *seeTestTaskButton = new QToolButton;
    seeTestTaskButton->setIcon(seePix);
    seeTestTaskButton->setToolTip(tr("See select item"));
    connect(seeTestTaskButton,SIGNAL(clicked()),this,SLOT(seeTestTaskRecord()));

    QToolButton *listTestTaskButton = new QToolButton;
    listTestTaskButton->setIcon(listPix);
    listTestTaskButton->setToolTip(tr("See list of item"));
    connect(listTestTaskButton,SIGNAL(clicked()),this,SLOT(listTestTaskRecord()));

    QHBoxLayout *testTaskLayout = new QHBoxLayout;
    testTaskLayout->addWidget(labelTestTask);
    testTaskLayout->addWidget(editTestTask);
    testTaskLayout->addWidget(addTestTaskButton);
    testTaskLayout->addWidget(seeTestTaskButton);
    testTaskLayout->addWidget(listTestTaskButton);

    nameLayout->addLayout(testTaskLayout);

    savePushButton = new QPushButton(tr("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(saveRecord()));
    savePushButton->setToolTip(tr("Save And Close Button"));

    cancelPushButton = new QPushButton(tr("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelRecord()));
    cancelPushButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    //Section Of Document

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

    tableWidget = new QTableWidget(0,1);

    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Name"));
    QHeaderView *head = tableWidget->horizontalHeader();
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidget->setAlternatingRowColors(true);
    tableWidget->resizeColumnsToContents();
    //tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    head->setStretchLastSection(true);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelSecurity,0,0);
    QHBoxLayout *firstLayout = new QHBoxLayout;
    firstLayout->addWidget(editSecurity);
    firstLayout->addWidget(addSecurityButton);
    firstLayout->addWidget(seeSecurityButton);
    firstLayout->addWidget(listSecurityButton);
    mainLayout->addLayout(firstLayout,0,1);
    mainLayout->addWidget(labelSafetyReq,1,0);
    QHBoxLayout *secondLayout = new QHBoxLayout;
    secondLayout->addWidget(editSafetyReq);
    secondLayout->addWidget(addSafetyReqButton);
    secondLayout->addWidget(seeSafetyReqButton);
    secondLayout->addWidget(listSafetyReqButton);
    mainLayout->addLayout(secondLayout,1,1);
    mainLayout->addWidget(labelTestTask,2,0);
    QHBoxLayout *thirdLayout = new QHBoxLayout;
    thirdLayout->addWidget(editCiphr);
    thirdLayout->addWidget(editTestTask);
    thirdLayout->addWidget(addTestTaskButton);
    thirdLayout->addWidget(seeTestTaskButton);
    thirdLayout->addWidget(listTestTaskButton);
    mainLayout->addLayout(thirdLayout,2,1);
    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addLayout(nameLayout);
    if(!onlyForRead){
        mainLayout->addLayout(tableButtonBox,3,0,1,2);
    }
    mainLayout->addWidget(tableWidget,4,0,1,2);
    //mainLayout->addLayout(summaLayout);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,5,0,1,2);
        //editName->selectAll();
    }

    //double summa = 0;
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM srp WHERE srpid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            idSecurity = query.value(1).toString();
            idSafetyReq = query.value(2).toString();
            idTestTask = query.value(4).toString();
            QSqlQuery securityQuery;
            securityQuery.prepare("SELECT securityname FROM security WHERE securityid = :id;");
            securityQuery.bindValue(":id",idSecurity);
            securityQuery.exec();
            securityQuery.next();
            editSecurity->setText(securityQuery.value(0).toString());

            QSqlQuery safetyReqQuery;
            safetyReqQuery.prepare("SELECT safetyreqname FROM safetyreq WHERE safetyreqid = :id;");
            safetyReqQuery.bindValue(":id",idSafetyReq);
            safetyReqQuery.exec();
            safetyReqQuery.next();
            editSafetyReq->setText(safetyReqQuery.value(0).toString());

            QSqlQuery testTaskQuery;
            testTaskQuery.prepare("SELECT testtaskname, cipher FROM testtask WHERE testtaskid = :id;");
            testTaskQuery.bindValue(":id",idTestTask);
            testTaskQuery.exec();
            testTaskQuery.next();
            editTestTask->setText(testTaskQuery.value(0).toString());
            editCiphr->setText(query.value(3).toString());

            QSqlQuery queryTable;
            queryTable.prepare("SELECT legalactsid FROM srptable WHERE srpid = :id");
            queryTable.bindValue(":id",indexTemp);
            queryTable.exec();
            if(!queryTable.isActive()){
                QMessageBox::warning(this,QObject::tr("Safety Req Prod Table, SELECT Table ERROR!"),queryTable.lastError().text());
            }
            int row = 0;
            while(queryTable.next()){
                QSqlQuery queryName;
                queryName.prepare("SELECT legalactsname FROM legalacts WHERE legalactsid = :id");
                queryName.bindValue(":id",queryTable.value(0).toString());
                queryName.exec();
                if(!queryName.isActive()){
                    QMessageBox::warning(this,QObject::tr("Safety Req Prod Table, SELECT Name ERROR!"),queryName.lastError().text());
                }
                while(queryName.next()){
                    tableWidget->insertRow(row);
                    QTableWidgetItem *item = new QTableWidgetItem;
                    tableWidget->setItem(row,0,item);
                    tableWidget->item(row,0)->setText(queryName.value(0).toString());
                    ++row;
                }
            }
        }
        updateRecord = true;
    }else{
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("srp");
        if(indexTemp == ""){
            close();
        }
        updateRecord = false;
    }
    setLayout(mainLayout);

    setWindowTitle(tr("Safety Requirement Production"));

    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(seeRecordOfTable()));
}

void SafetyReqProd::editRecord()
{
    if(checkingFill()){
        if(updateRecord){
            QSqlQuery query;
            query.prepare("UPDATE srp SET "
                          "securityid = :securityid, "
                          "safetyreqid = :safetyreqid, "
                          "testtaskid = :testtaskid, "
                          "cipher = :cipher "
                          "WHERE srpid = :id");
            query.bindValue(":securityid",idSecurity);
            query.bindValue(":safetyreqid",idSafetyReq);
            query.bindValue(":testtaskid",idTestTask);
            query.bindValue(":id",indexTemp);
            query.bindValue(":cipher",editCiphr->text());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::tr("Safety Req Prod, UPDATE ERROR!"),query.lastError().text());
            }
        }else{
            QSqlQuery query;
            query.prepare("SELECT * FROM srp WHERE srpid = :id");
            query.bindValue(":id",indexTemp);
            query.exec();
            query.next();
            if(!query.isValid()){
                QSqlQuery queryInsert;
                queryInsert.prepare("INSERT INTO srp ("
                                    "srpid, "
                                    "securityid, "
                                    "safetyreqid, "
                                    "testtaskid, "
                                    "cipher"
                                    ") VALUES(:id, "
                                    ":securityid, "
                                    ":safetyreqid, "
                                    ":testtaskid, "
                                    ":cipher"
                                    ");");
                queryInsert.bindValue(":id",indexTemp);
                queryInsert.bindValue(":securityid",idSecurity);
                queryInsert.bindValue(":safetyreqid",idSafetyReq);
                queryInsert.bindValue(":testtaskid",idTestTask);
                queryInsert.bindValue(":cipher",editCiphr->text());
                queryInsert.exec();
                if(!queryInsert.isActive()){
                    QMessageBox::warning(this,QObject::tr("Safety Req Prod, INSERT ERROR!"),queryInsert.lastError().text());
                }
            }else{
                QString tempString = "Document - Safety Requirement Production";
                tempString += QObject::tr(" is availble!");
                QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
            }
        }
    }
}

void SafetyReqProd::deleteRecord()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "srp");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"srp",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM srp WHERE srpid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }

    settings.remove("Table");
    settings.remove("Index");
}

void SafetyReqProd::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void SafetyReqProd::addRecordOfTable()
{
    if(checkingFill()){
        ViewListTable viewListTable("","legalacts",this);
        viewListTable.exec();
        QString transportIndex = viewListTable.returnValue();

        QSqlQuery queryTransport;
        queryTransport.prepare("SELECT legalactsname FROM legalacts WHERE legalactsid = :id");
        queryTransport.bindValue(":id",transportIndex);
        queryTransport.exec();
        while(queryTransport.next()){

            int row = tableWidget->rowCount();
            bool insert = true;
            if (row != 0){
                for(int kk = 0; kk < row; ++kk){
                    QString yy = tableWidget->item(kk,0)->text();
                    QString pp = queryTransport.value(0).toString();
                    if(yy == pp){
                        QString tempString = queryTransport.value(0).toString();
                        tempString += QObject::tr(" is availble!");
                        QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
                        insert = false;
                        break;
                    }
                }
            }
            if(insert){
                addIntoTable = true;
                int rowCount = tableWidget->rowCount();
                tableWidget->insertRow(rowCount);
                QTableWidgetItem *itemName = new QTableWidgetItem;
                tableWidget->setItem(rowCount,0,itemName);
                tableWidget->item(rowCount,0)->setText(queryTransport.value(0).toString());
                //for(int ii = 0; ii < tableWidget->rowCount(); ++ii){
                    NumPrefix numPrefix;
                    QString indexCost = numPrefix.getPrefix("srptable");
                    if(indexCost == ""){
                        close();
                    }
                    //Проверка на существование записи
                    ///QSqlQuery queryId;
                    ///queryId.prepare("SELECT legalactsid FROM legalacts WHERE legalactsname = :name");
                    //queryId.bindValue(":name",tableWidget->item(ii,0)->text());
                    //queryId.exec();
                    //queryId.next();
                    QSqlQuery queryCT;
                    queryCT.prepare("INSERT INTO srptable ("
                                    "srptableid, srpid, legalactsid"
                                    ") VALUES(:idcosttable, :idcost, :id)");
                    queryCT.bindValue(":idcosttable",indexCost);
                    queryCT.bindValue(":idcost",indexTemp);
                    queryCT.bindValue(":id",transportIndex);
                    queryCT.exec();
                    if(!queryCT.isActive()){
                        QMessageBox::warning(this,QObject::tr("Safe Requipment Table, INSERT ERROR!"),queryCT.lastError().text());
                    }
                //}


            }
        }
    }
}

void SafetyReqProd::deleteRecordOfTable()
{
    int k = QMessageBox::question(this,tr("Attention!!"),
                              tr("Really delete?"),
                                  QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(k == QMessageBox::Yes){

        QSqlQuery queryIn;
        queryIn.prepare("DELETE FROM srptable WHERE srpid = :cost");
        queryIn.bindValue(":cost",indexTemp);
        queryIn.exec();

        if(!queryIn.isActive()){
            QMessageBox::warning(this,QObject::tr("Amortization Cost Table, DELETE ERROR!"),queryIn.lastError().text());
            return;
        }
        tableWidget->removeRow(tableWidget->currentRow());
        tableWidget->repaint();
    }
}

void SafetyReqProd::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("SRP").toByteArray());
}

void SafetyReqProd::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("SRP", saveGeometry());
}

void SafetyReqProd::saveRecord()
{
    editRecord();
    emit accept();
}

void SafetyReqProd::cancelRecord()
{
    if(addIntoTable){
        //Удаление табличной части документа

        QSqlQuery queryCT;
        queryCT.prepare("DELETE FROM srptable WHERE srpid = :idcost");
        queryCT.bindValue(":idcost",indexTemp);
        queryCT.exec();
        if(!queryCT.isActive()){
            QMessageBox::warning(this,QObject::tr("Amortization Cost Table, INSERT ERROR!"),queryCT.lastError().text());
        }
    }
    emit accept();
}

void SafetyReqProd::addSecurityRecord()
{
    Security openForm("",this,false);
    openForm.exec();
    idSecurity = openForm.returnValue();
    if(idSecurity != ""){
        QSqlQuery query;
        query.prepare("SELECT securityname From security WHERE securityid = :id");
        query.bindValue(":id",idSecurity);
        query.exec();
        query.next();
        editSecurity->setText(query.value(0).toString());
    }
}

void SafetyReqProd::seeSecurityRecord()
{
    QSqlQuery query;
    query.prepare("SELECT securityid FROM security WHERE securityname = :name");
    query.bindValue(":name",editSecurity->text());
    query.exec();
    while(query.next()){
        Security openForm(query.value(0).toString(),this,true);
        openForm.exec();
    }
}

void SafetyReqProd::listSecurityRecord()
{
    QSqlQuery query;
    query.prepare("SELECT securityid FROM security WHERE securityname = :name");
    query.bindValue(":name",editSecurity->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"security",this);
    listTemp.exec();
    idSecurity = listTemp.returnValue();
    if(idSecurity != ""){
        QSqlQuery query;
        query.prepare("SELECT securityname FROM security WHERE securityid = :id");
        query.bindValue(":id",idSecurity);
        query.exec();
        query.next();
        editSecurity->setText(query.value(0).toString());
    }
}

void SafetyReqProd::addSafetyReqRecord()
{
    SafetyRequirements openForm("",this,false);
    openForm.exec();
    idSafetyReq = openForm.returnValue();
    if(idSafetyReq != ""){
        QSqlQuery query;
        query.prepare("SELECT safetyreqname From safetyreq WHERE safetyreqid = :id");
        query.bindValue(":id",idSafetyReq);
        query.exec();
        query.next();
        editSafetyReq->setText(query.value(0).toString());
    }
}

void SafetyReqProd::seeSafetyReqRecord()
{
    QSqlQuery query;
    query.prepare("SELECT safetyreqid FROM safetyreq WHERE safetyreqname = :name");
    query.bindValue(":name",editSafetyReq->text());
    query.exec();
    while(query.next()){
        SafetyRequirements openForm(query.value(0).toString(),this,true);
        openForm.exec();
    }
}

void SafetyReqProd::listSafetyReqRecord()
{
    QSqlQuery query;
    query.prepare("SELECT safetyreqid FROM safetyreq WHERE safetyreqname = :name");
    query.bindValue(":name",editSafetyReq->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"safetyreq",this);
    listTemp.exec();
    idSafetyReq = listTemp.returnValue();
    if(idSafetyReq != ""){
        QSqlQuery query;
        query.prepare("SELECT safetyreqname FROM safetyreq WHERE safetyreqid = :id");
        query.bindValue(":id",idSafetyReq);
        query.exec();
        query.next();
        editSafetyReq->setText(query.value(0).toString());
    }
}

void SafetyReqProd::addTestTaskRecord()
{
    TestTask openForm("",this,false);
    openForm.exec();
    idTestTask = openForm.returnValue();
    if(idTestTask != ""){
        QSqlQuery query;
        query.prepare("SELECT testtaskname, cipher FROM testtask WHERE testtaskid = :id");
        query.bindValue(":id",idTestTask);
        query.exec();
        query.next();
        editTestTask->setText(query.value(0).toString());
        editCiphr->setText(query.value(1).toString());
    }
}

void SafetyReqProd::seeTestTaskRecord()
{
    QSqlQuery query;
    query.prepare("SELECT testtaskid FROM testtask WHERE testtaskname = :name");
    query.bindValue(":name",editTestTask->text());
    query.exec();
    while(query.next()){
        TestTask openForm(query.value(0).toString(),this,true);
        openForm.exec();
    }
}

void SafetyReqProd::listTestTaskRecord()
{
    QSqlQuery query;
    query.prepare("SELECT testtaskid FROM testtask WHERE testtaskname = :name");
    query.bindValue(":name",editTestTask->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"testtask",this);
    listTemp.exec();
    idTestTask = listTemp.returnValue();
    if(idTestTask != ""){
        QSqlQuery query;
        query.prepare("SELECT testtaskname, cipher FROM testtask WHERE testtaskid = :id");
        query.bindValue(":id",idTestTask);
        query.exec();
        query.next();
        editTestTask->setText(query.value(0).toString());
        editCiphr->setText(query.value(1).toString());
    }
}

void SafetyReqProd::seeRecordOfTable()
{
    int i = tableWidget->currentRow();
    QSqlQuery query;
    query.prepare("SELECT legalactsid FROM legalacts WHERE legalactsname = :name;");
    query.bindValue(":name",tableWidget->item(i,0)->text());
    query.exec();
    query.next();
    LegalAct legalAct(query.value(0).toString(),this,false);
    legalAct.exec();
}

bool SafetyReqProd::checkingFill()
{
    bool fill = true;
    if(editSecurity->text() == ""){
        editSecurity->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editSecurity->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editSafetyReq->text() == ""){
        editSafetyReq->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editSafetyReq->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editTestTask->text() == ""){
        editTestTask->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editTestTask->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}
