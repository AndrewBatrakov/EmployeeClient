#include "grafikprombezop.h"
#include "subdivision.h"
#include "testtask.h"
#include "viewlisttable.h"

GrafiKPromBezop::GrafiKPromBezop(QWidget *parent) :
    QDialog(parent)
{
    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());

    labelSMU = new QLabel(trUtf8("SMU:"));
    editSMU = new LineEdit;
    QSqlQueryModel *smuModel = new QSqlQueryModel;
    smuModel->setQuery("SELECT subdivisionname FROM subdivision");
    QCompleter *smuCompleter = new QCompleter(smuModel);
    smuCompleter->setCompletionMode(QCompleter::PopupCompletion);
    smuCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editSMU->setCompleter(smuCompleter);

//    QToolButton *addButton = new QToolButton;
//    QPixmap addPix(":/add.png");
//    addButton->setIcon(addPix);
//    addButton->setToolTip(trUtf8("Add new record"));
//    connect(addButton,SIGNAL(clicked()),this,SLOT(addOrgRecord()));
//    addButton->setStyleSheet(styleSheetString);

    QToolButton *seeSubButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeSubButton->setIcon(seePix);
    seeSubButton->setToolTip(trUtf8("See select item"));
    connect(seeSubButton,SIGNAL(clicked()),this,SLOT(seeSubRecord()));
    seeSubButton->setStyleSheet(styleSheetString);

    QToolButton *listSubButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listSubButton->setIcon(listPix);
    listSubButton->setToolTip(trUtf8("See list of item"));
    connect(listSubButton,SIGNAL(clicked()),this,SLOT(listSubRecord()));
    listSubButton->setStyleSheet(styleSheetString);


    labelTestTask = new QLabel(trUtf8("Test task:"));
    editTestTask = new LineEdit;
    QSqlQueryModel *testTaskModel = new QSqlQueryModel;
    testTaskModel->setQuery("SELECT cipher FROM testtask");
    QCompleter *testTaskCompleter = new QCompleter(testTaskModel);
    testTaskCompleter->setCompletionMode(QCompleter::PopupCompletion);
    testTaskCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editTestTask->setCompleter(testTaskCompleter);

    QToolButton *seeTTButton = new QToolButton;
    seeTTButton->setIcon(seePix);
    seeTTButton->setToolTip(trUtf8("See select item"));
    connect(seeTTButton,SIGNAL(clicked()),this,SLOT(seeTestTaskRecord()));
    seeTTButton->setStyleSheet(styleSheetString);

    QToolButton *listTTButton = new QToolButton;
    listTTButton->setIcon(listPix);
    listTTButton->setToolTip(trUtf8("See list of item"));
    connect(listTTButton,SIGNAL(clicked()),this,SLOT(listTestTaskReccord()));
    listTTButton->setStyleSheet(styleSheetString);

    QPushButton *execute = new QPushButton(trUtf8("Execute"));
    connect(execute,SIGNAL(clicked()),this,SLOT(printPreview()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelSMU,0,0);
    mainLayout->addWidget(editSMU,0,1);
    mainLayout->addWidget(seeSubButton,0,2);
    mainLayout->addWidget(listSubButton,0,3);
    mainLayout->addWidget(labelTestTask,1,0);
    mainLayout->addWidget(editTestTask,1,1);
    mainLayout->addWidget(seeTTButton,1,2);
    mainLayout->addWidget(listTTButton,1,3);
    mainLayout->addWidget(execute,2,0,1,2);
    setLayout(mainLayout);
    setWindowTitle(trUtf8("Grafik Prom Bezop"));
}

void GrafiKPromBezop::printPreview()
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

int GrafiKPromBezop::printParagraph(QString paragraphString, QPainter *painter, QRect *rect, int spacing)
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

