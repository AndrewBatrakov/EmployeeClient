#include "scoreform.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"

ScoreForm::ScoreForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;
    addIntoTable = false;

    labelNumber = new QLabel(trUtf8("Номер:"));
    editNumber = new LineEdit;
    editNumber->setReadOnly(true);

    labelDateDoc = new QLabel(trUtf8("Дата:"));
    editDateDoc = new QDateEdit;
    editDateDoc->setCalendarPopup(true);

    QHBoxLayout *firstLayout = new QHBoxLayout;
    firstLayout->addWidget(editNumber);
    firstLayout->addWidget(labelDateDoc);
    firstLayout->addWidget(editDateDoc);
    firstLayout->addStretch();

    labelOrganization = new QLabel(tr("Поставщик:"));
    editOrganization = new LineEdit;
    labelOrganization->setBuddy(editOrganization);

    QSqlQueryModel *orgModel = new QSqlQueryModel;
    orgModel->setQuery("SELECT organizationname FROM organization");
    QCompleter *orgCompleter = new QCompleter(orgModel);
    orgCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    orgCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editOrganization->setCompleter(orgCompleter);

    labelScore = new QLabel(trUtf8("Счет:"));
    editScore = new LineEdit;
    editScore->setReadOnly(onlyForRead);
    labelScore->setBuddy(editScore);

    labelPaid = new QLabel(trUtf8("Оплачен:"));
    editPaid = new QCheckBox;

    QHBoxLayout *secondLayout = new QHBoxLayout;
    secondLayout->addWidget(editScore);
    secondLayout->addWidget(labelPaid);
    secondLayout->addWidget(editPaid);
    secondLayout->addStretch();

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

    QPushButton *printTableButton = new QPushButton(tr("Print"));
    QPixmap pixPrint(":/print.png");
    printTableButton->setIcon(pixPrint);
    connect(printTableButton,SIGNAL(clicked()),this,SLOT(printTable()));

    QHBoxLayout *buttonTableBox = new QHBoxLayout;
    buttonTableBox->addWidget(addTableButton);
    buttonTableBox->addWidget(deleteTableButton);
    buttonTableBox->addWidget(editTableButton);
    buttonTableBox->addWidget(printTableButton);
    buttonTableBox->addStretch();

    //*****************************************************
    //employee TableView
    //*****************************************************
    employeeView = new QTableWidget;
    employeeView->setColumnCount(3);
    employeeView->setHorizontalHeaderLabels(QStringList()<<trUtf8("ID")<<trUtf8("Номенклатура")<<trUtf8("Количество"));
    QHeaderView *head = employeeView->horizontalHeader();
    head->setStretchLastSection(true);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    //employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->setColumnHidden(0,true);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM score WHERE scoreid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editNumber->setText(indexTemp);
            editScore->setText(query.value(1).toString());
            editDateDoc->setDate(query.value(2).toDate());

            QSqlQuery queryOrg;
            queryOrg.prepare("SELECT organizationname FROM organization WHERE organizationid = :organizationid");
            queryOrg.bindValue(":organizationid",query.value(3).toString());
            queryOrg.exec();
            queryOrg.next();
            editOrganization->setText(queryOrg.value(0).toString());
            queryOrg.clear();

            if(query.value(4).toBool()){
                editPaid->setChecked(true);
            }else{
                editPaid->setChecked(false);
            }

            QSqlQuery queryID;
            queryID.prepare("SELECT * FROM scoretable WHERE scoreid = :scoreid");
            queryID.bindValue(":scoreid",indexTemp);
            queryID.exec();
            int rowCount = 0;
            while(queryID.next()){
                QSqlQuery queryName;
                queryName.prepare("SELECT scoretable.nomenid, (SELECT nomen.nomenname FROM nomen WHERE nomen.nomenid = scoretable.nomenid), "
                                  "scoretable.number "
                                  "FROM scoretable WHERE scoretable.scoretableid = :id");
                queryName.bindValue(":id",queryID.value(0).toString());
                queryName.exec();
                queryName.next();

                employeeView->insertRow(rowCount);

                QTableWidgetItem *itemID = new QTableWidgetItem;
                employeeView->setItem(rowCount,0,itemID);
                employeeView->item(rowCount,0)->setText(queryName.value(0).toString());

                QTableWidgetItem *itemNom = new QTableWidgetItem;
                employeeView->setItem(rowCount,1,itemNom);
                employeeView->item(rowCount,1)->setText(queryName.value(1).toString());

                QTableWidgetItem *itemNum = new QTableWidgetItem;
                employeeView->setItem(rowCount,2,itemNum);
                employeeView->item(rowCount,2)->setText(queryName.value(2).toString());

                ++rowCount;
            }
        }
    }else{
        editScore->clear();
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("score");
        updateRecord = false;
        editDateDoc->setDate(QDate::currentDate());
        editPaid->setChecked(false);
    }


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelNumber,0,0);
    mainLayout->addLayout(firstLayout,0,1);
    mainLayout->addWidget(labelOrganization,1,0);
    mainLayout->addWidget(editOrganization,1,1);
    mainLayout->addWidget(labelScore,2,0);
    mainLayout->addLayout(secondLayout,2,1);
    mainLayout->addLayout(buttonTableBox,3,0,1,2);
    mainLayout->addWidget(employeeView,4,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,5,1);
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Счета СИЗ"));
    readSettings();
    createContextMenu();
    editNumber->setText(indexTemp);
    employeeView->setColumnWidth(1,300);
}

void ScoreForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void ScoreForm::saveRecord()
{
    editRecord();
    emit accept();
}

void ScoreForm::cancelRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(addIntoTable){
        //Удаление табличной части документа
        for(int row = 0; row < employeeView->rowCount(); ++row){
            QSqlQuery queryCD;
            queryCD.prepare("DELETE FROM scoretable WHERE scoreid = :id");
            queryCD.bindValue(":id",indexTemp);
            queryCD.exec();
            if(!queryCD.isActive()){
                QMessageBox::warning(this,QObject::tr("Score Table, DELETE ERROR!"),queryCD.lastError().text());
                return;
            }
            line += "DELETE FROM scoretable WHERE scoreid = '";
            line += indexTemp.toUtf8();
            line += "'";
            line += "\r\n";
            stream<<line;
        }
    }
    emit accept();
}

void ScoreForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSqlQuery queryIns;
    queryIns.prepare("SELECT * FROM score WHERE (scoreid = :id AND datedoc = :datedoc)");
    queryIns.bindValue(":id",indexTemp);
    queryIns.bindValue(":datedoc",editDateDoc->date());
    queryIns.exec();
    queryIns.next();

    bool checkState;
    if(editPaid->isChecked()){
        checkState = true;
    }else{
        checkState = false;
    }
    if(queryIns.isValid()){

        QSqlQuery query;
        query.prepare("UPDATE score SET "
                      "scorename = :scorename, "
                      "scoredoc = :scoredoc, "
                      "organizationid = :organizationid, "
                      "paid = :paid "
                      "WHERE scoreid = :scoreid");
        query.bindValue(":scoreid",indexTemp);
        query.bindValue(":scorename",editScore->text().simplified());
        query.bindValue(":scoredoc",editDateDoc->date());
        line += "UPDATE score SET scorename = '";
        line += editScore->text().simplified().toUtf8();
        line += "', ";
        line += "scoredoc = '";
        line += editDateDoc->date().toString("yyyy-MM-dd");
        line += "', ";
        QSqlQuery queryVar;
        queryVar.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
        queryVar.bindValue(":name",editOrganization->text());
        queryVar.exec();
        queryVar.next();
        query.bindValue(":organizationid",queryVar.value(0).toString());
        line += "organizationid = '";
        line += queryVar.value(0).toString().toUtf8();
        line += "', ";
        line += "paid = '";
        line += checkState;
        query.bindValue(":paid",checkState);
        line += "' WHERE scoreid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Score, UPDATE ERROR!"),query.lastError().text());
            return;
        }
    }else{
        QSqlQuery queryInsert;
        queryInsert.prepare("INSERT INTO score ("
                            "scoreid, "
                            "scorename, "
                            "scoredoc, "
                            "organizationid, "
                            "paid"
                            ") VALUES(:scoreid, "
                            ":scorename, "
                            ":scoredoc, "
                            ":organizationid, "
                            ":paid"
                            ");");
        queryInsert.bindValue(":scoreid",indexTemp);
        queryInsert.bindValue(":scorename",editScore->text().simplified());
        queryInsert.bindValue(":scoredoc",editDateDoc->date());
        queryInsert.bindValue(":paid",checkState);

        line += "INSERT INTO score (scoreid, scorename, "
                "organizationid, paidid) VALUES('";
        line += indexTemp.toUtf8();
        line += "', '";
        line += editScore->text().simplified().toUtf8();
        line += "', '";
        line += editDateDoc->date().toString("yyyy-MM-dd");
        line += "', '";
        QSqlQuery queryVar;
        queryVar.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
        queryVar.bindValue(":name",editOrganization->text());
        queryVar.exec();
        queryVar.next();
        queryInsert.bindValue(":organizationid",queryVar.value(0).toString());
        line += queryVar.value(0).toString().toUtf8();
        line += "', '";
        queryInsert.bindValue(":paid",checkState);
        queryInsert.exec();
        line += checkState;
        line += "')";
        line += "\r\n";
        stream<<line;
        if(!queryInsert.isActive()){
            QMessageBox::warning(this,QObject::tr("Score, INSERT ERROR!"),queryInsert.lastError().text());
            return;
        }
    }
}


void ScoreForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "score");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"score",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM score WHERE scoreid = :scoreid");
        query.bindValue(":scoreid",indexTemp);
        query.exec();
        query.next();

        line += "DELETE FROM score WHERE scoreid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }

    settings.remove("Table");
    settings.remove("Index");
}

void ScoreForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "ScoreForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void ScoreForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "ScoreForm");
    settings.setValue("Geometry", saveGeometry());
}

void ScoreForm::addRecordOfTable()
{
    if(checkingFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }

        ViewListTable listTable("","nomen",this);
        listTable.exec();
        QString nomenId = listTable.returnValue();

        QSqlQuery query;
        query.prepare("SELECT * FROM nomen WHERE nomenid = :id");
        query.bindValue(":id",nomenId);
        query.exec();
        while(query.next()){
            int rowCount = employeeView->rowCount();
            bool insert = true;
            //Проверка на существование записи
            if (rowCount != 0){
                for(int kk = 0; kk < rowCount; ++kk){
                    QString yy = employeeView->item(kk,0)->text();
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
//            QSqlQuery queryDoc;
//            queryDoc.prepare("SELECT (SELECT o.dateobuch  FROM obuchlul AS o WHERE o.obuchlulid = ot.obuchlulid)"
//                             "FROM obuchlultable AS ot WHERE ot.employeeid = :id");
//            queryDoc.bindValue(":id",employeeId);
//            queryDoc.exec();
//            queryDoc.next();
//            if(queryDoc.isValid()){
//                QString tempString = query.value(1).toString();
//                tempString += QObject::tr(" trained - ");
//                tempString += queryDoc.value(0).toDate().toString("dd.MM.yyyy");
//                QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
//                insert = false;
//            }
            if(insert){
                addIntoTable = true;
                employeeView->insertRow(rowCount);

                QTableWidgetItem *itemID = new QTableWidgetItem;
                employeeView->setItem(rowCount,0,itemID);
                employeeView->item(rowCount,0)->setText(query.value(0).toString());

                QTableWidgetItem *itemName = new QTableWidgetItem;
                //itemName->setTextAlignment(Qt::AlignCenter);
                employeeView->setItem(rowCount,1,itemName);
                employeeView->item(rowCount,1)->setText(query.value(1).toString());

//                QSqlQuery queryPost;
//                queryPost.prepare("SELECT postname FROM post WHERE postid = :id");
//                queryPost.bindValue(":id",query.value(5).toString());
//                queryPost.exec();
//                queryPost.next();

//                QTableWidgetItem *itemPost = new QTableWidgetItem;
//                employeeView->setItem(rowCount,2,itemPost);
//                employeeView->item(rowCount,2)->setText(queryPost.value(0).toString());

//                NumPrefix numPrefix;
//                QString idTable = numPrefix.getPrefix("obuchlultable");

//                QSqlQuery queryPSL;
//                queryPSL.prepare("INSERT INTO obuchlultable ("
//                                "obuchlultableid, obuchlulid, employeeid"
//                                ") VALUES(:obuchlultableid, :obuchlulid, :employeeid)");
//                queryPSL.bindValue(":obuchlultableid",idTable);
//                queryPSL.bindValue(":obuchlulid",indexTemp);
//                queryPSL.bindValue(":employeeid",query.value(0).toString());
//                queryPSL.exec();
//                if(!queryPSL.isActive()){
//                    QMessageBox::warning(this,QObject::tr("Obuch Lul List Table, INSERT ERROR!"),queryPSL.lastError().text());
//                    return;
//                }
//                line += "INSERT INTO obuchlultable (obuchlultableid, obuchlulid, employeeid) VALUES('";
//                line += idTable.toUtf8();
//                line += "', '";
//                line += indexTemp.toUtf8();
//                line += "', '";
//                line += query.value(0).toString().toUtf8();
//                line += "')";
//                line += "\r\n";
//                stream<<line;
                employeeView->setCurrentItem(itemName);
            }
        }
    }
}

void ScoreForm::deleteRecordOfTable()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

//    int k = QMessageBox::question(this,tr("Attention!!"),
//                              tr("Really delete?"),
//                                  QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
//    if(k == QMessageBox::Yes){
//        QSqlQuery query;
//        query.prepare("DELETE FROM obuchlultable WHERE (obuchlulid = :obuchlulid AND employeeid = :employeeid)");
//        query.bindValue(":obuchlulid",indexTemp);
//        query.bindValue(":employeeid",employeeView->item(employeeView->currentRow(),0)->text());
//        query.exec();

//        if(!query.isActive()){
//            QMessageBox::warning(this,QObject::tr("Post Siz Table, DELETE ERROR!"),
//                                 query.lastError().text());
//            return;
//        }
//        line += "DELETE FROM obuchlultable WHERE (obuchlulid = '";
//        line += indexTemp.toUtf8();
//        line += "' AND employeeid = '";
//        line += employeeView->item(employeeView->currentRow(),0)->text().toUtf8();
//        line += "')";
//        line += "\r\n";
//        stream<<line;
//        employeeView->removeRow(employeeView->currentRow());
//        employeeView->repaint();
//    }
}

