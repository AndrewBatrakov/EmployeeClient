#include <QtGui>
#include <QtSql>

#include "trainingsafety.h"
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"
#include "viewlisttable.h"
#include "venue.h"
#include "employeecertification.h"

TrainingSafety::TrainingSafety(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();

    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString toolButtonStyle = QLatin1String(file.readAll());

    addIntoTable = false;

    //Header Of Document
    indexTemp = id;

    labelNumberDoc = new QLabel(tr("Number:"));
    editNumberDoc = new QLabel;
    editNumberDoc->setStyleSheet("font: bold; color: darkblue;");
    labelNumberDoc->setBuddy(editNumberDoc);

    labelDateDoc = new QLabel(tr("Date:"));
    editDateDoc = new QDateTimeEdit;
    editDateDoc->setCalendarPopup(true);
    editDateDoc->setReadOnly(onlyForRead);
    editDateDoc->setDateTime(QDateTime::currentDateTime());
    labelDateDoc->setBuddy(editDateDoc);


    labelVenue = new QLabel(tr("Venue:"));
    editVenue = new LineEdit;
    editVenue->setReadOnly(onlyForRead);

    QSqlQueryModel *venueModel = new QSqlQueryModel;
    venueModel->setQuery("SELECT venuename FROM venue");
    QCompleter *venueCompleter = new QCompleter(venueModel);
    venueCompleter->setCompletionMode(QCompleter::PopupCompletion);
    venueCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editVenue->setCompleter(venueCompleter);

    QToolButton *addVenueButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addVenueButton->setIcon(addPix);
    addVenueButton->setToolTip(tr("Add new record"));
    addVenueButton->setStyleSheet(toolButtonStyle);
    connect(addVenueButton,SIGNAL(clicked()),this,SLOT(addVenueRecord()));

    QToolButton *seeVenueButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeVenueButton->setIcon(seePix);
    seeVenueButton->setToolTip(tr("See select item"));
    seeVenueButton->setStyleSheet(toolButtonStyle);
    connect(seeVenueButton,SIGNAL(clicked()),this,SLOT(seeVenueRecord()));

    QToolButton *listVenueButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listVenueButton->setIcon(listPix);
    listVenueButton->setToolTip(tr("See list of item"));
    listVenueButton->setStyleSheet(toolButtonStyle);
    connect(listVenueButton,SIGNAL(clicked()),this,SLOT(listVenueRecord()));


    labelVenueDate = new QLabel(tr("Venue Date:"));
    editVenueDate = new QDateEdit;
    editVenueDate->setReadOnly(onlyForRead);
    editVenueDate->setCalendarPopup(true);
    editVenueDate->setDate(QDate::currentDate());
    labelVenueDate->setBuddy(editVenueDate);

    labelProtokol = new QLabel(tr("N protokola:"));
    editProtokol = new LineEdit;
    editProtokol->setReadOnly(onlyForRead);
    labelProtokol->setBuddy(editProtokol);

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

    //Section Of Document

    QPushButton *addTableLine = new QPushButton(tr("Add record"));
    addTableLine->setIcon(addPix);
    connect(addTableLine,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPushButton *deleteTableLine = new QPushButton(tr("Delete record"));
    QPixmap pixDelete(":/delete.png");
    deleteTableLine->setIcon(pixDelete);
    deleteTableLine->setStyleSheet("QPushButton:hover {color: red}");
    connect(deleteTableLine,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QPushButton *printButton = new QPushButton(tr("Print"));
    QPixmap pixPrint(":/print.png");
    printButton->setIcon(pixPrint);
    connect(printButton,SIGNAL(clicked()),this,SLOT(printPreviewTS()));

    tableWidget = new QTableWidget(0,3);

    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("FIO")<<tr("Cipher"));
    QHeaderView *head = tableWidget->horizontalHeader();
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *tableButtonBox = new QHBoxLayout;
    tableButtonBox->addWidget(addTableLine);
    tableButtonBox->addWidget(deleteTableLine);
    tableButtonBox->addStretch();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelNumberDoc,0,0,Qt::AlignRight);
    mainLayout->addWidget(editNumberDoc,0,1,1,4);
    mainLayout->addWidget(labelDateDoc,1,0,Qt::AlignRight);
    mainLayout->addWidget(editDateDoc,1,1,1,4);
    mainLayout->addWidget(labelVenue,2,0,Qt::AlignRight);
    mainLayout->addWidget(editVenue,2,1);
    mainLayout->addWidget(addVenueButton,2,2);
    mainLayout->addWidget(seeVenueButton,2,3);
    mainLayout->addWidget(listVenueButton,2,4);
    mainLayout->addWidget(labelVenueDate,3,0,Qt::AlignRight);
    mainLayout->addWidget(editVenueDate,3,1,1,4);
    mainLayout->addWidget(labelProtokol,4,0);
    mainLayout->addWidget(editProtokol,4,1,1,4);
    if(!onlyForRead){
        mainLayout->addLayout(tableButtonBox,5,0,1,5);
    }
    mainLayout->addWidget(tableWidget,6,0,1,5);
    if(!onlyForRead){

        mainLayout->addWidget(buttonBox,7,0,1,5);
        mainLayout->addWidget(printButton,7,0);
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM trainingsafety WHERE trainingsafetyid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editNumberDoc->setText(indexTemp);
            editDateDoc->setDateTime(query.value(1).toDateTime());
            idVenue = query.value(2).toString();
            QSqlQuery venueQuery;
            venueQuery.prepare("SELECT venuename FROM venue WHERE venueid = :id;");
            venueQuery.bindValue(":id",idVenue);
            venueQuery.exec();
            venueQuery.next();
            editVenue->setText(venueQuery.value(0).toString());
            editVenueDate->setDate(query.value(3).toDate());

            QSqlQuery queryTable;
            queryTable.prepare("SELECT * FROM trainingsafetytable WHERE trainingsafetyid = :idt;");
            queryTable.bindValue(":idt",indexTemp);
            queryTable.exec();
            if(!queryTable.isActive()){
                QMessageBox::warning(this,QObject::tr("EmpCert Table, SELECT Table ERROR!"),queryTable.lastError().text());
            }

            int row = 0;
            while(queryTable.next()){

                QSqlQuery queryName;
                queryName.prepare("SELECT employeename FROM employee WHERE employeeid IN "
                                  "(SELECT employeeid FROM empcert WHERE empcertid = :id);");
                queryName.bindValue(":id",queryTable.value(2).toString());
                queryName.exec();
                if(!queryName.isActive()){
                    QMessageBox::warning(this,QObject::tr("EmpCert Table, SELECT Testtask ERROR!"),queryName.lastError().text());
                    return;
                }
                while(queryName.next()){
                    tableWidget->insertRow(row);
                    QTableWidgetItem *item = new QTableWidgetItem;
                    tableWidget->setItem(row,0,item);
                    tableWidget->item(row,0)->setText(queryName.value(0).toString());

                    QString ciperString = "";
                    QSqlQuery queryCipher;
                    queryCipher.prepare("(SELECT cipher FROM testtask AS ttt , empcertdate AS aaa, empcerttable AS qqq "
                                        "WHERE (qqq.empcertdateid = aaa.empcertdateid) AND "
                                        "(ttt.testtaskid = aaa.testtaskid) AND empcertid = :id) ORDER BY cipher ASC;");
                    queryCipher.bindValue(":id",queryTable.value(2).toString());
                    queryCipher.exec();
                    if(!queryCipher.isActive()){
                        QMessageBox::warning(this,QObject::tr("EmpCert Table, SELECT Cipher ERROR!"),queryCipher.lastError().text());
                    }
                    while(queryCipher.next()){
                        ciperString += queryCipher.value(0).toString();
                        ciperString += ", ";
                    }
                    ciperString.replace(ciperString.length()-2,1,".");
                    QTableWidgetItem *item1 = new QTableWidgetItem;
                    tableWidget->setItem(row,1,item1);
                    tableWidget->item(row,1)->setText(ciperString);

                    //empcertid
                    QTableWidgetItem *item2 = new QTableWidgetItem;
                    tableWidget->setItem(row,2,item2);
                    QSqlQuery queryC;
                    queryC.prepare("SELECT postid FROM employee AS emp, empcert AS cert WHERE "
                                   "(emp.employeeid = cert.employeeid) AND empcertid = :id;");
                    queryC.bindValue(":id",queryTable.value(2).toString());
                    queryC.exec();
                    queryC.next();
                    if(!queryC.isActive()){
                        QMessageBox::warning(this,QObject::tr("Post ID, SELECT ERROR!"),queryC.lastError().text());
                    }
                    tableWidget->item(row,2)->setText(queryC.value(0).toString());
                    ++row;
                }
            }
        }
    }else{
        NumPrefix numPrefix;
        indexTemp = numPrefix.getPrefix("trainingsafety");
        if(indexTemp == ""){
            close();
        }
    }
    tableWidget->setAlternatingRowColors(true);
    tableWidget->resizeColumnsToContents();
    tableWidget->setColumnHidden(4,true);
    //tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    head->setStretchLastSection(true);
    setLayout(mainLayout);
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editRecordOfTable()));

    setWindowTitle(tr("Training Safety"));
}

