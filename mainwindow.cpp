#include <QtWidgets>
#include <QtSql>

#include "databasedirection.h"
#include "mainwindow.h"
#include "lineedit.h"
#include "photolabel.h"
#include "employee.h"
#include "backupftp.h"
#include "prefixnumber.h"
#include "organization.h"
#include "subdivision.h"
#include "postform.h"
#include "employee.h"
#include "xmlimport.h"
#include "searchform.h"
#include "photoload.h"
#include "contract.h"
#include "grafikrabot.h"
#include "ftpform.h"
#include "security.h"
#include "safetyrequirements.h"
#include "testtask.h"
#include "legalact.h"
#include "safetyreqprod.h"
#include "venue.h"
#include "trainingsafety.h"
#include "grafikprombezop.h"
#include "boolitemdelegate.h"
#include "laspecial.h"
#include "lanormlinkform.h"
#include "latermform.h"
#include "bolditemdelegate.h"
#include "laclassform.h"
#include "employeecertification.h"
#include "numprefix.h"
#include "uploadbase.h"
#include "laproductionfactor.h"
#include "sizform.h"
#include "dateitemdelegate.h"
#include "harform.h"
#include "postsizform.h"
//#include "postsiztableform.h"
#include "siznaimform.h"
#include "importxmltable.h"
#include "allemployeesiz.h"
#include "razmerstatistic.h"
#include "programmsettings.h"
#include "prichinaobuchform.h"
#include "vidatestlulform.h"
#include "obosoblform.h"
#include "typedocform.h"
#include "obuchenielulform.h"
#include "sroknoskiform.h"
#include "importexcelform.h"
#include "putfile.h"
#include "scoreform.h"
#include "nomenform.h"
#include "obuchenieptmform.h"
#include "obuchenievisform.h"
#include "sizpostform.h"
#include "smtp.h"
#include "sqlform.h"
#include "stagirovkaform.h"
#include "kommisiyaform.h"
#include "nashafirmaform.h"
#include "attestpbpostform.h"
#include "exchangefile.h"
#include "update.h"
#include "coloritemdelegate.h"

MainWindow::MainWindow()
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);

    //::setFont(QFont("Arial",12,QFont::Normal));
    setFilter =false;
    readSettings();
    createActions();
    createMenus();
    createRightPanel();
    createContextMenu();

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(1,1);
    setCentralWidget(splitter);

    QString programmName = trUtf8("Сотрудники предприятия v.");
    QSettings iniSettings("EmployeeClient.ini",QSettings::IniFormat);
    QString version = iniSettings.value("Version").toString();
    programmName += version;
    setWindowTitle(programmName);

    QPixmap pixWater(":/client.png");
    setWindowIcon(pixWater);

    searchString = new QLabel(this);
    searchString->setGeometry(4,4,200,30);
    searchString->setMargin(4);
    searchString->setStyleSheet("QLabel{font: bold; background-color : #FFFF99; color : blue; }" );
    searchString->setVisible(false);

    viewEmployee();

    fileExchange.setFileName("Message.txt");
    fileExchange.open(QIODevice::ReadWrite);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(fileToServer()));
    timer->start(300000);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QString tm = templateModel->tableName();
    if( event->key() == Qt::Key_Escape ){
        searchString->setText("");
        if(searchString->isVisible()){
            searchString->setVisible(false);
            statusBar()->setVisible(false);
        }
        setFilter = false;
        viewTemplateTable(tm);
    }
    if((event->key() >= 0x0410 && event->key() < 0x044f) || event->key() == Qt::Key_Space || event->key() == Qt::Key_Backspace){
        if(!searchString->isVisible()){
            statusBar()->addWidget(searchString);
            statusBar()->show();
            searchString->show();
            searchString->setFocus();
        }
        QString txt = searchString->text();
        if(event->key() == Qt::Key_Backspace){
            txt.chop(1);
        }else{
            txt += event->text();
        }
        searchString->setText(txt);
        QString rr = searchString->text().toUpper();
        filterTable = rr;
        setFilter = true;
        viewTemplateTable(tm);
    }
    if(event->key() == Qt::Key_F5){
        tableView->reset();
        templateModel->select();
        tableView->repaint();
        tableView->selectRow(0);
    }
}

MainWindow::~MainWindow()
{   
    QSqlQuery query;
    query.exec("select tbl_name from sqlite_master where type = 'table'");

    QSqlQuery queryT;

    while(query.next()){
      queryT.exec(QString("TRUNCATE TABLE %1").arg(query.value(0).toString()));
      queryT.clear();
      queryT.exec(QString("DROP TABLE %1").arg(query.value(0).toString()));
      queryT.clear();
    }

    QSqlDatabase db = QSqlDatabase::database();
    db.close();

    QFile file;
    file.setFileName("CE_SQLite.arh");
    file.remove();
    file.setFileName("CE_SQLite.dat");
    file.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
    file.remove();
    QFile forDel;
    forDel.setFileName("Message.txt");
    forDel.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
    forDel.close();
    forDel.remove();
}

void MainWindow::createRightPanel()
{
    rightPanel = new QWidget(this);
    tableView = new QTableView(this);
    tableView->setStyleSheet("QTableView::item {selection-color: black; selection-background-color: rgba(30, 144, 255, 30%);}");
    templateModel = new QSqlRelationalTableModel(this);
    templateModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    queryModel = new QSqlQueryModel(this);
    tableLabel = new QLabel;
    QPixmap pixAdd(":/add.png");
    addRecordTable = new QPushButton(trUtf8("Добавить"));
    addRecordTable->setIcon(pixAdd);
    connect(addRecordTable,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPixmap pixDelete(":/delete.png");
    deleteRecordTable = new QPushButton(trUtf8("Удалить"));
    deleteRecordTable->setIcon(pixDelete);
    connect(deleteRecordTable,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QPixmap pixEdit(":/edit.png");
    editRecordTable = new QPushButton(trUtf8("Редактировать"));
    editRecordTable->setIcon(pixEdit);
    connect(editRecordTable,SIGNAL(clicked()),this,SLOT(editRecordOfTable()));

    QHBoxLayout *buttonBox = new QHBoxLayout;

    buttonBox->addWidget(addRecordTable);
    buttonBox->addWidget(deleteRecordTable);
    buttonBox->addWidget(editRecordTable);
    buttonBox->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(buttonBox);
    rightLayout->addWidget(tableLabel);
    rightLayout->addWidget(tableView);
    rightPanel->setLayout(rightLayout);
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editRecordOfTable()));
}

void MainWindow::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("MainWindow").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("MainWindow", saveGeometry());
    settings.remove("CurrentUser");

    getProcedure();
}

