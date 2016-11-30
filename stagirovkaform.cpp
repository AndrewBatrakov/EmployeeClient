#include <QtSql>
#include <QAxObject>
#include "stagirovkaform.h"
#include "employee.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"

StagirovkaForm::StagirovkaForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString toolButtonStyle = QLatin1String(file.readAll());

    indexTemp = id;

    labelNumber = new QLabel(trUtf8("Номер:"));
    editNumber = new LineEdit;
    editNumber->setReadOnly(true);
    labelNumber->setBuddy(editNumber);

    labelDate = new QLabel(trUtf8("Дата:"));
    editDate = new QDateEdit;
    editDate->setCalendarPopup(true);

    labelEmployee = new QLabel(trUtf8("Сотрудник:"));
    editEmployee = new LineEdit;
    editEmployee->setReadOnly(onlyForRead);
    labelEmployee->setBuddy(editEmployee);

    QSqlQueryModel *empModel = new QSqlQueryModel;
    empModel->setQuery("SELECT employeename FROM employee");
    QCompleter *empCompleter = new QCompleter(empModel);
    empCompleter->setCompletionMode(QCompleter::PopupCompletion);
    empCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    empCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editEmployee->setCompleter(empCompleter);

    QToolButton *addEmpButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addEmpButton->setIcon(addPix);
    addEmpButton->setToolTip(tr("Добавить новую запись"));
    addEmpButton->setStyleSheet(toolButtonStyle);
    connect(addEmpButton,SIGNAL(clicked()),this,SLOT(addEmpRecord()));

    QToolButton *seeEmpButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeEmpButton->setIcon(seePix);
    seeEmpButton->setToolTip(tr("Смотреть выбранную запись"));
    seeEmpButton->setStyleSheet(toolButtonStyle);
    connect(seeEmpButton,SIGNAL(clicked()),this,SLOT(seeEmpRecord()));

    QToolButton *listEmpButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listEmpButton->setIcon(listPix);
    listEmpButton->setToolTip(tr("Смотреть список записей"));
    listEmpButton->setStyleSheet(toolButtonStyle);
    connect(listEmpButton,SIGNAL(clicked()),this,SLOT(listEmpRecord()));

    QHBoxLayout *employeeLayout = new QHBoxLayout;
    employeeLayout->addWidget(editEmployee);
    if(!onlyForRead){
        employeeLayout->addWidget(addEmpButton);
        employeeLayout->addWidget(seeEmpButton);
        employeeLayout->addWidget(listEmpButton);
    }

    labelStartDate = new QLabel(trUtf8("Дата начала стажировки:"));
    editStartDate = new QDateEdit;
    editStartDate->setCalendarPopup(true);

    labelEndDate = new QLabel(trUtf8("Дата окончания стажировки:"));
    editEndDate = new QDateEdit;
    editEndDate->setCalendarPopup(true);

    labelDay = new QLabel(trUtf8("Дней стажировки:"));
    editDay = new LineEdit;
    editDay->setText("0");

    labelObyom = new QLabel(trUtf8("Объем часов:"));
    editObyom = new LineEdit;
    editObyom->setText("10");

    labelOtvetstv = new QLabel(trUtf8("Ответственный:"));
    editOtvetstv = new LineEdit;
    editOtvetstv->setReadOnly(onlyForRead);
    labelOtvetstv->setBuddy(editOtvetstv);