void TrainingSafety::editRecord()
{
    if(checkingFill()){
        if(updateRecord){
            QSqlQuery query;
            query.prepare("UPDATE trainingsafety SET "
                          "trainingsafetydate = :trainingsafetydate, "
                          "venueid = :venueid, "
                          "venuedate = :venuedate, "
                          "protokolnumber = :protokolnumber "
                          "WHERE trainingsafetyid = :id");
            query.bindValue(":trainingsafetydate",editDateDoc->dateTime());
            query.bindValue(":venueid",idVenue);
            query.bindValue(":venuedate",editVenueDate->date());
            query.bindValue(":id",indexTemp);
            query.bindValue(":protokolnumber",editProtokol->text().simplified());
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::tr("Traning Safety, UPDATE ERROR!"),query.lastError().text());
            }
        }else{
            QSqlQuery query;
            query.prepare("SELECT * FROM trainingsafety WHERE trainingsafetyid = :id");
            query.bindValue(":id",indexTemp);
            query.exec();
            query.next();
            if(!query.isValid()){
                QSqlQuery queryInsert;
                queryInsert.prepare("INSERT INTO trainingsafety ("
                                    "trainingsafetyid, "
                                    "trainingsafetydate, "
                                    "venueid, "
                                    "venuedate, "
                                    "protokolnumber"
                                    ") VALUES(:id, "
                                    ":trainingsafetydate, "
                                    ":venueid, "
                                    ":venuedate, "
                                    ":protokolnumber"
                                    ");");
                queryInsert.bindValue(":id",indexTemp);
                queryInsert.bindValue(":trainingsafetydate",editDateDoc->dateTime());
                queryInsert.bindValue(":venueid",idVenue);
                queryInsert.bindValue(":venuedate",editVenueDate->date());
                queryInsert.bindValue(":protokolnumber",editProtokol->text().simplified());
                queryInsert.exec();
                if(!queryInsert.isActive()){
                    QMessageBox::warning(this,QObject::tr("Traning Safety, INSERT ERROR!"),queryInsert.lastError().text());
                }
            }
        }
    }
}

