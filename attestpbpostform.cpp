#include "attestpbpostform.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"
#include <QAxObject>

AttestPBPostForm::AttestPBPostForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString toolButtonStyle = QLatin1String(file.readAll());

    indexTemp = id;
    addIntoTable = false;

    labelShifr = new QLabel(trUtf8("Шифр:"));
    editShifr = new LineEdit;
    editShifr->setReadOnly(onlyForRead);
    labelShifr->setBuddy(editShifr);
    connect(editShifr,SIGNAL(textChanged(QString)),this,SLOT(shifrChange(QString)));

    QSqlQueryModel *shifrModel = new QSqlQueryModel;
    shifrModel->setQuery("SELECT cipher FROM testtask");
    QCompleter *shifrCompleter = new QCompleter(shifrModel);
    shifrCompleter->setCompletionMode(QCompleter::PopupCompletion);
    shifrCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editShifr->setCompleter(shifrCompleter);

//    QToolButton *addShifrButton = new QToolButton;
//    QPixmap addPix(":/add.png");
//    addShifrButton->setIcon(addPix);
//    addShifrButton->setToolTip(trUtf8("Добавить новую запись"));
//    addShifrButton->setStyleSheet(toolButtonStyle);
//    connect(addShifrButton,SIGNAL(clicked()),this,SLOT(addKomRecord()));

//    QToolButton *seeShifrButton = new QToolButton;
//    QPixmap seePix(":/see.png");
//    seeShifrButton->setIcon(seePix);
//    seeShifrButton->setToolTip(trUtf8("Смотреть выбранную запись"));
//    seeShifrButton->setStyleSheet(toolButtonStyle);
//    connect(seeShifrButton,SIGNAL(clicked()),this,SLOT(seeKomRecord()));

//    QToolButton *listShifrButton = new QToolButton;
//    QPixmap listPix(":/list.png");
//    listShifrButton->setIcon(listPix);
//    listShifrButton->setToolTip(trUtf8("Смотреть список записей"));
//    listShifrButton->setStyleSheet(toolButtonStyle);
//    connect(listShifrButton,SIGNAL(clicked()),this,SLOT(listKomRecord()));

    QHBoxLayout *shifrLayout = new QHBoxLayout;
    shifrLayout->addWidget(editShifr);
//    if(!onlyForRead){
//        shifrLayout->addWidget(addShifrButton);
//        shifrLayout->addWidget(seeShifrButton);
//        shifrLayout->addWidget(listShifrButton);
//    }

    labelTestZad = new QLabel(trUtf8("Тестовые задания:"));
    editTestZad = new QTextEdit;
    editTestZad->setReadOnly(true);
    labelTestZad->setBuddy(editTestZad);
    editTestZad->setFont(QFont("Times New Roman",12,QFont::Normal));

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(saveRecord()));
    savePushButton->setToolTip(trUtf8("Кнопка \"Записать и закрыть\""));

    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelRecord()));
    cancelPushButton->setToolTip(trUtf8("Кнопка отмены"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QPushButton *addTableButton = new QPushButton(trUtf8("Добавить"));
    QPixmap pixAdd(":/add.png");
    addTableButton->setIcon(pixAdd);
    connect(addTableButton,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPushButton *deleteTableButton = new QPushButton(trUtf8("Удалить"));
    QPixmap pixDelete(":/delete.png");
    deleteTableButton->setIcon(pixDelete);
    connect(deleteTableButton,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QPushButton *editTableButton = new QPushButton(trUtf8("Редактировать"));
    QPixmap pixEdit(":/edit.png");
    editTableButton->setIcon(pixEdit);
    connect(editTableButton,SIGNAL(clicked()),this,SLOT(editRecordOfTable()));

    QHBoxLayout *buttonTableBox = new QHBoxLayout;
    buttonTableBox->addWidget(addTableButton);
    buttonTableBox->addWidget(deleteTableButton);
    buttonTableBox->addWidget(editTableButton);
    buttonTableBox->addStretch();

    //*****************************************************
    //post TableView
    //*****************************************************
    postView = new QTableWidget;
    postView->setColumnCount(2);
    postView->setHorizontalHeaderLabels(QStringList()<<trUtf8("ID")<<trUtf8("Post"));
    QHeaderView *head = postView->horizontalHeader();
    head->setStretchLastSection(true);
    postView->setSelectionMode(QAbstractItemView::SingleSelection);
    postView->setSelectionBehavior(QAbstractItemView::SelectRows);
    postView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    postView->setColumnHidden(0,true);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM attestpbpost WHERE attestpbpostid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editShifr->setText(query.value(1).toString());
            QSqlQuery queryPost;
            queryPost.prepare("SELECT testtaskname FROM testtask WHERE cipher = :cipher");
            queryPost.bindValue(":cipher",query.value(1).toString());
            queryPost.exec();
            queryPost.next();
            editTestZad->setText(queryPost.value(0).toString());
//            int rowCount = 0;
//            while(queryID.next()){
//                QSqlQuery queryName;
//                queryName.prepare("SELECT employee.employeename, (SELECT post.postname FROM post WHERE post.postid = employee.postid), "
//                                  "employee.employeeid "
//                                  "FROM employee WHERE employee.employeeid = :id");
//                queryName.bindValue(":id",queryID.value(2).toString());
//                queryName.exec();
//                queryName.next();

//                employeeView->insertRow(rowCount);

//                QTableWidgetItem *itemID = new QTableWidgetItem;
//                employeeView->setItem(rowCount,0,itemID);
//                employeeView->item(rowCount,0)->setText(queryName.value(2).toString());

//                QTableWidgetItem *itemFIO = new QTableWidgetItem;
//                employeeView->setItem(rowCount,1,itemFIO);
//                employeeView->item(rowCount,1)->setText(queryName.value(0).toString());

//                QTableWidgetItem *itemPost = new QTableWidgetItem;
//                employeeView->setItem(rowCount,2,itemPost);
//                employeeView->item(rowCount,2)->setText(queryName.value(1).toString());

//                ++rowCount;
            //}
        }
    }else{
        editShifr->clear();
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("attestpbpost");
        updateRecord = false;
    }


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelTestZad,0,0);
    mainLayout->addWidget(editTestZad,0,1);
    mainLayout->addWidget(labelShifr,1,0);
    mainLayout->addLayout(shifrLayout,1,1);
    mainLayout->addLayout(buttonTableBox,5,0,1,2);
    mainLayout->addWidget(postView,6,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,7,1);
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Область аттестации промбезопасности"));
    readSettings();
    createContextMenu();
    postView->setColumnWidth(1,300);
    editShifr->setFocus();
}

void AttestPBPostForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void AttestPBPostForm::saveRecord()
{
    editRecord();
    emit accept();
}

void AttestPBPostForm::cancelRecord()
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
            queryCD.prepare("DELETE FROM attestpbposttable WHERE attestpbpostid = :id");
            queryCD.bindValue(":id",indexTemp);
            queryCD.exec();
            if(!queryCD.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("attestpbpost Table, DELETE ERROR!"),queryCD.lastError().text());
                return;
            }
            line += "DELETE FROM attestpbposttable WHERE attestpbpostid = '";
            line += indexTemp.toUtf8();
            line += "'";
            line += "\r\n";
            stream<<line;
        }
    }
    emit accept();
}

void AttestPBPostForm::editRecord()
{
//    QTextStream stream(&exchangeFile);
//    QString line;
//    while(!stream.atEnd()){
//        stream.readLine();
//    }

//    QSqlQuery queryIns;
//    queryIns.prepare("SELECT * FROM obuchlul WHERE (obuchlulid = :id AND datedoc = :datedoc)");
//    queryIns.bindValue(":id",indexTemp);
//    queryIns.bindValue(":datedoc",editDateDoc->date());
//    queryIns.exec();
//    queryIns.next();
//    if(queryIns.isValid()){

//        QSqlQuery query;
//        query.prepare("UPDATE obuchlul SET "
//                      "dogovor = :dogovor, "
//                      "datedoc = :datedoc, "
//                      "datecor = :datecor, "
//                      "dateobuch = :dateobuch, "
//                      "prichinaobuchid = :prichinaobuchid, "
//                      "vidatestlulid = :vidatestlulid, "
//                      "numberbuch = :numberbuch "
//                      "WHERE obuchlulid = :obuchlulid");
//        query.bindValue(":obuchlulid",indexTemp);
//        query.bindValue(":dogovor",editDogovor->text().simplified());
//        query.bindValue(":datedoc",editDateDoc->date());
//        query.bindValue(":datecor",editDateCor->date());
//        query.bindValue(":dateobuch",editDateObuch->date());
//        query.bindValue(":numberbuch",editProtocolObuch->text().simplified());
//        line += "UPDATE obuchlul SET dogovor = '";
//        line += editDogovor->text().simplified().toUtf8();
//        line += "', ";
//        line += "datedoc = '";
//        line += editDateDoc->date().toString("yyyy-MM-dd");
//        line += "', ";
//        line += "datecor = '";
//        line += editDateCor->date().toString("yyyy-MM-dd");
//        line += "', ";
//        line += "dateobuch = '";
//        line += editDateObuch->date().toString("yyyy-MM-dd");
//        line += "', ";
//        line += "numberbuch = '";
//        line += editProtocolObuch->text().toUtf8();
//        line += "', ";
//        QSqlQuery queryVar;
//        queryVar.prepare("SELECT prichinaobuchid FROM prichinaobuch WHERE prichinaobuchname = :name");
//        queryVar.bindValue(":name",editPrichina->text());
//        queryVar.exec();
//        queryVar.next();
//        query.bindValue(":prichinaobuchid",queryVar.value(0).toString());
//        line += "prichinaobuchid = '";
//        line += queryVar.value(0).toString().toUtf8();
//        line += "', ";
//        queryVar.clear();
//        queryVar.prepare("SELECT vidatestlulid FROM vidatestlul WHERE vidatestlulname = :name");
//        queryVar.bindValue(":name",editVidAtest->text());
//        queryVar.exec();
//        queryVar.next();
//        query.bindValue(":vidatestlulid",queryVar.value(0).toString());
//        query.exec();
//        line += "vidatestlulid = '";
//        line += queryVar.value(0).toString().toUtf8();
//        line += "' WHERE obuchlulid = '";
//        line += indexTemp.toUtf8();
//        line += "'";
//        line += "\r\n";
//        stream<<line;
//        if(!query.isActive()){
//            QMessageBox::warning(this,QObject::trUtf8("Obuch Lul, UPDATE ERROR!"),query.lastError().text());
//            return;
//        }
//    }else{
//        QSqlQuery queryInsert;
//        queryInsert.prepare("INSERT INTO obuchlul ("
//                            "obuchlulid, "
//                            "dogovor, "
//                            "datedoc, "
//                            "datecor, "
//                            "dateobuch, "
//                            "prichinaobuchid, "
//                            "vidatestlulid, "
//                            "numberbuch"
//                            ") VALUES(:obuchlulid, "
//                            ":dogovor, "
//                            ":datedoc, "
//                            ":datecor, "
//                            ":dateobuch, "
//                            ":prichinaobuchid, "
//                            ":vidatestlulid, "
//                            ":numberbuch"
//                            ");");
//        queryInsert.bindValue(":obuchlulid",indexTemp);
//        queryInsert.bindValue(":dogovor",editDogovor->text().simplified());
//        queryInsert.bindValue(":datedoc",editDateDoc->date());
//        queryInsert.bindValue(":datecor",editDateCor->date());
//        queryInsert.bindValue(":dateobuch",editDateObuch->date());
//        queryInsert.bindValue(":numberbuch",editProtocolObuch->text().simplified());

//        line += "INSERT INTO obuchlul (obuchlulid, dogovor, datedoc, datecor, "
//                "dateobuch, numberbuch, prichinaobuchid, vidatestlulid) VALUES('";
//        line += indexTemp.toUtf8();
//        line += "', '";
//        line += editDogovor->text().simplified().toUtf8();
//        line += "', '";
//        line += editDateDoc->date().toString("yyyy-MM-dd");
//        line += "', '";
//        line += editDateCor->date().toString("yyyy-MM-dd");
//        line += "', '";
//        line += editDateObuch->date().toString("yyyy-MM-dd");
//        line += "', '";
//        line += editProtocolObuch->text().toUtf8();
//        line += "', '";
//        QSqlQuery queryVar;
//        queryVar.prepare("SELECT prichinaobuchid FROM prichinaobuch WHERE prichinaobuchname = :name");
//        queryVar.bindValue(":name",editPrichina->text());
//        queryVar.exec();
//        queryVar.next();
//        queryInsert.bindValue(":prichinaobuchid",queryVar.value(0).toString());
//        line += queryVar.value(0).toString().toUtf8();
//        line += "', '";
//        queryVar.clear();
//        queryVar.prepare("SELECT vidatestlulid FROM vidatestlul WHERE vidatestlulname = :name");
//        queryVar.bindValue(":name",editVidAtest->text());
//        queryVar.exec();
//        queryVar.next();
//        queryInsert.bindValue(":vidatestlulid",queryVar.value(0).toString());
//        queryInsert.exec();
//        line += queryVar.value(0).toString().toUtf8();
//        line += "')";
//        line += "\r\n";
//        stream<<line;
//        if(!queryInsert.isActive()){
//            QMessageBox::warning(this,QObject::trUtf8("Obuch Lul, INSERT ERROR!"),queryInsert.lastError().text());
//            return;
//        }
//    }
}


