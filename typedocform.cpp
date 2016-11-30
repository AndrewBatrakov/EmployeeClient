#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"
#include "typedocform.h"

TypeDocForm::TypeDocForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    //QString styleSheetString = QLatin1String(file.readAll());

    labelName = new QLabel(tr("Prichina Obuch:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 ()\" -]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
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
        query.prepare("SELECT typedocname FROM typedoc WHERE typedocid = :typedocid");
        query.bindValue(":typedocid",indexTemp);
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

    setWindowTitle(tr("Type doc"));
    readSettings();
}

void TypeDocForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void TypeDocForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE typedoc SET "
                      "typedocname = :typedocname "
                      "WHERE typedocid = :typedocid");
        query.bindValue(":typedocid",indexTemp);
        query.bindValue(":typedocname",editName->text().simplified());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Type Doc, UPDATE ERROR!"),query.lastError().text());
            return;
        }
        line += "UPDATE typedoc SET typedocname = '";
        line += indexTemp.toUtf8();
        line += "' WHERE typedocid = '";
        line += editName->text().simplified().toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("typedoc");
        QSqlQuery queryInsert;
        queryInsert.prepare("INSERT INTO typedoc ("
                            "typedocid, "
                            "typedocname"
                            ") VALUES(:id, "
                            ":name"
                            ");");
        queryInsert.bindValue(":id",indexTemp);
        queryInsert.bindValue(":name",editName->text().simplified());
        queryInsert.exec();
        if(!queryInsert.isActive()){
            QMessageBox::warning(this,QObject::tr("Type Doc, INSERT ERROR!"),queryInsert.lastError().text());
            return;
        }
        line += "INSERT INTO typedoc (typedocid, typedocname) VALUES('";
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


void TypeDocForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }


    ForDelete forDelete(indexTemp,"typedoc",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM typedoc WHERE typedocid = :typedocid");
        query.bindValue(":typedocid",indexTemp);
        query.exec();
        query.next();

        line += "DELETE FROM typedoc WHERE typedocid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }
}

void TypeDocForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("TypeDocForm").toByteArray());
}

void TypeDocForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("TypeDocForm", saveGeometry());
}
