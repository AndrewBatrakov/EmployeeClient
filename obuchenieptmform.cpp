#include "obuchenieptmform.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"
#include <QAxObject>

ObucheniePtmForm::ObucheniePtmForm(QString id, QWidget *parent, bool onlyForRead) : QDialog(parent)
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

    labelDogovor = new QLabel(trUtf8("Договор:"));
    editDogovor = new LineEdit;
    editDogovor->setReadOnly(onlyForRead);
    //QRegExp regExp("[\\x0410-\\x044f 0-9 ()\" -]{150}");
    //editPostSIZ->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelDogovor->setBuddy(editDogovor);

    labelDateDoc = new QLabel(trUtf8("Дата:"));
    editDateDoc = new QDateEdit;
    editDateDoc->setCalendarPopup(true);

    labelDateCor = new QLabel(trUtf8("Корректировка:"));
    editDateCor = new QDateEdit;
    editDateCor->setCalendarPopup(true);

    labelDateObuch = new QLabel(trUtf8("Обучение:"));
    editDateObuch = new QDateEdit;
    editDateObuch->setCalendarPopup(true);

    labelProtokolObuch = new QLabel(trUtf8("Протокол №:"));
    editProtocolObuch = new LineEdit;

    QHBoxLayout *dateLayout = new QHBoxLayout;
    //dateLayout->addWidget(labelDateDoc);
    dateLayout->addWidget(editDateDoc);
    dateLayout->addWidget(labelDateCor);
    dateLayout->addWidget(editDateCor);
    dateLayout->addWidget(labelDateObuch);
    dateLayout->addWidget(editDateObuch);
    dateLayout->addStretch();

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

    QPushButton *exportTableButton = new QPushButton(tr("Excel"));
    QPixmap excelPrint(":/microsoft_excel.png");
    exportTableButton->setIcon(excelPrint);
    connect(exportTableButton,SIGNAL(clicked()),this,SLOT(excelExport()));

    QHBoxLayout *buttonTableBox = new QHBoxLayout;
    buttonTableBox->addWidget(addTableButton);
    buttonTableBox->addWidget(deleteTableButton);
    buttonTableBox->addWidget(editTableButton);
    buttonTableBox->addWidget(printTableButton);
    buttonTableBox->addWidget(exportTableButton);
    buttonTableBox->addStretch();

    //*****************************************************
    //employee TableView
    //*****************************************************
    employeeView = new QTableWidget;
    employeeView->setColumnCount(3);
    employeeView->setHorizontalHeaderLabels(QStringList()<<tr("ID")<<tr("FIO")<<tr("Post"));
    QHeaderView *head = employeeView->horizontalHeader();
    head->setStretchLastSection(true);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->setColumnHidden(0,true);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM obuchptm WHERE obuchptmid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editNumber->setText(indexTemp);
            editDogovor->setText(query.value(1).toString());
            editDateDoc->setDate(query.value(2).toDate());
            editDateCor->setDate(query.value(3).toDate());
            editDateObuch->setDate(query.value(4).toDate());
            editProtocolObuch->setText(query.value(7).toString());
            QSqlQuery queryID;
            queryID.prepare("SELECT * FROM obuchptmtable WHERE obuchptmid = :obuchid");
            queryID.bindValue(":obuchid",indexTemp);
            queryID.exec();
            int rowCount = 0;
            while(queryID.next()){
                QSqlQuery queryName;
                queryName.prepare("SELECT employee.employeename, (SELECT post.postname FROM post WHERE post.postid = employee.postid), "
                                  "employee.employeeid "
                                  "FROM employee WHERE employee.employeeid = :id");
                queryName.bindValue(":id",queryID.value(2).toString());
                queryName.exec();
                queryName.next();

                employeeView->insertRow(rowCount);

                QTableWidgetItem *itemID = new QTableWidgetItem;
                employeeView->setItem(rowCount,0,itemID);
                employeeView->item(rowCount,0)->setText(queryName.value(2).toString());

                QTableWidgetItem *itemFIO = new QTableWidgetItem;
                employeeView->setItem(rowCount,1,itemFIO);
                employeeView->item(rowCount,1)->setText(queryName.value(0).toString());

                QTableWidgetItem *itemPost = new QTableWidgetItem;
                employeeView->setItem(rowCount,2,itemPost);
                employeeView->item(rowCount,2)->setText(queryName.value(1).toString());

                ++rowCount;
            }
        }
    }else{
        editDogovor->clear();
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("obuchptm");
        updateRecord = false;
        editDateDoc->setDate(QDate::currentDate());
        editDateCor->setDate(QDate::currentDate());
        editDateObuch->setDate(QDate::currentDate());
    }


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelNumber,0,0);
    mainLayout->addWidget(editNumber,0,1);
    mainLayout->addWidget(labelDateDoc,1,0);
    mainLayout->addLayout(dateLayout,1,1,1,2);
    mainLayout->addWidget(labelDogovor,2,0);
    mainLayout->addWidget(editDogovor,2,1);
    mainLayout->addWidget(labelProtokolObuch,3,0);
    mainLayout->addWidget(editProtocolObuch,3,1);
    mainLayout->addLayout(buttonTableBox,4,0,1,2);
    mainLayout->addWidget(employeeView,5,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,6,1);
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Обучение ПТМ"));
    readSettings();
    createContextMenu();
    editNumber->setText(indexTemp);
    employeeView->setColumnWidth(1,300);
}

void ObucheniePtmForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void ObucheniePtmForm::saveRecord()
{
    editRecord();
    emit accept();
}

void ObucheniePtmForm::cancelRecord()
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
            queryCD.prepare("DELETE FROM obuchptmtable WHERE obuchptmid = :id");
            queryCD.bindValue(":id",indexTemp);
            queryCD.exec();
            if(!queryCD.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("Obuc PTM Table, DELETE ERROR!"),queryCD.lastError().text());
                return;
            }
            line += "DELETE FROM obuchptmtable WHERE obuchptmid = '";
            line += indexTemp.toUtf8();
            line += "'";
            line += "\r\n";
            stream<<line;
        }
    }
    emit accept();
}

void ObucheniePtmForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSqlQuery queryIns;
    queryIns.prepare("SELECT * FROM obuchptm WHERE (obuchptmid = :id AND datedoc = :datedoc)");
    queryIns.bindValue(":id",indexTemp);
    queryIns.bindValue(":datedoc",editDateDoc->date());
    queryIns.exec();
    queryIns.next();
    if(queryIns.isValid()){
        QSqlQuery query;
        query.prepare("UPDATE obuchptm SET "
                      "dogovor = :dogovor, "
                      "datedoc = :datedoc, "
                      "datecor = :datecor, "
                      "dateobuch = :dateobuch, "
                      "numberbuch = :numberbuch "
                      "WHERE obuchptmid = :obuchptmid");
        query.bindValue(":obuchptmid",indexTemp);
        query.bindValue(":dogovor",editDogovor->text().simplified());
        query.bindValue(":datedoc",editDateDoc->date());
        query.bindValue(":datecor",editDateCor->date());
        query.bindValue(":dateobuch",editDateObuch->date());
        query.bindValue(":numberbuch",editProtocolObuch->text().simplified());
        query.exec();
        line += "UPDATE obuchptm SET dogovor = '";
        line += editDogovor->text().simplified().toUtf8();
        line += "', ";
        line += "datedoc = '";
        line += editDateDoc->date().toString("yyyy-MM-dd");
        line += "', ";
        line += "datecor = '";
        line += editDateCor->date().toString("yyyy-MM-dd");
        line += "', ";
        line += "dateobuch = '";
        line += editDateObuch->date().toString("yyyy-MM-dd");
        line += "', ";
        line += "numberbuch = '";
        line += editProtocolObuch->text().toUtf8();
        line += "' WHERE obuchptmid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Obuch PTM, UPDATE ERROR!"),query.lastError().text());
            return;
        }
    }else{
        QSqlQuery queryInsert;
        queryInsert.prepare("INSERT INTO obuchptm ("
                            "obuchptmid, "
                            "dogovor, "
                            "datedoc, "
                            "datecor, "
                            "dateobuch, "
                            "numberbuch"
                            ") VALUES(:obuchptmid, "
                            ":dogovor, "
                            ":datedoc, "
                            ":datecor, "
                            ":dateobuch, "
                            ":numberbuch"
                            ");");
        queryInsert.bindValue(":obuchptmid",indexTemp);
        queryInsert.bindValue(":dogovor",editDogovor->text().simplified());
        queryInsert.bindValue(":datedoc",editDateDoc->date());
        queryInsert.bindValue(":datecor",editDateCor->date());
        queryInsert.bindValue(":dateobuch",editDateObuch->date());
        queryInsert.bindValue(":numberbuch",editProtocolObuch->text().simplified());

        line += "INSERT INTO obuchptm (obuchptmid, dogovor, datedoc, datecor, "
                "dateobuch, numberbuch) VALUES('";
        line += indexTemp.toUtf8();
        line += "', '";
        line += editDogovor->text().simplified().toUtf8();
        line += "', '";
        line += editDateDoc->date().toString("yyyy-MM-dd");
        line += "', '";
        line += editDateCor->date().toString("yyyy-MM-dd");
        line += "', '";
        line += editDateObuch->date().toString("yyyy-MM-dd");
        line += "', '";
        line += editProtocolObuch->text().toUtf8();
        queryInsert.exec();
        line += "')";
        line += "\r\n";
        stream<<line;
        if(!queryInsert.isActive()){
            QMessageBox::warning(this,QObject::tr("Obuch PTM, INSERT ERROR!"),queryInsert.lastError().text());
            return;
        }
    }
}


void ObucheniePtmForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "obuchptm");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"obuchptm",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM obuchptm WHERE obuchptmid = :obuchptmid");
        query.bindValue(":obuchptmid",indexTemp);
        query.exec();
        query.next();

        line += "DELETE FROM obuchptm WHERE obuchptmid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }

    settings.remove("Table");
    settings.remove("Index");
}

void ObucheniePtmForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("ObuchPtmForm").toByteArray());
}

void ObucheniePtmForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("ObuchPtmForm", saveGeometry());
}

void ObucheniePtmForm::addRecordOfTable()
{
    if(checkingFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }

        ViewListTable listTable("","employee",this);
        listTable.exec();
        QString employeeId = listTable.returnValue();

        QSqlQuery query;
        query.prepare("SELECT * FROM employee WHERE employeeid = :id");
        query.bindValue(":id",employeeId);
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
            QSqlQuery queryDoc;
            queryDoc.prepare("SELECT (SELECT o.dateobuch  FROM obuchptm AS o WHERE o.obuchptmid = ot.obuchptmid)"
                             "FROM obuchptmtable AS ot WHERE ot.employeeid = :id");
            queryDoc.bindValue(":id",employeeId);
            queryDoc.exec();
            queryDoc.next();
            if(queryDoc.isValid()){
                QString tempString = query.value(1).toString();
                tempString += QObject::tr(" trained - ");
                tempString += queryDoc.value(0).toDate().toString("dd.MM.yyyy");
                QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
                insert = false;
            }
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

                QSqlQuery queryPost;
                queryPost.prepare("SELECT postname FROM post WHERE postid = :id");
                queryPost.bindValue(":id",query.value(5).toString());
                queryPost.exec();
                queryPost.next();

                QTableWidgetItem *itemPost = new QTableWidgetItem;
                employeeView->setItem(rowCount,2,itemPost);
                employeeView->item(rowCount,2)->setText(queryPost.value(0).toString());

                NumPrefix numPrefix;
                QString idTable = numPrefix.getPrefix("obuchptmtable");

                QSqlQuery queryPSL;
                queryPSL.prepare("INSERT INTO obuchptmtable ("
                                "obuchptmtableid, obuchptmid, employeeid"
                                ") VALUES(:obuchptmtableid, :obuchptmid, :employeeid)");
                queryPSL.bindValue(":obuchptmtableid",idTable);
                queryPSL.bindValue(":obuchptmid",indexTemp);
                queryPSL.bindValue(":employeeid",query.value(0).toString());
                queryPSL.exec();
                if(!queryPSL.isActive()){
                    QMessageBox::warning(this,QObject::tr("Obuch PTM List Table, INSERT ERROR!"),queryPSL.lastError().text());
                    return;
                }
                line += "INSERT INTO obuchptmtable (obuchptmtableid, obuchptmid, employeeid) VALUES('";
                line += idTable.toUtf8();
                line += "', '";
                line += indexTemp.toUtf8();
                line += "', '";
                line += query.value(0).toString().toUtf8();
                line += "')";
                line += "\r\n";
                stream<<line;
                employeeView->setCurrentItem(itemName);
            }
        }
    }
}

