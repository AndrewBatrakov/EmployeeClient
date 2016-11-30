#include "protocollaborprotectionform.h"
#include <QtSql>
#include "numprefix.h"

ProtocolLaborprotectionForm::ProtocolLaborprotectionForm(QString id, QString idEmp, QWidget *parent, bool onlyForRead) :
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
    //empCompleter->setCurrentRow(0);
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

    labelSub = new QLabel(trUtf8("Подразделение:"));
    editSub = new LineEdit;
    editSub->setReadOnly(true);

    labelPost = new QLabel(trUtf8("Должность:"));
    editPost = new LineEdit;
    editPost->setReadOnly(true);

    labelProgObuch = new QLabel(trUtf8("Программа обучения:"));
    editProgObuch = new LineEdit;

    QSqlQueryModel *progModel = new QSqlQueryModel;
    progModel->setQuery("SELECT (SELECT employee.employeename FROM employee WHERE employee.employeeid = "
                       "komissiya.emponeid) FROM komissiya");
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

    labelKommis = new QLabel(trUtf8("Коммисия:"));
    editKommis = new LineEdit;

    QSqlQueryModel *komModel = new QSqlQueryModel;
    komModel->setQuery("SELECT (SELECT employee.employeename FROM employee WHERE employee.employeeid = "
                       "komissiya.emponeid) FROM komissiya");
    QCompleter *komCompleter = new QCompleter(komModel);
    komCompleter->setCompletionMode(QCompleter::PopupCompletion);
    komCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editKommis->setCompleter(komCompleter);

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
    komLayout->addWidget(editKommis);
    if(!onlyForRead){
        komLayout->addWidget(addKomButton);
        komLayout->addWidget(seeKomButton);
        komLayout->addWidget(listKomButton);
    }
    QGroupBox *komGroup = new QGroupBox;
    komGroup->setTitle(trUtf8("Коммисия:"));
    komGroup->setLayout(komLayout);

    saveButton = new QPushButton(trUtf8("Записать"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(trUtf8("Отмена"));
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
//        editStartDate->setDate(query.value(2).toDate());
//        editEndDate->setDate(query.value(3).toDate());
//        editDay->setText(query.value(4).toString());
//        editObyom->setText(query.value(5).toString());
//        editOtvetstv->setText(query.value(6).toString());
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
        indexTemp = numPref.getPrefix("stagirovka");

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
    //mainLayout->addWidget(labelKommis,6,0);
    mainLayout->addWidget(komGroup,6,1,1,2);
//    mainLayout->addWidget(labelOtvetstv,7,0);
//    mainLayout->addLayout(otvLayout,7,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,10,1);
    }
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Протокол Охраны Труда"));
    readSettings();
}

void ProtocolLaborprotectionForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void ProtocolLaborprotectionForm::deleteRecord()
{

}

void ProtocolLaborprotectionForm::printTable()
{

}

void ProtocolLaborprotectionForm::editRecord()
{

}

void ProtocolLaborprotectionForm::addEmpRecord()
{

}

void ProtocolLaborprotectionForm::seeEmpRecord()
{

}

void ProtocolLaborprotectionForm::listEmpRecord()
{

}

void ProtocolLaborprotectionForm::addProgRecord()
{

}

void ProtocolLaborprotectionForm::seeProgRecord()
{

}

void ProtocolLaborprotectionForm::listProgRecord()
{

}

void ProtocolLaborprotectionForm::addKomRecord()
{

}

void ProtocolLaborprotectionForm::seeKomRecord()
{

}

void ProtocolLaborprotectionForm::listKomRecord()
{

}

void ProtocolLaborprotectionForm::readSettings()
{

}

void ProtocolLaborprotectionForm::writeSettings()
{

}

void ProtocolLaborprotectionForm::print(QPrinter *)
{

}

int ProtocolLaborprotectionForm::printParagraph(QString, QPainter*, QRect*, int)
{

}
