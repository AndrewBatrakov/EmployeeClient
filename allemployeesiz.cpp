#include "allemployeesiz.h"
#include <QtSql>

AllEmployeeSiz::AllEmployeeSiz(QWidget *parent) : QDialog(parent)
{
    QPushButton *reportButton = new QPushButton(trUtf8("Сформировать"));
    //QPixmap printAdd(":/print.png");
    //printButton->setIcon(printAdd);
    connect(reportButton,SIGNAL(clicked()),this,SLOT(reportView()));

    //    labelDate = new QLabel(trUtf8("Отчет на дату:"));
    //    editDate = new QDateEdit;
    //    editDate->setCalendarPopup(true);
    //    editDate->setDate(QDate::currentDate());

    QPushButton *printButton = new QPushButton(tr("Print"));
    QPixmap printAdd(":/print.png");
    printButton->setIcon(printAdd);
    connect(printButton,SIGNAL(clicked()),this,SLOT(printPreview()));

    //*****************************************************
    //employee TableView
    //*****************************************************
    employeeView = new QTableView;
    //employeeView->setColumnCount(5);
    //employeeView->setHorizontalHeaderLabels(QStringList()<<trUtf8("Наименование СИЗ")<<trUtf8("Количество"));
    QHeaderView *head = employeeView->horizontalHeader();
    head->setStretchLastSection(true);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->setColumnWidth(0,300);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(reportButton,0,0);
    mainLayout->addWidget(printButton,0,1);
    //mainLayout->addWidget(labelDate,1,0);
    //mainLayout->addWidget(editDate,1,1);
    mainLayout->addWidget(employeeView,2,0,1,2);
    setLayout(mainLayout);
    setWindowTitle(trUtf8("Потребность замены СИЗ"));
    readSettings();
}


void AllEmployeeSiz::printPreview()
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

int AllEmployeeSiz::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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