void ObucheniePtmForm::deleteRecordOfTable()
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
        query.prepare("DELETE FROM obuchptmtable WHERE (obuchptmid = :obuchptmid AND employeeid = :employeeid)");
        query.bindValue(":obuchptmid",indexTemp);
        query.bindValue(":employeeid",employeeView->item(employeeView->currentRow(),0)->text());
        query.exec();

        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Obuch PTM Table, DELETE ERROR!"),
                                 query.lastError().text());
            return;
        }
        line += "DELETE FROM obuchptmtable WHERE (obuchptmid = '";
        line += indexTemp.toUtf8();
        line += "' AND employeeid = '";
        line += employeeView->item(employeeView->currentRow(),0)->text().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
        employeeView->removeRow(employeeView->currentRow());
        employeeView->repaint();
    }
}

void ObucheniePtmForm::editRecordOfTable()
{
    QMessageBox::warning(this,"Attention!!!","You are not able to edit!");
}

bool ObucheniePtmForm::checkingFill()
{
    bool fill = true;
    if(editDogovor->text() == ""){
        editDogovor->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editDogovor->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}

void ObucheniePtmForm::updateTable()
{

}

void ObucheniePtmForm::createContextMenu()
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

void ObucheniePtmForm::printTable()
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

int ObucheniePtmForm::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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

void ObucheniePtmForm::print(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    int heightPage = viewRegion.height();
    //int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawStaticText(3000,0,QObject::trUtf8("Приложение №3 к договору"));
    //QRect secondString(0,180,widthPage,100);
    //painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawStaticText(3000,120,editDogovor->text());
    painter.setFont(QFont("Times New Roman",12));
    painter.drawStaticText(150,240,QObject::trUtf8("Перечень сотрудников направляемых на обучение и проверку знаний "
                                                   "\"Пожарно-технического минимума\"\n"));
    QString val = QObject::trUtf8("в количестве ");
    val += QString::number(employeeView->rowCount());
    val += QObject::trUtf8(" человек(а)");
    painter.drawStaticText(0,360,val);


    int shapkaTop = 250;
    int shapkaBottom = 380;

    QRect numberCol(0,shapkaTop,200,shapkaBottom);
    QRect fioCol(200,shapkaTop,1500,shapkaBottom);
    QRect postCol(1700,shapkaTop,1300,shapkaBottom);


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

    //************************************************************
    //int currentRow = 0;

    newLineHight = numberCol.bottom();//shapkaTop+shapkaBottom;
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

        newLineHight = numberCol.bottom();
    }
}

void ObucheniePtmForm::excelExport()
{
    QAxObject *excel = new QAxObject("Excel.Application",this);
    excel->dynamicCall("SetVisible(bool)",true);
    QAxObject *workbooks = excel->querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Add()");
    QAxObject *sheets = workbook->querySubObject("Sheets");
    QAxObject *sheet = sheets->querySubObject("Item(1)");
    sheet->dynamicCall("Select()");
    //sheet->dynamicCall("SetStandardFontSize(double",9);

    QAxObject *cell = sheet->querySubObject("cells(1, 1)");
    cell->dynamicCall("Select()");
    QString rr = trUtf8("Протокол №");
    rr += editProtocolObuch->text().toUtf8();
    cell->setProperty("Value", rr);
    QAxObject *font = cell->querySubObject("Font");
    font->setProperty("Bold",true);
    font->setProperty("Size",12);
    cell->setProperty("ColumnWidth",54);

    for(int currentRow = 0; currentRow - employeeView->rowCount(); ++currentRow){
        QAxObject *cel = sheet->querySubObject("cells(int, int)", currentRow + 2,1);
        QString s = employeeView->item(currentRow,1)->text();
        cel->setProperty("Value",s);
    }
    for(int currentRow = 0; currentRow - employeeView->rowCount(); ++currentRow){
        QAxObject *cel = sheet->querySubObject("cells(int, int)", currentRow + 2,2);
        QString s = employeeView->item(currentRow,2)->text();
        cel->setProperty("Value",s);
    }
}