void MainWindow::createActions()
{
    //File Action
    prefixAction = new QAction(trUtf8("Префикс..."),this);
    connect(prefixAction,SIGNAL(triggered()),this,SLOT(prefixProcedure()));
    getXMLAction = new QAction(trUtf8("Импорт из XML файла..."),this);
    connect(getXMLAction,SIGNAL(triggered()),this,SLOT(getXMLProcedure()));
    getPhotoAction = new QAction(trUtf8("Импорт фото..."),this);
    connect(getPhotoAction,SIGNAL(triggered()),this,SLOT(getPhotoProcedure()));
    importXmlTableAction = new QAction(trUtf8("Импорт XML таблицы..."),this);
    connect(importXmlTableAction,SIGNAL(triggered()),this,SLOT(importXmlTable()));
    importExcelAction = new QAction(trUtf8("Импорт остатков по складам (Excel)..."),this);
    connect(importExcelAction,SIGNAL(triggered()),this,SLOT(importExcelOstatki()));
    nashaFirmaAction = new QAction(trUtf8("Наименование нашей фирмы..."),this);
    connect(nashaFirmaAction,SIGNAL(triggered()),this,SLOT(viewNashaFirma()));
    exitAction = new QAction(trUtf8("Выход..."),this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    //Reference Action
    employeeAction = new QAction(trUtf8("Сотрудники..."),this);
    connect(employeeAction,SIGNAL(triggered()),this,SLOT(viewEmployee()));
    organizationAction = new QAction(trUtf8("Организации..."),this);
    connect(organizationAction,SIGNAL(triggered()),this,SLOT(viewOrganization()));
    subdivisionAction = new QAction(trUtf8("Подразделения..."),this);
    connect(subdivisionAction,SIGNAL(triggered()),this,SLOT(viewSubdivision()));
    postAction = new QAction(trUtf8("Должности..."),this);
    connect(postAction,SIGNAL(triggered()),this,SLOT(viewPost()));
    grafikRabot = new QAction(trUtf8("График работ..."),this);
    connect(grafikRabot,SIGNAL(triggered()),this,SLOT(viewGrafikRabot()));
    prichinaObuchAction = new QAction(trUtf8("Причина обучения..."),this);
    connect(prichinaObuchAction,SIGNAL(triggered()),this,SLOT(viewPrichinaObuch()));
    vidAtestLulAction = new QAction(trUtf8("Вид аттестации (люльки)..."),this);
    connect(vidAtestLulAction,SIGNAL(triggered()),this,SLOT(viewVidAtestLul()));
    obosoblAction = new QAction(trUtf8("Обособленка..."),this);
    connect(obosoblAction,SIGNAL(triggered()),this,SLOT(viewObosobl()));
    typeDocAction = new QAction(trUtf8("Тип документа..."),this);
    connect(typeDocAction,SIGNAL(triggered()),this,SLOT(viewTypeDoc()));

//    sizAction = new QAction(trUtf8("СИЗ..."),this);
//    connect(sizAction,SIGNAL(triggered()),this,SLOT(viewSiz()));
//    postSizTableAction = new QAction(trUtf8("Normi SIZ..."),this);
//    connect(postSizTableAction,SIGNAL(triggered()),this,SLOT(viewPostSizTable()));
    harAction = new QAction(trUtf8("Характеристика..."),this);
    connect(harAction,SIGNAL(triggered()),this,SLOT(viewHar()));
    postSizAction = new QAction(trUtf8("Нормы СИЗ..."),this);
    connect(postSizAction,SIGNAL(triggered()),this,SLOT(viewPostSiz()));
//    sizNaimAction = new QAction(trUtf8("SIZ Naim..."),this);
//    connect(sizNaimAction,SIGNAL(triggered()),this,SLOT(viewSizNaim()));
//    nomenAction = new QAction(trUtf8("Номенклатура СИЗ..."),this);
//    connect(nomenAction,SIGNAL(triggered()),this,SLOT(viewNomen()));

    securityProductionAction = new QAction(trUtf8("Безопасность..."),this);
    connect(securityProductionAction,SIGNAL(triggered()),this,SLOT(viewSecurityProduction()));
    safetyRequirements = new QAction(trUtf8("Safety requirements..."),this);
    connect(safetyRequirements,SIGNAL(triggered()),this,SLOT(viewSafetyRequirements()));
    testTask = new QAction(trUtf8("Test tasks..."),this);
    connect(testTask,SIGNAL(triggered()),this,SLOT(viewTestTask()));
    legalActs = new QAction(trUtf8("Legal acts..."),this);
    connect(legalActs,SIGNAL(triggered()),this,SLOT(viewLegalActs()));
    venueAction = new QAction(trUtf8("Venue..."),this);
    connect(venueAction,SIGNAL(triggered()),this,SLOT(viewVenue()));

    laProductionFactors = new QAction(trUtf8("Production factors..."),this);
    connect(laProductionFactors,SIGNAL(triggered()),this,SLOT(viewLaProductionFactor()));
    laSpecial = new QAction(trUtf8("Special Tables..."),this);
    connect(laSpecial,SIGNAL(triggered()),this,SLOT(viewLaSpecial()));
    laNormLink = new QAction(trUtf8("Norm link..."),this);
    connect(laNormLink,SIGNAL(triggered()),this,SLOT(viewLaNormLink()));
    laTerm = new QAction(trUtf8("Term..."),this);
    connect(laTerm,SIGNAL(triggered()),this,SLOT(viewLaTerm()));
    laClass = new QAction(trUtf8("Class..."),this);
    connect(laClass,SIGNAL(triggered()),this,SLOT(viewLaClass()));
    komissiyaAction = new QAction(trUtf8("Комиссия..."),this);
    connect(komissiyaAction,SIGNAL(triggered()),this,SLOT(viewKomissiya()));

    //Documents Action
    contractAction = new QAction(trUtf8("Контракт безопасности движения..."),this);
    connect(contractAction,SIGNAL(triggered()),this,SLOT(viewContract()));
    internshipAction = new QAction(trUtf8("Internship..."),this);
    trafficInfractionAction = new QAction(trUtf8("Traffic Infraction..."),this);
    accidentsAction = new QAction(trUtf8("Accidents..."),this);
    preventiveWorkAction = new QAction(trUtf8("Preventive Work..."),this);
    medicalExaminationAction = new QAction(trUtf8("Medical Examination..."),this);
    srpAction = new QAction(trUtf8("Safety Requirement Production..."),this);
    connect(srpAction,SIGNAL(triggered()),this,SLOT(viewSRP()));
    trainingSafetyAction = new QAction(trUtf8("Training Safety..."),this);
    connect(trainingSafetyAction,SIGNAL(triggered()),this,SLOT(viewTrainingSafety()));
    laProtocol = new QAction(trUtf8("Протокол..."),this);
    employeeCertificationAction = new QAction(trUtf8("Employee Certification..."),this);
    connect(employeeCertificationAction,SIGNAL(triggered()),this,SLOT(viewEmpCert()));
    obuchLulAction = new QAction(trUtf8("Обучение (Люльки)..."),this);
    connect(obuchLulAction,SIGNAL(triggered()),this,SLOT(viewObuchLul()));
    obuchVisAction = new QAction(trUtf8("Обучение (Высота)..."),this);
    connect(obuchVisAction,SIGNAL(triggered()),this,SLOT(viewObuchVis()));
    obuchPtmAction = new QAction(trUtf8("Обучение ПТМ..."),this);
    connect(obuchPtmAction,SIGNAL(triggered()),this,SLOT(viewObuchPtm()));
    scoreAction = new QAction(trUtf8("Счета СИЗ..."),this);
    connect(scoreAction,SIGNAL(triggered()),this,SLOT(viewScore()));
    stagirovkaAction = new QAction(trUtf8("Стажировка..."),this);
    connect(stagirovkaAction,SIGNAL(triggered()),this,SLOT(viewStagirovka()));
    attestPBPostAction = new QAction(trUtf8("Область аттестации (Промбезопасность)..."),this);
    connect(attestPBPostAction,SIGNAL(triggered()),this,SLOT(viewAttestPBPost()));


    //Reports Action
    grafikPromBez = new QAction(trUtf8("График пром безопасности..."),this);
    connect(grafikPromBez,SIGNAL(triggered()),this,SLOT(reportGrafikPromBez()));
    allEmployeeSiz = new QAction(trUtf8("Потребность выдачи СИЗ..."),this);
    connect(allEmployeeSiz,SIGNAL(triggered()),this,SLOT(reportAllEmployeeSiz()));
    razmeriAction = new QAction(trUtf8("Размеры..."),this);
    connect(razmeriAction,SIGNAL(triggered()),this,SLOT(reportStatRazmerov()));
    srokNoskiAction = new QAction(trUtf8("Окончание сроков носки СИЗ..."),this);
    connect(srokNoskiAction,SIGNAL(triggered()),this,SLOT(reportSrokNoski()));

    //Services Action
    programmSettingsAction = new QAction(trUtf8("Установки программы..."),this);
    connect(programmSettingsAction,SIGNAL(triggered()),this,SLOT(programmSettings()));
    ftpAction = new QAction(trUtf8("FTP..."),this);
    connect(ftpAction,SIGNAL(triggered()),this,SLOT(ftpForm()));
    putAction = new QAction(trUtf8("Выложить базу на FTP"),this);
    connect(putAction,SIGNAL(triggered()),this,SLOT(putProcedure()));
    getAction = new QAction(trUtf8("Получить базу с FTP"),this);
    connect(getAction,SIGNAL(triggered()),this,SLOT(getProcedure()));
    vaccumAction = new QAction(trUtf8("Vaccum Базы"),this);
    connect(vaccumAction,SIGNAL(triggered()),this,SLOT(vaccumProcedure()));
    sqlAction = new QAction(trUtf8("SQL запрос"),this);
    connect(sqlAction,SIGNAL(triggered()),this,SLOT(sqlQuery()));

    //Help Action
    aboutQtAction = new QAction(trUtf8("О Qt..."),this);
    connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    searchAction = new QAction(trUtf8("Поиск..."),this);
    connect(searchAction,SIGNAL(triggered()),this,SLOT(searchProcedure()));
    aboutProgAction = new QAction(trUtf8("О программе..."),this);
    connect(aboutProgAction,SIGNAL(triggered()),this,SLOT(aboutProgProcedure()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(trUtf8("Файл"));
    fileMenu->addAction(prefixAction);
    fileMenu->addAction(nashaFirmaAction);
    importMenu = fileMenu->addMenu(trUtf8("Импорт ..."));
    importMenu->addAction(importExcelAction);
    importMenu->addAction(getXMLAction);
    importMenu->addAction(getPhotoAction);
    importMenu->addAction(importXmlTableAction);
    fileMenu->addAction(exitAction);

    referenceMenu = menuBar()->addMenu(trUtf8("Справочники"));
    referenceMenu->addAction(employeeAction);
    referenceMenu->addAction(organizationAction);
    referenceMenu->addAction(subdivisionAction);
    referenceMenu->addAction(postAction);
    referenceMenu->addAction(obosoblAction);
    referenceMenu->addAction(grafikRabot);
    referenceMenu->addAction(prichinaObuchAction);
    referenceMenu->addAction(vidAtestLulAction);
    referenceMenu->addAction(typeDocAction);
    referenceMenu->addAction(komissiyaAction);
    sizMenu = referenceMenu->addMenu(trUtf8("СИЗ..."));
//    sizMenu->addAction(sizNaimAction);
//    sizMenu->addAction(sizAction);
    sizMenu->addAction(postSizAction);
//    sizMenu->addAction(postSizTableAction);
    sizMenu->addAction(harAction);
    //sizMenu->addAction(nomenAction);
    referenceMenu->addSeparator();
    securityProductionMenu = referenceMenu->addMenu(trUtf8("Безопасность производства..."));
    securityProductionMenu->addAction(attestPBPostAction);
    securityProductionMenu->addAction(securityProductionAction);
    securityProductionMenu->addAction(safetyRequirements);
    securityProductionMenu->addAction(testTask);
    securityProductionMenu->addAction(legalActs);
    securityProductionMenu->addAction(venueAction);
    referenceMenu->addSeparator();
    laboratoriyaRef = referenceMenu->addMenu(trUtf8("Лаборатория..."));
    laboratoriyaRef->addAction(laProductionFactors);
    laboratoriyaRef->addAction(laSpecial);
    laboratoriyaRef->addAction(laNormLink);
    laboratoriyaRef->addAction(laTerm);
    laboratoriyaRef->addAction(laClass);

    documentMenu = menuBar()->addMenu(trUtf8("Документы"));
    trafficSecurityMenu = documentMenu->addMenu(trUtf8("Безопасность движения..."));
    trafficSecurityMenu->addAction(contractAction);
    trafficSecurityMenu->addAction(internshipAction);
    trafficSecurityMenu->addAction(trafficInfractionAction);
    trafficSecurityMenu->addAction(accidentsAction);
    trafficSecurityMenu->addAction(preventiveWorkAction);
    trafficSecurityMenu->addAction(medicalExaminationAction);
    documentMenu->addSeparator();
    securityProduction = documentMenu->addMenu(trUtf8("Безопасность производства..."));
    securityProduction->addAction(srpAction);
    securityProduction->addAction(trainingSafetyAction);
    securityProduction->addAction(employeeCertificationAction);
    documentMenu->addSeparator();
    laboratoriyaDoc = documentMenu->addMenu(trUtf8("Лаборатория..."));
    laboratoriyaDoc->addAction(laProtocol);
    documentMenu->addSeparator();
    obuchDoc = documentMenu->addMenu(trUtf8("Обучение..."));
    obuchDoc->addAction(obuchLulAction);
    obuchDoc->addAction(obuchVisAction);
    obuchDoc->addAction(obuchPtmAction);
    scoreMenu = documentMenu->addMenu(trUtf8("Счета..."));
    scoreMenu->addAction(scoreAction);
    ohranaTrudaMenu = documentMenu->addMenu(trUtf8("Охрана труда..."));
    ohranaTrudaMenu->addAction(stagirovkaAction);

    reportMenu = menuBar()->addMenu(trUtf8("Отчеты"));
    reportMenu->addAction(grafikPromBez);
    reportMenu->addAction(allEmployeeSiz);
    reportMenu->addAction(srokNoskiAction);
    reportStatistic = reportMenu->addMenu(trUtf8("Статистика..."));
    reportStatistic->addAction(razmeriAction);

    serviceMenu = menuBar()->addMenu(trUtf8("Сервис"));
    serviceMenu->addAction(programmSettingsAction);
    serviceMenu->addAction(searchAction);
    serviceMenu->addAction(ftpAction);
    serviceMenu->addAction(putAction);
    serviceMenu->addAction(getAction);
    serviceMenu->addAction(vaccumAction);
    serviceMenu->addAction(sqlAction);

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(trUtf8("Помощь"));
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(aboutProgAction);

}

void MainWindow::aboutProgProcedure()
{
    QSettings iniSettings("EmployeeClient.ini",QSettings::IniFormat);
    QString version = iniSettings.value("Version").toString();
//    int fontId = iniSettings.value("Font").toInt();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(trUtf8("Сотрудники предприятия"));
    QPixmap pixWater(":/client.png");
    msgBox.setIconPixmap(pixWater);
    QString textMessage = trUtf8("<p style=\"color:darkblue\"; style=\"font-family:Times New Roman\"; "
                             "style=\"font-size :20pt\">"
                             "Сотрудники предприятия "
            "<br/> (SQLite version) ");
    textMessage += version;
    textMessage += trUtf8("</p>"
            "<p style=\"color:darkgreen\"; style=\"font-family:Times New Roman\"; "
            "style=\"font-size :12pt\">Copyright 2011-16 A+O Batrakov Inc.</p>"
            "<p style=\"font-size :12pt\">"
            "Программа предназначена для ведения учета сотрудников на предприятии.</p>"
            "<p style=\"font-family:Times New Roman\"; style=\"font-size :10pt\">"
            "По вопросам улучшения или изменения (расширения) функционала программы, пишите на почту: "
            "<a href=\"www.mail.ru\">hothit@list.ru</a>;</p>");
    msgBox.setText(textMessage);
    //msgBox.setFont(QFont(QFontDatabase::applicationFontFamilies(fontId).first()));
    msgBox.addButton(QMessageBox::Yes);
    msgBox.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(trUtf8("Сотрудники предприятия"));
    msgBox.setText(trUtf8("Закрыть программу?"));
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *yesButton = msgBox.addButton(QMessageBox::Yes);
    yesButton->setText(trUtf8("Да"));

    QPushButton *noButton = msgBox.addButton(QMessageBox::No);
    noButton->setText(trUtf8("Отмена"));
    msgBox.setDefaultButton(noButton);

    noButton->setStyleSheet("QPushButton:hover {color: red}");
    int r = msgBox.exec();
    if(r == QMessageBox::Yes){
        MainWindow::hide();
        writeSettings();
        event->accept();

        qApp->quit();
        removeFiles();
    }else if(r == QMessageBox::No){
        event->ignore();
    }
}

void MainWindow::prefixProcedure()
{
    PrefixNumber prefix(this);
    prefix.exec();
}

void MainWindow::viewOrganization()
{
    viewTemplateTable("organization");
}

void MainWindow::viewSubdivision()
{
    viewTemplateTable("subdivision");
}

void MainWindow::viewPost()
{
    viewTemplateTable("post");
}

void MainWindow::viewEmployee()
{
    viewTemplateTable("employee");
}

void MainWindow::viewContract()
{
    viewTemplateTable("contractdoc");
}

void MainWindow::viewGrafikRabot()
{
    viewTemplateTable("grafikrabot");
}

void MainWindow::viewSecurityProduction()
{
    viewTemplateTable("security");
}

void MainWindow::viewSafetyRequirements()
{
    viewTemplateTable("safetyreq");
}

void MainWindow::viewTestTask()
{
    viewTemplateTable("testtask");
}

void MainWindow::viewLegalActs()
{
    viewTemplateTable("legalacts");
}

void MainWindow::viewSRP()
{
    viewTemplateTable("srp");
}

void MainWindow::viewVenue()
{
    viewTemplateTable("venue");
}

void MainWindow::viewTrainingSafety()
{
    viewTemplateTable("trainingsafety");
}

void MainWindow::viewLaNormLink()
{
    viewTemplateTable("lanormlink");
}

void MainWindow::viewLaTerm()
{
    viewTemplateTable("laterm");
}

void MainWindow::viewLaClass()
{
    viewTemplateTable("laclass");
}

void MainWindow::viewLaSpecial()
{
    viewTemplateTable("laspecial");
}

void MainWindow::viewEmpCert()
{
    viewTemplateTable("empcert");
}

void MainWindow::viewLaProductionFactor()
{
    viewTemplateTable("laproductionfactor");
}

void MainWindow::viewSiz()
{
    viewTemplateTable("siz");
}

void MainWindow::viewHar()
{
    viewTemplateTable("har");
}

void MainWindow::viewPostSiz()
{
    viewTemplateTable("postsiz");
}

//void MainWindow::viewPostSizTable()
//{
//    viewTemplateTable("postsiztable");
//}

void MainWindow::viewSizNaim()
{
    viewTemplateTable("siznaim");
}

void MainWindow::viewPrichinaObuch()
{
    viewTemplateTable("prichinaobuch");
}

void MainWindow::viewVidAtestLul()
{
    viewTemplateTable("vidatestlul");
}

void MainWindow::viewObosobl()
{
    viewTemplateTable("obosobl");
}

void MainWindow::viewTypeDoc()
{
    viewTemplateTable("typedoc");
}

void MainWindow::viewObuchLul()
{
    viewTemplateTable("obuchlul");
}

void MainWindow::viewObuchVis()
{
    viewTemplateTable("obuchvis");
}

void MainWindow::viewObuchPtm()
{
    viewTemplateTable("obuchptm");
}

void MainWindow::viewScore()
{
    viewTemplateTable("score");
}

void MainWindow::viewNomen()
{
    viewTemplateTable("nomen");
}

void MainWindow::viewStagirovka()
{
    viewTemplateTable("stagirovka");
}

void MainWindow::viewKomissiya()
{
    viewTemplateTable("komissiya");
}

void MainWindow::viewAttestPBPost()
{
    viewTemplateTable("attestpbpost");
}

void MainWindow::viewTemplateTable(QString tempTable)
{
    if(tableView->model())
    for(int i=0; i<tableView->model()->columnCount();i++){
        QAbstractItemDelegate* delegate = tableView->itemDelegateForColumn( i );
        //Remove the delegate from the view
        tableView->setItemDelegateForColumn( i, NULL );
        delete delegate;
    }
    tableView->clearSpans();
    templateModel->clear();
    templateModel->setTable(tempTable);

    QString strivgValue; bool delAll = false;
    if(tempTable == "organization"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        if(setFilter){
            templateModel->setFilter(QString("organizationname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Организация");
    }else if(tempTable == "subdivision"){
        templateModel->setHeaderData(1,Qt::Horizontal, trUtf8("Наименование подразделения"));
        templateModel->setHeaderData(2,Qt::Horizontal, trUtf8("Организация"));
        templateModel->setRelation(2,QSqlRelation("organization","organizationid","organizationname"));
        if(setFilter){
            templateModel->setFilter(QString("subdivisionname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Подразделение");
    }else if(tempTable == "post"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("ИТР"));
        if(setFilter){
            templateModel->setFilter(QString("postname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Должность");
    }
    else if(tempTable == "employee"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("ФИО"));
        templateModel->setRelation(5,QSqlRelation("subdivision","subdivisionid","subdivisionname"));
        templateModel->setHeaderData(5,Qt::Horizontal,trUtf8("Подразделение"));
        templateModel->setRelation(6,QSqlRelation("post","postid","postname"));
        templateModel->setHeaderData(6,Qt::Horizontal,trUtf8("Должность"));
        templateModel->setHeaderData(8,Qt::Horizontal,trUtf8("День рождения"));
        if(setFilter){
            templateModel->setFilter(QString("employeenameupper LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Сотрудники");
    }else if(tempTable == "contractdoc"){
        templateModel->setHeaderData(0,Qt::Horizontal,trUtf8("Номер"));
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Дата"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Сотрудник"));
        templateModel->setRelation(2,QSqlRelation("employee","employeeid","employeename"));
        if(setFilter){
            templateModel->setFilter(QString("contractdocname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Контракт");
    }else if(tempTable == "grafikrabot"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        if(setFilter){
            templateModel->setFilter(QString("grafikrabotname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("График работ");
    }else if(tempTable == "security"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        if(setFilter){
            templateModel->setFilter(QString("securityname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Безопасность");
    }else if(tempTable == "safetyreq"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        if(setFilter){
            templateModel->setFilter(QString("safetyreqname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Safety requirement");
    }else if(tempTable == "legalacts"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        if(setFilter){
            templateModel->setFilter(QString("legalactsname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Legal act");
    }else if(tempTable == "testtask"){
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Name"));
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Cipher"));
        if(setFilter){
            templateModel->setFilter(QString("testtaskname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Test task");
    }else if(tempTable == "srp"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Security"));
        templateModel->setRelation(1,QSqlRelation("security","securityid","securityname"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Safety Requirment"));
        templateModel->setRelation(2,QSqlRelation("safetyreq","safetyreqid","safetyreqname"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Cipher"));
        templateModel->setHeaderData(4,Qt::Horizontal,trUtf8("Test tasks"));
        templateModel->setRelation(4,QSqlRelation("testtask","testtaskid","testtaskname"));
        if(setFilter){
            templateModel->setFilter(QString("srp LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Safety Requirement Production");
    }else if(tempTable == "venue"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        if(setFilter){
            templateModel->setFilter(QString("venuename LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Venue");
    }else if(tempTable == "trainingsafety"){
        templateModel->setHeaderData(0,Qt::Horizontal,trUtf8("Номер"));
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Дата"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Venue"));
        templateModel->setRelation(2,QSqlRelation("venue","venueid","venuename"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Venue date"));
        templateModel->setHeaderData(4,Qt::Horizontal,trUtf8("Гомер протокола"));
        if(setFilter){
            templateModel->setFilter(QString("trainingsafetydate LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Training Safety");
    }else if(tempTable == "lanormlink"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Production Factor"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Number"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Description"));
        if(setFilter){
            templateModel->setFilter(QString("lanormlinkpfid LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Norm link");
    }else if(tempTable == "laterm"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("latermname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Term");
    }else if(tempTable == "laclass"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("laclassname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Class");
    }else if(tempTable == "laspecial"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("laspecialname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Special Tables");
    }else if(tempTable == "empcert"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("FIO"));
        templateModel->setRelation(1,QSqlRelation("employee","employeeid","employeename"));
        if(setFilter){
            templateModel->setFilter(QString("employeename LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Employee Certification");
    }else if(tempTable == "siz"){
        strivgValue = trUtf8("SIZ");
    }else if(tempTable == "har"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("harname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Harakteristika");
    }else if(tempTable == "postsiz"){
            strivgValue = trUtf8("Нормы СИЗ - Список кадровых должностей");

    }else if(tempTable == "siznaim"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("PostSiz Name"));
        templateModel->setRelation(1,QSqlRelation("postsiz","postsizid","postsizname"));
        if(setFilter){
            templateModel->setFilter(QString("siznaimname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Name SIZ");
    }else if(tempTable == "prichinaobuch"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("prichinaobuchname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Prichina Obuch");
    }else if(tempTable == "vidatestlul"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("vidatestlulname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Vid Atest Lul");
    }else if(tempTable == "obosobl"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("obosoblname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Obosoblenka");
    }else if(tempTable == "typedoc"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("typedocname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Type doc");
    }else if(tempTable == "obuchlul"){
        templateModel->setHeaderData(0,Qt::Horizontal,trUtf8("Number"));
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Dogovor"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Date"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Дата\nкорректировки"));
        templateModel->setHeaderData(4,Qt::Horizontal,trUtf8("Date Obuch"));
        templateModel->setHeaderData(5,Qt::Horizontal,trUtf8("Prichina"));
        templateModel->setRelation(5,QSqlRelation("prichinaobuch","prichinaobuchid","prichinaobuchname"));
        templateModel->setHeaderData(6,Qt::Horizontal,trUtf8("Вид обучения"));
        templateModel->setRelation(6,QSqlRelation("vidatestlul","vidatestlulid","vidatestlulname"));
        templateModel->setHeaderData(7,Qt::Horizontal,trUtf8("Номер протокола"));
        /*if(setFilter){
            templateModel->setFilter(QString("typedocname LIKE '%%1%'").arg(filterTable));
        }*/
        strivgValue = trUtf8("Obuchenie Lul");
    }else if(tempTable == "obuchvis"){
        templateModel->setHeaderData(0,Qt::Horizontal,trUtf8("Номер"));
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Договор"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Дата"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Дата\nкорректировки"));
        templateModel->setHeaderData(4,Qt::Horizontal,trUtf8("Дата\nобучения"));
        templateModel->setHeaderData(5,Qt::Horizontal,trUtf8("Группа"));
        templateModel->setHeaderData(6,Qt::Horizontal,trUtf8("Номер протокола"));
        /*if(setFilter){
            templateModel->setFilter(QString("typedocname LIKE '%%1%'").arg(filterTable));
        }*/
        strivgValue = trUtf8("Обучение (Высота)");
    }else if(tempTable == "obuchptm"){
        templateModel->setHeaderData(0,Qt::Horizontal,trUtf8("Номер"));
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Договор"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Дата"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Дата\nкорректировки"));
        templateModel->setHeaderData(4,Qt::Horizontal,trUtf8("Дата\nобучения"));
        templateModel->setHeaderData(5,Qt::Horizontal,trUtf8("Номер протокола"));
        /*if(setFilter){
            templateModel->setFilter(QString("typedocname LIKE '%%1%'").arg(filterTable));
        }*/
        strivgValue = trUtf8("Обучение ПТМ");
    }else if(tempTable == "score"){
        /*templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        if(setFilter){
            templateModel->setFilter(QString("typedocname LIKE '%%1%'").arg(filterTable));
        }*/
        strivgValue = trUtf8("Счета");
    }else if(tempTable == "nomen"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        if(setFilter){
            templateModel->setFilter(QString("nomenname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Номенклатура СИЗ");
    }else if(tempTable == "stagirovka"){
//        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
//        if(setFilter){
//            templateModel->setFilter(QString("nomenname LIKE '%%1%'").arg(filterTable));
//        }
        strivgValue = trUtf8("Стажировка");
    }else if(tempTable == "komissiya"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Председатель"));
        templateModel->setRelation(1,QSqlRelation("employee","employeeid","employeename"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Член комиссии"));
        templateModel->setRelation(2,QSqlRelation("employee","employeeid","employeename"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Член комиссии"));
        templateModel->setRelation(3,QSqlRelation("employee","employeeid","employeename"));
        strivgValue = trUtf8("Комиссия");
    }else if(tempTable == "attestpbpost"){
//        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Председатель"));
//        templateModel->setRelation(1,QSqlRelation("employee","employeeid","employeename"));
//        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Член комиссии"));
//        templateModel->setRelation(2,QSqlRelation("employee","employeeid","employeename"));
//        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Член комиссии"));
//        templateModel->setRelation(3,QSqlRelation("employee","employeeid","employeename"));
        strivgValue = trUtf8("Области аттестации ПБ");
    }
    else{
        tableView->setModel(0);
        tableLabel->clear();
        delAll = true;
    }
    if(!delAll){
        templateModel->select();
        while(templateModel->canFetchMore())
            templateModel->fetchMore();
        QHeaderView *head = tableView->horizontalHeader();
        connect(head,SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));

        if(tempTable == "siz"){
            QSqlQueryModel *model = new QSqlQueryModel;
            model->setQuery("SELECT (SELECT employeename FROM employee "
                            "AS emp WHERE emp.employeeid = si.employeeid) FROM siz AS si GROUP BY employeeid");
            tableView->setModel(model);
        }else{
            tableView->setModel(templateModel);
        }
        tableView->setWordWrap(true);
        if(tempTable == "users" || tempTable == "trainingsafety" || tempTable == "contractdoc" ||
                tempTable == "siz"){
            tableView->setColumnHidden(0, false);
        }else{
            tableView->setColumnHidden(0, true);
        }

        if(tempTable == "employee"){
            tableView->setColumnHidden(2,true);
            tableView->setColumnHidden(3,true);
            tableView->setColumnHidden(4,true);
            tableView->setColumnHidden(7,true);
            tableView->setItemDelegateForColumn(8,new DateItemDelegate);
            for(int i = 9; i < 19; ++i){
                tableView->setColumnHidden(i,true);
            }
            tableView->setColumnWidth(5,300);
            tableView->setColumnWidth(6,300);            
            tableView->setItemDelegate(new ColorItemDelegate);
        }

        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        if(tempTable == "siz"){
            tableView->setItemDelegateForColumn(4, new BoldItemDelegate(Qt::red));
            tableView->setItemDelegateForColumn(4, new DateItemDelegate);
        }
        if(tempTable == "post"){
            tableView->setColumnHidden(2,true);
            tableView->setItemDelegateForColumn(3, new BoolItemDelegate);
        }
        if(tempTable == "obuchlul" || tempTable == "obuchptm" || tempTable == "obuchvis"){
            tableView->setColumnHidden(0,false);
            tableView->setColumnHidden(1,true);
            tableView->setItemDelegateForColumn(2, new DateItemDelegate);
            tableView->setItemDelegateForColumn(3, new DateItemDelegate);
            tableView->setItemDelegateForColumn(4, new DateItemDelegate);
        }
        if(tempTable == "score"){
            tableView->setColumnHidden(0, false);
        }
        tableView->setAlternatingRowColors(true);
        if(tempTable == "post"){
            tableView->setColumnWidth(1,600);
        }else{
            tableView->setColumnWidth(1,300);
        }
        head->setStretchLastSection(true);
        tableLabel->clear();
        tableLabel->setText(trUtf8("Наименование таблицы: %1").arg(strivgValue));
        tableLabel->setStyleSheet("font: bold; color: darkblue;");
        setFilter = false;
        sortTable(1);
    }
}

void MainWindow::editRecordOfTable()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString userName = settings.value("CurrentUser").toString();
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE userid = :id");
    query.bindValue(":id",userName);
    query.exec();
    query.next();

    QString valueTemp, iDTemp;
    valueTemp = templateModel->tableName();
    QModelIndex index = tableView->currentIndex();
    if(index.isValid()){
        QSqlRecord record =templateModel->record(index.row());
        if(valueTemp == "organization"){
            iDTemp = record.value("organizationid").toString();
            if(query.value(0).toInt() == 1){
                OrganizationForm openForm(iDTemp, this,false);
                openForm.exec();
            }else{
                OrganizationForm openForm(iDTemp, this,true);
                openForm.exec();
            }
        }else if(valueTemp == "subdivision"){
            iDTemp = record.value("subdivisionid").toString();
            if(query.value(0).toInt() == 1){
                SubDivisionForm openForm(iDTemp, this,false);
                openForm.exec();
            }else{
                SubDivisionForm openForm(iDTemp, this,true);
                openForm.exec();
            }

        }else if(valueTemp == "post"){
            iDTemp = record.value("postid").toString();
            if(query.value(0).toInt() == 1){
                PostForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                PostForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "employee"){
            iDTemp = record.value("employeeid").toString();
            if(query.value(0).toInt() == 1){
                EmployeeForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                EmployeeForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "contractdoc"){
            iDTemp = record.value("contractdocid").toString();
            if(query.value(0).toInt() == 1){
                ContractForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                ContractForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "grafikrabot"){
            iDTemp = record.value("grafikrabotid").toString();
            if(query.value(0).toInt() == 1){
                GrafikRabotForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                GrafikRabotForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "security"){
            iDTemp = record.value("securityid").toString();
            if(query.value(0).toInt() == 1){
                Security openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                Security openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "safetyreq"){
            iDTemp = record.value("safetyreqid").toString();
            if(query.value(0).toInt() == 1){
                SafetyRequirements openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                SafetyRequirements openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "legalacts"){
            iDTemp = record.value("legalactsid").toString();
            if(query.value(0).toInt() == 1){
                LegalAct openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                LegalAct openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "testtask"){
            iDTemp = record.value("testtaskid").toString();
            if(query.value(0).toInt() == 1){
                TestTask openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                TestTask openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "srp"){
            iDTemp = record.value("srpid").toString();
            if(query.value(0).toInt() == 1){
                SafetyReqProd openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                SafetyReqProd openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "venue"){
            iDTemp = record.value("venueid").toString();
            if(query.value(0).toInt() == 1){
                Venue openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                Venue openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "trainingsafety"){
            iDTemp = record.value("trainingsafetyid").toString();
            if(query.value(0).toInt() == 1){
                TrainingSafety openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                TrainingSafety openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "lanormlink"){
            iDTemp = record.value("lanormlinkid").toString();
            if(query.value(0).toInt() == 1){
                LaNormLinkForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                LaNormLinkForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "laterm"){
            iDTemp = record.value("latermid").toString();
            if(query.value(0).toInt() == 1){
                LaTermForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                LaTermForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "laclass"){
            iDTemp = record.value("classid").toString();
            if(query.value(0).toInt() == 1){
                LaClassForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                LaClassForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "laspecial"){
            iDTemp = record.value("laspecialid").toString();
            if(query.value(0).toInt() == 1){
                LaSpecialForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                LaSpecialForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "empcert"){
            iDTemp = record.value("empcertid").toString();
            if(query.value(0).toInt() == 1){
                EmployeeCertification openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                EmployeeCertification openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "laproductionfactor"){
            iDTemp = record.value("laproductionfactorid").toString();
            if(query.value(0).toInt() == 1){
                LaProductionFactor openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                LaProductionFactor openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "siz"){
            iDTemp = record.value("sizid").toString();
            if(query.value(0).toInt() == 1){
                SizForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                SizForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "har"){
            iDTemp = record.value("harid").toString();
            if(query.value(0).toInt() == 1){
                HarForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                HarForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "postsiz"){
            iDTemp = record.value("postsizid").toString();
            if(query.value(0).toInt() == 1){
                SizPostForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                SizPostForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "siznaim"){
            iDTemp = record.value("siznaimid").toString();
            if(query.value(0).toInt() == 1){
                SizNaimForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                SizNaimForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "obosobl"){
            iDTemp = record.value("obosoblid").toString();
            if(query.value(0).toInt() == 1){
                ObosoblForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                ObosoblForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "prichinaobuch"){
            iDTemp = record.value("prichinaobuchid").toString();
            if(query.value(0).toInt() == 1){
                PrichinaObuchForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                PrichinaObuchForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "vidatestlul"){
            iDTemp = record.value("vidatestlulid").toString();
            if(query.value(0).toInt() == 1){
                VidAtestLulForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                VidAtestLulForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "typedoc"){
            iDTemp = record.value("typedocid").toString();
            if(query.value(0).toInt() == 1){
                TypeDocForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                TypeDocForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "obuchlul"){
            iDTemp = record.value("obuchlulid").toString();
            if(query.value(0).toInt() == 1){
                ObuchenieLulForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                ObuchenieLulForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "score"){
            iDTemp = record.value("scoreid").toString();
            if(query.value(0).toInt() == 1){
                ScoreForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                ScoreForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "nomen"){
            iDTemp = record.value("nomenid").toString();
            if(query.value(0).toInt() == 1){
                NomenForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                NomenForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "obuchptm"){
            iDTemp = record.value("obuchptmid").toString();
            if(query.value(0).toInt() == 1){
                ObucheniePtmForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                ObucheniePtmForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "obuchvis"){
            iDTemp = record.value("obuchvisid").toString();
            if(query.value(0).toInt() == 1){
                ObuchenieVisForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                ObuchenieVisForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "stagirovka"){
            iDTemp = record.value("stagirovkaid").toString();
            if(query.value(0).toInt() == 1){
                StagirovkaForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                StagirovkaForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "komissiya"){
            iDTemp = record.value("komissiyaid").toString();
            if(query.value(0).toInt() == 1){
                KommisiyaForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                KommisiyaForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }else if(valueTemp == "attestpbpost"){
            iDTemp = record.value("attestpbpostid").toString();
            if(query.value(0).toInt() == 1){
                AttestPBPostForm openForm(iDTemp,this,false);
                openForm.exec();
            }else{
                AttestPBPostForm openForm(iDTemp,this,true);
                openForm.exec();
            }
        }
    }
    updateRightPanel(index);
}

void MainWindow::addRecordOfTable()
{
    QString idTable;
    QString valueTemp = templateModel->tableName();
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString userName = settings.value("CurrentUser").toString();
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE userid = :id");
    query.bindValue(":id",userName);
    query.exec();
    query.next();
    if(query.value(0).toInt() == 1){     
        if(valueTemp == "organization"){
            OrganizationForm orgForm("",this,false);
            orgForm.exec();
            idTable = orgForm.returnValue();
        }else if(valueTemp == "subdivision"){
            SubDivisionForm subForm("",this,false);
            subForm.exec();
            idTable = subForm.returnValue();
        }else if(valueTemp == "post"){
            PostForm postForm("",this,false);
            postForm.exec();
            idTable = postForm.returnValue();;
        }else if(valueTemp == "employee"){
            EmployeeForm employeeForm("",this,false);
            employeeForm.exec();
            idTable = employeeForm.returnValue();
        }else if(valueTemp == "contractdoc"){
            ContractForm contractForm("",this,false);
            contractForm.exec();
            idTable = contractForm.returnValue();
        }else if(valueTemp == "grafikrabot"){
            GrafikRabotForm grafikForm("",this,false);
            grafikForm.exec();
            idTable = grafikForm.returnValue();
        }else if(valueTemp == "security"){
            Security openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "safetyreq"){
            SafetyRequirements openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "legalacts"){
            LegalAct openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "testtask"){
            TestTask openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "srp"){
            SafetyReqProd openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "venue"){
            Venue openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "trainingsafety"){
            TrainingSafety openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "lanormlink"){
            LaNormLinkForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "laterm"){
            LaTermForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "laclass"){
            LaClassForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "laspecial"){
            LaSpecialForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "empcert"){
            EmployeeCertification openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "laproductionfactor"){
            LaProductionFactor openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "siz"){
            SizForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "har"){
            HarForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "postsiz"){
            SizPostForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "siznaim"){
            SizNaimForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "obosobl"){
            ObosoblForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "prichinaobuch"){
            PrichinaObuchForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "vidatestlul"){
            VidAtestLulForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "typedoc"){
            TypeDocForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "obuchlul"){
            ObuchenieLulForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "score"){
            ScoreForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "nomen"){
            NomenForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "obuchptm"){
            ObucheniePtmForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "obuchvis"){
            ObuchenieVisForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "stagirovka"){
            StagirovkaForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "komissiya"){
            KommisiyaForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }else if(valueTemp == "attestpbpost"){
            AttestPBPostForm openForm("",this,false);
            openForm.exec();
            idTable = openForm.returnValue();
        }

        QSqlQuery query;
        QString val1 = "SELECT * FROM ";
        val1 += valueTemp;
        val1 += " WHERE ";
        val1 += valueTemp;
        val1 += "id = :id";
        query.prepare(val1);

        query.bindValue(":id",idTable);
        query.exec();
        query.next();

        filterTable = query.value(2).toString();
        setFilter = true;
        viewTemplateTable(valueTemp);
        QModelIndex modIndex = tableView->currentIndex();
        MainWindow::updateRightPanel(modIndex);
    }else{
        QMessageBox::warning(this,"Attention!!!","You don't add record, Seed Role!");
    }
}

void MainWindow::deleteRecordOfTable()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString userName = settings.value("CurrentUser").toString();
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE userid = :id");
    query.bindValue(":id",userName);
    query.exec();
    query.next();
    if(query.value(0).toInt() == 1){
        QModelIndex index = tableView->currentIndex();
        if(index.isValid()){
            QMessageBox msgBox(this);
            msgBox.setWindowTitle(trUtf8("Attention!!!"));
            msgBox.setText(trUtf8("Really delete?"));

            msgBox.setIcon(QMessageBox::Question);
            msgBox.addButton(QMessageBox::Yes);
            QPushButton *noButton = msgBox.addButton(QMessageBox::No);

            msgBox.setDefaultButton(noButton);

            noButton->setStyleSheet("QPushButton:hover {color: red}");
            int k = msgBox.exec();
            if(k == QMessageBox::Yes){
                QString valueTemp, iDValue;
                valueTemp = templateModel->tableName();
                QSqlRecord record = templateModel->record(index.row());
                if(valueTemp == "organization"){
                    iDValue = record.value("organizationid").toString();
                    OrganizationForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "subdivision"){
                    iDValue = record.value("subdivisionid").toString();
                    SubDivisionForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "post"){
                    iDValue = record.value("postid").toString();
                    PostForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "employee"){
                    iDValue = record.value("employeeid").toString();
                    EmployeeForm openForm(iDValue,this,false);
                    openForm.deleteEmployee();
                }else if(valueTemp == "contractdoc"){
                    iDValue = record.value("contractdocid").toString();
                    ContractForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "grafikrabot"){
                    iDValue = record.value("grafikrabotid").toString();
                    GrafikRabotForm openForm(iDValue,this,false);
                    openForm.exec();//??????????????????????????????????
                }else if(valueTemp == "security"){
                    iDValue = record.value("securityid").toString();
                    Security openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "safetyreq"){
                    iDValue = record.value("safetyreqid").toString();
                    SafetyRequirements openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "legalacts"){
                    iDValue = record.value("legalactsid").toString();
                    LegalAct openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "testtask"){
                    iDValue = record.value("testtaskid").toString();
                    TestTask openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "srp"){
                    iDValue = record.value("srpid").toString();
                    SafetyReqProd openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "venue"){
                    iDValue = record.value("srpid").toString();
                    Venue openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "trainingsafety"){
                    iDValue = record.value("trainingsafetyid").toString();
                    TrainingSafety openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "lanormlink"){
                    iDValue = record.value("lanormlinkid").toString();
                    LaNormLinkForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "laterm"){
                    iDValue = record.value("latermid").toString();
                    LaTermForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "laclass"){
                    iDValue = record.value("laclassid").toString();
                    LaClassForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "laspecial"){
                    iDValue = record.value("laspecialid").toString();
                    LaSpecialForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "empcert"){
                    iDValue = record.value("empcertid").toString();
                    EmployeeCertification openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "siz"){
                    iDValue = record.value("sizid").toString();
                    SizForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "har"){
                    iDValue = record.value("harid").toString();
                    HarForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "postsiz"){
                    iDValue = record.value("postsizid").toString();
                    SizPostForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "siznaim"){
                    iDValue = record.value("siznaimid").toString();
                    SizNaimForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "obosobl"){
                    iDValue = record.value("obosoblid").toString();
                    ObosoblForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "prichinaobuch"){
                    iDValue = record.value("prichinaobuchid").toString();
                    PrichinaObuchForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "vidatestlul"){
                    iDValue = record.value("vidatestlulid").toString();
                    VidAtestLulForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "typedoc"){
                    iDValue = record.value("typedocid").toString();
                    TypeDocForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "obuchlul"){
                    iDValue = record.value("obuchlulid").toString();
                    ObuchenieLulForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "score"){
                    iDValue = record.value("scoreid").toString();
                    ScoreForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "nomen"){
                    iDValue = record.value("nomenid").toString();
                    NomenForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "obuchptm"){
                    iDValue = record.value("obuchptmid").toString();
                    ObucheniePtmForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "obuchvis"){
                    iDValue = record.value("obuchvisid").toString();
                    ObuchenieVisForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "stagirovka"){
                    iDValue = record.value("obuchvisid").toString();
                    StagirovkaForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "komissiya"){
                    iDValue = record.value("komissiyaid").toString();
                    KommisiyaForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "attestpbpost"){
                    iDValue = record.value("attestpbpostid").toString();
                    AttestPBPostForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }
            }
        }
        updateRightPanel(index);
    }else{
        QMessageBox::warning(this,"Attention!!!","You don't add record, Seed Role!");
    }
}

void MainWindow::copyRecordOfTable()
{
    QString valueTemp, iDTemp;
    valueTemp = templateModel->tableName();
    if(valueTemp == "postsiz"){
        QModelIndex index = tableView->currentIndex();
        if(index.isValid()){
            QSqlRecord record =templateModel->record(index.row());
            if(valueTemp == "postsiz"){
                iDTemp = record.value("postsizid").toString();
                QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
                NumPrefix numP;
                settings.setValue("PostSizID", numP.getPrefix("postsiz"));
                settings.setValue("CopyPostSiz", 1);
                SizPostForm openForm(iDTemp, this,false);
                openForm.exec();
                settings.remove("PostSizID");
                settings.remove("CopyPostSiz");
            }
        }
        updateRightPanel(index);
    }
}

void MainWindow::updateRightPanel(QModelIndex inDex)
{
    templateModel->select();
    QHeaderView *head = tableView->horizontalHeader();
    head->setStretchLastSection(true);
    while(templateModel->canFetchMore())
        templateModel->fetchMore();
    tableView->setCurrentIndex(inDex);
    //tableView->set
}

void MainWindow::createContextMenu()
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
    editAction = new QAction(trUtf8("Edit Action"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTable()));

    copyAction = new QAction(trUtf8("Создать Копированием"),this);
    //copyAction->setIcon(pixEdit);
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copyRecordOfTable()));


    tableView->addAction(addAction);
    tableView->addAction(deleteAction);
    tableView->addAction(editAction);
    //if(templateModel->tableName() == "postsiz"){
        tableView->addAction(copyAction);
    //}
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::getXMLProcedure()
{
    XMLImport xmlImport(this);
    xmlImport.updateDir();
}

void MainWindow::ftpForm()
{
    FtpForm ftpF(this);
    ftpF.exec();
}

void MainWindow::searchProcedure()
{
    QString valueTempModel = templateModel->tableName();

    SearchForm searchForm(valueTempModel, this);
    searchForm.exec();

    QString rr = searchForm.returnValue();
    if(valueTempModel == "employee"){
       rr = rr.toUpper();
    }
    filterTable = rr;
    setFilter = true;

    viewTemplateTable(valueTempModel);
}

void MainWindow::getPhotoProcedure()
{
    PhotoLoad photoLoad(this);
    photoLoad.importPhotoProcedure();
}

void MainWindow::putProcedure()
{
    BackUpFTP backUp(this);
    backUp.BackUpProcedure();
}

void MainWindow::fileToServer()
{
    getProcedure();

    QFile file;

    file.setFileName("null.txt");
    file.remove();

    Update upDateFile(this);
    upDateFile.iniVersion();

    UpLoadBase upLoad(this);
    upLoad.exeVersion();

    removeFiles();
}

void MainWindow::getProcedure()
{
    fileExchange.close();
    fileExchange.setFileName("Message.txt");
    if(fileExchange.size() == 0){
        fileExchange.remove();
        return;
    }else{
        fileExchange.close();
    }
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString fileName = settings.value("numprefix").toString();
    fileName += "_SRV.txt";

    fileExchange.rename(fileName);
    fileExchange.close();

    PutFile putFile;
    putFile.putFile(fileName);

    PutFile putFtp1;
    QString nullFileName = "Null.txt";

    QFile nullFile;
    nullFile.setFileName(nullFileName);
    nullFile.open(QIODevice::WriteOnly);
    QByteArray rr = "22\n10";
    nullFile.write(rr);
    nullFile.close();
    putFtp1.putFile(nullFileName);
    nullFile.remove();
}

void MainWindow::vaccumProcedure()
{
    QSqlQuery query;
    query.exec("VACUUM");
}

void MainWindow::splashScreen()
{
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/client.png"));
    splash->show();
    qApp->processEvents();
}

void MainWindow::reportGrafikPromBez()
{
    GrafiKPromBezop reportGrafikPromBez(this);
    reportGrafikPromBez.exec();
}

void MainWindow::hideStatusBar()
{

}

void MainWindow::importXmlTable()
{
    ImportXmlTable import(this);
    import.importXMLTable();
}

void MainWindow::reportAllEmployeeSiz()
{
    AllEmployeeSiz openForm(this);
    openForm.exec();
}

void MainWindow::sortTable(int index)
{
    templateModel->setSort(index,Qt::AscendingOrder);
    templateModel->select();
    while(templateModel->canFetchMore())
        templateModel->fetchMore();
}

void MainWindow::reportStatRazmerov()
{
    RazmerStatistic openForm(this);
    openForm.printPreview();
}

void MainWindow::programmSettings()
{
    ProgrammSettings psOpen(this);
    psOpen.exec();
}

void MainWindow::reportSrokNoski()
{
    SrokNoskiForm openForm(this);
    openForm.exec();
}

void MainWindow::importExcelOstatki()
{
    ImportExcelForm openForm(this);
    openForm.exec();
}

void MainWindow::smtpProcedure()
{
    //Smtp smtpForm;

}

void MainWindow::removeFiles()
{
    QFile file;

    file.setFileName("null.txt");
    file.remove();
}

void MainWindow::sqlQuery()
{
    SQLForm openForm(this);
    openForm.exec();
}

void MainWindow::viewNashaFirma()
{
    NashaFirmaForm openForm(this);
    openForm.exec();
}