void TrainingSafety::deleteRecord()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("Table", "trainingsafety");
    settings.setValue("Index",indexTemp);

    ForDelete forDelete(indexTemp,"trainingsafety",this);

    forDelete.exec();
    int k = QMessageBox::warning(this,tr("Attention!!!"),tr("Delete item with the replacement for default value?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();
        QSqlQuery query;
        query.prepare("DELETE FROM trainingsafety WHERE trainingsafetyid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }

    settings.remove("Table");
    settings.remove("Index");
}

void TrainingSafety::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void TrainingSafety::addRecordOfTable()
{
    if(checkingFill()){
        ViewListTable openForm("","empcert",this);
        //EmployeeCertification openForm("",this,false);
        openForm.exec();
        QString empCertId = openForm.returnValue();

        QSqlQuery query;
        query.prepare("SELECT * FROM empcert WHERE empcertid = :id");
        query.bindValue(":id",empCertId);
        query.exec();
        while(query.next()){
            int row = tableWidget->rowCount();
            bool insert = true;
            if (row != 0){
                for(int kk = 0; kk < row; ++kk){
                    QString yy = tableWidget->item(kk,0)->text();
                    QString pp = query.value(2).toString();
                    if(yy == pp){
                        QString tempString = query.value(2).toString();
                        tempString += QObject::tr(" is availble!");
                        QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
                        insert = false;
                        break;
                    }
                }
            }
            if(insert){
                addIntoTable = true;
                QSqlQuery queryName;
                queryName.prepare("SELECT employeename FROM employee WHERE employeeid IN (SELECT employeeid FROM empcert WHERE empcertid = :id)");
                queryName.bindValue(":id",empCertId);
                queryName.exec();
                if(!queryName.isActive()){
                    QMessageBox::warning(this,QObject::tr("EmpCert Table, SELECT Testtask ERROR!"),queryName.lastError().text());
                }
                int row = tableWidget->rowCount();
                while(queryName.next()){
                    tableWidget->insertRow(row);
                    QTableWidgetItem *item = new QTableWidgetItem;
                    tableWidget->setItem(row,0,item);
                    tableWidget->item(row,0)->setText(queryName.value(0).toString());

                    QString ciperString = "";
                    QSqlQuery queryCipher;
                    queryCipher.prepare("(SELECT cipher FROM testtask AS ttt , empcertdate AS aaa, empcerttable AS qqq "
                                        "WHERE (qqq.empcertdateid = aaa.empcertdateid) AND "
                                        "(ttt.testtaskid = aaa.testtaskid) AND empcertid = :id) ORDER BY cipher ASC;");
                    queryCipher.bindValue(":id",empCertId);
                    queryCipher.exec();
                    if(!queryCipher.isActive()){
                        QMessageBox::warning(this,QObject::tr("EmpCert Table, SELECT Cipher ERROR!"),queryCipher.lastError().text());
                    }
                    while(queryCipher.next()){
                        ciperString += queryCipher.value(0).toString();
                        ciperString += ", ";
                    }
                    ciperString.replace(ciperString.length()-2,1,".");
                    QTableWidgetItem *item1 = new QTableWidgetItem;
                    tableWidget->setItem(row,1,item1);
                    tableWidget->item(row,1)->setText(ciperString);

                    //empcertid
                    QTableWidgetItem *item2 = new QTableWidgetItem;
                    tableWidget->setItem(row,2,item2);
                    tableWidget->item(row,2)->setText(empCertId);
                    ++row;
                }

                NumPrefix numPrefix;
                QString indexCost = numPrefix.getPrefix("trainingsafetytable");
                if(indexCost == ""){
                    close();
                }

                QSqlQuery queryCT;
                queryCT.prepare("INSERT INTO trainingsafetytable ("
                                "trainingsafetytableid, trainingsafetyid, empcertid"
                                ") VALUES(:trainingsafetytableid, :trainingsafetyid, :empcertid)");
                queryCT.bindValue(":trainingsafetytableid",indexCost);
                queryCT.bindValue(":trainingsafetyid",indexTemp);
                queryCT.bindValue(":empcertid",empCertId);
                queryCT.exec();
                if(!queryCT.isActive()){
                    QMessageBox::warning(this,QObject::tr("Training Safety Table, INSERT ERROR!"),queryCT.lastError().text());
                }
            }
        }
    }
}

void TrainingSafety::editRecordOfTable()
{
    int i = tableWidget->currentRow();

    EmployeeCertification showForm(tableWidget->item(i,2)->text(),this,false);
    showForm.exec();
}

void TrainingSafety::deleteRecordOfTable()
{
    int k = QMessageBox::question(this,tr("Attention!!"),
                              tr("Really delete?"),
                                  QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(k == QMessageBox::Yes){

        int i = tableWidget->currentRow();

        QSqlQuery queryIn;
        queryIn.prepare("DELETE FROM trainingsafetytable WHERE empcertid = :id;");
        queryIn.bindValue(":id",tableWidget->item(i,2)->text());
        queryIn.exec();

        if(!queryIn.isActive()){
            QMessageBox::warning(this,QObject::tr("Table Value, DELETE ERROR!"),queryIn.lastError().text());
            return;
        }

        QSqlQuery queryEmpCert;
        queryEmpCert.prepare("DELETE FROM empcert WHERE empcertid = :id;");
        queryEmpCert.bindValue(":id",tableWidget->item(i,2)->text());
        queryEmpCert.exec();

        if(!queryEmpCert.isActive()){
            QMessageBox::warning(this,QObject::tr("Empcert ID, DELETE ERROR!"),queryEmpCert.lastError().text());
            return;
        }
        tableWidget->removeRow(tableWidget->currentRow());
        tableWidget->repaint();
    }
}

void TrainingSafety::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("TrainingSafety").toByteArray());
}

void TrainingSafety::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("TrainingSafety", saveGeometry());
}

