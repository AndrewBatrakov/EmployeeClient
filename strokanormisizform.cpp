#include <QtSql>
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"
#include "strokanormisizform.h"

StrokaNormiSizForm::StrokaNormiSizForm(QString id, QString idPS, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    indexTemp = id;
    idPostSiz = idPS;

    labelName = new QLabel(trUtf8("Наименование СИЗ:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    QSqlQueryModel *sizModel = new QSqlQueryModel;
    sizModel->setQuery("SELECT siznaimname FROM siznaim");
    QCompleter *sizCompleter = new QCompleter(sizModel);
    sizCompleter->setCompletionMode(QCompleter::PopupCompletion);
    //sizCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    sizCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    sizCompleter->setWrapAround(false);
    editName->setCompleter(sizCompleter);

    labelHar = new QLabel(trUtf8("Характеристика:"));
    editHar = new LineEdit;
    editHar->setValidator(new QRegExpValidator(regExp,this));
    labelHar->setBuddy(editHar);

    QSqlQueryModel *harModel = new QSqlQueryModel;
    harModel->setQuery("SELECT harname FROM har");
    QCompleter *harCompleter = new QCompleter(harModel);
    harCompleter->setCompletionMode(QCompleter::PopupCompletion);
    harCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    harCompleter->setWrapAround(false);
    editHar->setCompleter(harCompleter);

    labelNumber = new QLabel(trUtf8("Количество:"));
    QRegExp regExpNum("[0-9]{3}");
    editNumber = new LineEdit;
    editNumber->setValidator(new QRegExpValidator(regExpNum,this));
    labelNumber->setBuddy(editNumber);

    labelSrok = new QLabel(trUtf8("Срок использования:"));
    editSrok = new LineEdit;
    QRegExp regExpNum1("[0-9]{2}");
    editSrok->setValidator(new QRegExpValidator(regExpNum1,this));

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
        query.prepare("SELECT * FROM postsiztable WHERE postsiztableid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            QSqlQuery queryN;
            queryN.prepare("SELECT * FROM siznaim WHERE siznaimid = :siznaimid");
            idSizNaim = query.value(2).toString();
            queryN.bindValue(":siznaimid",idSizNaim);
            queryN.exec();
            queryN.next();
            editName->setText(queryN.value(1).toString());

            queryN.clear();
            queryN.prepare("SELECT * FROM har WHERE harid = :harid");
            idHar = query.value(3).toString();
            queryN.bindValue(":harid",idHar);
            queryN.exec();
            queryN.next();
            editHar->setText(queryN.value(1).toString());
            editNumber->setText(query.value(4).toString());
            editSrok->setText(query.value(5).toString());
        }
    }else{
        editName->clear();
        editHar->clear();
        editNumber->clear();
        editSrok->clear();
        idSizNaim = "";
        idHar = "";
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addWidget(labelHar,1,0);
    mainLayout->addWidget(editHar,1,1);
    mainLayout->addWidget(labelNumber,2,0);
    mainLayout->addWidget(editNumber,2,1);
    mainLayout->addWidget(labelSrok,3,0);
    mainLayout->addWidget(editSrok,3,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,4,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Строка состава нормы СИЗ"));
    readSettings();
}

void StrokaNormiSizForm::editRecord()
{
    if(checkingFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        QSqlQuery queryN;
        queryN.prepare("SELECT * FROM siznaim WHERE siznaimname = :siznaimname");
        queryN.bindValue(":siznaimname",editName->text().simplified());
        queryN.exec();
        queryN.next();
        idSizNaim = queryN.value(0).toString();

        QSqlQuery queryH;
        queryH.prepare("SELECT * FROM har WHERE harname = :harname");
        queryH.bindValue(":harname",editHar->text().simplified());
        queryH.exec();
        queryH.next();
        idHar = queryH.value(0).toString();

        if(indexTemp != ""){
            QSqlQuery query;
            query.prepare("UPDATE postsiztable SET postsizid = :postsizid, "
                          "siznaimid = :siznaimid, "
                          "harid = :harid, "
                          "kolvo = :kolvo, "
                          "srok = :srok "
                          "WHERE  postsiztableid = :postsiztableid");
            query.bindValue(":postsizid",idPostSiz);
            query.bindValue(":siznaimid",idSizNaim);
            query.bindValue(":harid",idHar);
            query.bindValue(":kolvo",editNumber->text().toInt());
            query.bindValue(":srok",editSrok->text().toInt());
            query.bindValue(":postsiztableid",indexTemp);
            query.exec();
            line += "UPDATE postsiztable SET postsizid = '";
            line += idPostSiz.toUtf8();
            line += "', siznaimid = '";
            line += queryN.value(0).toString().toUtf8();
            line += "', harid = '";
            line += queryH.value(0).toString().toUtf8();
            line += "', kolvo = '";
            line += editNumber->text();
            line += "', srok = '";
            line += editSrok->text();
            line += "' WHERE postsiztableid = '";
            line += indexTemp.toUtf8();
            line += "'";
            line += "\r\n";
            stream<<line;
        }else{
            QSqlQuery query;
            query.prepare("SELECT * FROM postsiztable WHERE (postsizid = :postsizid AND "
                          "siznaimid = :siznaimid AND harid = :harid)");
            query.bindValue(":postsizid",idPostSiz);
            query.bindValue(":siznaimid",idSizNaim);
            query.bindValue(":harid",idHar);
            query.exec();
            query.next();
            if(!query.isValid()){
                NumPrefix numPrefix;
                indexTemp = numPrefix.getPrefix("postsiztable");
                QSqlQuery query;
                query.prepare("INSERT INTO postsiztable (postsiztableid, postsizid, siznaimid, harid, kolvo, srok) VALUES("
                              ":postsiztableid, :postsizid, :siznaimid, :harid, :kolvo, :srok)");
                query.bindValue(":postsiztableid",indexTemp);
                query.bindValue(":postsizid",idPostSiz);
                query.bindValue(":siznaimid",idSizNaim);
                query.bindValue(":harid",idHar);
                query.bindValue(":kolvo",editNumber->text().toInt());
                query.bindValue(":srok",editSrok->text().toInt());
                query.exec();
                line += "INSERT INTO postsiztable (postsiztableid, postsizid, siznaimid, harid, kolvo, srok) VALUES('";
                line += indexTemp.toUtf8();
                line += "', '";
                line += idPostSiz;
                line += "', '";
                line += idSizNaim;
                line += "', '";
                line += idHar;
                line += "', '";
                line += editNumber->text();
                line += "', '";
                line += editSrok->text();
                line += "')";
                line += "\r\n";
                stream<<line;
            }else{
                QString tempString = editName->text();
                tempString += "\n";
                tempString += editHar->text();
                tempString += "\n";
                tempString += QObject::trUtf8(" присутствует в списке норм СИЗ!");
                QMessageBox::warning(this,QObject::trUtf8("Внимание!!!"),tempString);
                return;
            }
        }
        emit accept();
        close();
    }
}

void StrokaNormiSizForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    //QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    //if(settings.value("CurrentUser") == "root" || settings.value("CurrentUser") == "Nataly"){
    ForDelete forDelete(indexTemp,"postsiztable",this);

    forDelete.exec();
    forDelete.deleteOnDefault();

    QSqlQuery query;
    query.prepare("DELETE FROM postsiztable WHERE postsiztableid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM postsiztable WHERE postsiztableid = '";
    line += indexTemp.toUtf8();
    line += "'";
    line += "\r\n";
    stream<<line;
}

void StrokaNormiSizForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void StrokaNormiSizForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "StrokaNormiSizForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void StrokaNormiSizForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "StrokaNormiSizForm");
    settings.setValue("Geometry", saveGeometry());
}

bool StrokaNormiSizForm::checkingFill()
{
    bool fill = true;
    if(editName->text() == ""){
        editName->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editName->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editHar->text() == ""){
        editHar->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editHar->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editNumber->text() == ""){
        editNumber->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editNumber->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editSrok->text() == ""){
        editSrok->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editSrok->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}