void AllEmployeeSiz::print(QPrinter *printer)
{
//    QPainter painter(printer);

//    QRect viewRegion(painter.viewport());
//    int heightPage = viewRegion.height();
//    //int widthPage = viewRegion.width();

//    painter.setFont(QFont("Times New Roman",12,QFont::Normal));

//    int shapkaTop = 250;
//    int shapkaBottom = 320;

//    QRect numberCol(0,shapkaTop,200,shapkaBottom);
//    QRect employeeCol(200,shapkaTop,1320,shapkaBottom);
//    QRect postCol(1520,shapkaTop,1320,shapkaBottom);
//    QRect prichCol(2840,shapkaTop,1320,shapkaBottom);
//    QRect normiCol(4160,shapkaTop,530,shapkaBottom);
//    //QRect primechCol(4140,shapkaTop,550,shapkaBottom);

//    painter.drawRect(numberCol);
//    painter.setFont(QFont("Times New Roman",9,QFont::Bold));
//    painter.drawText(numberCol,Qt::AlignCenter,tr("N\np/p"));

//    painter.drawRect(employeeCol);
//    painter.drawText(employeeCol,Qt::AlignCenter,tr("FIO"));

//    painter.drawRect(postCol);
//    painter.drawText(postCol,Qt::AlignCenter,tr("Post"));

//    painter.drawRect(prichCol);
//    painter.drawText(prichCol,Qt::AlignCenter,tr("SIZ"));

//    painter.drawRect(normiCol);
//    painter.drawText(normiCol,Qt::AlignCenter,tr("Srok"));

//    QSqlQuery query;
//    query.prepare("SELECT emp.employeename, (SELECT po.postname FROM post AS po WHERE po.postid = emp.postid), emp.postid, employeeid FROM employee AS emp LIMIT 50");
//    query.exec();

//    int newLineHight = shapkaTop+shapkaBottom;
//    int mHight;
//    int spacing = 30;

//    int numberEmployee = 0;

//    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
//    bool printFIO = true;
//    while(query.next()) {
//        QString sotrudnik = query.value(0).toString();
//        QString post = query.value(1).toString();
//        QString prich;
//        int srok;

//        QSqlQuery querySIZNorm;
//        querySIZNorm.prepare("SELECT (SELECT sizna.siznaimname FROM siznaim AS sizna WHERE sizna.siznaimid = pst.siznaimid), "
//                             "(SELECT h.harname FROM har AS h WHERE h.harid = pst.harid), "
//                             "kolvo, "
//                             "srok, "
//                             "siznaimid "
//                             "FROM postsiztable AS pst WHERE pst.postsizid IN "
//                             "(SELECT ps.postsizid FROM postsizlist AS ps WHERE ps.postid = :postid);");
//        querySIZNorm.bindValue(":postid",query.value(2).toString());
//        querySIZNorm.exec();
//        while(querySIZNorm.next()){
//            if(newLineHight > heightPage - 200){
//                newLineHight = 0;
//                printer->newPage();
//            }
//            prich = querySIZNorm.value(0).toString();
//            srok = querySIZNorm.value(3).toInt();

//            mHight = qMax(printParagraph(QString::number(numberEmployee),&painter,&numberCol,spacing),
//                          printParagraph(sotrudnik,&painter,&employeeCol,spacing));
//            mHight = qMax(mHight,printParagraph(post,&painter,&postCol,spacing));
//            mHight = qMax(mHight,printParagraph(prich,&painter,&prichCol,spacing));
//            mHight = qMax(mHight,printParagraph(QString::number(srok),&painter,&normiCol,spacing));

//            numberCol.moveTop(newLineHight);
//            numberCol.setHeight(mHight);
//            painter.drawRect(numberCol);
//            if(printFIO){
//                ++numberEmployee;
//                painter.drawText(numberCol,Qt::AlignCenter,QString::number(numberEmployee));
//            }

//            employeeCol.moveTop(newLineHight);
//            employeeCol.setHeight(mHight);
//            painter.drawRect(employeeCol);

//            QRect tempSotrCol = employeeCol;
//            tempSotrCol.setLeft(employeeCol.left()+spacing);
//            if(printFIO){
//                painter.drawText(tempSotrCol,Qt::AlignVCenter,sotrudnik);
//            }

//            postCol.moveTop(newLineHight);
//            postCol.setHeight(mHight);
//            painter.drawRect(postCol);
//            QRect tempDolCol = postCol;
//            tempDolCol.setLeft(postCol.left()+spacing);
//            if(printFIO){
//                painter.drawText(tempDolCol,Qt::AlignVCenter,post);
//                printFIO = false;
//            }

//            //*************************************************************
//            QSqlQuery queryBuh;
//            queryBuh.prepare("SELECT * FROM siz WHERE (employeeid = :employeeid AND siznaimid = :siznaimid)");
//            queryBuh.bindValue(":employeeid",query.value(3).toString());
//            queryBuh.bindValue(":siznaimid",querySIZNorm.value(4).toString());
//            queryBuh.exec();
//            bool poluchil = false;
//            while(queryBuh.next()){
//                poluchil = true;
//            }

//            if(!poluchil){
//                painter.setPen(Qt::darkGreen);
//            }else{
//                painter.setPen(Qt::darkRed);
//            }

//            prichCol.moveTop(newLineHight);
//            prichCol.setHeight(mHight);
//            painter.drawRect(prichCol);
//            QRect tempPodrCol = prichCol;
//            tempPodrCol.setLeft(prichCol.left()+spacing);
//            painter.drawText(tempPodrCol,querySIZNorm.value(0).toString());

//            normiCol.moveTop(newLineHight);
//            normiCol.setHeight(mHight);
//            painter.drawRect(normiCol);
//            QRect tempnormiCol = normiCol;
//            tempnormiCol.setLeft(normiCol.left()+spacing);
//            painter.drawText(tempnormiCol,Qt::AlignCenter,QString::number(srok));
//            painter.setPen(Qt::black);

//            newLineHight = numberCol.bottom();

//        }
//        printFIO = true;
//    }
}