void TrainingSafety::saveRecord()
{
    editRecord();
    emit accept();
}

void TrainingSafety::cancelRecord()
{
    if(addIntoTable){
        //Удаление табличной части документа

        QSqlQuery queryCT;
        queryCT.prepare("DELETE FROM trainingsafetytable WHERE trainingsafetyid = :idcost");
        queryCT.bindValue(":idcost",indexTemp);
        queryCT.exec();
        if(!queryCT.isActive()){
            QMessageBox::warning(this,QObject::tr("Training Security Table, DELETE ERROR!"),queryCT.lastError().text());
        }
    }
    emit accept();
}

void TrainingSafety::addVenueRecord()
{
    Venue openForm("",this,false);
    openForm.exec();
    idVenue = openForm.returnValue();
    if(idVenue != ""){
        QSqlQuery query;
        query.prepare("SELECT venuename FROM venue WHERE venueid = :id");
        query.bindValue(":id",idVenue);
        query.exec();
        query.next();
        editVenue->setText(query.value(0).toString());
    }
}

void TrainingSafety::seeVenueRecord()
{
    QSqlQuery query;
    query.prepare("SELECT venueid FROM venue WHERE venuename = :name");
    query.bindValue(":name",editVenue->text());
    query.exec();
    while(query.next()){
        Venue openForm(query.value(0).toString(),this,true);
        openForm.exec();
    }
}

