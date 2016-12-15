#include "sizpostform.h"
#include "viewlisttable.h"
#include "fordelete.h"
#include "numprefix.h"
#include "postform.h"
#include "strokanormisizform.h"

SizPostForm::SizPostForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString newIndexTemp = settings.value("PostSizID").toString();
    QString copyDoc = settings.value("CopyPostSiz").toString();

    indexTemp = id;
    addIntoTable = false;
    labelPostSIZ = new QLabel(trUtf8("Наименование:"));
    editPostSIZ = new LineEdit;

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(saveRecord()));
    savePushButton->setToolTip(trUtf8("Save And Close Button"));

    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelRecord()));
    cancelPushButton->setToolTip(trUtf8("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    postView = new QTableWidget(0,3);
    postView->setHorizontalHeaderLabels(QStringList()<<trUtf8("ID")<<trUtf8("postID")<<trUtf8("Профессия по кадрам"));
    postView->hideColumn(0);
    postView->hideColumn(1);
    postView->setColumnWidth(2,300);
    postView->setAlternatingRowColors(true);
    QHeaderView *headPost = postView->horizontalHeader();
    headPost->setStretchLastSection(true);
    postView->setSelectionMode(QAbstractItemView::SingleSelection);
    postView->setSelectionBehavior(QAbstractItemView::SelectRows);
    postView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    sizView = new QTableWidget(0,6);
    sizView->setHorizontalHeaderLabels(QStringList()<<trUtf8("ID")<<trUtf8("postID")<<trUtf8("Наименование СИЗ")<<
                                       trUtf8("Характеристика")<<trUtf8("Количество")<<trUtf8("Срок эксп.(мес)"));
    sizView->hideColumn(0);
    sizView->hideColumn(1);
    sizView->setColumnWidth(2,300);
    sizView->setColumnWidth(3,300);
    sizView->setAlternatingRowColors(true);
    QHeaderView *headSiz = sizView->horizontalHeader();
    headSiz->setStretchLastSection(true);
    sizView->setSelectionMode(QAbstractItemView::SingleSelection);
    sizView->setSelectionBehavior(QAbstractItemView::SelectRows);
    sizView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT postsizname FROM postsiz WHERE postsizid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editPostSIZ->setText(query.value(0).toString());

            QSqlQuery query;
            query.prepare("SELECT * FROM postsizlist WHERE postsizid = :postsizid");
            query.bindValue(":postsizid",indexTemp);
            query.exec();
            int rowCount = 0;
            while(query.next()){
                postView->insertRow(rowCount);

                QTableWidgetItem *itemID = new QTableWidgetItem;
                postView->setItem(rowCount,0,itemID);
                postView->item(rowCount,0)->setText(query.value(0).toString());

                QTableWidgetItem *itemPostID = new QTableWidgetItem;
                postView->setItem(rowCount,1,itemPostID);
                postView->item(rowCount,1)->setText(query.value(1).toString());

                QSqlQuery queryName;
                queryName.prepare("SELECT postname FROM post WHERE postid = :postid");
                queryName.bindValue(":postid",query.value(2).toString());
                queryName.exec();
                queryName.next();
                QTableWidgetItem *itemPostSiz = new QTableWidgetItem;
                postView->setItem(rowCount,2,itemPostSiz);
                postView->item(rowCount,2)->setText(queryName.value(0).toString());

                ++rowCount;
            }
            updateSiz();
            if(copyDoc == "1"){
                indexTemp = newIndexTemp;
                QString tt = editPostSIZ->text();
                tt += " (1)";
                editPostSIZ->setText(tt);
            }
            updateRecord = true;
        }
    }else{
        editPostSIZ->clear();
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("postsiz");
        updateRecord = false;
    }

    connect(postView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editRecordOfTablePost()));
    connect(sizView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editRecordOfTableSiz()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelPostSIZ,0,0);
    mainLayout->addWidget(editPostSIZ,0,1);
    mainLayout->addWidget(postView,1,0,3,2);
    mainLayout->addWidget(sizView,5,0,5,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,10,1);
        editPostSIZ->selectAll();
    }
    sizView->setHorizontalHeaderLabels(QStringList()<<trUtf8("ID")<<trUtf8("postID")<<trUtf8("Наименование СИЗ")<<
                                       trUtf8("Характеристика")<<trUtf8("Количество")<<trUtf8("Срок эксп.(мес)"));
    setLayout(mainLayout);
    setWindowTitle(trUtf8("Профессия по нормам СИЗ"));
    readSettings();
    createContextMenuPost();
    createContextMenuSiz();
}

void SizPostForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void SizPostForm::deleteRecord()
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
    int k = QMessageBox::warning(this,trUtf8("Attention!!!"),trUtf8("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
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

void SizPostForm::saveRecord()
{
    editRecord();
    emit accept();
}

void SizPostForm::cancelRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(addIntoTable){
        //Удаление табличной части документа

        QSqlQuery queryPost;
        queryPost.prepare("DELETE FROM postsizlist WHERE postsizid = :id");
        queryPost.bindValue(":id",indexTemp);
        queryPost.exec();

        QSqlQuery querySiz;
        querySiz.prepare("DELETE FROM postsiztable WHERE postsizid = :id");
        querySiz.bindValue(":id",indexTemp);
        querySiz.exec();

        line += "DELETE FROM postsizlist WHERE postsizid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";

        line += "DELETE FROM postsiztable WHERE postsizid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";

        stream<<line;
    }
    emit accept();
}

void SizPostForm::editRecord()
{
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
        query.bindValue(":postsizid",indexTemp);
        query.bindValue(":postsizname",editPostSIZ->text().simplified());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Post SIZ, UPDATE ERROR!"),query.lastError().text());
            return;
        }
        line += "UPDATE postsiz SET postsizname = '";
        line += editPostSIZ->text().simplified().toUtf8();
        line += "' WHERE postsizid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
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
        line += "', '";
        line += editPostSIZ->text().simplified().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
    }
}

void SizPostForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "SizPostForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void SizPostForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "SizPostForm");
    settings.setValue("Geometry", saveGeometry());
}

void SizPostForm::addRecordOfTableSiz()
{
    //NumPrefix numPref;
    //QString idTable = numPref.getPrefix("postsiztable");
    StrokaNormiSizForm openForm("",indexTemp, this,false);
    openForm.exec();

    updateSiz();
}

void SizPostForm::deleteRecordOfTableSiz()
{
    QString idTable = sizView->item(sizView->currentRow(),0)->text();
    StrokaNormiSizForm openForm(idTable, indexTemp, this,false);
    openForm.deleteRecord();
    updateSiz();
}

void SizPostForm::editRecordOfTableSiz()
{
    QString idTable = sizView->item(sizView->currentRow(),0)->text();
    StrokaNormiSizForm openForm(idTable, indexTemp, this,false);
    openForm.exec();

    updateSiz();
}

void SizPostForm::addRecordOfTablePost()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    if(checkingFill()){
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
                    QString yy = postView->item(kk,2)->text();
                    QString pp = query.value(1).toString();
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
                addIntoTable = true;
                postView->insertRow(rowCount);

                NumPrefix numPrefix;
                QString idPostSizList = numPrefix.getPrefix("postsizlist");

                QTableWidgetItem *itemID = new QTableWidgetItem;
                postView->setItem(rowCount,0,itemID);
                postView->item(rowCount,0)->setText(idPostSizList);

                QTableWidgetItem *itemPostSiz = new QTableWidgetItem;
                postView->setItem(rowCount,1,itemPostSiz);
                postView->item(rowCount,1)->setText(indexTemp);

                QTableWidgetItem *itemPost = new QTableWidgetItem;
                postView->setItem(rowCount,2,itemPost);
                postView->item(rowCount,2)->setText(query.value(1).toString());

                QSqlQuery queryPSL;
                queryPSL.prepare("INSERT INTO postsizlist ("
                                 "postsizlistid, postsizid, postid"
                                 ") VALUES(:postsizlistid, :postsizid, :postid)");
                queryPSL.bindValue(":postsizlistid",idPostSizList);
                queryPSL.bindValue(":postsizid",indexTemp);
                queryPSL.bindValue(":postid",query.value(0).toString());
                queryPSL.exec();
                if(!queryPSL.isActive()){
                    QMessageBox::warning(this,QObject::trUtf8("Post SIZ List Table, INSERT ERROR!"),queryPSL.lastError().text());
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

void SizPostForm::deleteRecordOfTablePost()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    int k = QMessageBox::question(this,trUtf8("Attention!!"),
                              trUtf8("Really delete?"),
                                  QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(k == QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("DELETE FROM postsizlist WHERE (postid = :postid AND postsizid = :postsizid)");
        query.bindValue(":postsizid",indexTemp);
        QSqlQuery queryID;
        queryID.prepare("SELECT postid FROM post WHERE postname = :postname");
        queryID.bindValue(":postname",postView->item(postView->currentRow(),2)->text());
        queryID.exec();
        queryID.next();
        query.bindValue(":postid",queryID.value(0).toString());
        query.exec();

        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Post Siz Table, DELETE ERROR!"),
                                 query.lastError().text());
            return;
        }
        line += "DELETE FROM postsizlist WHERE (postid = '";
        line += queryID.value(0).toString().toUtf8();
        line += "' AND postsizid = '";
        line += indexTemp.toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
        postView->removeRow(postView->currentRow());
        postView->repaint();
    }
}