void AttestPBPostForm::deleteRecord()
{
//    QTextStream stream(&exchangeFile);
//    QString line;
//    while(!stream.atEnd()){
//        stream.readLine();
//    }

//    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
//    settings.setValue("Table", "obuchlul");
//    settings.setValue("Index",indexTemp);

//    ForDelete forDelete(indexTemp,"obuchlul",this);

//    forDelete.exec();
//    int k = QMessageBox::warning(this,trUtf8("Attention!!!"),trUtf8("Delete item with the replacement for default value?"),
//                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
//    if(k == QMessageBox::Yes){
//        forDelete.deleteOnDefault();
//        QSqlQuery query;
//        query.prepare("DELETE FROM obuchlul WHERE obuchlulid = :obuchlulid");
//        query.bindValue(":obuchlulid",indexTemp);
//        query.exec();
//        query.next();

//        line += "DELETE FROM obuchlul WHERE obuchlulid = '";
//        line += indexTemp.toUtf8();
//        line += "'";
//        line += "\r\n";
//        stream<<line;
//    }

//    settings.remove("Table");
//    settings.remove("Index");
}

void AttestPBPostForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("AttestPBPostForm").toByteArray());
}

void AttestPBPostForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("AttestPBPostForm", saveGeometry());
}

void AttestPBPostForm::addRecordOfTable()
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
                        tempString += QObject::trUtf8(" is availble!");
                        QMessageBox::warning(this,QObject::trUtf8("Atention!!!"),tempString);
                        insert = false;
                        break;
                    }
                }
            }
