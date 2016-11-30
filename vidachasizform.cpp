#include "vidachasizform.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"
#include "noedititemdelegat.h"
#include <QAxObject>

VidachaSIZForm::VidachaSIZForm(QString idDoc, QString idEmp, QString nPost, QWidget *parent) : QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = idDoc;
    idEmployee = idEmp;
    namePost = nPost;
    printDoc = false;

    labelNumber = new QLabel(trUtf8("Номер:"));
    editNumber = new LineEdit;
    NumPrefix numPrefix;
    indexTemp = numPrefix.getPrefix("vidachasiz");
    editNumber->setText(indexTemp);

    labelDate = new QLabel(trUtf8("Дата"));
    editDate = new QDateEdit;
    editDate->setCalendarPopup(true);
    editDate->setDate(QDate::currentDate());

    QHBoxLayout *numberLayout = new QHBoxLayout;
    numberLayout->addWidget(labelNumber);
    numberLayout->addWidget(editNumber);
    numberLayout->addWidget(labelDate);
    numberLayout->addWidget(editDate);

    labelEmployee = new QLabel(trUtf8("ФИО:"));
    editEmployee = new LineEdit;
    QSqlQuery query;
    query.prepare("SELECT employee.employeename "
                  "FROM employee WHERE employee.employeeid = :employeeid");
    query.bindValue(":employeeid",idEmployee);
    query.exec();
    query.next();
    editEmployee->setText(query.value(0).toString());

    QHBoxLayout *empLayout = new QHBoxLayout;
    empLayout->addWidget(labelEmployee);
    empLayout->addWidget(editEmployee);

    QLabel *labelNormaSiz = new QLabel(trUtf8("Норма СИЗ:"));
    editNormaSIZ = new LineEdit;

    tableSizWidget = new QTableWidget;
    tableSizWidget->setColumnCount(5);
    QString siz0 = trUtf8("Наименование СИЗ");
    QString siz1 = trUtf8("Характеристика");
    QString siz2 = trUtf8("Остаток");
    QString siz3 = trUtf8("Дата получения");
    QString siz4 = trUtf8("Срок\nиспользования");
    tableSizWidget->setHorizontalHeaderLabels(QStringList()<<siz0<<siz1<<siz2<<siz3<<siz4);
    QHeaderView *head = tableSizWidget->horizontalHeader();
    head->setStretchLastSection(true);
    tableSizWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableSizWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //tableSizWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableSizWidget->setAlternatingRowColors(true);
    tableSizWidget->setColumnWidth(0,300);
    tableSizWidget->setColumnWidth(1,300);

    QHBoxLayout *stringLayout = new QHBoxLayout;
    stringLayout->addWidget(labelNormaSiz);
    stringLayout->addWidget(editNormaSIZ);

    saveButton = new QPushButton(trUtf8("Записать"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(trUtf8("Отмена"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

    QPushButton *printTableButton = new QPushButton(trUtf8("Печать"));
    QPixmap pixPrint(":/print.png");
    printTableButton->setIcon(pixPrint);
    connect(printTableButton,SIGNAL(clicked()),this,SLOT(printTable()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(printTableButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    createTableSIZ();
    createContextSIZ();

    QVBoxLayout *sizLayout = new QVBoxLayout;
    sizLayout->addLayout(numberLayout);
    sizLayout->addLayout(empLayout);
    sizLayout->addLayout(stringLayout);
    sizLayout->addWidget(tableSizWidget);
    sizLayout->addWidget(buttonBox);
    setLayout(sizLayout);

    setWindowTitle(trUtf8("Выдача СИЗ"));
    readSettings();
}

void VidachaSIZForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void VidachaSIZForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("VidachaSIZForm").toByteArray());
}

void VidachaSIZForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("VidachaSIZForm", saveGeometry());
}

void VidachaSIZForm::printTable()
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

int VidachaSIZForm::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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

void VidachaSIZForm::print(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    int heightPage = viewRegion.height();
    //    //int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    painter.drawStaticText(2700,0,QObject::trUtf8("Приложение №2 к договору"));
    painter.drawStaticText(2700,70,QObject::trUtf8("к Правилам обеспечения работников специальной одеждой,"));
    painter.drawStaticText(2700,140,QObject::trUtf8("специальной обувью и другими средствами индивидуальной защиты,"));
    painter.drawStaticText(2700,210,QObject::trUtf8("утв. постановлением Минтруда РФ"));
    painter.drawStaticText(2700,280,QObject::trUtf8("от 18 декабря 1998 г. № 51"));

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    QString pp = trUtf8("ЛИЧНАЯ КАРТОЧКА № ");
    pp += editNumber->text();
    painter.drawStaticText(1500,400,pp);
    painter.drawStaticText(1200,520,QObject::trUtf8("учёта выдачи средств индивидуальной защиты"));

    QSqlQuery query;
    query.prepare("SELECT employee.genderid, "
                  "employee.employeename, "
                  "employee.tabnumber, "
                  "(SELECT subdivision.subdivisionname FROM subdivision WHERE subdivision.subdivisionid = employee.subdivisionid), "
                  "(SELECT post.postname FROM post WHERE post.postid = employee.postid), "
                  "employee.withorganization "
                  "FROM employee WHERE employee.employeeid = :employeeid");
    query.bindValue(":employeeid",idEmployee);
    query.exec();
    query.next();

    int shapkaTop = 500;
    int shapkaBottom = 150;

    QRect oneCol(250,shapkaTop,1200,shapkaBottom);
    QRect twoCol(1450,shapkaTop,1550,shapkaBottom);
    QRect threeCol(3000,shapkaTop,500,shapkaBottom);
    QRect fourCol(3500,shapkaTop,600,shapkaBottom);
    QRect fiveCol(4100,shapkaTop,600,shapkaBottom);

    int newLineHight;
    int mHight;
    int spacing = 30;

    //    //************************************************************
    newLineHight = oneCol.bottom();
    mHight = oneCol.height();

    QSqlQuery queryOveral;
    queryOveral.prepare("SELECT * FROM overal WHERE employeeid = :id");
    queryOveral.bindValue(":id",idEmployee);
    queryOveral.exec();
    queryOveral.next();

    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8));
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Пол:"));
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter,query.value(0).toString());

    newLineHight = fourCol.bottom();
    mHight = fourCol.height();
    mHight = qMax(mHight,printParagraph(query.value(1).toString(),&painter,&twoCol,spacing));
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8));
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Фамилия Имя Отчество"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",10,QFont::Bold));
    painter.drawText(twoCol,Qt::AlignCenter | Qt::TextWordWrap,query.value(1).toString());
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Рост:"));
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter | Qt::AlignVCenter,queryOveral.value(2).toString());

    newLineHight = fourCol.bottom();
    mHight = fourCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Табельный номер:"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.drawText(twoCol,Qt::AlignLeft | Qt::AlignVCenter,query.value(2).toString());
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Размер:"));
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter | Qt::AlignVCenter,trUtf8("Зима:"));
    fiveCol.moveTop(newLineHight);
    fiveCol.setHeight(mHight);
    painter.drawText(fiveCol,Qt::AlignCenter | Qt::AlignVCenter,trUtf8("Лето:"));

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    mHight = qMax(mHight,printParagraph(query.value(3).toString(),&painter,&twoCol,spacing));
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Структурное подразделение:"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8,QFont::Bold));
    painter.drawText(twoCol,Qt::AlignCenter | Qt::TextWordWrap,query.value(3).toString());
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Одежда:"));
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter | Qt::AlignVCenter,queryOveral.value(4).toString());
    fiveCol.moveTop(newLineHight);
    fiveCol.setHeight(mHight);
    painter.drawText(fiveCol,Qt::AlignCenter | Qt::AlignVCenter,queryOveral.value(5).toString());

    newLineHight = oneCol.bottom();
    mHight = shapkaBottom;
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Профессия (должность):"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8,QFont::Bold));
    painter.drawText(twoCol,Qt::AlignCenter | Qt::TextWordWrap,query.value(4).toString());
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Обуви:"));
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter | Qt::AlignVCenter,queryOveral.value(6).toString());
    fiveCol.moveTop(newLineHight);
    fiveCol.setHeight(mHight);
    painter.drawText(fiveCol,Qt::AlignCenter | Qt::AlignVCenter,queryOveral.value(7).toString());

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Дата поступления на работу"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8,QFont::Bold));
    painter.drawText(twoCol,Qt::AlignCenter | Qt::TextWordWrap,query.value(5).toString());
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("головного убора"));
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());
    fourCol.moveTop(newLineHight);
    fourCol.setHeight(mHight);
    painter.drawText(fourCol,Qt::AlignCenter | Qt::AlignVCenter,queryOveral.value(3).toString());
    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Дата изменения профессии (должность)"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("респиратора"));
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());
    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("или перевода в другое структурное"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("рукавиц"));
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());
    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("подразделение"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawText(threeCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("перчаток"));
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneCol.moveTop(newLineHight);
    oneCol.setHeight(mHight);
    painter.drawText(oneCol,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Предусмотрено по нормам бесплатной"));
    twoCol.moveTop(newLineHight);
    twoCol.setHeight(mHight);
    threeCol.moveTop(newLineHight);
    threeCol.setHeight(mHight);
    painter.drawLine(twoCol.x(),twoCol.y(),threeCol.x(),threeCol.y());

    QRect oneColTab(250,shapkaTop,2750,shapkaBottom);
    QRect twoColTab(3000,shapkaTop,500,shapkaBottom);
    QRect threeColTab(3500,shapkaTop,600,shapkaBottom);
    QRect fourColTab(4100,shapkaTop,600,shapkaBottom);

    newLineHight = oneCol.bottom();
    mHight = oneCol.height();
    oneColTab.moveTop(newLineHight);
    oneColTab.setHeight(mHight);
    painter.setFont(QFont("Times New Roman",8,QFont::Bold));
    painter.drawText(oneColTab,Qt::AlignCenter,trUtf8("Наименование средств индивидуальной защиты"));
    painter.drawRect(oneColTab);
    twoColTab.moveTop(newLineHight);
    twoColTab.setHeight(mHight);
    painter.drawText(twoColTab,Qt::AlignCenter,trUtf8("Пункт по норм"));
    painter.drawRect(twoColTab);
    threeColTab.moveTop(newLineHight);
    threeColTab.setHeight(mHight);
    painter.drawText(threeColTab,Qt::AlignCenter,trUtf8("Количество"));
    painter.drawRect(threeColTab);
    fourColTab.moveTop(newLineHight);
    fourColTab.setHeight(mHight);
    painter.drawText(fourColTab,Qt::AlignCenter,trUtf8("Срок месяцев"));
    painter.drawRect(fourColTab);
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));

    QRect tempCol;
    for(int currentRow = 0; currentRow - tableSizWidget->rowCount(); ++currentRow){
        if(newLineHight > heightPage - 200){
            newLineHight = 0;
            printer->newPage();
        }

        QString sizNaim = tableSizWidget->item(currentRow,0)->text();
        //        mHight = qMax(printParagraph(QString::number(currentRow + 1),&painter,&numberCol,spacing),
        //                      printParagraph(sotrudnik,&painter,&fioCol,spacing));
        //        mHight = qMax(mHight,printParagraph(employeeView->item(currentRow,2)->text(),&painter,&postCol,spacing));
        //        //mHight = qMax(mHight,printParagraph(editPrichina->text(),&painter,&prichinaCol,spacing));
        //        //mHight = qMax(mHight,printParagraph(editVidAtest->text(),&painter,&vidCol,spacing));

        //        //newLineHight = numberCol.bottom();
        //        //mHight = numberCol.height();

        newLineHight = oneColTab.bottom();
        mHight = oneColTab.height();
        oneColTab.moveTop(newLineHight);
        oneColTab.setHeight(mHight);
        tempCol = oneColTab;
        tempCol.setLeft(oneColTab.left()+spacing);
        painter.drawText(tempCol,Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap,sizNaim);
        painter.drawRect(oneColTab);

        twoColTab.moveTop(newLineHight);
        twoColTab.setHeight(mHight);
        //painter.drawText(twoColTab,Qt::AlignCenter,tableSizWidget->item(currentRow,1)->text());
        painter.drawRect(twoColTab);

        threeColTab.moveTop(newLineHight);
        threeColTab.setHeight(mHight);
        painter.drawText(threeColTab,Qt::AlignCenter,tableSizWidget->item(currentRow,2)->text());
        painter.drawRect(threeColTab);

        fourColTab.moveTop(newLineHight);
        fourColTab.setHeight(mHight);
        painter.drawText(fourColTab,Qt::AlignCenter,tableSizWidget->item(currentRow,4)->text());
        painter.drawRect(fourColTab);
    }
    newLineHight = oneColTab.bottom();
    mHight = oneColTab.height();
    oneColTab.moveTop(newLineHight);
    oneColTab.setHeight(mHight);
    newLineHight = oneColTab.bottom();
    mHight = oneColTab.height();
    oneColTab.moveTop(newLineHight);
    oneColTab.setHeight(mHight);
    painter.drawText(oneColTab,Qt::AlignLeft | Qt::AlignVCenter,
                     QObject::trUtf8("С порядком и нормами обеспечения работников спецодеждой, спецобувью и другими"));
    newLineHight = oneColTab.bottom();
    mHight = oneColTab.height();
    oneColTab.moveTop(newLineHight);
    oneColTab.setHeight(mHight);
    painter.drawText(oneColTab,Qt::AlignLeft | Qt::AlignVCenter,
                     QObject::trUtf8("СИЗ установленными в ООО \"Севергазстрой\"      ОЗНАКОМЛЕН:"));

    newLineHight = oneColTab.bottom();
    mHight = oneColTab.height();
    fourColTab.moveTop(newLineHight);
    fourColTab.setHeight(mHight);
    painter.drawText(fourColTab,Qt::AlignLeft | Qt::AlignVCenter,editDate->text());
    newLineHight = fourColTab.bottom();
    mHight = fourColTab.height();
    twoColTab.moveTop(newLineHight);
    twoColTab.setHeight(mHight);
    threeColTab.moveTop(newLineHight);
    threeColTab.setHeight(mHight);
    painter.drawLine(twoColTab.x(),twoColTab.y(),threeColTab.x(),threeColTab.y());
    printDoc = true;
}