void TrainingSafety::listVenueRecord()
{
    QSqlQuery query;
    query.prepare("SELECT venueid FROM venue WHERE venuename = :name");
    query.bindValue(":name",editVenue->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"venue",this);
    listTemp.exec();
    idVenue = listTemp.returnValue();
    if(idVenue != ""){
        QSqlQuery query;
        query.prepare("SELECT venuename FROM venue WHERE venueid = :id");
        query.bindValue(":id",idVenue);
        query.exec();
        query.next();
        editVenue->setText(query.value(0).toString());
    }
}

bool TrainingSafety::checkingFill()
{
    bool fill = true;
    if(editVenue->text() == ""){
        editVenue->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,"Attention!!!","The field can not be empty!");
        editVenue->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}

void TrainingSafety::printPreviewTS()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    QPrintPreviewDialog preview(&printer,this);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(printTS(QPrinter*)));
    preview.showMaximized();
    preview.exec();
}

void TrainingSafety::printTS(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    int heightPage = viewRegion.height();
    //int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawStaticText(1700,0,tr("Prilozenie k zayavke na obucenie"));
    painter.setFont(QFont("Times New Roman",14,QFont::Bold));
    painter.drawStaticText(2000,100,tr("Spisok ucasihsa"));

    int shapkaTop = 250;
    int shapkaBottom = 320;

    QRect numerCol(0,shapkaTop,220,shapkaBottom);
    QRect employeeCol(220,shapkaTop,1520,shapkaBottom);
    QRect postCol(1740,shapkaTop,1520,shapkaBottom);
    QRect prichCol(3260,shapkaTop,600,shapkaBottom);
    QRect normiCol(3860,shapkaTop,850,shapkaBottom);

    painter.drawRect(numerCol);
    painter.setFont(QFont("Times New Roman",10,QFont::Bold));
    painter.drawText(numerCol,Qt::AlignCenter,tr("N\np/p"));

    painter.drawRect(employeeCol);
    painter.drawText(employeeCol,Qt::AlignCenter,tr("FIO"));

    painter.drawRect(postCol);
    painter.drawText(postCol,Qt::AlignCenter,tr("Post"));

    painter.drawRect(prichCol);
    painter.drawText(prichCol,Qt::AlignCenter,tr("Prichina\nattestazii"));

    painter.drawRect(normiCol);
    painter.drawText(normiCol,Qt::AlignCenter,tr("Normi pravil"));

    int newLineHight = shapkaTop + shapkaBottom;
    int mHight;
    int spacing = 30;

    QString employee, post, prichina, norma;

    for(int i = 0; i < tableWidget->rowCount(); ++i){
        if(newLineHight > heightPage){
            newLineHight = 0;
            printer->newPage();
        }
        employee = tableWidget->item(i,0)->text();
        QSqlQuery query;
        query.prepare("SELECT postname FROM post WHERE postid = :id;");
        query.bindValue("id",tableWidget->item(i,2)->text());
        query.exec();
        query.next();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::tr("Post ID, SELECT ERROR!"),query.lastError().text());
            //return;
        }
        post = query.value(0).toString();
        norma = tableWidget->item(i,1)->text();

        painter.setFont(QFont("Times New Roman",10,QFont::Bold));
        painter.setFont(QFont("Times New Roman",8,QFont::Normal));

        mHight = qMax(printParagraph(employee,&painter,&employeeCol,spacing),
                      printParagraph(post,&painter,&postCol,spacing));
        mHight = qMax(mHight,printParagraph(norma,&painter,&normiCol,spacing));
        //mHight = qMax(mHight,printParagraph(normiCol,&painter,&normiCol,spacing));

        numerCol.moveTop(newLineHight);
        numerCol.setHeight(mHight);
        painter.drawRect(numerCol);
        painter.drawText(numerCol,Qt::AlignCenter,QString::number(i+1));

        painter.setFont(QFont("Times New Roman",8,QFont::Normal));
        employeeCol.moveTop(newLineHight);
        employeeCol.setHeight(mHight);
        painter.drawRect(employeeCol);
        QRect tempEmployeeCol = employeeCol;
        tempEmployeeCol.setLeft(employeeCol.left() + spacing);
        painter.drawText(tempEmployeeCol,Qt::AlignLeft | Qt::AlignVCenter,employee);

        postCol.moveTop(newLineHight);
        postCol.setHeight(mHight);
        painter.drawRect(postCol);
        QRect tempPostCol = postCol;
        tempPostCol.setLeft(postCol.left() + spacing);
        painter.drawText(tempPostCol,post);

        prichCol.moveTop(newLineHight);
        prichCol.setHeight(mHight);
        painter.drawRect(prichCol);
        painter.drawText(prichCol,Qt::AlignCenter,tr("Prichina"));

        normiCol.moveTop(newLineHight);
        normiCol.setHeight(mHight);
        painter.drawRect(normiCol);
        QRect tempNormiCol = normiCol;
        tempNormiCol.setLeft(normiCol.left() + spacing);
        painter.drawText(tempNormiCol,norma);

        newLineHight = numerCol.bottom();
    }
}

int TrainingSafety::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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

//int TrainingSafety::paintWord(QPainter *, int, int, const QString &word, int, int, int)
//{

//}
