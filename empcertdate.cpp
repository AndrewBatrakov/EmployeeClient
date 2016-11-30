#include "empcertdate.h"
#include "viewlisttable.h"
#include "numprefix.h"
#include <QtSql>

EmpCertDate::EmpCertDate(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString toolButtonStyle = QLatin1String(file.readAll());

    labelFIO = new QLabel(tr("FIO:"));
    editFIO = new LineEdit;
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    employeeId = settings.value("Employeeid").toString();
    empcertId = settings.value("Empcertid").toString();
    QSqlQuery query;
    query.prepare("SELECT employeename FROM employee WHERE employeeid = :id;");
    query.bindValue(":id",employeeId);
    query.exec();
    query.next();
    editFIO->setText(query.value(0).toString());
    editFIO->setReadOnly(true);

    labelProtocolDate = new QLabel(tr("Date:"));
    editProtocolDate = new QDateEdit;
    editProtocolDate->setCalendarPopup(true);
    labelProtocolDate->setBuddy(editProtocolDate);
    editFIO->setFocus();

    labelProtocolNumber = new QLabel(tr("Number:"));
    editProtocolNumber = new LineEdit;
    editProtocolNumber->setReadOnly(onlyForRead);
    labelProtocolNumber->setBuddy(editProtocolNumber);

    labelCipher = new QLabel(tr("Cipher:"));
    editCipher = new LineEdit;
    editCipher->setReadOnly(onlyForRead);
    labelCipher->setBuddy(editCipher);

    QToolButton *seeCipherButton = new QToolButton;
    QPixmap seePix(":/list.png");
    seeCipherButton->setIcon(seePix);
    seeCipherButton->setToolTip(tr("See list of item"));
    seeCipherButton->setStyleSheet(toolButtonStyle);
    connect(seeCipherButton,SIGNAL(clicked()),this,SLOT(listTestTask()));

    labelName = new QLabel(tr("Name:"));
    editName = new QLabel;
    editName->textFormat();
    editName->setWordWrap(true);
    labelName->setBuddy(editName);

    savePushButton = new QPushButton(tr("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(tr("Save And Close Button"));

    cancelPushButton = new QPushButton(tr("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM empcertdate WHERE empcertdateid = :id;");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editProtocolDate->setDate(query.value(1).toDate());
        editProtocolNumber->setText(query.value(2).toString());
        QSqlQuery queryTT;
        queryTT.prepare("SELECT * FROM testtask WHERE testtaskid = :id;");
        testtaskId = query.value(3).toString();
        queryTT.bindValue(":id",query.value(3).toString());
        queryTT.exec();
        queryTT.next();
        editCipher->setText(queryTT.value(1).toString());
        editName->setText(queryTT.value(2).toString());
    }else{

    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(editProtocolDate,1,1);
    mainLayout->addWidget(labelFIO,0,0);
    mainLayout->addWidget(editFIO,0,1);
    mainLayout->addWidget(labelProtocolDate,1,0);

    mainLayout->addWidget(labelProtocolNumber,2,0);
    mainLayout->addWidget(editProtocolNumber,2,1);
    mainLayout->addWidget(labelCipher,3,0);
    mainLayout->addWidget(editCipher,3,1);
    mainLayout->addWidget(seeCipherButton,3,2);
    mainLayout->addWidget(labelName,4,0);
    mainLayout->addWidget(editName,4,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,5,0,1,3);
    }


    //editProtocolNumber->setFocus(Qt::ActiveWindowFocusReason);

    //QWidget::setTabOrder(editProtocolDate,editProtocolNumber);
    //this->setTabOrder(editProtocolNumber,seeCipherButton);
    //QWidget::setTabOrder(seeCipherButton,savePushButton);
    //QWidget::setTabOrder(savePushButton,editProtocolDate);

    setLayout(mainLayout);

    setWindowTitle(tr("Employee Certification Date, Number"));
    resize(300,200);
}

void EmpCertDate::deleteRecord()
{

}

void EmpCertDate::editRecord()
{
    //if(empcertId != "" && employeeId != ""){
    //    if(testtaskId != ""){
            if(indexTemp != ""){
                QSqlQuery query;
                query.prepare("UPDATE empcertdate SET "
                              "protocoldate = :protocoldate, protocolnumber = :protocolnumber, "
                              "testtaskid = :testtaskid, employeeid = :employeeid "
                              "WHERE empcertdateid = :empcertdateid;");
                query.bindValue(":empcertdateid",indexTemp);
                query.bindValue(":protocoldate",editProtocolDate->date());
                query.bindValue(":protocolnumber",editProtocolNumber->text());
                query.bindValue(":testtaskid",testtaskId);
                query.bindValue(":employeeid",employeeId);
                query.exec();
                query.next();
                if(!query.isActive()){
                    QMessageBox::warning(this,QObject::tr("empcertdate, UPDATE ERROR!"),query.lastError().text());
                }
            }else{
                QSqlQuery query;
                query.prepare("SELECT * FROM empcertdate WHERE empcertdateid = :id");
                query.bindValue(":id",indexTemp);
                query.exec();
                query.next();
                if(!query.isValid()){
                    NumPrefix numPrefix;
                    indexTemp = numPrefix.getPrefix("empcertdate");
                    if(indexTemp == ""){
                        close();
                    }
                    QSqlQuery queryInsert;
                    queryInsert.prepare("INSERT INTO empcertdate ("
                                        "empcertdateid, "
                                        "protocoldate, "
                                        "protocolnumber, "
                                        "testtaskid, "
                                        "employeeid"
                                        ") VALUES (:id, "
                                        ":protocoldate, "
                                        ":protocolnumber, "
                                        ":testtaskid, "
                                        ":employeeid"
                                        ");");
                    queryInsert.bindValue(":id",indexTemp);
                    //query.bindValue("empcertid",empcertId);
                    queryInsert.bindValue(":protocoldate",editProtocolDate->date());
                    queryInsert.bindValue(":protocolnumber",editProtocolNumber->text());
                    queryInsert.bindValue(":testtaskid",testtaskId);
                    queryInsert.bindValue(":employeeid",employeeId);
                    queryInsert.exec();
                    if(!queryInsert.isActive()){
                        QMessageBox::warning(this,QObject::tr("empcertdate, INSERT ERROR!"),queryInsert.lastError().text());
                    }
                }
            }
        //}else{
        //    QMessageBox::warning(this,QObject::tr("Attention!!!"),QObject::tr("Select the Cipher\n of test task!!!"));
        //}
    //}
    emit accept();
    close();
}

void EmpCertDate::done(int result)
{
    QDialog::done(result);
}

void EmpCertDate::listTestTask()
{
    ViewListTable testTask("","testtask",this);
    testTask.exec();
    testtaskId = testTask.returnValue();
    QSqlQuery query;
    query.prepare("SELECT cipher, testtaskname FROM testtask WHERE testtaskid = :id;");
    query.bindValue(":id",testtaskId);
    query.exec();
    query.next();
    editCipher->setText(query.value(0).toString());
    editName->setText(query.value(1).toString());
}

bool EmpCertDate::checkingFill()
{
    bool fill = true;
    if(editCipher->text() == ""){
        editCipher->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editCipher->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editFIO->text() == ""){
        editFIO->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editFIO->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}