//            QSqlQuery queryDoc;
//            queryDoc.prepare("SELECT (SELECT o.dateobuch  FROM obuchlul AS o WHERE o.obuchlulid = ot.obuchlulid)"
//                             "FROM obuchlultable AS ot WHERE ot.employeeid = :id");
//            queryDoc.bindValue(":id",employeeId);
//            queryDoc.exec();
//            queryDoc.next();
//            if(queryDoc.isValid()){
//                QString tempString = query.value(1).toString();
//                tempString += QObject::trUtf8(" trained - ");
//                tempString += queryDoc.value(0).toDate().toString("dd.MM.yyyy");
//                QMessageBox::warning(this,QObject::trUtf8("Atention!!!"),tempString);
//                insert = false;
//            }
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
                QString idTable = numPrefix.getPrefix("obuchlultable");

                QSqlQuery queryPSL;
                queryPSL.prepare("INSERT INTO attestpbposttable ("
                                "attestpbposttableid, attestpbpostid, postid"
                                ") VALUES(:attestpbposttableid, :attestpbpostid, :postid)");
                queryPSL.bindValue(":attestpbposttableid",idTable);
                queryPSL.bindValue(":attestpbpostid",indexTemp);
                queryPSL.bindValue(":postid",query.value(0).toString());
                queryPSL.exec();
                if(!queryPSL.isActive()){
                    QMessageBox::warning(this,QObject::trUtf8("Attestatpbtable Table, INSERT ERROR!"),queryPSL.lastError().text());
                    return;
                }
                line += "INSERT INTO attestpbposttable (attestpbposttableid, attestpbpostid, postid) VALUES('";
                line += idTable.toUtf8();
                line += "', '";
                line += indexTemp.toUtf8();
                line += "', '";
                line += query.value(0).toString().toUtf8();
                line += "')";
                line += "\r\n";
                stream<<line;
                postView->setCurrentItem(itemName);
            }
        }
    }
}

void AttestPBPostForm::deleteRecordOfTable()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    int k = QMessageBox::question(this,trUtf8("Внимание!"),
                              trUtf8("Действительно удалить?"),
                                  QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(k == QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("DELETE FROM attestpbposttable WHERE (attestpbpostid = :attestpbpostid AND postid = :postid)");
        query.bindValue(":attestpbpostid",indexTemp);
        query.bindValue(":postid",postView->item(postView->currentRow(),0)->text());
        query.exec();

        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Atestpbposttable Table, DELETE ERROR!"),
                                 query.lastError().text());
            return;
        }
        line += "DELETE FROM attestpbposttable WHERE (attestpbpostid = '";
        line += indexTemp.toUtf8();
        line += "' AND postid = '";
        line += postView->item(postView->currentRow(),0)->text().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
        postView->removeRow(postView->currentRow());
        postView->repaint();
    }
}

void AttestPBPostForm::editRecordOfTable()
{
    QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("У Вас не прав на редактирование!"));
}

bool AttestPBPostForm::checkingFill()
{
    bool fill = true;
    if(editShifr->text() == ""){
        editShifr->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editShifr->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}

void AttestPBPostForm::updateTable()
{

}

void AttestPBPostForm::createContextMenu()
{
    addAction = new QAction(trUtf8("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecordOfTable()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(trUtf8("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordOfTable()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(trUtf8("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTable()));

    postView->addAction(addAction);
    postView->addAction(deleteAction);
    postView->addAction(editAction);
    postView->setContextMenuPolicy(Qt::ActionsContextMenu);
}


void AttestPBPostForm::shifrChange(QString shifr)
{
   QSqlQuery query;
   query.prepare("SELECT testtaskname FROM testtask WHERE cipher = :cipher");
   query.bindValue(":cipher",shifr);
   query.exec();
   query.next();
   editTestZad->setText(query.value(0).toString());
}
