#include "prichinaobuchform.h"

#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"

PrichinaObuchForm::PrichinaObuchForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    //QString styleSheetString = QLatin1String(file.readAll());

    labelName = new QLabel(trUtf8("Prichina Obuch:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 ()\" -]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    savePushButton = new QPushButton(trUtf8("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Save And Close Button"));

    cancelPushButton = new QPushButton(trUtf8("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT prichinaobuchname FROM prichinaobuch WHERE prichinaobuchid = :prichinaobuchid");
        query.bindValue(":prichinaobuchid",indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());
        }
    }else{
        editName->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Prichina Obuch"));
    readSettings();
}

void PrichinaObuchForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void PrichinaObuchForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE prichinaobuch SET "
                      "prichinaobuchname = :prichinaobuchname "
                      "WHERE prichinaobuchid = :prichinaobuchid");
        query.bindValue(":prichinaobuchid",indexTemp);
        query.bindValue(":prichinaobuchname",editName->text().simplified());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("PrichinaObuch, UPDATE ERROR!"),query.lastError().text());
            return;
        }
        line += "UPDATE prichinaobuch SET prichinaobuchname = '";
        line += editName->text().simplified().toUtf8();
        line += "' WHERE prichinaobuchid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("prichinaobuch");
        QSqlQuery queryInsert;
        queryInsert.prepare("INSERT INTO prichinaobuch ("
                            "prichinaobuchid, "
                            "prichinaobuchname"
                            ") VALUES(:prichinaobuchid, "
                            ":prichinaobuchname"
                            ");");
        queryInsert.bindValue(":prichinaobuchid",indexTemp);
        queryInsert.bindValue(":prichinaobuchname",editName->text().simplified());
        queryInsert.exec();
        if(!queryInsert.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Prichina Obuch, INSERT ERROR!"),queryInsert.lastError().text());
            return;
        }
        line += "INSERT INTO prichinaobuch (prichinaobuchid, prichinaobuchname) VALUES('";
        line += indexTemp.toUtf8();
        line += "', '";
        line += editName->text().simplified().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
    }
    emit accept();
    close();
}


void PrichinaObuchForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }


    ForDelete forDelete(indexTemp,"prichinaobuch",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,trUtf8("Attention!!!"),trUtf8("Delete item?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM prichinaobuch WHERE prichinaobuchid = :prichinaobuchid");
        query.bindValue(":prichinaobuchid",indexTemp);
        query.exec();
        query.next();

        line += "DELETE FROM prichinaobuch WHERE prichinaobuchid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }
}

void PrichinaObuchForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("PrichinaObuchForm").toByteArray());
}

void PrichinaObuchForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("PrichinaObuchForm", saveGeometry());
}
