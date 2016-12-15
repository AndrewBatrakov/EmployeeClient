#include "razmerstatistic.h"
#include <QtSql>

RazmerStatistic::RazmerStatistic(QWidget *parent) : QDialog(parent)
{
//    QPushButton *execute = new QPushButton(trUtf8("Execute"));
//    connect(execute,SIGNAL(clicked()),this,SLOT(printPreview()));

//    QGridLayout *mainLayout = new QGridLayout;
//    mainLayout->addWidget(execute,0,0);
//    setLayout(mainLayout);
//    setWindowTitle(trUtf8("Statistika Razmerov"));
    //printPreview();
}

void RazmerStatistic::printPreview()
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

int RazmerStatistic::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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

void RazmerStatistic::print(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    //int heightPage = viewRegion.height();
    int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    QRect firstString(0,70,widthPage,100);
    painter.drawText(firstString,Qt::AlignHCenter,trUtf8("Статистика размеров сотрудников ЗПГС"));
//    QRect secondString(0,170,widthPage,100);
//    painter.drawText(secondString,Qt::AlignHCenter,trUtf8(""));

    int shapkaTop = 250;
    int shapkaBottom = 320;

    QRect allCol(0,shapkaTop,4690,shapkaBottom);
    QRect razmerCol(0,shapkaTop,1563,shapkaBottom);
    QRect winCol(1563,shapkaTop,782,shapkaBottom);
    QRect winPerCol(2345,shapkaTop,782,shapkaBottom);
    QRect sumCol(3126,shapkaTop,782,shapkaBottom);
    QRect sumPerCol(3908,shapkaTop,782,shapkaBottom);


    //************************************************************
    painter.drawRect(allCol);
    painter.drawText(allCol,Qt::AlignCenter,trUtf8("Одежда"));

    int newLineHight;
    int mHight;

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("Размер"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    painter.drawText(winCol,Qt::AlignCenter,trUtf8("Зима"));

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    painter.drawText(sumCol,Qt::AlignCenter,trUtf8("Лето"));

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();
    QSqlQuery queryA;
    queryA.exec("SELECT COUNT(*) FROM overal;");
    queryA.next();
    float queryAllWinClouth = queryA.value(0).toFloat();
    float znach;
    float per;

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("48 <"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM overal WHERE (winclothes < 48);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumclothes < 48);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("48 - 50"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winclothes BETWEEN 48 AND 50);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumclothes BETWEEN 48 AND 50);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("52 - 54"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winclothes BETWEEN 52 AND 54);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumclothes BETWEEN 52 AND 54);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("54 >"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winclothes > 54);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumclothes > 54);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());
    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    allCol.moveTop(newLineHight);
    allCol.setHeight(mHight);
    painter.drawRect(allCol);
    painter.drawText(allCol,Qt::AlignCenter,trUtf8("Shoes"));

    //************************************************************
    newLineHight = allCol.bottom();
    mHight = allCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("40 <"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winshoes < 40);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes < 40);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("40 - 41"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winshoes BETWEEN 40 AND 41);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes BETWEEN 40 AND 41);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("42 - 43"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winshoes BETWEEN 42 AND 43);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes BETWEEN 42 AND 43);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("44 - 45"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winshoes BETWEEN 44 AND 45);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes BETWEEN 44 AND 45);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("45 >"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (winshoes > 45);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes > 45);");
    query.next();
    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    sumPerCol.moveTop(newLineHight);
    sumPerCol.setHeight(mHight);
    painter.drawRect(sumPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(sumPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    allCol.moveTop(newLineHight);
    allCol.setHeight(mHight);
    painter.drawRect(allCol);
    painter.drawText(allCol,Qt::AlignCenter,trUtf8("Головной убор"));

    //************************************************************
    newLineHight = allCol.bottom();
    mHight = allCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("50 <"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (headdress < 50);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
//    query.clear();
//    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes < 40);");
//    query.next();
//    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("50 - 52"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (headdress BETWEEN 50 AND 52);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
//    query.clear();
//    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes BETWEEN 40 AND 41);");
//    query.next();
//    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("54 - 56"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (headdress BETWEEN 54 AND 56);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
//    query.clear();
//    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes BETWEEN 42 AND 43);");
//    query.next();
//    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("57 - 60"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (headdress BETWEEN 57 AND 60);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
//    query.clear();
//    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes BETWEEN 44 AND 45);");
//    query.next();
//    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());

    //************************************************************
    newLineHight = razmerCol.bottom();
    mHight = razmerCol.height();

    razmerCol.moveTop(newLineHight);
    razmerCol.setHeight(mHight);
    painter.drawRect(razmerCol);
    //painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(razmerCol,Qt::AlignCenter,trUtf8("60 >"));

    winCol.moveTop(newLineHight);
    winCol.setHeight(mHight);
    painter.drawRect(winCol);
    query.clear();
    query.exec("SELECT COUNT(*) FROM overal WHERE (headdress > 60);");
    query.next();
    painter.drawText(winCol,Qt::AlignCenter,query.value(0).toString());

    winPerCol.moveTop(newLineHight);
    winPerCol.setHeight(mHight);
    painter.drawRect(winPerCol);
    znach = query.value(0).toFloat();
    per = znach / queryAllWinClouth * 100;
    per = round(per*100)/100;
    painter.setPen(Qt::darkGreen);
    painter.drawText(winPerCol,Qt::AlignCenter,QString::number(per));
    painter.setPen(Qt::black);

    sumCol.moveTop(newLineHight);
    sumCol.setHeight(mHight);
    painter.drawRect(sumCol);
//    query.clear();
//    query.exec("SELECT COUNT(*) FROM overal WHERE (sumshoes > 45);");
//    query.next();
//    painter.drawText(sumCol,Qt::AlignCenter,query.value(0).toString());
}
