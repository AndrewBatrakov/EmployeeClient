#include "postsizform.h"
#include <QtSql>
#include "postform.h"
#include "viewlisttable.h"
#include "numprefix.h"
#include "fordelete.h"
#include "noedititemdelegat.h"
#include "postsiztableform.h"

PostSizTableForm::PostSizTableForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    indexTemp = id;
    addIntoTable = false;

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QVBoxLayout *labelLayout = new QVBoxLayout;
    QVBoxLayout *editLayout = new QVBoxLayout;

    //**********************************************************
    //SIZ
    //**********************************************************
    labelPostSIZ = new QLabel(trUtf8("PostSIZ Name:"));
    editPostSIZ = new LineEdit;
    editPostSIZ->setReadOnly(onlyForRead);
    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 \" -]{150}");
    editPostSIZ->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelPostSIZ->setBuddy(editPostSIZ);

    labelLayout->addWidget(labelPostSIZ);
    editLayout->addWidget(editPostSIZ);


    savePushButton = new QPushButton(trUtf8("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(saveRecord()));
    savePushButton->setToolTip(trUtf8("Save And Close Button"));

    cancelPushButton = new QPushButton(trUtf8("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelRecord()));
    cancelPushButton->setToolTip(trUtf8("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QPushButton *addTableButton = new QPushButton(trUtf8("Add"));
    QPixmap pixAdd(":/add.png");
    addTableButton->setIcon(pixAdd);
    connect(addTableButton,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPushButton *deleteTableButton = new QPushButton(trUtf8("Delete"));
    QPixmap pixDelete(":/delete.png");
    deleteTableButton->setIcon(pixDelete);
    connect(deleteTableButton,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QPushButton *editTableButton = new QPushButton(trUtf8("Edit"));
    QPixmap pixEdit(":/edit.png");
    editTableButton->setIcon(pixEdit);
    connect(editTableButton,SIGNAL(clicked()),this,SLOT(editRecordOfTable()));

    QHBoxLayout *buttonTableBox = new QHBoxLayout;
    buttonTableBox->addWidget(addTableButton);
    buttonTableBox->addWidget(deleteTableButton);
    buttonTableBox->addWidget(editTableButton);
    buttonTableBox->addStretch();

    //*****************************************************
    //SIZ TableView
    //*****************************************************
    sizView = new QTableWidget(0,6);
    sizView->setHorizontalHeaderLabels(QStringList()<<trUtf8("ID")<<trUtf8("P")<<trUtf8("Наименование СИЗ")<<
                                       trUtf8("Характеристика")<<trUtf8("Количество")<<trUtf8("Срок"));
    sizView->hideColumn(0);
    sizView->hideColumn(1);
    sizView->setColumnWidth(2,300);
    sizView->setColumnWidth(3,300);
    sizView->setAlternatingRowColors(true);
    QHeaderView *head = sizView->horizontalHeader();
    head->setStretchLastSection(true);
    sizView->setSelectionMode(QAbstractItemView::SingleSelection);
    sizView->setSelectionBehavior(QAbstractItemView::SelectRows);
    sizView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT postsizname FROM postsiz WHERE postsizid = :postsizid");
        query.bindValue(":postsizid",indexTemp);
        query.exec();
        query.next();
        editPostSIZ->setText(query.value(0).toString());
        //editPostSIZ->setReadOnly(true);
        QSqlQuery queryTable;
        queryTable.prepare("SELECT(SELECT siznaim.siznaimname FROM siznaim WHERE siznaim.siznaimid ="
                           " postsiztable.siznaimid),(SELECT har.harname FROM har WHERE har.harid = "
                           "postsiztable.harid), kolvo, srok FROM postsiztable WHERE "
                           "postsiztable.postsizid = :postsizid");
        queryTable.bindValue(":postsizid",indexTemp);
        queryTable.exec();
        int rowCount = 0;
        while(queryTable.next()){
            sizView->insertRow(rowCount);
            QTableWidgetItem *itemName = new QTableWidgetItem;
            sizView->setItem(rowCount,2,itemName);
            sizView->item(rowCount,2)->setText(queryTable.value(0).toString());

            QTableWidgetItem *itemHar = new QTableWidgetItem;
            sizView->setItem(rowCount,3,itemHar);
            sizView->item(rowCount,3)->setText(queryTable.value(1).toString());

            QTableWidgetItem *itemKol = new QTableWidgetItem;
            sizView->setItem(rowCount,4,itemKol);
            sizView->item(rowCount,4)->setText(queryTable.value(2).toString());

            QTableWidgetItem *itemSrok = new QTableWidgetItem;
            sizView->setItem(rowCount,5,itemSrok);
            sizView->item(rowCount,5)->setText(queryTable.value(3).toString());
        }
    }else{
        editPostSIZ->clear();
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("postsiz");
        updateRecord = false;

    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(labelLayout,0,0);
    mainLayout->addLayout(editLayout,0,1);
    mainLayout->addLayout(buttonTableBox,2,0,1,2);
    mainLayout->addWidget(sizView,3,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,4,1);
        editPostSIZ->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Нормы СИЗ"));
    readSettings();
}

void PostSizTableForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void PostSizTableForm::saveRecord()
{
    editRecord();
    emit accept();
}

void PostSizTableForm::cancelRecord()
{
    if(addIntoTable){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        //Удаление табличной части документа
        QSqlQuery query;
        query.prepare("DELETE FROM postsiztable WHERE postsizid = :postsizid");
        query.bindValue(":postsizid",indexTemp);
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Post Siz Table, DELETE ERROR!"),query.lastError().text());
            return;
        }
        line += "DELETE FROM postsiztable WHERE postsizid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }
    emit accept();
}

void PostSizTableForm::editRecord()
{
    if(checkingFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        if(updateRecord){
            QSqlQuery query;
            query.prepare("UPDATE postsiz SET "
                          "postsizname = :postsizname "
                          "WHERE postsizid = :postsizid");
            query.bindValue(":postsizname",editPostSIZ->text().simplified());
            query.bindValue(":postsizid",indexTemp);
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("Post SIZ, UPDATE ERROR!"),query.lastError().text());
                return;
            }
            line += "UPDATE postsiz SET postsizname = '";
            line += editPostSIZ->text().toUtf8();
            line += "' WHERE postsizid = '";
            line += indexTemp.toUtf8();
            line += "'";
            line += "\r\n";
            stream<<line;
        }else{
            QSqlQuery query;
            query.prepare("SELECT * FROM postsiz WHERE postsizname = :postsizname");
            query.bindValue(":postsizname",editPostSIZ->text().simplified());
            query.exec();
            query.next();
            if(!query.isValid()){
                QSqlQuery queryInsert;
                queryInsert.prepare("INSERT INTO postsiz ("
                                    "postsizid, "
                                    "postsizname"
                                    ") VALUES(:postsizid, "
                                    ":postsizname"
                                    ");");
                queryInsert.bindValue(":postsizid",indexTemp);
                queryInsert.bindValue(":postsizname",editPostSIZ->text().simplified());
                queryInsert.exec();
                if(!queryInsert.isActive()){
                    QMessageBox::warning(this,QObject::trUtf8("Post SIZ, INSERT ERROR!"),queryInsert.lastError().text());
                    return;
                }
                line += "INSERT INTO postsiz (postsizid, postsizname) VALUES('";
                line += indexTemp.toUtf8();
                line += "', ";
                line += editPostSIZ->text().simplified().toUtf8();
                line += "'";
                line += "\r\n";
                stream<<line;
            }else{
                QString tempString = trUtf8("Документ Сиз-Нормы");
                tempString += editPostSIZ->text().toUtf8();
                tempString += QObject::trUtf8(" - существует!");
                QMessageBox::warning(this,QObject::trUtf8("Внимание!!!"),tempString);
            }
        }
    }
}

void PostSizTableForm::deleteRecord()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "postsiz");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"postsiz",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,trUtf8("Attention!!!"),trUtf8("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM postsiz WHERE postsizid = :postsizid");
        query.bindValue(":postsizid",indexTemp);
        query.exec();
        query.next();
        line += "DELETE FROM postsiz WHERE postsizid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }

    settings.remove("Table");
    settings.remove("Index");
}

void PostSizTableForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "PostSizTableForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void PostSizTableForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "PostSizTableForm");
    settings.setValue("Geometry", saveGeometry());
}

void PostSizTableForm::addRecordOfTable()
{
    if(checkingFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }

        ViewListTable listTable("","siznaim",this);
        listTable.exec();
        QString postId = listTable.returnValue();

        QSqlQuery query;
        query.prepare("SELECT * FROM siznaim WHERE siznaimid = :id");
        query.bindValue(":id",postId);
        query.exec();
        while(query.next()){
            int rowCount = sizView->rowCount();
            bool insert = true;
            //Проверка на существование записи
            if (rowCount != 0){
                for(int kk = 0; kk < rowCount; ++kk){
                    QString yy = sizView->item(kk,0)->text();
                    QString pp = query.value(0).toString();
                    if(yy == pp){
                        QString tempString = query.value(1).toString();
                        tempString += QObject::trUtf8(" is availble!");
                        QMessageBox::warning(this,QObject::trUtf8("Atention!!!"),tempString);
                        insert = false;
                        break;
                    }
                }
            }
            if(insert){
//                addIntoTable = true;
//                sizView->insertRow(rowCount);

//                QTableWidgetItem *itemID = new QTableWidgetItem;
//                postView->setItem(rowCount,1,itemID);
//                postView->item(rowCount,1)->setText(query.value(0).toString());

//                QTableWidgetItem *itemName = new QTableWidgetItem;
//                //itemName->setTextAlignment(Qt::AlignCenter);
//                postView->setItem(rowCount,2,itemName);
//                postView->item(rowCount,3)->setText(query.value(1).toString());

//                NumPrefix numPrefix;
//                QString idPostSizList = numPrefix.getPrefix("postsiztable");

//                QSqlQuery queryPSL;
//                queryPSL.prepare("INSERT INTO postsizlist ("
//                                "postsizlistid, postsizid, postid"
//                                ") VALUES(:postsizlistid, :postsizid, :postid)");
//                queryPSL.bindValue(":postsizlistid",idPostSizList);
//                queryPSL.bindValue(":postsizid",indexTemp);
//                queryPSL.bindValue(":postid",query.value(0).toString());
//                queryPSL.exec();
//                if(!queryPSL.isActive()){
//                    QMessageBox::warning(this,QObject::trUtf8("Post SIZ List Table, INSERT ERROR!"),queryPSL.lastError().text());
//                    return;
//                }
//                line += "INSERT INTO postsizlist (postsizlistid, postsizid, postid) VALUES('";
//                line += idPostSizList.toUtf8();
//                line += "', '";
//                line += indexTemp.toUtf8();
//                line += "', '";
//                line += query.value(0).toString().toUtf8();
//                line += "')";
//                line += "\r\n";
//                stream<<line;
            }
        }
    }
}

void PostSizTableForm::deleteRecordOfTable()
{

}

void PostSizTableForm::editRecordOfTable()
{

}

bool PostSizTableForm::checkingFill()
{
    bool fill = true;
    if(editPostSIZ->text() == ""){
        editPostSIZ->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editPostSIZ->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}