//    QSqlQueryModel *empModel = new QSqlQueryModel;
//    empModel->setQuery("SELECT employeename FROM employee");
    QCompleter *otvCompleter = new QCompleter(empModel);
    otvCompleter->setCompletionMode(QCompleter::PopupCompletion);
    otvCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    otvCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editOtvetstv->setCompleter(otvCompleter);

    QToolButton *addOtvButton = new QToolButton;
    addOtvButton->setIcon(addPix);
    addOtvButton->setToolTip(trUtf8("Добавить новую запись"));
    addOtvButton->setStyleSheet(toolButtonStyle);
    connect(addOtvButton,SIGNAL(clicked()),this,SLOT(addOtvRecord()));

    QToolButton *seeOtvButton = new QToolButton;
    seeOtvButton->setIcon(seePix);
    seeOtvButton->setToolTip(trUtf8("Смотреть выбранную запись"));
    seeOtvButton->setStyleSheet(toolButtonStyle);
    connect(seeOtvButton,SIGNAL(clicked()),this,SLOT(seeOtvRecord()));

    QToolButton *listOtvButton = new QToolButton;
    listOtvButton->setIcon(listPix);
    listOtvButton->setToolTip(trUtf8("Смотреть список записей"));
    listOtvButton->setStyleSheet(toolButtonStyle);
    connect(listOtvButton,SIGNAL(clicked()),this,SLOT(listOtvRecord()));

    QHBoxLayout *otvLayout = new QHBoxLayout;
    otvLayout->addWidget(editOtvetstv);
    if(!onlyForRead){
        otvLayout->addWidget(addOtvButton);
        otvLayout->addWidget(seeOtvButton);
        otvLayout->addWidget(listOtvButton);
    }

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    QPushButton *printButton = new QPushButton(trUtf8("Печать"));
    connect(printButton,SIGNAL(clicked(bool)),this,SLOT(printTable()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(printButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    newRecord = false;
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT stagirovka.docdate, (SELECT employee.employeename FROM employee "
                      "WHERE employee.employeeid = stagirovka.employeeid), stagirovka.startdate, "
                      "stagirovka.enddate, stagirovka.day, stagirovka.obyom, (SELECT employee.employeename "
                      "FROM employee WHERE employee.employeeid = stagirovka.otvestv) FROM stagirovka "
                      "WHERE stagirovka.stagirovkaid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editNumber->setText(indexTemp);
        editDate->setDate(query.value(0).toDate());
        editEmployee->setText(query.value(1).toString());
        editStartDate->setDate(query.value(2).toDate());
        editEndDate->setDate(query.value(3).toDate());
        editDay->setText(query.value(4).toString());
        editObyom->setText(query.value(5).toString());
        editOtvetstv->setText(query.value(6).toString());
    }else{
        NumPrefix numPref;
        indexTemp = numPref.getPrefix("stagirovka");

        editNumber->setText(indexTemp);
        newRecord = true;
        editDate->setDate(QDate::currentDate());
        editStartDate->setDate(QDate::currentDate());
        editEndDate->setDate(QDate::currentDate());
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelNumber,0,0);
    mainLayout->addWidget(editNumber,0,1);
    mainLayout->addWidget(labelDate,1,0);
    mainLayout->addWidget(editDate,1,1);
    mainLayout->addWidget(labelEmployee,2,0);
    mainLayout->addLayout(employeeLayout,2,1);
    mainLayout->addWidget(labelStartDate,3,0);
    mainLayout->addWidget(editStartDate,3,1);
    mainLayout->addWidget(labelEndDate,4,0);
    mainLayout->addWidget(editEndDate,4,1);
    mainLayout->addWidget(labelDay,5,0);
    mainLayout->addWidget(editDay,5,1);
    mainLayout->addWidget(labelObyom,6,0);
    mainLayout->addWidget(editObyom,6,1);
    mainLayout->addWidget(labelOtvetstv,7,0);
    mainLayout->addLayout(otvLayout,7,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,10,1);
    }
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Стажировка"));
    readSettings();
}

void StagirovkaForm::editRecord()
{
    QString index;
    if(newRecord){
        QSqlQuery query;
        query.prepare("INSERT INTO stagirovka (stagirovka.stagirovkaid, stagirovka.docdate, "
                      "stagirovka.employeeid, stagirovka.startdate, "
                      "stagirovka.enddate, stagirovka.day, stagirovka.obyom, stagirovka.otvestv) "
                      "VALUES(:stagirovkaid, :docdate, "
                      "(SELECT employeeid FROM employee WHERE employeename = :empname), "
                      ":startdate, :enddate, :day, :obyom, (SELECT employeeid FROM employee WHERE "
                      "employeename = :otvname))");
        query.bindValue(":subid",index);
        query.bindValue(":subname",editNumber->text());
        query.bindValue(":orgname",editEmployee->text());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("DataBase ERROR!"),query.lastError().text());
        }
    }else{
        index = indexTemp;
        QSqlQuery query;
        query.prepare("UPDATE subdivision SET subdivisionname = :subname, "
                      "organizationid = (SELECT organizationid FROM organization "
                      "WHERE organizationname = :orgname) WHERE subdivisionid = :subid");
        query.bindValue(":subid",index);
        query.bindValue(":subname",editNumber->text());
        query.bindValue(":orgname",editEmployee->text());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("DataBase ERROR!"),query.lastError().text());
        }
    }
    emit accept();
    close();
}

void StagirovkaForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"subdivision",this);
    //if(forDelete.result() == QDialog::Accepted){
        forDelete.exec();
        //int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
        //                     QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
        //if(k == QMessageBox::Yes){
            forDelete.deleteOnDefault();
            if(indexTemp != "OWN000000001"){
                QSqlQuery query;
                query.prepare("DELETE FROM subdivision WHERE subdivisionid = :id");
                query.bindValue(":id",indexTemp);
                query.exec();
                query.next();
            }else{
                QMessageBox::warning(this,QObject::tr("Attention"),QObject::tr("You dont may delete default value!"));
            }
        //}
   // }
}

void StagirovkaForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void StagirovkaForm::addEmpRecord()
{
    EmployeeForm openForm("",this,false);
    openForm.exec();
    if(openForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename FROM employee WHERE employeeid = :id");
        query.bindValue(":id",openForm.returnValue());
        query.exec();
        query.next();
        editEmployee->setText(query.value(0).toString());
    }
}

void StagirovkaForm::seeEmpRecord()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editEmployee->text());
    query.exec();
    while(query.next()){
        EmployeeForm openForm(query.value(0).toString(),this,true);
        openForm.exec();
    }
}

void StagirovkaForm::listEmpRecord()
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

void StagirovkaForm::addOtvRecord()
{
    EmployeeForm openForm("",this,false);
    openForm.exec();
    if(openForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename FROM employee WHERE employeeid = :id");
        query.bindValue(":id",openForm.returnValue());
        query.exec();
        query.next();
        editOtvetstv->setText(query.value(0).toString());
    }
}

void StagirovkaForm::seeOtvRecord()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editOtvetstv->text());
    query.exec();
    while(query.next()){
        EmployeeForm openForm(query.value(0).toString(),this,true);
        openForm.exec();
    }
}

void StagirovkaForm::listOtvRecord()
{
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee WHERE employeename = :name");
    query.bindValue(":name",editOtvetstv->text());
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
        editOtvetstv->setText(query.value(0).toString());
    }
}

void StagirovkaForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("Stagirovka").toByteArray());
}

void StagirovkaForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Stagirovka", saveGeometry());
}

void StagirovkaForm::printTable()
{
//    QAxObject *excel = new QAxObject("Excel.Application",this);
//    excel->dynamicCall("SetVisible(bool)",true);
//    QAxObject *workbooks = excel->querySubObject("Workbooks");

//    qDebug()<<QDir::currentPath();
//    QFile file("./Stagirovka.xlsx");
//    QFileInfo infoFile;
//    infoFile.setFile(file);

//    qDebug()<<file.fileName()<<" "<<file.exists();
//    QString nativeFilePath=QDir::toNativeSeparators(infoFile.absoluteFilePath());

//    //QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", nativeFilePath);

//    qDebug()<<nativeFilePath;
//    QAxObject* workbook=workbooks->querySubObject("Open(const QString&)", nativeFilePath);

//    if(QAxObject* sheet=workbook->querySubObject("Worksheets(const QVariant&)", 1))
//    {

//    }



    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    QPrintPreviewDialog preview(&printer,this);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(print(QPrinter*)));
    preview.showMaximized();
    preview.exec();
}

int StagirovkaForm::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
{
    QTextDocument document;
    document.setPlainText(paragraphString);
    int y = 0;
    QTextBlock block = document.begin();
    int x = 0;
    for(QTextBlock::iterator i = block.begin(); !i.atEnd(); ++i)
    {
        QTextFragment fragment = i.fragment();
        if(fragment.isValid()){
            foreach (QString word, fragment.text().split(QRegExp("\\s+"))) {
                int widthW = painter->fontMetrics().width(word);
                if(x + widthW + spacing*2 > rect->width()){
                    x = 0;
                    y += painter->fontMetrics().lineSpacing();
                }else if(x != rect->x()){
                    word.prepend(" ");
                }
                x += painter->fontMetrics().width(word);
            }
        }
    }
    return y += painter->fontMetrics().lineSpacing();
}

void StagirovkaForm::print(QPrinter *printer)
{
    QPainter painter(printer);

    //QRect viewRegion(painter.viewport());
    //int heightPage = viewRegion.height();
    //int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    painter.drawStaticText(3400,0,QObject::trUtf8("Приложение №2 к приказу"));
    //QRect secondString(0,180,widthPage,100);
    //painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawStaticText(3400,120,QObject::trUtf8("от \"___\" ________________"));
    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawStaticText(1800,300,QObject::trUtf8("РАСПОРЯЖЕНИЕ"));
    QString stringTemp = QObject::trUtf8("О ПРОХОЖДЕНИИ СТАЖИРОВКИ №");
    stringTemp += editNumber->text().toUtf8();
    painter.drawStaticText(1200,450,stringTemp);
    painter.setFont(QFont("Times New Roman",12,QFont::Normal));
    painter.drawStaticText(100,650,QObject::trUtf8("1. Рабочий"));

    painter.setPen(Qt::SolidLine);
    painter.drawLine(200,800,2000,800);
}