void AllEmployeeSiz::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "AllEmployeeSizForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void AllEmployeeSiz::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "AllEmployeeSizForm");
    settings.setValue("Geometry", saveGeometry());
}

void AllEmployeeSiz::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void AllEmployeeSiz::reportView()
{
//    for(int i = employeeView->rowCount(); i > -1; --i){
//        employeeView->removeRow(i);
//    }

    QColor colG(Qt::darkGreen);
    QColor colR(Qt::darkRed);
    QColor col;
    QSqlQuery query;
    query.prepare("SELECT employeeid FROM employee");
    query.exec();
    while(query.next()){
        int rowCount = 0;
        QSqlQueryModel *querySIZNorm = new QSqlTableModel;
        QString qq =("SELECT siznaim.siznaimid, siznaim.siznaimname FROM employee LEFT JOIN postsizlist "
                             "LEFT JOIN postsiztable LEFT JOIN siznaim WHERE "
                             "(employee.postid = postsizlist.postid AND postsiztable.postsizid = "
                             "postsizlist.postsizid AND postsiztable.siznaimid = siznaim.siznaimid "
                             "AND employee.employeeid = '");
        qq += query.value(0).toString();
        qq += "');";

        querySIZNorm->setQuery(qq);
//        while(querySIZNorm.next()){
//            QSqlQuery queryBuh;
//            queryBuh.prepare("SELECT * FROM siz WHERE (employeeid = :employeeid AND siznaimid = :siznaimid)");
//            queryBuh.bindValue(":employeeid",query.value(0).toString());
//            queryBuh.bindValue(":siznaimid",querySIZNorm.value(0).toString());
//            queryBuh.exec();
//            bool poluchil = false;
//            QString tt;
//            while(queryBuh.next()){
//                poluchil = true;
//                tt = queryBuh.value(4).toDate().toString("dd.MM.yyyy");
//            }

//            if(poluchil){
//                col = colG;
//            }else{
//                col = colR;
//            }

//            employeeView->insertRow(rowCount);

//            QTableWidgetItem *itemSIZName = new QTableWidgetItem;
//            employeeView->setItem(rowCount,0,itemSIZName);
//            employeeView->item(rowCount,0)->setText(querySIZNorm.value(1).toString());
//            employeeView->item(rowCount,0)->setTextColor(col);

//            QTableWidgetItem *itemHar = new QTableWidgetItem;
//            employeeView->setItem(rowCount,1,itemHar);
//            //employeeView->item(rowCount,1)->setText(queryBuh.value(1).toString());
//            //employeeView->item(rowCount,1)->setTextColor(col);

//            QTableWidgetItem *itemNumber = new QTableWidgetItem;
//            employeeView->setItem(rowCount,2,itemNumber);
//            //employeeView->item(rowCount,2)->setText(queryBuh.value(2).toString());
//            //employeeView->item(rowCount,2)->setTextColor(col);

//            QTableWidgetItem *itemPoluch = new QTableWidgetItem;
//            employeeView->setItem(rowCount,3,itemPoluch);
//            //employeeView->item(rowCount,3)->setText(tt);
//            //employeeView->item(rowCount,3)->setTextColor(col);

//            QTableWidgetItem *itemSrok = new QTableWidgetItem;
//            employeeView->setItem(rowCount,4,itemSrok);
//            //employeeView->item(rowCount,4)->setText(queryBuh.value(3).toString());
//            //employeeView->item(rowCount,4)->setTextColor(col);

//            ++rowCount;
//        }
        employeeView->setModel(querySIZNorm);
        //employeeView->selectAll();
        employeeView->show();
    }
}


//(SELECT siznaim.siznaimname FROM siznaim WHERE siznaim.siznaimid = postsiztable.siznaimid),
