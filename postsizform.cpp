#include "postsizform.h"
#include <QtSql>
#include "postform.h"
#include "viewlisttable.h"
#include "numprefix.h"
#include "fordelete.h"
#include "noedititemdelegat.h"
#include "postform.h"

PostSizForm::PostSizForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;
    addIntoTable = false;

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());

    //**********************************************************
    //SIZ
    //**********************************************************
    labelPostSIZ = new QLabel(tr("PostSIZ Name:"));
    editPostSIZ = new LineEdit;
    editPostSIZ->setReadOnly(onlyForRead);
    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 ()\" -]{150}");
    editPostSIZ->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelPostSIZ->setBuddy(editPostSIZ);

    savePushButton = new QPushButton(tr("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(saveRecord()));
    savePushButton->setToolTip(tr("Save And Close Button"));

    cancelPushButton = new QPushButton(tr("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelRecord()));
    cancelPushButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QPushButton *addTableButton = new QPushButton(tr("Add"));
    QPixmap pixAdd(":/add.png");
    addTableButton->setIcon(pixAdd);
    connect(addTableButton,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPushButton *deleteTableButton = new QPushButton(tr("Delete"));
    QPixmap pixDelete(":/delete.png");
    deleteTableButton->setIcon(pixDelete);
    connect(deleteTableButton,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QPushButton *editTableButton = new QPushButton(tr("Edit"));
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
    postView = new QTableWidget(0,2);
    postView->setHorizontalHeaderLabels(QStringList()<<tr("ID")<<tr("Post Name"));
    QHeaderView *head = postView->horizontalHeader();
    head->setStretchLastSection(true);
    postView->setSelectionMode(QAbstractItemView::SingleSelection);
    postView->setSelectionBehavior(QAbstractItemView::SelectRows);
    postView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    postView->setColumnHidden(0,true);


    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT postsizname FROM postsiz WHERE postsizid = :postsizid");
        query.bindValue(":postsizid",indexTemp);
        query.exec();
        while(query.next()){
            editPostSIZ->setText(query.value(0).toString());
            QSqlQuery queryID;
            queryID.prepare("SELECT postid FROM postsizlist WHERE postsizid = :postsizid");
            queryID.bindValue(":postsizid",indexTemp);
            queryID.exec();
            int rowCount = 0;
            while(queryID.next()){
                postView->insertRow(rowCount);
                QTableWidgetItem *itemID = new QTableWidgetItem;
                postView->setItem(rowCount,0,itemID);
                postView->item(rowCount,0)->setText(queryID.value(0).toString());

                QSqlQuery queryName;
                queryName.prepare("SELECT postName FROM post WHERE postid = :postid");
                queryName.bindValue(":postid",queryID.value(0).toString());
                queryName.exec();
                queryName.next();

                QTableWidgetItem *itemName = new QTableWidgetItem;
                postView->setItem(rowCount,1,itemName);
                postView->item(rowCount,1)->setText(queryName.value(0).toString());
                ++rowCount;
            }
        }
    }else{
        editPostSIZ->clear();
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("postsiz");
        updateRecord = false;
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelPostSIZ,0,0);
    mainLayout->addWidget(editPostSIZ,0,1);
    mainLayout->addLayout(buttonTableBox,2,0,1,2);
    mainLayout->addWidget(postView,3,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,4,1);
        editPostSIZ->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Post Siz"));
    readSettings();
    createContextMenu();
}

void PostSizForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void PostSizForm::saveRecord()
{
    editRecord();
    emit accept();
}

void PostSizForm::cancelRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(addIntoTable){
        //Удаление табличной части документа
        for(int row = 0; row < postView->rowCount(); ++row){
            QSqlQuery queryCD;
            queryCD.prepare("DELETE FROM postsizlist WHERE postsizlistid = :id");
            queryCD.bindValue(":id",postView->item(row,0)->text());
            queryCD.exec();
            if(!queryCD.isActive()){
                QMessageBox::warning(this,QObject::tr("EmpCert Table, DELETE ERROR!"),queryCD.lastError().text());
                return;
            }
            line += "DELETE FROM postsizlist WHERE postsizlistid = '";
            line += postView->item(row,0)->text().toUtf8();
            line += "'";
            line += "\r\n";
            stream<<line;
        }
    }
    emit accept();
}

void PostSizForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSqlQuery queryIns;
    queryIns.prepare("SELECT * FROM postsiz WHERE postsizid = :id");
    queryIns.bindValue(":id",indexTemp);
    queryIns.exec();
    queryIns.next();
    if(!queryIns.isValid()){
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
            QMessageBox::warning(this,QObject::tr("Post SIZ, INSERT ERROR!"),queryInsert.lastError().text());
            return;
        }
        line += "INSERT INTO postsiz (postsizid, postsizname) VALUES('";
        line += indexTemp.toUtf8();
        line += "', '";
        line += editPostSIZ->text().simplified().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("UPDATE postsiz SET "
                      "postsizname = :postsizname "
                      "WHERE postsizid = :postsizid");
        query.bindValue(":postsizid",indexTemp);
        query.bindValue(":postsizname",editPostSIZ->text().simplified());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Post SIZ, UPDATE ERROR!"),query.lastError().text());
            return;
        }
        line += "UPDATE postsiz SET postsizid = '";
        line += indexTemp.toUtf8();
        line += "' WHERE postsizname = '";
        line += editPostSIZ->text().simplified().toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }
}


void PostSizForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "postsiz");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"postsiz",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM postsiz WHERE postsizid = :postsizid");
        query.bindValue(":postsizid",indexTemp);
        query.exec();
        query.next();

        line += "DELETE FROM postsiz WHERE postsizlistid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }

    settings.remove("Table");
    settings.remove("Index");
}

void PostSizForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "PostSizForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void PostSizForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "PostSizForm");
    settings.setValue("Geometry", saveGeometry());
}

void PostSizForm::addRecordOfTable()
{
    if(checkingFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }

        ViewListTable listTable("","post",this);
        listTable.exec();
        QString postId = listTable.returnValue();

        QSqlQuery query;
        query.prepare("SELECT * FROM post WHERE postid = :id");
        query.bindValue(":id",postId);
        query.exec();
        while(query.next()){
            int rowCount = postView->rowCount();
            bool insert = true;
            //Проверка на существование записи
            if (rowCount != 0){
                for(int kk = 0; kk < rowCount; ++kk){
                    QString yy = postView->item(kk,0)->text();
                    QString pp = query.value(0).toString();
                    if(yy == pp){
                        QString tempString = query.value(1).toString();
                        tempString += QObject::tr(" is availble!");
                        QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
                        insert = false;
                        break;
                    }
                }
            }
            if(insert){
                addIntoTable = true;
                postView->insertRow(rowCount);

                QTableWidgetItem *itemID = new QTableWidgetItem;
                postView->setItem(rowCount,0,itemID);
                postView->item(rowCount,0)->setText(query.value(0).toString());

                QTableWidgetItem *itemName = new QTableWidgetItem;
                //itemName->setTextAlignment(Qt::AlignCenter);
                postView->setItem(rowCount,1,itemName);
                postView->item(rowCount,1)->setText(query.value(1).toString());

                NumPrefix numPrefix;
                QString idPostSizList = numPrefix.getPrefix("postsizlist");

                QSqlQuery queryPSL;
                queryPSL.prepare("INSERT INTO postsizlist ("
                                "postsizlistid, postsizid, postid"
                                ") VALUES(:postsizlistid, :postsizid, :postid)");
                queryPSL.bindValue(":postsizlistid",idPostSizList);
                queryPSL.bindValue(":postsizid",indexTemp);
                queryPSL.bindValue(":postid",query.value(0).toString());
                queryPSL.exec();
                if(!queryPSL.isActive()){
                    QMessageBox::warning(this,QObject::tr("Post SIZ List Table, INSERT ERROR!"),queryPSL.lastError().text());
                    return;
                }
                line += "INSERT INTO postsizlist (postsizlistid, postsizid, postid) VALUES('";
                line += idPostSizList.toUtf8();
                line += "', '";
                line += indexTemp.toUtf8();
                line += "', '";
                line += query.value(0).toString().toUtf8();
                line += "')";
                line += "\r\n";
                stream<<line;
            }
        }
    }
}

void PostSizForm::deleteRecordOfTable()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    int k = QMessageBox::question(this,tr("Attention!!"),
                              tr("Really delete?"),
                                  QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(k == QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("DELETE FROM postsizlist WHERE (postid = :postid AND postsizid = :postsizid)");
        query.bindValue(":postsizid",indexTemp);
        query.bindValue(":postid",postView->item(postView->currentRow(),0)->text());
        query.exec();

        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Post Siz Table, DELETE ERROR!"),
                                 query.lastError().text());
            return;
        }
        line += "DELETE FROM postsizlist WHERE (postid = '";
        line += postView->item(postView->currentRow(),0)->text().toUtf8();
        line += "' AND postsizid = '";
        line += indexTemp.toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
        postView->removeRow(postView->currentRow());
        postView->repaint();
    }
}

void PostSizForm::editRecordOfTable()
{
    PostForm openForm(indexTemp,this,false);
    openForm.exec();
}

bool PostSizForm::checkingFill()
{
    bool fill = true;
    if(editPostSIZ->text() == ""){
        editPostSIZ->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editPostSIZ->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}

void PostSizForm::updateTable()
{

}

void PostSizForm::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecordOfTable()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordOfTable()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTable()));

    postView->addAction(addAction);
    postView->addAction(deleteAction);
    postView->addAction(editAction);
    postView->setContextMenuPolicy(Qt::ActionsContextMenu);
}