void GrafiKPromBezop::print(QPrinter *printer)
{
    QPainter painter(printer);

    QRect viewRegion(painter.viewport());
    int heightPage = viewRegion.height();
    int widthPage = viewRegion.width();

    painter.setFont(QFont("Times New Roman",12,QFont::Normal));
    painter.drawStaticText(3000,0,trUtf8("Utverzdau"));
    painter.drawStaticText(3000,120,trUtf8("Zamestitel directora"));
    painter.drawStaticText(3000,240,trUtf8("po bezopasnosti proizvodstva"));
    painter.drawStaticText(3000,360,trUtf8("_______________ Gataulin F.A."));
    painter.drawStaticText(3000,480,trUtf8("\"___\"_____________ 20__ goda."));

    painter.setFont(QFont("Times New Roman",12,QFont::Bold));
    QRect firstString(0,600,widthPage,100);
    painter.drawText(firstString,Qt::AlignHCenter,trUtf8("Grafik"));
    QRect secondString(0,700,widthPage,100);
    painter.drawText(secondString,Qt::AlignHCenter,trUtf8("prohozdeniya attestacii rukovoditeley i spezialistov"));
    QRect thirdhString(0,800,widthPage,100);
    QString val1 = trUtf8("podrazdeleniya: ");
    val1 += editSMU->text();
    painter.drawText(thirdhString,Qt::AlignHCenter|Qt::TextWordWrap,val1);
    QSqlQuery query;
    query.prepare("SELECT testtaskname FROM testtask WHERE cipher = :cipher;");
    query.bindValue(":cipher",editTestTask->text());
    query.exec();
    query.next();
    QString val2 = query.value(0).toString();
    QRect fourthString(0,900,widthPage,200);
    int mHight = printParagraph(val2,&painter,&fourthString,0);
    painter.drawText(fourthString,Qt::AlignHCenter,val2);

    int shapkaTop = 250;
    int shapkaBottom = 320;

    QRect numerCol(0,shapkaTop,200,shapkaBottom);
    QRect employeeCol(200,shapkaTop,1320,shapkaBottom);
    QRect postCol(1520,shapkaTop,1320,shapkaBottom);
    QRect prichCol(2840,shapkaTop,800,shapkaBottom);
    QRect normiCol(3640,shapkaTop,500,shapkaBottom);
    QRect primechCol(4140,shapkaTop,550,shapkaBottom);

    painter.drawRect(numerCol);
    painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(numerCol,Qt::AlignCenter,trUtf8("N\np/p"));

    painter.drawRect(employeeCol);
    painter.drawText(employeeCol,Qt::AlignCenter,trUtf8("FIO"));

    painter.drawRect(postCol);
    painter.drawText(postCol,Qt::AlignCenter,trUtf8("Post"));

    painter.drawRect(prichCol);
    painter.drawText(prichCol,Qt::AlignCenter,trUtf8("N protokola i data\nattestazii"));

    painter.drawRect(normiCol);
    painter.drawText(normiCol,Qt::AlignCenter,trUtf8("Data\nsledujuzei\nattestazii"));

    painter.drawRect(primechCol);
    painter.drawText(primechCol,Qt::AlignCenter,trUtf8("Primechanie"));


}

void GrafiKPromBezop::seeSubRecord()
{
    QSqlQuery query;
    query.prepare("SELECT subdivisionid FROM subdivision WHERE subdivisionname = :name AND organizationid = 'OWN000000001';");
    query.bindValue(":name",editSMU->text());
    query.exec();
    query.next();
    SubDivisionForm subForm(query.value(0).toString(),this,false);
    subForm.exec();
}

void GrafiKPromBezop::listSubRecord()
{
    QSqlQuery query;
    query.prepare("SELECT subdivisionid FROM subdivision WHERE subdivisionname = :name AND organizationid = 'OWN000000001';");
    query.bindValue(":name",editSMU->text());
    query.exec();
    query.next();
    ViewListTable viewListTable(query.value(0).toString(),"subdivision",this);
    viewListTable.exec();
    QSqlQuery queryName;
    queryName.prepare("SELECT subdivisionname FROM subdivision WHERE subdivisionid = :id;");
    queryName.bindValue(":id",viewListTable.returnValue());
    queryName.exec();
    queryName.next();
    editSMU->setText(queryName.value(0).toString());
}

void GrafiKPromBezop::seeTestTaskRecord()
{
    QSqlQuery query;
    query.prepare("SELECT testtaskid FROM testtask WHERE cipher = :cipher;");
    query.bindValue(":cipher",editTestTask->text());
    query.exec();
    query.next();
    TestTask ttForm(query.value(0).toString(),this,false);
    ttForm.exec();
}

void GrafiKPromBezop::listTestTaskReccord()
{
    QSqlQuery query;
    query.prepare("SELECT testtaskid FROM testtask WHERE cipher = :cipher;");
    query.bindValue(":cipher",editTestTask->text());
    query.exec();
    query.next();
    ViewListTable viewListTable(query.value(0).toString(),"testtask",this);
    viewListTable.exec();
    QSqlQuery queryName;
    queryName.prepare("SELECT cipher FROM testtask WHERE testtaskid = :id;");
    queryName.bindValue(":id",viewListTable.returnValue());
    queryName.exec();
    queryName.next();
    editTestTask->setText(queryName.value(0).toString());
}
