#include "sroknoskiform.h"
#include <QtSql>

SrokNoskiForm::SrokNoskiForm(QWidget *parent) : QDialog(parent)
{
    QPushButton *reportButton = new QPushButton(trUtf8("Сформировать"));
    //QPixmap printAdd(":/print.png");
    //printButton->setIcon(printAdd);
    connect(reportButton,SIGNAL(clicked()),this,SLOT(reportView()));

    labelDate = new QLabel(trUtf8("Отчет на дату:"));
    editDate = new QDateEdit;
    editDate->setCalendarPopup(true);
    editDate->setDate(QDate::currentDate());

    QPushButton *printButton = new QPushButton(tr("Print"));
    QPixmap printAdd(":/print.png");
    printButton->setIcon(printAdd);
    connect(printButton,SIGNAL(clicked()),this,SLOT(printTable()));

    //*****************************************************
    //employee TableView
    //*****************************************************
    employeeView = new QTableWidget;
    employeeView->setColumnCount(2);
    employeeView->setHorizontalHeaderLabels(QStringList()<<trUtf8("Наименование СИЗ")<<trUtf8("Количество"));
    QHeaderView *head = employeeView->horizontalHeader();
    head->setStretchLastSection(true);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->setColumnWidth(0,300);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(reportButton,0,0);
    mainLayout->addWidget(printButton,0,1);
    mainLayout->addWidget(labelDate,1,0);
    mainLayout->addWidget(editDate,1,1);
    mainLayout->addWidget(employeeView,2,0,1,2);
    setLayout(mainLayout);
    setWindowTitle(trUtf8("Срок использования СИЗ"));
    readSettings();
}

void SrokNoskiForm::printTable()
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

int SrokNoskiForm::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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

void SrokNoskiForm::print(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    int heightPage = viewRegion.height();
    //int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    painter.drawStaticText(1200,0,QObject::trUtf8("СИЗ для выдачи на дату: %1").arg(editDate->date().toString("dd.MM.yyyy")));

    int shapkaTop = 250;
    int shapkaBottom = 200;

    QRect numberCol(0,shapkaTop,2345,shapkaBottom);
    QRect fioCol(2345,shapkaTop,2345,shapkaBottom);
//    QRect postCol(1700,shapkaTop,1300,shapkaBottom);
//    QRect prichinaCol(3000,shapkaTop,750,shapkaBottom);
//    QRect vidCol(3750,shapkaTop,940,shapkaBottom);


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
    painter.drawText(numberCol,Qt::AlignCenter,trUtf8("Наименование СИЗ"));

    fioCol.moveTop(newLineHight);
    fioCol.setHeight(mHight);
    painter.drawRect(fioCol);
    painter.drawText(fioCol,Qt::AlignCenter,trUtf8("Количество для выдачи"));

    //newLineHight = shapkaTop+shapkaBottom;
    //int mHight;
    int spacing = 30;

    QSqlQuery query;
    query.prepare("SELECT (SELECT siznaimname FROM siznaim WHERE siznaimid = sz.siznaimid), "
                  "count(sz.ostatok) FROM siz AS sz WHERE strftime('%Y-%m-%d',sz.dataperedachi, "
                  "'+' || srokisp || ' month' ) < date(:date) GROUP BY sz.siznaimid ORDER BY count(sz.ostatok)");
    query.bindValue(":date",editDate->date());
    query.exec();
    painter.setFont(QFont("Times New Roman",10));

    while(query.next()){
        if(newLineHight > heightPage - 200){
            newLineHight = 0;
            printer->newPage();
        }else{
        newLineHight = numberCol.bottom();
        mHight = numberCol.height();
        }

        QString sizName = query.value(0).toString();
        QString number = query.value(1).toString();
        mHight = qMax(printParagraph(sizName,&painter,&numberCol,spacing),
                      printParagraph(number,&painter,&fioCol,spacing));

        numberCol.moveTop(newLineHight);
        numberCol.setHeight(mHight);
        painter.drawRect(numberCol);
        QRect tempCol = numberCol;
        tempCol.setLeft(numberCol.left()+spacing);
        painter.drawText(tempCol, sizName);

        fioCol.moveTop(newLineHight);
        fioCol.setHeight(mHight);
        painter.drawRect(fioCol);
        painter.drawText(fioCol,Qt::AlignHCenter | Qt::AlignVCenter, number);
    }
}

void SrokNoskiForm::reportView()
{
    for(int i = employeeView->rowCount(); i > -1; --i){
        employeeView->removeRow(i);
    }
    //Количество спецодежды на замену, дата 01-08-2016
    QSqlQuery query;
//    query.prepare("SELECT (SELECT siznaimname FROM siznaim WHERE siznaimid = sz.siznaimid), "
//                  "count(sz.ostatok) FROM siz AS sz WHERE strftime('%Y-%m-%d',sz.dataperedachi, "
//                  "'+' || srokisp || ' month' ) < date(:date) GROUP BY sz.siznaimid ORDER BY count(sz.ostatok)");
    query.prepare("SELECT * FROM siz WHERE dataokon > :date GROUP BY siznaimid");
    query.bindValue(":date",editDate->date());
    query.exec();
    int rowCount = 0;
    while(query.next()){
        employeeView->insertRow(rowCount);

        QTableWidgetItem *itemSIZName = new QTableWidgetItem;
        employeeView->setItem(rowCount,0,itemSIZName);
        employeeView->item(rowCount,0)->setText(query.value(0).toString());

        QTableWidgetItem *itemNumber = new QTableWidgetItem;
        employeeView->setItem(rowCount,1,itemNumber);
        employeeView->item(rowCount,1)->setText(query.value(1).toString());

        ++rowCount;
    }
}

void SrokNoskiForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "ReportSrokNoskiForm");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void SrokNoskiForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "ReportSrokNoskiForm");
    settings.setValue("Geometry", saveGeometry());
}

void SrokNoskiForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}
