#include <QtWidgets>
#include <QtSql>

#include "viewlisttable.h"
#include "organization.h"
#include "subdivision.h"
#include "legalact.h"
#include "safetyrequirements.h"
#include "testtask.h"
#include "searchform.h"
#include "empcertdate.h"
#include "postform.h"
#include "harform.h"
#include "sizpostform.h"

ViewListTable::ViewListTable(QString idTable, QString nameTable, QWidget *parent) :
    QDialog(parent)
{
    tableName = nameTable;
    iDTemp = idTable;
    labelName = "";

    stBar = new QStatusBar;
    addButton = new QPushButton(trUtf8("Добавить"));
    QPixmap pixAdd(":/add.png");
    addButton->setIcon(pixAdd);
    connect(addButton,SIGNAL(clicked()),this,SLOT(addRecord()));

    deleteButton = new QPushButton(trUtf8("Удалить"));
    QPixmap pixDel(":/delete.png");
    deleteButton->setIcon(pixDel);
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteRecord()));

    editButton = new QPushButton(trUtf8("Редактировать"));
    QPixmap pixEdit(":/edit.png");
    editButton->setIcon(pixEdit);
    connect(editButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    searchButton = new QPushButton(trUtf8("Поиск (по наименованию)"));
    connect(searchButton,SIGNAL(clicked()),this,SLOT(searchProcedure()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addStretch();

    tableView = new QTableView;
    tableView->clearSpans();

    templateModel = new QSqlRelationalTableModel(this);

    viewTemplateTable(tableName);
    if(iDTemp != ""){
        for(int row = 0; row  < templateModel->rowCount(); ++row){
            QSqlRecord record = templateModel->record(row);
            QModelIndex index = templateModel->index(row,1);
            if(record.value(0).toString() == iDTemp){
                tableView->setCurrentIndex(index);
                break;
            }
        }
    }else{
        tableView->setCurrentIndex(templateModel->index(0,0));
    }

    //tableView->resizeColumnsToContents();
    tableView->setAlternatingRowColors(true);
    QHeaderView *header = tableView->horizontalHeader();
    header->setStretchLastSection(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(tableView);
    setLayout(mainLayout);

    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));

    setWindowTitle(tr("List of %1").arg(labelName));
    resize(tableView->size().width()+200,tableView->size().height());

    createContextMenu();
    readSettings();

    searchString = new QLabel(this);
    int wL = this->width() - 204;
    searchString->setGeometry(wL,4,200,30);
    searchString->setMargin(4);
    searchString->setStyleSheet("QLabel{font: bold; background-color : #FFFF99; color : blue; }" );
    searchString->setVisible(false);
}

void ViewListTable::viewTemplateTable(QString)
{
    templateModel->setTable(tableName);

    if(tableName == "organization"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        labelName = trUtf8("Организации");
        if(setFilter){
            templateModel->setFilter(QString("organizationname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "subdivision"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        templateModel->setRelation(2,QSqlRelation("organization","organizationid","organizationname"));
        templateModel->setHeaderData(2,Qt::Horizontal,tr("Organization Name"));
        labelName = trUtf8("ПОдразделения");
        if(setFilter){
            templateModel->setFilter(QString("subdivisionname LIKE '%%1%'").arg(filterTable));
        }
    }if(tableName == "empcert"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("FIO"));
        templateModel->setRelation(1,QSqlRelation("employee","employeeid","employeename"));
        labelName = "Employee Certification";
        if(setFilter){
            templateModel->setFilter(QString("employeename LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "testtask"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        labelName = "Test task";
        if(setFilter){
            templateModel->setFilter(QString("testtaskname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "safetyreq"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        tableView->setColumnHidden(0,true);
        labelName = "Safety Requipment";
        if(setFilter){
            templateModel->setFilter(QString("safetyreqname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "legalacts"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        tableView->setColumnHidden(0,true);
        labelName = "Legal Acts";
        if(setFilter){
            templateModel->setFilter(QString("legalactsname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "employee"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("FIO"));
        templateModel->setHeaderData(3,Qt::Horizontal,tr("Organization"));
        templateModel->setRelation(3,QSqlRelation("organization","organizationid","organizationname"));
        templateModel->setHeaderData(4,Qt::Horizontal,tr("Subdivision"));
        templateModel->setRelation(4,QSqlRelation("subdivision","subdivisionid","subdivisionname"));
        templateModel->setHeaderData(5,Qt::Horizontal,tr("Post"));
        templateModel->setRelation(5,QSqlRelation("post","postid","postname"));
        labelName = trUtf8("Сотрудники");
        if(setFilter){
            templateModel->setFilter(QString("employeenameupper LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "empcertdate"){
//        templateModel->setHeaderData(2,Qt::Horizontal,tr("Protokol date"));
//        templateModel->setHeaderData(3,Qt::Horizontal,tr("Protokol number"));
//        templateModel->setRelation(4,QSqlRelation("testtask","testtaskid","testtaskname"));
//        templateModel->setHeaderData(4,Qt::Horizontal,tr("Test task"));
//        templateModel->setHeaderData(5,Qt::Horizontal,tr("FIO"));
//        templateModel->setRelation(5,QSqlRelation("employee","employeeid","employeename"));
        labelName = "Employees Certification Date";
//        if(setFilter){
//            templateModel->setFilter(QString("employeename LIKE '%%1%'").arg(filterTable));
//        }
    }else if(tableName == "post"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("ИТР"));
        labelName = trUtf8("Должность");
        if(setFilter){
            templateModel->setFilter(QString("postuppername LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "har"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        labelName = trUtf8("Характеристики");
        if(setFilter){
            templateModel->setFilter(QString("postname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "postsiz"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Профессия СИЗ"));
        labelName = trUtf8("Профессия СИЗ - Профессия кадры");
        if(setFilter){
            templateModel->setFilter(QString("postsizname LIKE '%%1%'").arg(filterTable));
        }
    }

    templateModel->setSort(1,Qt::AscendingOrder);
    templateModel->select();

    tableView->setModel(templateModel);

    tableView->setColumnHidden(0,true);
    tableView->setColumnWidth(1,300);
    if(tableName == "employee"){
        tableView->setColumnHidden(2,true);
        tableView->setColumnHidden(3,true);
        //tableView->setColumnHidden(5,true);
        for(int i = 6; i < 18; ++i){
            tableView->setColumnHidden(i,true);
        }
        tableView->setColumnWidth(4,350);
        tableView->setColumnWidth(5,300);
    }else if(tableName == "empcertdate"){
        //tableView->setColumnHidden(1,true);
    }else if(tableName == "post"){
        tableView->setColumnHidden(2,true);
    }
    tableView->setItemDelegate(new QSqlRelationalDelegate(this));
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //tableView->setRowHeight();
}

void ViewListTable::done(int res)
{
    writeSettings();
    if(res == QDialog::Accepted){
        QModelIndex index = tableView->currentIndex();
        if(index.isValid()){
            QSqlRecord record = templateModel->record(index.row());
            QString per = tableName;
            per += "id";
            iDTemp = record.value(per).toString();
        }else{
            iDTemp = "";
        }
    }
    QDialog::done(res);
}

void ViewListTable::addRecord()
{
    QString nameList;
    if(tableName == "organization"){
        OrganizationForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "subdivision"){
        SubDivisionForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "legalacts"){
        LegalAct listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "safetyreq"){
        SafetyRequirements listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "testtask"){
        TestTask listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "empcertdate"){
        EmpCertDate listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "post"){
        PostForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "har"){
        HarForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "postsiz"){
        SizPostForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }
    templateModel->select();
    for(int row = 0; row < templateModel->rowCount(); ++row){
        QSqlRecord record = templateModel->record(row);
        QModelIndex index = templateModel->index(row,1);
        if(record.value(0).toString() == nameList){
            tableView->setCurrentIndex(index);
            updatePanel(index);
            break;
        }
    }
}

void ViewListTable::deleteRecord()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Attention!!!"));
    msgBox.setText(tr("Really delete?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(QMessageBox::Yes);
    QPushButton *noButton = msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(noButton);

    noButton->setStyleSheet("QPushButton:hover {color: red}");
    int k = msgBox.exec();
    if(k == QMessageBox::Yes){
        QModelIndex index = tableView->currentIndex();
        QSqlRecord record = templateModel->record(index.row());
        QString idList = record.value(0).toString();
        if(tableName == "organization"){
            OrganizationForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "subdivision"){
            SubDivisionForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "legalacts"){
            LegalAct listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "safetyreq"){
            SafetyRequirements listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "testtask"){
            TestTask listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "post"){
            PostForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "har"){
            HarForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "postsiz"){
            SizPostForm listForm(idList,this,false);
            listForm.deleteRecord();
        }
        updatePanel(index);
    }
}

void ViewListTable::editRecord()
{
    QModelIndex index = tableView->currentIndex();
    QSqlRecord record = templateModel->record(index.row());
    QString idList = record.value(0).toString();
    if(tableName == "organization"){
        OrganizationForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "subdivision"){
        SubDivisionForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "legalacts"){
        LegalAct listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "safetyreq"){
        SafetyRequirements listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "testtask"){
        TestTask listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "empcertdate"){
        EmpCertDate listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "post"){
        PostForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "har"){
        HarForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "postsiz"){
        SizPostForm listFrom(idList,this,false);
        listFrom.exec();
    }
    updatePanel(index);
}

void ViewListTable::updatePanel(QModelIndex index)
{
    templateModel->select();
    tableView->setCurrentIndex(index);
}

void ViewListTable::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecord()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecord()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecord()));

    tableView->addAction(addAction);
    tableView->addAction(deleteAction);
    tableView->addAction(editAction);
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void ViewListTable::searchProcedure()
{
    QString valueTempModel = templateModel->tableName();

    SearchForm searchForm(valueTempModel, this);
    searchForm.exec();
    filterTable = searchForm.returnValue();
    setFilter = true;

    viewTemplateTable(valueTempModel);
}

void ViewListTable::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void ViewListTable::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Geometry", saveGeometry());
}

void ViewListTable::keyReleaseEvent(QKeyEvent *event)
{
    QString tm = templateModel->tableName();
    if( event->key() == Qt::Key_Escape ){
        searchString->setText("");
        if(searchString->isVisible()){
            searchString->setVisible(false);
            //statusBar()->setVisible(false);
        }
        setFilter = false;
        viewTemplateTable(tm);
    }
    if((event->key() >= 0x0410 && event->key() < 0x044f) || event->key() == Qt::Key_Space || event->key() == Qt::Key_Backspace){
        if(!searchString->isVisible()){
//            statusBar()->addWidget(searchString);
//            statusBar()->show();
            searchString->show();
            searchString->setFocus();
        }
        QString txt = searchString->text();
        if(event->key() == Qt::Key_Backspace){
            txt.chop(1);
        }else{
            txt += event->text();
        }
        searchString->setText(txt);
        QString rr = searchString->text().toUpper();
        filterTable = rr;
        setFilter = true;
        viewTemplateTable(tm);
    }
}