void ScoreForm::editRecordOfTable()
{
    QMessageBox::warning(this,"Attention!!!","You are not able to edit!");
}

bool ScoreForm::checkingFill()
{
    bool fill = true;
    if(editOrganization->text() == ""){
        editOrganization->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editOrganization->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editScore->text() == ""){
        editScore->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editScore->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}

void ScoreForm::updateTable()
{

}

void ScoreForm::createContextMenu()
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

    employeeView->addAction(addAction);
    employeeView->addAction(deleteAction);
    employeeView->addAction(editAction);
    employeeView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void ScoreForm::printTable()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    QPrintPreviewDialog preview(&printer,this);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(print(QPrinter*)));
    preview.showMaximized();
    preview.exec();
}

int ScoreForm::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
{
    QTextDocument document;
    document.setPlainText(paragraphString);
    int y = 0;
    QTextBlock block = document.begin();
    int x = 0;
    for(QTextBlock::iterator i = block.begin(); !i.atEnd(); ++i)
    {
        QTextFragment fragment = i.fragment();
        if(fragment.isValid()){
            foreach (QString word, fragment.text().split(QRegExp("\\s+"))) {
                int widthW = painter->fontMetrics().width(word);
                if(x + widthW + spacing*2 > rect->width()){
                    x = 0;
                    y += painter->fontMetrics().lineSpacing();
                }else if(x != rect->x()){
                    word.prepend(" ");
                }
                x += painter->fontMetrics().width(word);
            }
        }
    }
    return y += painter->fontMetrics().lineSpacing();
}