void VidachaSIZForm::excelExport()
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
    //rr += editProtocolObuch->text().toUtf8();
    cell->setProperty("Value", rr);
    QAxObject *font = cell->querySubObject("Font");
    font->setProperty("Bold",true);
    font->setProperty("Size",12);
    cell->setProperty("ColumnWidth",54);

    //for(int currentRow = 0; currentRow - employeeView->rowCount(); ++currentRow){
    //        QAxObject *cel = sheet->querySubObject("cells(int, int)", currentRow + 2,1);
    //        QString s = employeeView->item(currentRow,1)->text();
    //        cel->setProperty("Value",s);
    //    }
    //    for(int currentRow = 0; currentRow - employeeView->rowCount(); ++currentRow){
    //        QAxObject *cel = sheet->querySubObject("cells(int, int)", currentRow + 2,2);
    //        QString s = employeeView->item(currentRow,2)->text();
    //        cel->setProperty("Value",s);
    //    }
}

void VidachaSIZForm::createTableSIZ()
{
    int rowCount = 0;
    QColor colG(Qt::darkGreen);
    QColor colR(Qt::darkRed);
    QColor col;

    int rowC = tableSizWidget->rowCount();

    for(int i = rowC; i >= 0; --i){
        tableSizWidget->removeRow(i);
        tableSizWidget->repaint();
    }

    QSqlQuery querySIZNorm;
    querySIZNorm.prepare("SELECT (SELECT sizna.siznaimname FROM siznaim AS sizna WHERE sizna.siznaimid = pst.siznaimid), "
                         "(SELECT h.harname FROM har AS h WHERE h.harid = pst.harid), "
                         "kolvo, "
                         "srok, "
                         "siznaimid, "
                         "postsizid "
                         "FROM postsiztable AS pst WHERE pst.postsizid IN "
                         "(SELECT ps.postsizid FROM postsizlist AS ps WHERE ps.postid = :postid);");
    QSqlQuery queryPostId;
    queryPostId.prepare("SELECT postid FROM post WHERE postname = :postname");
    queryPostId.bindValue(":postname",namePost);
    queryPostId.exec();
    queryPostId.next();
    querySIZNorm.bindValue(":postid",queryPostId.value(0).toString());
    querySIZNorm.exec();
    while(querySIZNorm.next()){
        QSqlQuery queryPostSiz;
        queryPostSiz.prepare("SELECT * FROM postsiz WHERE postsizid = :postsizid");
        queryPostSiz.bindValue(":postsizid",querySIZNorm.value(5).toString());
        queryPostSiz.exec();
        queryPostSiz.next();
        editNormaSIZ->setText(queryPostSiz.value(1).toString());
        tableSizWidget->insertRow(rowCount);

        QSqlQuery queryBuh;
        queryBuh.prepare("SELECT * FROM siz WHERE (employeeid = :employeeid AND siznaimid = :siznaimid)");
        queryBuh.bindValue(":employeeid",idEmployee);
        queryBuh.bindValue(":siznaimid",querySIZNorm.value(4).toString());
        queryBuh.exec();
        bool poluchil = false;
        QString tt, tt1;

        while(queryBuh.next()){
            poluchil = true;
            tt = queryBuh.value(4).toDate().toString("dd.MM.yyyy");
            QDateTime dd = queryBuh.value(4).toDateTime();
            QDateTime dd1 = dd.addMonths(querySIZNorm.value(3).toInt());
            tt1 = dd1.toString("dd.MM.yyyy");
            //qDebug()<<tt1;
        }

        if(poluchil){
            col = colG;
        }else{
            col = colR;
        }

        QTableWidgetItem *itemSIZName = new QTableWidgetItem;
        tableSizWidget->setItem(rowCount,0,itemSIZName);
        tableSizWidget->item(rowCount,0)->setText(querySIZNorm.value(0).toString());
        tableSizWidget->item(rowCount,0)->setTextAlignment(Qt::TextWordWrap);
        tableSizWidget->item(rowCount,0)->setTextColor(col);
        tableSizWidget->setItemDelegateForColumn(0, new NoEditItemDelegat);

        QTableWidgetItem *itemHar = new QTableWidgetItem;
        tableSizWidget->setItem(rowCount,1,itemHar);
        tableSizWidget->item(rowCount,1)->setText(querySIZNorm.value(1).toString());
        tableSizWidget->item(rowCount,1)->setTextColor(col);
        tableSizWidget->setItemDelegateForColumn(1, new NoEditItemDelegat);

        QTableWidgetItem *itemNumber = new QTableWidgetItem;
        tableSizWidget->setItem(rowCount,2,itemNumber);
        tableSizWidget->item(rowCount,2)->setText(querySIZNorm.value(2).toString());
        tableSizWidget->item(rowCount,2)->setTextColor(col);

        QTableWidgetItem *itemPoluch = new QTableWidgetItem;
        tableSizWidget->setItem(rowCount,3,itemPoluch);
        tableSizWidget->item(rowCount,3)->setText(tt1);
        tableSizWidget->item(rowCount,3)->setTextColor(col);

        QTableWidgetItem *itemSrok = new QTableWidgetItem;
        tableSizWidget->setItem(rowCount,4,itemSrok);
        tableSizWidget->item(rowCount,4)->setText(querySIZNorm.value(3).toString());
        tableSizWidget->item(rowCount,4)->setTextColor(col);
        tableSizWidget->setItemDelegateForColumn(4, new NoEditItemDelegat);

        ++rowCount;
    }
}

