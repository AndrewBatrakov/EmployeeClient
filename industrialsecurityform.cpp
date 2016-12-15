#include "industrialsecurityform.h"
#include <QtSql>
#include "numprefix.h"
#include "kommisiyaform.h"
#include "viewlisttable.h"
#include "laborprotprogform.h"
#include "fordelete.h"
#include "initials.h"

IndustrialSecurityForm::IndustrialSecurityForm(QString id, QString idEmp, QWidget *parent, bool onlyForRead) :
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
    idEmployee = idEmp;

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
    empCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editEmployee->setCompleter(empCompleter);


    QToolButton *addEmpButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addEmpButton->setIcon(addPix);
    addEmpButton->setToolTip(trUtf8("Добавить новую запись"));
    addEmpButton->setStyleSheet(toolButtonStyle);
    connect(addEmpButton,SIGNAL(clicked()),this,SLOT(addEmpRecord()));

    QToolButton *seeEmpButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeEmpButton->setIcon(seePix);
    seeEmpButton->setToolTip(trUtf8("Смотреть выбранную запись"));
    seeEmpButton->setStyleSheet(toolButtonStyle);
    connect(seeEmpButton,SIGNAL(clicked()),this,SLOT(seeEmpRecord()));

    QToolButton *listEmpButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listEmpButton->setIcon(listPix);
    listEmpButton->setToolTip(trUtf8("Смотреть список записей"));
    listEmpButton->setStyleSheet(toolButtonStyle);
    connect(listEmpButton,SIGNAL(clicked()),this,SLOT(listEmpRecord()));

    QHBoxLayout *employeeLayout = new QHBoxLayout;
    employeeLayout->addWidget(editEmployee);
    if(!onlyForRead){
        employeeLayout->addWidget(addEmpButton);
        employeeLayout->addWidget(seeEmpButton);
        employeeLayout->addWidget(listEmpButton);
    }

    labelSub = new QLabel(trUtf8("Подразделение:"));
    editSub = new LineEdit;
    editSub->setReadOnly(true);

    labelPost = new QLabel(trUtf8("Должность:"));
    editPost = new LineEdit;
    editPost->setReadOnly(true);

    labelProgObuch = new QLabel(trUtf8("Программа обучения:"));
    editProgObuch = new LineEdit;

    QSqlQueryModel *progModel = new QSqlQueryModel;
    progModel->setQuery("SELECT pbprogrammaname FROM pbprogramma");
    QCompleter *progCompleter = new QCompleter(progModel);
    progCompleter->setCompletionMode(QCompleter::PopupCompletion);
    progCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editProgObuch->setCompleter(progCompleter);

    QToolButton *addProgButton = new QToolButton;
    addProgButton->setIcon(addPix);
    addProgButton->setToolTip(trUtf8("Добавить новую запись"));
    addProgButton->setStyleSheet(toolButtonStyle);
    connect(addProgButton,SIGNAL(clicked()),this,SLOT(addProgRecord()));

    QToolButton *seeProgButton = new QToolButton;
    seeProgButton->setIcon(seePix);
    seeProgButton->setToolTip(trUtf8("Смотреть выбранную запись"));
    seeProgButton->setStyleSheet(toolButtonStyle);
    connect(seeProgButton,SIGNAL(clicked()),this,SLOT(seeProgRecord()));

    QToolButton *listProgButton = new QToolButton;
    listProgButton->setIcon(listPix);
    listProgButton->setToolTip(trUtf8("Смотреть список записей"));
    listProgButton->setStyleSheet(toolButtonStyle);
    connect(listProgButton,SIGNAL(clicked()),this,SLOT(listProgRecord()));

    QHBoxLayout *progLayout = new QHBoxLayout;
    progLayout->addWidget(editProgObuch);
    if(!onlyForRead){
        progLayout->addWidget(addProgButton);
        progLayout->addWidget(seeProgButton);
        progLayout->addWidget(listProgButton);
    }

    labelPrichina = new QLabel(trUtf8("Причина:"));
    editPrichina = new LineEdit;
    editPrichina->setReadOnly(onlyForRead);
    labelPrichina->setBuddy(editPrichina);

    QSqlQueryModel *prichModel = new QSqlQueryModel;
    prichModel->setQuery("SELECT prichinaobuchname FROM prichinaobuch");
    QCompleter *prichCompleter = new QCompleter(prichModel);
    prichCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    prichCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editPrichina->setCompleter(prichCompleter);

    labelEmp1 = new QLabel(trUtf8("Председатель:"));
    editEmp1 = new LineEdit;

    QSqlQueryModel *komModel = new QSqlQueryModel;
    komModel->setQuery("SELECT (SELECT employee.employeename FROM employee WHERE employee.employeeid = "
                       "komissiya.emponeid) FROM komissiya");
    QCompleter *komCompleter = new QCompleter(komModel);
    komCompleter->setCompletionMode(QCompleter::PopupCompletion);
    komCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editEmp1->setCompleter(komCompleter);

    QToolButton *addKomButton = new QToolButton;
    addKomButton->setIcon(addPix);
    addKomButton->setToolTip(trUtf8("Добавить новую запись"));
    addKomButton->setStyleSheet(toolButtonStyle);
    connect(addKomButton,SIGNAL(clicked()),this,SLOT(addKomRecord()));

    QToolButton *seeKomButton = new QToolButton;
    seeKomButton->setIcon(seePix);
    seeKomButton->setToolTip(trUtf8("Смотреть выбранную запись"));
    seeKomButton->setStyleSheet(toolButtonStyle);
    connect(seeKomButton,SIGNAL(clicked()),this,SLOT(seeKomRecord()));

    QToolButton *listKomButton = new QToolButton;
    listKomButton->setIcon(listPix);
    listKomButton->setToolTip(trUtf8("Смотреть список записей"));
    listKomButton->setStyleSheet(toolButtonStyle);
    connect(listKomButton,SIGNAL(clicked()),this,SLOT(listKomRecord()));

    QHBoxLayout *komLayout = new QHBoxLayout;
    komLayout->addWidget(labelEmp1);
    komLayout->addWidget(editEmp1);
    if(!onlyForRead){
        komLayout->addWidget(addKomButton);
        komLayout->addWidget(seeKomButton);
        komLayout->addWidget(listKomButton);
    }
    labelEmp2 = new QLabel(trUtf8("Член комиссии:"));
    editEmp2 = new LineEdit;
    QHBoxLayout *kom2Layout = new QHBoxLayout;
    kom2Layout->addWidget(labelEmp2);
    kom2Layout->addWidget(editEmp2);
    labelEmp3 = new QLabel(trUtf8("Член комиссии:"));
    editEmp3 = new LineEdit;
    QHBoxLayout *kom3Layout = new QHBoxLayout;
    kom3Layout->addWidget(labelEmp3);
    kom3Layout->addWidget(editEmp3);

    QVBoxLayout *kLayout = new QVBoxLayout;
    kLayout->addLayout(komLayout);
    kLayout->addLayout(kom2Layout);
    kLayout->addLayout(kom3Layout);
    QGroupBox *komGroup = new QGroupBox;
    komGroup->setTitle(trUtf8("Коммисия:"));
    komGroup->setLayout(kLayout);
    komGroup->setFlat(false);
    QFile fileGB(":/GroupBoxStyle.txt");
    fileGB.open(QFile::ReadOnly);
    QString groopBoxStyle = QLatin1String(fileGB.readAll());
    komGroup->setStyleSheet(groopBoxStyle);

    saveButton = new QPushButton(trUtf8("Записать"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(trUtf8("Отмена"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    QPushButton *printButton = new QPushButton(trUtf8("Печать"));
    connect(printButton,SIGNAL(clicked(bool)),this,SLOT(printTable()));

    QPushButton *printUdButton = new QPushButton(trUtf8("Печать Удостоверения"));
    connect(printUdButton,SIGNAL(clicked(bool)),this,SLOT(printUdTable()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(printUdButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(printButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    newRecord = false;
    if(indexTemp != ""){
        QSqlQuery queryLP;
        queryLP.prepare("SELECT "
                        "(SELECT employee.employeename FROM employee WHERE employee.employeeid = industrialsecurity.employeeid), "
                        "industrialsecuritydate, "
                        "(SELECT pbprogramma.pbprogrammaname FROM pbprogramma WHERE "
                        "pbprogramma.pbprogrammaid = industrialsecurity.pbprogrammaid), "
                        "industrialsecuritynumber, "
                        "komid, "
                        "(SELECT prichinaobuch.prichinaobuchname FROM prichinaobuch WHERE "
                        "prichinaobuch.prichinaobuchid = industrialsecurity.prichinaid) "
                        "FROM industrialsecurity WHERE industrialsecurityid = :id");
        queryLP.bindValue(":id",indexTemp);
        queryLP.exec();
        queryLP.next();
        QSqlQuery query;
        query.prepare("SELECT "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.emponeid), "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.emptwoid), "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.empthreeid) "
                      "FROM komissiya WHERE komissiya.komissiyaid = :id");
        query.bindValue(":id",queryLP.value(4).toString());
        query.exec();
        query.next();

        QSqlQuery queryS;
        queryS.prepare("SELECT "
                       "(SELECT subdivision.subdivisionname FROM subdivision WHERE subdivision.subdivisionid = employee.subdivisionid), "
                       "(SELECT post.postname FROM post WHERE post.postid = employee.postid), "
                       "employee.employeename FROM employee WHERE employee.employeeid = :employeeid");
        queryS.bindValue(":employeeid",idEmployee);
        queryS.exec();
        queryS.next();
        editSub->setText(queryS.value(0).toString());
        editPost->setText(queryS.value(1).toString());
        editProgObuch->setText(queryLP.value(2).toString());

        editNumber->setText(indexTemp);
        editDate->setDate(queryLP.value(1).toDate());
        editEmployee->setText(queryLP.value(0).toString());
        editEmp1->setText(query.value(0).toString());
        editEmp2->setText(query.value(1).toString());
        editEmp3->setText(query.value(2).toString());
        editPrichina->setText(queryLP.value(5).toString());
    }else{
        QSqlQuery query;
        query.prepare("SELECT "
                      "(SELECT subdivision.subdivisionname FROM subdivision WHERE subdivision.subdivisionid = employee.subdivisionid), "
                      "(SELECT post.postname FROM post WHERE post.postid = employee.postid), "
                      "employee.employeename FROM employee WHERE employee.employeeid = :employeeid");
        query.bindValue(":employeeid",idEmployee);
        query.exec();
        query.next();
        editSub->setText(query.value(0).toString());
        editPost->setText(query.value(1).toString());
        editEmployee->setText(query.value(2).toString());
        NumPrefix numPref;
        indexTemp = numPref.getPrefix("industrialsecurity");

        editNumber->setText(indexTemp);
        newRecord = true;
        editDate->setDate(QDate::currentDate());
        //        editStartDate->setDate(QDate::currentDate());
        //        editEndDate->setDate(QDate::currentDate());
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelNumber,0,0);
    mainLayout->addWidget(editNumber,0,1);
    mainLayout->addWidget(labelDate,1,0);
    mainLayout->addWidget(editDate,1,1);
    mainLayout->addWidget(labelEmployee,2,0);
    mainLayout->addLayout(employeeLayout,2,1);
    mainLayout->addWidget(labelSub,3,0);
    mainLayout->addWidget(editSub,3,1);
    mainLayout->addWidget(labelPost,4,0);
    mainLayout->addWidget(editPost,4,1);
    mainLayout->addWidget(labelProgObuch,5,0);
    mainLayout->addLayout(progLayout,5,1);
    mainLayout->addWidget(labelPrichina,6,0);
    mainLayout->addWidget(editPrichina,6,1);
    mainLayout->addWidget(komGroup,7,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,10,1);
    }
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Протокол Промбезопасности"));
    readSettings();
}

void IndustrialSecurityForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void IndustrialSecurityForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"industrialsecurity",this);
    forDelete.exec();
    forDelete.deleteOnDefault();
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM industrialsecurity WHERE industrialsecurityid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM industrialsecurity WHERE industrialsecurityid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void IndustrialSecurityForm::printTable()
{
    if(checkFill()){
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
}

void IndustrialSecurityForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(newRecord){
        QSqlQuery queryDoc;
        queryDoc.prepare("INSERT INTO industrialsecurity (industrialsecurityid, employeeid, industrialsecuritydate, "
                         "pbprogrammaid, industrialsecuritynumber, industrialsecurityudnumber, industrialsecurityuddate, "
                         "komid, prichinaid"
                         ") VALUES("
                         ":industrialsecurityid, :employeeid, :industrialsecuritydate, :pbprogrammaid, :industrialsecuritynumber, "
                         ":industrialsecurityudnumber, :industrialsecurityuddate, :komid, :prichinaid)");
        queryDoc.bindValue(":industrialsecurityid",indexTemp);
        queryDoc.bindValue(":employeeid",idEmployee);
        queryDoc.bindValue(":industrialsecuritydate",editDate->date());
        QSqlQuery queryProg;
        queryProg.prepare("SELECT pbprogrammaid FROM pbprogramma WHERE pbprogrammaname = :pbprogrammaname");
        queryProg.bindValue(":pbprogrammaname",editProgObuch->text());
        queryProg.exec();
        queryProg.next();
        queryDoc.bindValue(":pbprogrammaid",queryProg.value(0).toString());
        qDebug()<<queryProg.value(0).toString();
        queryDoc.bindValue(":industrialsecuritynumber",editNumber->text());
        queryDoc.bindValue(":industrialsecurityudnumber",indexTemp);
        queryDoc.bindValue(":industrialsecurityuddate",editDate->date());
        QSqlQuery queryKom;
        queryKom.prepare("SELECT komissiyaid FROM komissiya WHERE emponeid IN (SELECT employeeid "
                         "FROM employee WHERE employeename = :employeename)");
        queryKom.bindValue(":employeename",editEmp1->text());
        queryKom.exec();
        queryKom.next();
        queryDoc.bindValue(":komid",queryKom.value(0).toString());
        QSqlQuery queryP;
        queryP.prepare("SELECT prichinaobuchid FROM prichinaobuch WHERE prichinaobuchname = :prichinaobuchname");
        queryP.bindValue(":prichinaobuchname",editPrichina->text());
        queryP.exec();
        queryP.next();
        queryDoc.bindValue(":prichinaid",queryP.value(0).toString());
        queryDoc.exec();
        line += "INSERT INTO industrialsecurity (industrialsecurityid, employeeid, industrialsecuritydate, "
                "pbprogrammaid, industrialsecuritynumber, industrialsecurityudnumber, industrialsecurityuddate, "
                "komid, prichinaid) VALUES('";
        line += indexTemp.toUtf8();
        line += "', '";
        line += idEmployee.toUtf8();
        line += "', '";
        line += editDate->date().toString("yyyy-MM-dd");
        line += "', '";
        line += queryProg.value(0).toString().toUtf8();
        line += "', '";
        line += editNumber->text().toUtf8();
        line += "', '";
        line += indexTemp.toUtf8();
        line += "', '";
        line += editDate->date().toString("yyyy-MM-dd");
        line += "', '";
        line += queryKom.value(0).toString().toUtf8();
        line += "', '";
        line += queryP.value(0).toString().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery queryDoc;
        queryDoc.prepare("UPDATE industrialsecurity SET employeeid = :employeeid, "
                         "industrialsecuritydate = :industrialsecuritydate, "
                         "pbprogrammaid = :pbprogrammaid, "
                         "industrialsecuritynumber = :industrialsecuritynumber, "
                         "industrialsecurityudnumber = :industrialsecurityudnumber, "
                         "industrialsecurityuddate = :industrialsecurityuddate, "
                         "komid = :komid, prichinaid = :prichinaid WHERE laborprotectionid = :laborprotectionid");
        queryDoc.bindValue(":employeeid",idEmployee);
        queryDoc.bindValue(":industrialsecuritydate",editDate->date());
        QSqlQuery queryProg;
        queryProg.prepare("SELECT pbprogrammaid FROM pbprogramma WHERE pbprogrammaname = :pbprogrammaname");
        queryProg.bindValue(":pbprogrammaname",editProgObuch->text());
        queryProg.exec();
        queryProg.next();
        queryDoc.bindValue(":pbprogrammaid",queryProg.value(0).toString());
        queryDoc.bindValue(":industrialsecuritynumber",editNumber->text());
        queryDoc.bindValue(":industrialsecurityudnumber",editNumber->text());
        queryDoc.bindValue(":industrialsecurityuddate",editDate->date());
        QSqlQuery queryKom;
        queryKom.prepare("SELECT komissiyaid FROM komissiya WHERE emponeid IN (SELECT employeeid "
                         "FROM employee WHERE employeename = :employeename)");
        queryKom.bindValue(":employeename",editEmp1->text());
        queryKom.exec();
        queryKom.next();
        queryDoc.bindValue(":komid",queryKom.value(0).toString());
        QSqlQuery queryP;
        queryP.prepare("SELECT prichinaobuchid FROM prichinaobuch WHERE prichinaobuchname = :prichinaobuchname");
        queryP.bindValue(":prichinaobuchname",editPrichina->text());
        queryP.exec();
        queryP.next();
        queryDoc.bindValue(":prichinaid",queryP.value(0).toString());
        queryDoc.bindValue(":industrialsecurityid",indexTemp);
        queryDoc.exec();
        line += "UPDATE industrialsecurity SET employeeid = '";
        line += idEmployee.toUtf8();
        line += "', industrialsecuritydate = '";
        line += editDate->date().toString("yyyy-MM-dd");
        line += "', pbprogrammaid = '";
        line += queryProg.value(0).toString().toUtf8();
        line += "', industrialsecuritynumber = '";
        line += editNumber->text().toUtf8();
        line += "', industrialsecurityudnumber = '";
        line += indexTemp.toUtf8();
        line += "', industrialsecurityuddate = '";
        line += editDate->date().toString("yyyy-MM-dd");
        line += "', komid = '";
        line += queryKom.value(0).toString().toUtf8();
        line += "', prichinaid = '";
        line += queryP.value(0).toString().toUtf8();
        line += "' WHERE industrialsecurityid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }
    emit accept();
}

void IndustrialSecurityForm::addProgRecord()
{
    LaborProtProgForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT pbprogrammaname FROM pbprogramma WHERE pbprogrammaid = :id");
        query.bindValue(":id",orgForm.returnValue());
        query.exec();
        query.next();
        editProgObuch->setText(query.value(0).toString());
    }
}

void IndustrialSecurityForm::seeProgRecord()
{
    QSqlQuery query;
    query.prepare("SELECT pbprogrammaid FROM pbprogramma WHERE pbprogrammaname = :name");
    query.bindValue(":name",editProgObuch->text());
    query.exec();
    while(query.next()){
        LaborProtProgForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void IndustrialSecurityForm::listProgRecord()
{
    QSqlQuery query;
    query.prepare("SELECT pbprogrammaid FROM pbprogramma WHERE pbprogrammaname = :name)");
    query.bindValue(":name",editProgObuch->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"pbprogramma",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT pbprogrammaname FROM pbprogramma WHERE pbprogrammaid = :id");
        query.bindValue(":id",listTemp.returnValue());
        query.exec();
        query.next();
        editProgObuch->setText(query.value(0).toString());
    }
}

void IndustrialSecurityForm::addKomRecord()
{
    KommisiyaForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT employeename FROM komissiya WHERE employeeid = :id");
        query.bindValue(":id",orgForm.returnValue());
        query.exec();
        query.next();
        editEmp1->setText(query.value(0).toString());
    }
}

void IndustrialSecurityForm::seeKomRecord()
{
    QSqlQuery query;
    query.prepare("SELECT komissiyaid FROM komissiya WHERE emponeid = "
                  "(SELECT employeeid FROM employee WHERE employeename = :name)");
    query.bindValue(":name",editEmp1->text());
    query.exec();
    while(query.next()){
        KommisiyaForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void IndustrialSecurityForm::listKomRecord()
{
    QSqlQuery query;
    query.prepare("SELECT komissiyaid FROM komissiya WHERE emponeid = "
                  "(SELECT employeeid FROM employee WHERE employeename = :name)");
    query.bindValue(":name",editEmp1->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"komissiya",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.emponeid), "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.emptwoid), "
                      "(SELECT employee.employeename FROM employee WHERE employee.employeeid = komissiya.empthreeid) "
                      "FROM komissiya WHERE komissiya.komissiyaid = :id");
        query.bindValue(":id",listTemp.returnValue());
        query.exec();
        query.next();
        editEmp1->setText(query.value(0).toString());
        editEmp2->setText(query.value(1).toString());
        editEmp3->setText(query.value(2).toString());
    }
}

void IndustrialSecurityForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("IndustrialSecurityForm").toByteArray());
}

void IndustrialSecurityForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("IndustrialSecurityForm", saveGeometry());
}

int IndustrialSecurityForm::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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

bool IndustrialSecurityForm::checkFill()
{
    bool fill = true;
    if(editProgObuch->text() == ""){
        editProgObuch->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editProgObuch->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editPrichina->text() == ""){
        editPrichina->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editPrichina->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
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

void IndustrialSecurityForm::print(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    //int heightPage = viewRegion.height();
    //    //int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    QSqlQuery query;
    query.exec("SELECT nashafirmaid FROM nashafirma");
    query.next();
    QRect rect1(0,0,4600,200);
    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawText(rect1,Qt::AlignCenter,trUtf8("АТТЕСТАЦИОННАЯ КОМИССИЯ"));
    QRect rect2(0,200,4600,200);
    painter.drawText(rect2,Qt::AlignCenter,query.value(0).toString());
    painter.drawLine(200,400,4400,400);
    QRect rect3(200,400,4400,100);
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    painter.drawText(rect3,Qt::AlignHCenter | Qt::AlignTop,trUtf8("(наименование организации)"));
    QRect rect4(200,800,4400,200);
    QString val = trUtf8("Протокол № ");
    val += editNumber->text();
    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawText(rect4,Qt::AlignCenter,val);
    QRect rect5(200,1000,4400,200);
    painter.setFont(QFont("Times New Roman",12,QFont::Normal));
    painter.drawText(rect5,Qt::AlignLeft | Qt::AlignVCenter, editDate->date().toString("dd.MM.yyyy"));
    QRect rect6(200,1200,1600,200);
    painter.drawText(rect6,Qt::AlignLeft | Qt::AlignVCenter, trUtf8("Аттестационная комиссия в составе:"));
    QRect rect7(200,1400,1000,200);
    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawText(rect7,Qt::AlignLeft | Qt::AlignVCenter, trUtf8("Председателя:"));
    QRect rect8(1200,1400,3200,200);
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    QString val1 = editEmp1->text();
    val1 += ", ";
    QSqlQuery queryPost;
    queryPost.prepare("SELECT (SELECT post.postname FROM post WHERE post.postid = employee.postid) FROM employee "
                      "WHERE employee.employeename = :employeename");
    queryPost.bindValue(":employeename",editEmp1->text());
    queryPost.exec();
    queryPost.next();
    val1 += queryPost.value(0).toString();
    painter.drawText(rect8,Qt::AlignLeft | Qt::AlignVCenter, val1);
    painter.drawLine(1200,1600,4000,1600);
    QRect rect9(200,1600,1000,200);
    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawText(rect9,Qt::AlignLeft | Qt::AlignVCenter, trUtf8("Членов комиссии:"));
    QRect rect10(1200,1600,3200,200);
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    QString val2 = editEmp2->text();
    val2 += ", ";
    queryPost.clear();
    queryPost.prepare("SELECT (SELECT post.postname FROM post WHERE post.postid = employee.postid) FROM employee "
                      "WHERE employee.employeename = :employeename");
    queryPost.bindValue(":employeename",editEmp2->text());
    queryPost.exec();
    queryPost.next();
    val2 += queryPost.value(0).toString();
    painter.drawText(rect10,Qt::AlignLeft | Qt::AlignVCenter, val2);
    painter.drawLine(1200,1800,4000,1800);
    QRect rect11(1200,1800,3200,200);
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    QString val3 = editEmp3->text();
    val3 += ", ";
    queryPost.clear();
    queryPost.prepare("SELECT (SELECT post.postname FROM post WHERE post.postid = employee.postid) FROM employee "
                      "WHERE employee.employeename = :employeename");
    queryPost.bindValue(":employeename",editEmp3->text());
    queryPost.exec();
    queryPost.next();
    val3 += queryPost.value(0).toString();
    painter.drawText(rect11,Qt::AlignLeft | Qt::AlignVCenter, val3);
    painter.drawLine(1200,2000,4000,2000);

    QRect rect12(200,2000,4600,200);
    painter.setFont(QFont("Times New Roman",12,QFont::Normal));
    painter.drawText(rect12,Qt::AlignLeft | Qt::AlignVCenter, trUtf8("Проведена проверка знаний руководителей и специалистов по"
                                                                     "промышленной безопасности"));
    QRect rect13(200,2200,4600,200);
    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawText(rect13,Qt::AlignCenter, query.value(0).toString());
    painter.drawLine(200,2400,4600,2400);
    QRect rect14(200,2400,4600,200);
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    painter.drawText(rect14,Qt::AlignHCenter | Qt::AlignTop, trUtf8("(наименование организации)"));

    QRect rect15(200,2500,4600,200);
    painter.setFont(QFont("Times New Roman",12,QFont::Normal));
    painter.drawText(rect15,Qt::AlignLeft | Qt::AlignVCenter, trUtf8("в объеме, соответствующем должностным обязанностям."));

    int shapkaTop = 2600;
    int shapkaBottom = 400;
    QRect oneCol(250,shapkaTop,100,shapkaBottom);
    QRect twoCol(350,shapkaTop,1000,shapkaBottom);
    QRect threeCol(1350,shapkaTop,650,shapkaBottom);
    QRect fourCol(2000,shapkaTop,500,shapkaBottom);
    QRect fiveCol(2500,shapkaTop,500,shapkaBottom);
    QRect sixCol(3000,shapkaTop,500,shapkaBottom);
    QRect sevenCol(3500,shapkaTop,500,shapkaBottom);
    QRect eightCol(4000,shapkaTop,500,shapkaBottom);
    QRect bigOneCol(2500,shapkaTop,1600,100);
    QRect bigTwoCol(2500,shapkaTop,1600,100);

    int newLineHight;
    int mHight;
    //int spacing = 30;

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8));
    painter.drawText(oneCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("№\nп/п"));
    painter.drawRect(oneCol);
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.drawText(twoCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Фамилия Имя Отчество"));
    painter.drawRect(twoCol);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Должность"));
    painter.drawRect(threeCol);
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Причина проверки знаний"));
    painter.drawRect(fourCol);

    QRect rect100(2500,3000,2000,100);
    painter.drawText(rect100,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Результаты проверки знаний"));
    painter.drawRect(rect100);
    QRect rect101(2500,3100,2000,100);
    painter.drawText(rect101,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Область аттестации*"));
    painter.drawRect(rect101);
    QRect rect102(2500,3200,500,200);
    painter.drawText(rect102,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("A"));
    painter.drawRect(rect102);
    QRect rect103(3000,3200,500,200);
    painter.drawText(rect103,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Б"));
    painter.drawRect(rect103);
    QRect rect104(3500,3200,500,200);
    painter.drawText(rect104,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Г"));
    painter.drawRect(rect104);
    QRect rect105(4000,3200,500,200);
    painter.drawText(rect105,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("Д"));
    painter.drawRect(rect105);


    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8));
    painter.drawText(oneCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("1"));
    painter.drawRect(oneCol);
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.drawText(twoCol,Qt::AlignCenter | Qt::TextWordWrap,editEmployee->text());
    painter.drawRect(twoCol);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignCenter | Qt::TextWordWrap,editPost->text());
    painter.drawRect(threeCol);
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter | Qt::TextWordWrap,editPrichina->text());
    painter.drawRect(fourCol);
    fiveCol.moveTop(newLineHight);
    fiveCol.setHeight(mHight);
    painter.drawText(fiveCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("сдано\n1"));
    painter.drawRect(fiveCol);
    sixCol.moveTop(newLineHight);
    sixCol.setHeight(mHight);
    painter.drawText(sixCol,Qt::AlignCenter | Qt::TextWordWrap,editProgObuch->text());
    painter.drawRect(sixCol);
    sevenCol.moveTop(newLineHight);
    sevenCol.setHeight(mHight);
    painter.drawText(sevenCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("сдано\n1"));
    painter.drawRect(sevenCol);
    eightCol.moveTop(newLineHight);
    eightCol.setHeight(mHight);
//    painter.drawText(eightCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("сдано\n1"));
    painter.drawRect(eightCol);

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("2"));
    painter.drawRect(oneCol);
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.drawRect(twoCol);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawRect(threeCol);
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawRect(fourCol);
    fiveCol.moveTop(newLineHight);
    fiveCol.setHeight(mHight);
    painter.drawRect(fiveCol);
    sixCol.moveTop(newLineHight);
    sixCol.setHeight(mHight);
    painter.drawRect(sixCol);
    sevenCol.moveTop(newLineHight);
    sevenCol.setHeight(mHight);
    painter.drawRect(sevenCol);
    eightCol.moveTop(newLineHight);
    eightCol.setHeight(mHight);
    painter.drawRect(eightCol);

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("3"));
    painter.drawRect(oneCol);
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.drawRect(twoCol);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawRect(threeCol);
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawRect(fourCol);
    fiveCol.moveTop(newLineHight);
    fiveCol.setHeight(mHight);
    painter.drawRect(fiveCol);
    sixCol.moveTop(newLineHight);
    sixCol.setHeight(mHight);
    painter.drawRect(sixCol);
    sevenCol.moveTop(newLineHight);
    sevenCol.setHeight(mHight);
    painter.drawRect(sevenCol);
    eightCol.moveTop(newLineHight);
    eightCol.setHeight(mHight);
    painter.drawRect(eightCol);

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignCenter | Qt::TextWordWrap,trUtf8("4"));
    painter.drawRect(oneCol);
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.drawRect(twoCol);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawRect(threeCol);
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawRect(fourCol);
    fiveCol.moveTop(newLineHight);
    fiveCol.setHeight(mHight);
    painter.drawRect(fiveCol);
    sixCol.moveTop(newLineHight);
    sixCol.setHeight(mHight);
    painter.drawRect(sixCol);
    sevenCol.moveTop(newLineHight);
    sevenCol.setHeight(mHight);
    painter.drawRect(sevenCol);
    eightCol.moveTop(newLineHight);
    eightCol.setHeight(mHight);
    painter.drawRect(eightCol);

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    int ww = newLineHight + mHight;
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    painter.drawStaticText(500,ww,trUtf8("Председатель комиссии:"));
    painter.drawLine(1350,ww + 85,3500,ww + 85);
    Initials initials;
    painter.drawStaticText(3500,ww,initials.getInitials(editEmp1->text()));
    painter.drawStaticText(500,ww + mHight,trUtf8("Член комиссии:"));
    painter.drawLine(1350,ww + 85 + mHight,3500,ww + 85 + mHight);
    painter.drawStaticText(3500,ww + mHight,initials.getInitials(editEmp2->text()));
    painter.drawStaticText(500,ww + mHight * 2,trUtf8("Член комиссии:"));
    painter.drawLine(1350,ww + 85 + mHight * 2,3500,ww + 85 + mHight * 2);
    painter.drawStaticText(3500,ww + mHight * 2,initials.getInitials(editEmp3->text()));
}