void ScoreForm::print(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    int heightPage = viewRegion.height();
    //int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawStaticText(3000,0,QObject::trUtf8("Приложение №2 к договору"));
    //QRect secondString(0,180,widthPage,100);
    //painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    //painter.drawStaticText(3000,120,editDogovor->text());
    painter.setFont(QFont("Times New Roman",12));
    painter.drawStaticText(150,240,QObject::trUtf8("Перечень сотрудников направляемых на обучение по курсу \"Безопасное выполнение работ,\n"
                                                   "в качестве"));
    QString val = QObject::trUtf8("рабочего люльки\" в количестве ");
    val += QString::number(employeeView->rowCount());
    val += QObject::trUtf8(" человек(а)");
    painter.drawStaticText(0,360,val);


    int shapkaTop = 250;
    int shapkaBottom = 380;

    QRect numberCol(0,shapkaTop,200,shapkaBottom);
    QRect fioCol(200,shapkaTop,1500,shapkaBottom);
    QRect postCol(1700,shapkaTop,1300,shapkaBottom);
    QRect prichinaCol(3000,shapkaTop,750,shapkaBottom);
    QRect vidCol(3750,shapkaTop,940,shapkaBottom);


    //************************************************************
    //painter.drawRect(allCol);
    //painter.drawText(allCol,Qt::AlignCenter,tr("Clothes"));

    int newLineHight;
    int mHight;

    //************************************************************
    newLineHight = numberCol.bottom();
    mHight = numberCol.height();

    numberCol.moveTop(newLineHight);
    numberCol.setHeight(mHight);
    painter.drawRect(numberCol);
    painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(numberCol,Qt::AlignCenter,trUtf8("№\n п/п"));

    fioCol.moveTop(newLineHight);
    fioCol.setHeight(mHight);
    painter.drawRect(fioCol);
    painter.drawText(fioCol,Qt::AlignCenter,trUtf8("ФИО"));

    postCol.moveTop(newLineHight);
    postCol.setHeight(mHight);
    painter.drawRect(postCol);
    painter.drawText(postCol,Qt::AlignCenter,trUtf8("Должность"));

    prichinaCol.moveTop(newLineHight);
    prichinaCol.setHeight(mHight);
    painter.drawRect(prichinaCol);
    painter.drawText(prichinaCol,Qt::AlignCenter,trUtf8("Причина проверки"));

    vidCol.moveTop(newLineHight);
    vidCol.setHeight(mHight);
    painter.drawRect(vidCol);
    painter.drawText(vidCol,Qt::AlignCenter,trUtf8("Вид аттестации"));

    //************************************************************
    //int currentRow = 0;

    //newLineHight = shapkaTop+shapkaBottom;
    //int mHight;
    int spacing = 30;

    for(int currentRow = 0; currentRow - employeeView->rowCount(); ++currentRow){
        if(newLineHight > heightPage - 200){
            newLineHight = 0;
            printer->newPage();
        }

        QString sotrudnik = employeeView->item(currentRow,1)->text();
        mHight = qMax(printParagraph(QString::number(currentRow + 1),&painter,&numberCol,spacing),
                      printParagraph(sotrudnik,&painter,&fioCol,spacing));
        mHight = qMax(mHight,printParagraph(employeeView->item(currentRow,2)->text(),&painter,&postCol,spacing));
      //  mHight = qMax(mHight,printParagraph(editPrichina->text(),&painter,&prichinaCol,spacing));
      //  mHight = qMax(mHight,printParagraph(editVidAtest->text(),&painter,&vidCol,spacing));

        //newLineHight = numberCol.bottom();
        //mHight = numberCol.height();

        numberCol.moveTop(newLineHight);
        numberCol.setHeight(mHight);
        painter.drawRect(numberCol);
        painter.setFont(QFont("Times New Roman",9));
        painter.drawText(numberCol,Qt::AlignCenter,QString::number(currentRow + 1));

        fioCol.moveTop(newLineHight);
        fioCol.setHeight(mHight);
        painter.drawRect(fioCol);
        QRect tempCol = fioCol;
        tempCol.setLeft(fioCol.left()+spacing);
        painter.drawText(tempCol,Qt::AlignLeft | Qt::AlignVCenter,sotrudnik);
        //painter.drawText(fioCol,Qt::AlignLeft | Qt::AlignVCenter,sotrudnik);

        postCol.moveTop(newLineHight);
        postCol.setHeight(mHight);
        painter.drawRect(postCol);
        tempCol = postCol;
        tempCol.setLeft(postCol.left()+spacing);
        painter.drawText(tempCol,employeeView->item(currentRow,2)->text());

        prichinaCol.moveTop(newLineHight);
        prichinaCol.setHeight(mHight);
        painter.drawRect(prichinaCol);
      //  painter.drawText(prichinaCol,editPrichina->text());

        vidCol.moveTop(newLineHight);
        vidCol.setHeight(mHight);
        painter.drawRect(vidCol);
    //    painter.drawText(vidCol,editVidAtest->text());

        newLineHight = numberCol.bottom();
    }
}