void VidachaSIZForm::deleteRecordTable()
{
    tableSizWidget->removeRow(tableSizWidget->currentRow());
    tableSizWidget->repaint();
}

void VidachaSIZForm::createContextSIZ()
{
    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(trUtf8("Удалить строку?"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordTable()));
    tableSizWidget->addAction(deleteAction);
    tableSizWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void VidachaSIZForm::editRecord()
{
    if(printDoc){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        QSqlQuery queryDoc;
        queryDoc.prepare("INSERT INTO vidachsiz (vidachsizid, docdate, employeeid) VALUES("
                         ":vidachsizid, :docdate, :employeeid)");
        queryDoc.bindValue(":vidachsizid",indexTemp);
        queryDoc.bindValue(":docdate",editDate->date());
        queryDoc.bindValue(":employeeid",idEmployee);
        queryDoc.exec();
        line += "INSERT INTO vidachsiz (vidachsizid, docdate, employeeid) VALUES('";
        line += indexTemp.toUtf8();
        line += "', '";
        line += editDate->date().toString("yyyy-MM-dd");
        line += "', '";
        line += idEmployee.toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;

        QSqlQuery query;
        NumPrefix prefixNum;
        QString prefNum;
        QSqlQuery queryTemp;
        QDate toPrint;

        for(int currentRow = 0; currentRow - tableSizWidget->rowCount(); ++currentRow){
            query.prepare("INSERT INTO siz (sizid, employeeid, siznaimid, ostatok, dataperedachi, srokisp, dataokon) "
                          "VALUES(:sizid, :employeeid, :siznaimid, :ostatok, :dataperedachi, :srokisp, :dataokon)");
            prefNum = prefixNum.getPrefix("siz");
            query.bindValue(":sizid", prefNum);
            query.bindValue(":employeeid",idEmployee);

            queryTemp.clear();
            queryTemp.prepare("SELECT siznaimid FROM siznaim WHERE siznaimname = :siznaimname");
            queryTemp.bindValue(":siznaimname",tableSizWidget->item(currentRow,0)->text());
            queryTemp.exec();
            queryTemp.next();
            query.bindValue(":siznaimid",queryTemp.value(0).toString());
            query.bindValue(":ostatok",tableSizWidget->item(currentRow,2)->text().toInt());
            query.bindValue(":dataperedachi",editDate->date());
            query.bindValue(":srokisp",tableSizWidget->item(currentRow,4)->text().toInt());
            QDate endDate(editDate->date());
            toPrint = endDate.addMonths(tableSizWidget->item(currentRow,4)->text().toInt());
            query.bindValue(":dataokon", toPrint);
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("Ошибка базы данных!"),query.lastError().text());
                return;
            }
            line += "INSERT INTO siz (sizid, employeeid, siznaimid, ostatok, dataperedachi, srokisp, dataokon) "
                    "VALUES('";
            line += prefNum.toUtf8();
            line += "', '";
            line += idEmployee.toUtf8();
            line += "', '";
            line += queryTemp.value(0).toString().toUtf8();
            line += "', '";
            line += QString::number(tableSizWidget->item(currentRow,2)->text().toInt());
            line += "', '";
            line += editDate->date().toString("yyyy-MM-dd");
            line += "', '";
            line += QString::number(tableSizWidget->item(currentRow,4)->text().toInt());
            line += "', '";
            line += toPrint.toString("yyyy-MM-dd");
            line += "')";
            line += "\r\n";
            stream<<line;
        }
        emit accept();
    }else{
        QMessageBox::warning(this,QObject::trUtf8("Внимание!"),"Перед записью необходимо напечатать документ!");
        return;
    }
}