void SizPostForm::editRecordOfTablePost()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM post WHERE postname = :postname");
    query.bindValue(":postname",postView->item(postView->currentRow(),2)->text());
    query.exec();
    query.next();
    QString idTable = query.value(0).toString();
    PostForm openForm(idTable,this,false);
    openForm.exec();
}

bool SizPostForm::checkingFill()
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

void SizPostForm::createContextMenuPost()
{
    QAction *addAction = new QAction(trUtf8("Добавить профессию"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecordOfTablePost()));

    QPixmap pixDelete(":/delete.png");
    QAction *deleteAction = new QAction(trUtf8("Удалить профессию"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordOfTablePost()));

    QPixmap pixEdit(":/edit.png");
    QAction *editAction = new QAction(trUtf8("Редактировать профессию"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTablePost()));


    postView->addAction(addAction);
    postView->addAction(deleteAction);
    postView->addAction(editAction);
    postView->setContextMenuPolicy(Qt::ActionsContextMenu);
}


void SizPostForm::createContextMenuSiz()
{
    QAction *addAction = new QAction(trUtf8("Добавить СИЗ"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecordOfTableSiz()));

    QPixmap pixDelete(":/delete.png");
    QAction *deleteAction = new QAction(trUtf8("Удалить СИЗ"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordOfTableSiz()));

    QPixmap pixEdit(":/edit.png");
    QAction *editAction = new QAction(trUtf8("Редактировать СИЗ"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTableSiz()));


    sizView->addAction(addAction);
    sizView->addAction(deleteAction);
    sizView->addAction(editAction);
    sizView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void SizPostForm::updateSiz()
{
    for(int i = sizView->rowCount(); i >= 0; --i){
        sizView->removeRow(i);
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM postsiztable WHERE postsizid = :postsizid");
    query.bindValue(":postsizid",indexTemp);
    query.exec();
    int rowCount = 0;
    while(query.next()){
        sizView->insertRow(rowCount);

        QTableWidgetItem *itemID = new QTableWidgetItem;
        sizView->setItem(rowCount,0,itemID);
        sizView->item(rowCount,0)->setText(query.value(0).toString());

        QTableWidgetItem *itemPostID = new QTableWidgetItem;
        sizView->setItem(rowCount,1,itemPostID);
        sizView->item(rowCount,1)->setText(query.value(1).toString());

        QSqlQuery queryName;
        queryName.prepare("SELECT siznaimname FROM siznaim WHERE siznaimid = :siznaimid");
        queryName.bindValue(":siznaimid",query.value(2).toString());
        queryName.exec();
        queryName.next();
        QTableWidgetItem *itemPostSiz = new QTableWidgetItem;
        sizView->setItem(rowCount,2,itemPostSiz);
        sizView->item(rowCount,2)->setText(queryName.value(0).toString());

        queryName.clear();
        queryName.prepare("SELECT harname FROM har WHERE harid = :harid");
        queryName.bindValue(":harid",query.value(3).toString());
        queryName.exec();
        queryName.next();
        QTableWidgetItem *itemHar = new QTableWidgetItem;
        sizView->setItem(rowCount,3,itemHar);
        sizView->item(rowCount,3)->setText(queryName.value(0).toString());

        QTableWidgetItem *itemNum = new QTableWidgetItem;
        sizView->setItem(rowCount,4,itemNum);
        sizView->item(rowCount,4)->setText(query.value(4).toString());

        QTableWidgetItem *itemSrok = new QTableWidgetItem;
        sizView->setItem(rowCount,5,itemSrok);
        sizView->item(rowCount,5)->setText(query.value(5).toString());

        ++rowCount;
        sizView->setCurrentItem(itemPostSiz);
    }

}

void SizPostForm::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Insert){
        addRecordOfTableSiz();
    }
}