void IndustrialSecurityForm::printUdTable()
{
    if(checkFill()){
        QPrinter printer(QPrinter::HighResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Portrait);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
        QPrintPreviewDialog preview(&printer,this);
        connect(&preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(printUd(QPrinter*)));
        preview.showMaximized();
        preview.exec();
    }
}

void IndustrialSecurityForm::printUd(QPrinter *printer)
{
    if(checkFill()){
        QPainter painter(printer);

        QRect rectUd(0,0,2160,100);
        painter.setFont(QFont("Times New Roman",7,QFont::Bold));
        QString rr = trUtf8("УДОСТОВЕРЕНИЕ № ");
        rr += editNumber->text();
        painter.drawText(rectUd,Qt::AlignCenter,rr);

        QRect rectStrA(2430,0,2160,100);
        painter.setFont(QFont("Times New Roman",7,QFont::Normal));
        painter.drawText(rectStrA,Qt::AlignCenter,trUtf8("СВЕДЕНИЯ"));
        QRect rectStrB(2430,100,2160,100);
        painter.drawText(rectStrB,Qt::AlignCenter,trUtf8("О ПОВТОРНЫХ ПРОВЕРКАХ ЗНАНИЙ ТРЕБОВАНИЙ ОХРАНЫ ТРУДА"));
        QRect rectStrC(0,200,270,100);
        painter.drawText(rectStrC,Qt::AlignRight | Qt::AlignVCenter,trUtf8("Выдано:"));
        QRect rectStrD(270,200,1890,100);
        painter.setFont(QFont("Times New Roman",9,QFont::Bold));
        painter.drawText(rectStrD,Qt::AlignCenter | Qt::TextWordWrap,editEmployee->text());
        painter.drawLine(270,300,2160,300);

        QRect rect1(270,300,1890,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect1,Qt::AlignTop | Qt::AlignHCenter,trUtf8("(Ф.И.О.)"));
        QRect rect2(2430,300,270,100);
        painter.setFont(QFont("Times New Roman",9,QFont::Normal));
        painter.drawText(rect2,Qt::AlignHCenter | Qt::AlignLeft,trUtf8("Ф.И.О."));
        painter.drawLine(2700,400,4860,400);
        QRect rect3(0,400,540,200);
        painter.drawText(rect3,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Место работы:"));
        QRect rect4(540,400,1620,200);
        painter.drawText(rect4,Qt::AlignCenter | Qt::TextWordWrap, editSub->text());
        painter.drawLine(540,600,2160,600);

        QRect rect5(2430,400,540,200);
        painter.drawText(rect5,Qt::AlignLeft | Qt::AlignVCenter, trUtf8("Место работы:"));
        painter.drawLine(2970,600,4860,600);

        QRect rect6(0,600,540,200);
        painter.drawText(rect6,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Должность:"));
        QRect rect7(540,600,1620,200);
        painter.setFont(QFont("Times New Roman",7,QFont::Normal));
        painter.drawText(rect7,Qt::AlignCenter | Qt::AlignVCenter| Qt::TextWordWrap, editPost->text());
        painter.drawLine(540,800,2160,800);
        QRect rect8(2430,600,540,200);
        painter.setFont(QFont("Times New Roman",9,QFont::Normal));
        painter.drawText(rect8,Qt::AlignLeft | Qt::AlignVCenter, trUtf8("Должность:"));
        painter.drawLine(2970,800,4860,800);

        QRect rect9(0,800,2160,250);
        painter.setFont(QFont("Times New Roman",7,QFont::Normal));
        painter.drawText(rect9,Qt::AlignJustify |
                         Qt::TextWordWrap,trUtf8("Проведена проверка знаний требований охраны "
                                                 "труда по программе производственного обучения, "
                                                 "инструктажа на рабочем месте и перечень вопросов "
                                                 "для проверки знаний и инструктажа по безопасным "
                                                 "методам и приемам труда для:"));
        QRect rect10(2430,800,2160,250);
        painter.drawText(rect10,Qt::AlignJustify |
                         Qt::TextWordWrap,trUtf8("Проведена проверка знаний требований охраны "
                                                 "труда по программе производственного обучения, "
                                                 "инструктажа на рабочем месте и перечень вопросов "
                                                 "для проверки знаний и инструктажа по безопасным "
                                                 "методам и приемам труда для:"));
        QRect rect11(0,1000,1350,150);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect11,Qt::AlignCenter | Qt::TextWordWrap, editProgObuch->text());
        painter.drawLine(0,1150,1350,1150);
        QRect rect12(1350,1050,270,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect12,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("в объёме"));
        QRect rect13(1600,1050,540,100);
        painter.drawText(rect13,Qt::AlignCenter,trUtf8("40"));
        painter.drawLine(1600,1150,2140,1150);
        QRect rect14(0,1150,1350,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect14,Qt::AlignTop | Qt::AlignHCenter,trUtf8("(наименование программы обучения по охране труда)"));
        QRect rect15(1600,1150,540,100);
        painter.drawText(rect15,Qt::AlignTop | Qt::AlignHCenter,trUtf8("(часов)"));

        painter.drawLine(2430,1150,3780,1150);
        QRect rect16(3780,1050,270,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect16,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("в объёме"));
        painter.drawLine(4050,1150,4590,1150);
        QRect rect17(2430,1150,1350,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect17,Qt::AlignTop | Qt::AlignHCenter,trUtf8("(наименование программы обучения по охране труда)"));
        QRect rect18(4050,1150,540,100);
        painter.drawText(rect18,Qt::AlignTop | Qt::AlignHCenter,trUtf8("(часов)"));

        QRect rect19(0,1250,270,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect19,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Протокол:"));
        QRect rect20(270,1250,540,100);
        painter.drawText(rect20,Qt::AlignCenter,editNumber->text());
        painter.drawLine(270,1350,810,1350);
        QRect rect21(810,1250,1350,100);
        painter.setFont(QFont("Times New Roman",7,QFont::Normal));
        painter.drawText(rect21,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("заседания комиссии по проверке знаний требований"));

        QRect rect22(2430,1250,270,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect22,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Протокол:"));
        painter.drawLine(2700,1350,3240,1350);
        QRect rect24(3240,1250,1350,100);
        painter.setFont(QFont("Times New Roman",7,QFont::Normal));
        painter.drawText(rect24,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("заседания комиссии по проверке знаний требований"));

        QRect rect25(0,1350,2160,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        QString qq = trUtf8("охраны труда работников ");
        QSqlQuery query;
        query.exec("SELECT * FROM nashafirma");
        query.next();
        qq += query.value(0).toString();
        painter.drawText(rect25,Qt::AlignCenter,qq);

        QRect rect26(2430,1350,2160,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect26,Qt::AlignVCenter | Qt::AlignLeft,trUtf8("охраны труда работников "));
        painter.drawLine(0,1450,2160,1450);
        painter.drawLine(2430,1450,4590,1450);

        QRect rect27(0,1450,2160,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect27,Qt::AlignHCenter | Qt::AlignTop,trUtf8("(наименование организации)"));

        QRect rect28(2430,1450,2160,100);
        painter.drawText(rect28,Qt::AlignHCenter | Qt::AlignTop,trUtf8("(наименование организации)"));

        QRect rect29(0,1550,1080,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect29,Qt::AlignCenter,editDate->date().toString("dd.MM.yyyy"));

        QRect rect30(0,1650,810,100);
        painter.drawText(rect30,Qt::AlignCenter | Qt::AlignHCenter,trUtf8("Председатель комиссии:"));
        QRect rect31(810,1650,1350,100);
        painter.drawText(rect31,Qt::AlignRight | Qt::AlignVCenter,trUtf8("С.В.Малиновский"));
        painter.drawLine(810,1750,2160,1750);
        QRect rect32(810,1750,1350,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect32,Qt::AlignHCenter | Qt::AlignTop,trUtf8("(Ф.И.О., подпись)"));

        QRect rect33(2430,1650,810,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect33,Qt::AlignCenter | Qt::AlignHCenter,trUtf8("Председатель комиссии:"));
        painter.drawLine(3240,1750,4590,1750);
        QRect rect35(3240,1750,1350,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect35,Qt::AlignHCenter | Qt::AlignTop,trUtf8("(Ф.И.О., подпись)"));

        QRect rect36(0,1750,810,100);
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        painter.drawText(rect36,Qt::AlignCenter,editDate->date().toString("dd.MM.yyyy"));

        QRect rect37(0,1850,810,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect37,Qt::AlignHCenter | Qt::AlignTop,trUtf8("Дата"));
        painter.drawLine(0,1850,810,1850);

        QRect rect38(1620,1850,270,100);
        painter.drawText(rect38,Qt::AlignHCenter | Qt::AlignLeft,trUtf8("М.П."));

        QRect rect39(2430,1850,810,100);
        painter.setFont(QFont("Times New Roman",5,QFont::Normal));
        painter.drawText(rect39,Qt::AlignHCenter | Qt::AlignTop,trUtf8("Дата"));
        painter.drawLine(2430,1850,3540,1850);

        QRect rect40(4050,1850,270,100);
        painter.drawText(rect40,Qt::AlignHCenter | Qt::AlignLeft,trUtf8("М.П."));
        painter.drawLine(0,1950,4590,1950);
    }
}
