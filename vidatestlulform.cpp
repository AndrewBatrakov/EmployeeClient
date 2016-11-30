#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"
#include "vidatestlulform.h"

VidAtestLulForm::VidAtestLulForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
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
        query.prepare("SELECT vidatestlulname FROM vidatestlul WHERE vidatestlulid = :id");
        query.bindValue(":id",indexTemp);
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

    setWindowTitle(tr("Vid atest lul"));
    readSettings();
}

void VidAtestLulForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void VidAtestLulForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE vidatestlul SET "
                      "vidatestlulname = :name "
                      "WHERE vidatestlulid = :id");
        query.bindValue(":id",indexTemp);
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Vidatestlul, UPDATE ERROR!"),query.lastError().text());
            return;
        }
        line += "UPDATE vidatestlul SET vidatestlulname = '";
        line += editName->text().simplified().toUtf8();
        line += "' WHERE vidatestlulid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("vidatestlul");
        QSqlQuery queryInsert;
        queryInsert.prepare("INSERT INTO vidatestlul ("
                            "vidatestlulid, "
                            "vidatestlulname"
                            ") VALUES(:id, "
                            ":name"
                            ");");
        queryInsert.bindValue(":id",indexTemp);
        queryInsert.bindValue(":name",editName->text().simplified());
        queryInsert.exec();
        if(!queryInsert.isActive()){
            QMessageBox::warning(this,QObject::tr("Vidatestlul, INSERT ERROR!"),queryInsert.lastError().text());
            return;
        }
        line += "INSERT INTO vidatestlul (vidatestlulid, vidatestlulname) VALUES('";
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


void VidAtestLulForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }


    ForDelete forDelete(indexTemp,"vidatestlul",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM vidatestlul WHERE vidatestlulid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();

        line += "DELETE FROM vidatestlul WHERE vidatestlulid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }
}

void VidAtestLulForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("VidAtestLulForm").toByteArray());
}

void VidAtestLulForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("VidAtestLulForm", saveGeometry());
}
