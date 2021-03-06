#include <QtWidgets>
#include <QtSql>
#include "employee.h"
#include "numprefix.h"
#include "viewlisttable.h"
#include "fordelete.h"
#include "organization.h"
#include "subdivision.h"
#include "postform.h"
#include "overal.h"
#include "dateitemdelegate.h"
#include "vidachasizform.h"
#include "protocollaborprotectionform.h"
#include "industrialsecurityform.h"
#include "fireprotectionform.h"
#include "exchangefile.h"
#include "electrobezform.h"
//#include "udrabform.h"

EmployeeForm::EmployeeForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("EmployeeForm").toByteArray());

    newImage = false;
    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());

    QFile fileGroup(":/GroupBoxStyle.txt");
    fileGroup.open(QFile::ReadOnly);
    QString styleGroupString = QLatin1String(fileGroup.readAll());

    indexTemp = id;
    //**************************************************************
    labelFIO = new QLabel(trUtf8("ФИО:"));
    editFIO = new LineEdit;
    labelFIO->setBuddy(editFIO);

    QSqlQueryModel *empModel = new QSqlQueryModel;
    empModel->setQuery("SELECT employeename FROM employee");
    QCompleter *empCompleter = new QCompleter(empModel);
    empCompleter->setCompletionMode(QCompleter::PopupCompletion);
    empCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editFIO->setCompleter(empCompleter);

    QVBoxLayout *nameLayout = new QVBoxLayout;
    QVBoxLayout *editLayout = new QVBoxLayout;
    nameLayout->addWidget(labelFIO);
    editLayout->addWidget(editFIO);

    //**************************************************************

    labelOrganization = new QLabel(trUtf8("Организация:"));
    editOrganization = new LineEdit;
    editOrganization->setReadOnly(onlyForRead);
    labelOrganization->setBuddy(editOrganization);

    QSqlQueryModel *orgModel = new QSqlQueryModel;
    orgModel->setQuery("SELECT organizationname FROM organization");
    QCompleter *orgCompleter = new QCompleter(orgModel);
    orgCompleter->setCompletionMode(QCompleter::PopupCompletion);
    orgCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editOrganization->setCompleter(orgCompleter);

    QToolButton *addButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addButton->setIcon(addPix);
    addButton->setToolTip(trUtf8("Add new record"));
    connect(addButton,SIGNAL(clicked()),this,SLOT(addOrgRecord()));
    addButton->setStyleSheet(styleSheetString);

    QToolButton *seeButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeButton->setIcon(seePix);
    seeButton->setToolTip(trUtf8("See select item"));
    connect(seeButton,SIGNAL(clicked()),this,SLOT(seeOrgRecord()));
    seeButton->setStyleSheet(styleSheetString);

    QToolButton *listButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listButton->setIcon(listPix);
    listButton->setToolTip(trUtf8("See list of item"));
    connect(listButton,SIGNAL(clicked()),this,SLOT(listOrgRecord()));
    listButton->setStyleSheet(styleSheetString);

    QHBoxLayout *editOrganizationLayout = new QHBoxLayout;
    nameLayout->addWidget(labelOrganization);
    editOrganizationLayout->addWidget(editOrganization);
    if(!onlyForRead){
        editOrganizationLayout->addWidget(addButton);
        editOrganizationLayout->addWidget(seeButton);
        editOrganizationLayout->addWidget(listButton);
    }

    //**************************************************************
    labelSubdivision = new QLabel(trUtf8("Подразделение:"));
    editSubdivision = new LineEdit;
    editSubdivision->setReadOnly(onlyForRead);
    labelSubdivision->setBuddy(editSubdivision);

    QSqlQueryModel *subModel = new QSqlQueryModel;
    subModel->setQuery("SELECT subdivisionname FROM subdivision");
    QCompleter *subCompleter = new QCompleter(subModel);
    subCompleter->setCompletionMode(QCompleter::PopupCompletion);
    subCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editSubdivision->setCompleter(subCompleter);

    QToolButton *addSubButton = new QToolButton;
    addSubButton->setIcon(addPix);
    addSubButton->setToolTip(trUtf8("Добавить новую запись"));
    connect(addSubButton,SIGNAL(clicked()),this,SLOT(addSubRecord()));
    addSubButton->setStyleSheet(styleSheetString);

    QToolButton *seeSubButton = new QToolButton;
    seeSubButton->setIcon(seePix);
    seeSubButton->setToolTip(trUtf8("Просмотр выбранной записи"));
    connect(seeSubButton,SIGNAL(clicked()),this,SLOT(seeSubRecord()));
    seeSubButton->setStyleSheet(styleSheetString);

    QToolButton *listSubButton = new QToolButton;
    listSubButton->setIcon(listPix);
    listSubButton->setToolTip(trUtf8("Просмотр списка записей"));
    connect(listSubButton,SIGNAL(clicked()),this,SLOT(listSubRecord()));
    listSubButton->setStyleSheet(styleSheetString);

    QHBoxLayout *nameEditSubdivisionLayout = new QHBoxLayout;
    nameLayout->addWidget(labelSubdivision);
    nameEditSubdivisionLayout->addWidget(editSubdivision);
    if(!onlyForRead){
        nameEditSubdivisionLayout->addWidget(addSubButton);
        nameEditSubdivisionLayout->addWidget(seeSubButton);
        nameEditSubdivisionLayout->addWidget(listSubButton);
    }

    //**************************************************************
    labelPost = new QLabel(trUtf8("Должность:"));
    editPost = new LineEdit;
    editPost->setReadOnly(onlyForRead);
    labelPost->setBuddy(editPost);

    QSqlQueryModel *postModel = new QSqlQueryModel;
    postModel->setQuery("SELECT postname FROM post");
    QCompleter *postCompleter = new QCompleter(postModel);
    postCompleter->setCompletionMode(QCompleter::PopupCompletion);
    postCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editPost->setCompleter(postCompleter);
    connect(editPost,SIGNAL(textChanged(QString)),SLOT(sizQuery()));

    QToolButton *addPostButton = new QToolButton;
    addPostButton->setIcon(addPix);
    addPostButton->setToolTip(trUtf8("Добавить новую запись"));
    connect(addPostButton,SIGNAL(clicked()),this,SLOT(addPostRecord()));
    addPostButton->setStyleSheet(styleSheetString);

    QToolButton *seePostButton = new QToolButton;
    seePostButton->setIcon(seePix);
    seePostButton->setToolTip(trUtf8("Просмотр выбранной записи"));
    connect(seePostButton,SIGNAL(clicked()),this,SLOT(seePostRecord()));
    seePostButton->setStyleSheet(styleSheetString);

    QToolButton *listPostButton = new QToolButton;
    listPostButton->setIcon(listPix);
    listPostButton->setToolTip(trUtf8("Просмотр списка записей"));
    connect(listPostButton,SIGNAL(clicked()),this,SLOT(listPostRecord()));
    listPostButton->setStyleSheet(styleSheetString);

    QHBoxLayout *nameEditPostLayout = new QHBoxLayout;
    nameLayout->addWidget(labelPost);
    nameEditPostLayout->addWidget(editPost);
    if(!onlyForRead){
        nameEditPostLayout->addWidget(addPostButton);
        nameEditPostLayout->addWidget(seePostButton);
        nameEditPostLayout->addWidget(listPostButton);
    }

    //********************************************************************
    labelDateBirthday = new QLabel(trUtf8("Дата рождения:"));
    editDateBirthday = new QDateEdit;
    editDateBirthday->setCalendarPopup(true);
    //nameEditDateBirthday->setDate(QDate::currentDate());

    nameLayout->addWidget(labelDateBirthday);

    editLayout->addLayout(editOrganizationLayout);
    editLayout->addLayout(nameEditSubdivisionLayout);
    editLayout->addLayout(nameEditPostLayout);
    editLayout->addWidget(editDateBirthday);

    QHBoxLayout *personalLayout = new QHBoxLayout;
    personalLayout->addLayout(nameLayout);
    personalLayout->addLayout(editLayout);

    //**************************************************************
    editFoto = new PhotoLabel;
    editFoto->setFrameShape(QFrame::NoFrame);
    editFoto->setCursor(Qt::PointingHandCursor);
    editFoto->setMinimumSize(100,100);
    editFoto->setAlignment(Qt::AlignCenter);
    editFoto->setToolTip(trUtf8("Нет фото"));
    editFoto->setStyleSheet("QLabel:hover {background-color: "
                            "qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                            "stop:0 #cfcccc,"
                            "stop:0.5 #333232,"
                            "stop:0.51 #000000,"
                            "stop:1 #585c5f);"
                            "color: #00cc00;"
                            "font: bold;}");

    connect(editFoto,SIGNAL(clicked()),this,SLOT(photoRead()));

    QHBoxLayout *fotoLayout = new QHBoxLayout;
    fotoLayout->addWidget(editFoto);
    fotoLayout->addLayout(personalLayout);

    saveButton = new QPushButton(trUtf8("Записать"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    saveButton->setToolTip(trUtf8("Кнопка \"Записать и Закрыть \""));

    cancelButton = new QPushButton(trUtf8("Отмена"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelButton->setToolTip(trUtf8("Кнопка Отмена"));

    QPushButton *printUdButton = new QPushButton(trUtf8("Печать Удостоверения"));
    connect(printUdButton,SIGNAL(clicked(bool)),this,SLOT(printUd()));

    buttonBox = new QDialogButtonBox;
    QSqlQuery queryItr;
    queryItr.prepare("SELECT "
                     "(SELECT post.itr FROM post WHERE post.postid = employee.postid), "
                     "employee.employeename FROM employee WHERE employee.employeeid = :employeeid");
    queryItr.bindValue(":employeeid",indexTemp);
    queryItr.exec();
    queryItr.next();
    if(!queryItr.value(0).toBool()){
        buttonBox->addButton(printUdButton,QDialogButtonBox::ActionRole);
    }
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);


    //*****************************************************
    //Passport
    //*****************************************************
    labelPassportSeriya = new QLabel(trUtf8("Серия:"));
    editPassportSeriya = new LineEdit;
    editPassportSeriya->setStyleSheet("QLineEdit {max-width: 50}");

    nameLabelPassportNumber = new QLabel(trUtf8("Номер:"));
    nameEditPassportNumber = new LineEdit;

    QHBoxLayout *passportLayout = new QHBoxLayout;
    passportLayout->addWidget(labelPassportSeriya);
    passportLayout->addWidget(editPassportSeriya);
    passportLayout->addWidget(nameLabelPassportNumber);
    passportLayout->addWidget(nameEditPassportNumber);

    nameLabelPassportIssuedBy = new QLabel(trUtf8("Кем выдан:"));
    nameEditPassportIssuedBy = new LineEdit;

    QHBoxLayout *pas2Layout = new QHBoxLayout;
    pas2Layout->addWidget(nameLabelPassportIssuedBy);
    pas2Layout->addWidget(nameEditPassportIssuedBy);

    nameLabelPassportIssuedDate = new QLabel(trUtf8("Дата выдачи:"));
    nameEditPassportIssuedDate = new QDateEdit;
    nameEditPassportIssuedDate->setCalendarPopup(true);

    labelGender = new QLabel(trUtf8("Пол:"));
    editGender = new LineEdit;

    QSqlQueryModel *genderModel = new QSqlQueryModel;
    genderModel->setQuery("SELECT genderid FROM gender");
    QCompleter *genderCompleter = new QCompleter(genderModel);
    genderCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    genderCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editGender->setCompleter(genderCompleter);

    QHBoxLayout *issDateLayout = new QHBoxLayout;
    issDateLayout->addWidget(nameLabelPassportIssuedDate);
    issDateLayout->addWidget(nameEditPassportIssuedDate);
    issDateLayout->addWidget(labelGender);
    issDateLayout->addWidget(editGender);

    QVBoxLayout *pas1Layout = new QVBoxLayout;
    pas1Layout->addLayout(passportLayout);
    pas1Layout->addLayout(pas2Layout);
    pas1Layout->addLayout(issDateLayout);

    passportWidget = new QWidget;
    passportWidget->setLayout(pas1Layout);

    //*****************************************************
    //Kadry
    labelTabNumber = new QLabel(trUtf8("Табельный номер:"));
    editTabNumber = new LineEdit;

    labelKarta = new QLabel(trUtf8("Код карты:"));
    editKarta = new LineEdit;

    labelExperience = new QLabel(trUtf8("Стаж работы:"));
    editExperience = new LineEdit;

    labelWithOrganization = new QLabel(trUtf8("Дата приема:"));
    editWithOrganization = new QDateEdit;
    editWithOrganization->setCalendarPopup(true);
    editWithOrganization->setDate(QDate::currentDate());

    nameLabelGrafikRabot = new QLabel(trUtf8("График работы:"));
    nameEditGrafikRabot = new LineEdit;

    labelObosobl = new QLabel(trUtf8("Обособленка:"));
    editObosobl = new LineEdit;
    labelOformlen = new QLabel(trUtf8("Пропуск на оформлении:"));
    editOformlen = new QCheckBox;

    QVBoxLayout *nameKadryLayout = new QVBoxLayout;
    nameKadryLayout->addWidget(labelTabNumber);
    nameKadryLayout->addWidget(labelKarta);
    nameKadryLayout->addWidget(labelExperience);
    nameKadryLayout->addWidget(labelWithOrganization);
    nameKadryLayout->addWidget(nameLabelGrafikRabot);
    nameKadryLayout->addWidget(labelObosobl);
    nameKadryLayout->addWidget(labelOformlen);

    QVBoxLayout *editKadryLayout = new QVBoxLayout;
    editKadryLayout->addWidget(editTabNumber);
    editKadryLayout->addWidget(editKarta);
    editKadryLayout->addWidget(editExperience);
    editKadryLayout->addWidget(editWithOrganization);
    editKadryLayout->addWidget(nameEditGrafikRabot);
    editKadryLayout->addWidget(editObosobl);
    editKadryLayout->addWidget(editOformlen);

    QHBoxLayout *allKadryLayout = new QHBoxLayout;
    allKadryLayout->addLayout(nameKadryLayout);
    allKadryLayout->addLayout(editKadryLayout);

    QWidget *kadryWidget = new QWidget;
    kadryWidget->setLayout(allKadryLayout);

    //*****************************************************
    //Address
    //*****************************************************
    QTableView *addressWidget = new QTableView;
    QSqlRelationalTableModel *addressModel = new QSqlRelationalTableModel;
    addressModel->setTable("personalinformation");
    QString filterP = "employeeid = '";
    filterP += indexTemp;
    filterP += "'";
    addressModel->setFilter(filterP);
    addressModel->setHeaderData(2,Qt::Horizontal,trUtf8("Information"));
    addressModel->select();
    addressWidget->setModel(addressModel);
    addressWidget->setColumnHidden(0,true);
    addressWidget->setColumnHidden(1,true);
    addressWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    addressWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    addressWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    addressWidget->setAlternatingRowColors(true);
    addressWidget->resizeColumnsToContents();
    //addressWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headAdress = addressWidget->horizontalHeader();
    headAdress->setStretchLastSection(true);

    //*****************************************************
    //Family
    //*****************************************************
    QTableView *familyWidget = new QTableView;
    QSqlRelationalTableModel *familyModel = new QSqlRelationalTableModel;
    familyModel->setTable("rodnie");
    familyModel->setFilter(filterP);
    familyModel->setHeaderData(2,Qt::Horizontal,trUtf8("Степень родства"));
    familyModel->setHeaderData(3,Qt::Horizontal,trUtf8("Имя"));
    familyModel->setHeaderData(4,Qt::Horizontal,trUtf8("День рождения"));
    familyModel->select();
    familyWidget->setModel(familyModel);
    familyWidget->setColumnHidden(0,true);
    familyWidget->setColumnHidden(1,true);
    familyWidget->setItemDelegateForColumn(4,new DateItemDelegate);
    familyWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    familyWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    familyWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    familyWidget->setAlternatingRowColors(true);
    familyWidget->resizeColumnsToContents();
    //familyWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headFamily = familyWidget->horizontalHeader();
    headFamily->setStretchLastSection(true);

    //*****************************************************
    //Education
    //*****************************************************
    QTableView *educationWidget = new QTableView;
    QSqlRelationalTableModel *educationModel = new QSqlRelationalTableModel;
    educationModel->setTable("education");
    educationModel->setFilter(filterP);
    educationModel->setHeaderData(2,Qt::Horizontal,trUtf8("Образование"));
    educationModel->setHeaderData(3,Qt::Horizontal,trUtf8("Учебное заведение"));
    educationModel->setHeaderData(4,Qt::Horizontal,trUtf8("Специальность"));
    educationModel->setHeaderData(5,Qt::Horizontal,trUtf8("Диплом"));
    educationModel->setHeaderData(6,Qt::Horizontal,trUtf8("Год окончания"));
    educationModel->setHeaderData(7,Qt::Horizontal,trUtf8("Квалификация"));
    educationModel->select();
    educationWidget->setModel(educationModel);
    educationWidget->setColumnHidden(0,true);
    educationWidget->setColumnHidden(1,true);
    educationWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    educationWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    educationWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    educationWidget->setAlternatingRowColors(true);
    educationWidget->resizeColumnsToContents();
    //educationWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headEducation = educationWidget->horizontalHeader();
    headEducation->setStretchLastSection(true);

    //***********************************************************
    //Driver License
    //***********************************************************
    nameLabelDriverLicenseSeriya = new QLabel(trUtf8("Seriya:"));
    nameEditDriverLicenseSeriya = new LineEdit;
    nameLabelDriverLicenseNumber = new QLabel(trUtf8("Number"));
    nameEditDriverLicenseNumber = new LineEdit;

    QHBoxLayout *lsnLayout = new QHBoxLayout;
    lsnLayout->addWidget(nameLabelDriverLicenseSeriya);
    lsnLayout->addWidget(nameEditDriverLicenseSeriya);
    lsnLayout->addWidget(nameLabelDriverLicenseNumber);
    lsnLayout->addWidget(nameEditDriverLicenseNumber);

    nameLabelDriverLicenseDate = new QLabel(trUtf8("Date Issued:"));
    nameEditDriverLicenseDate = new QDateEdit;
    nameEditDriverLicenseDate->setCalendarPopup(true);
    nameEditDriverLicenseDate->setDate(QDate::currentDate());

    QHBoxLayout *lsdLayout = new QHBoxLayout;
    lsdLayout->addWidget(nameLabelDriverLicenseDate);
    lsdLayout->addWidget(nameEditDriverLicenseDate);
    lsdLayout->addStretch();

    nameLabelDriverLicenseIssuedBy = new QLabel(trUtf8("Who's Issued:"));
    nameEditDriverLicenseIssuedBy = new LineEdit;

    QHBoxLayout *issLayout = new QHBoxLayout;
    issLayout->addWidget(nameLabelDriverLicenseIssuedBy);
    issLayout->addWidget(nameEditDriverLicenseIssuedBy);

    nameLabelDriverA = new QLabel("A");
    nameEditDriverA = new QCheckBox;
    nameLabelDriverB = new QLabel("B");
    nameEditDriverB = new QCheckBox;
    nameLabelDriverC = new QLabel("C");
    nameEditDriverC = new QCheckBox;
    nameLabelDriverD = new QLabel("D");
    nameEditDriverD = new QCheckBox;
    nameLabelDriverE = new QLabel("A");
    nameEditDriverE = new QCheckBox;
    QHBoxLayout *catLayout = new QHBoxLayout;
    catLayout->addWidget(nameLabelDriverA);
    catLayout->addWidget(nameEditDriverA);
    catLayout->addWidget(nameLabelDriverB);
    catLayout->addWidget(nameEditDriverB);
    catLayout->addWidget(nameLabelDriverC);
    catLayout->addWidget(nameEditDriverC);
    catLayout->addWidget(nameLabelDriverD);
    catLayout->addWidget(nameEditDriverD);
    catLayout->addWidget(nameLabelDriverE);
    catLayout->addWidget(nameEditDriverE);

    QGroupBox *permittedGroup = new QGroupBox(trUtf8("Permitted category"));
    permittedGroup->setStyleSheet(styleGroupString);
    permittedGroup->setLayout(catLayout);

    QVBoxLayout *mainDriverLayout = new QVBoxLayout;
    mainDriverLayout->addLayout(lsnLayout);
    mainDriverLayout->addLayout(lsdLayout);
    mainDriverLayout->addLayout(issLayout);
    mainDriverLayout->addWidget(permittedGroup);

    QWidget *driverWidget = new QWidget;
    driverWidget->setLayout(mainDriverLayout);
    //*****************************************************
    //Labor Protection
    //*****************************************************
    laborProtectionWidget = new QTableView;
    laborProtectionModel = new QSqlRelationalTableModel;
    laborProtectionModel->setTable("laborprotection");
    laborProtectionModel->setFilter(filterP);
    laborProtectionModel->setHeaderData(2,Qt::Horizontal,trUtf8("Дата"));
    laborProtectionModel->setHeaderData(3,Qt::Horizontal,trUtf8("Программа обучения"));
    laborProtectionModel->setRelation(3,QSqlRelation("otprogramma","otprogrammaid","otprogrammaname"));
    laborProtectionModel->setHeaderData(4,Qt::Horizontal,trUtf8("Номер"));
    laborProtectionModel->setHeaderData(5,Qt::Horizontal,trUtf8("Номер\nудостоверения"));
    laborProtectionModel->setHeaderData(6,Qt::Horizontal,trUtf8("Дата\nудостоверения"));
    laborProtectionModel->setHeaderData(8,Qt::Horizontal,trUtf8("Причина\nпроверки"));
    laborProtectionModel->setRelation(8,QSqlRelation("prichinaobuch","prichinaobuchid","prichinaobuchname"));
    laborProtectionModel->select();
    laborProtectionWidget->setModel(laborProtectionModel);
    laborProtectionWidget->setColumnHidden(0,true);
    laborProtectionWidget->setColumnHidden(1,true);
    laborProtectionWidget->setColumnHidden(7,true);
    laborProtectionWidget->setItemDelegateForColumn(2, new DateItemDelegate);
    laborProtectionWidget->setItemDelegateForColumn(6, new DateItemDelegate);
    laborProtectionWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    laborProtectionWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    laborProtectionWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    laborProtectionWidget->setAlternatingRowColors(true);
    laborProtectionWidget->resizeColumnsToContents();
    //laborProtectionWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headLabProt = laborProtectionWidget->horizontalHeader();
    headLabProt->setStretchLastSection(true);
    contextLaborProtection();
    connect(laborProtectionWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(laborProtection()));
    //*****************************************************
    //Inductrial Security
    //*****************************************************
    industrialSecurityWidget = new QTableView;
    industrialSecurityModel = new QSqlRelationalTableModel;
    industrialSecurityModel->setTable("industrialsecurity");
    industrialSecurityModel->setFilter(filterP);
    industrialSecurityModel->setHeaderData(2,Qt::Horizontal,trUtf8("Дата"));
    industrialSecurityModel->setHeaderData(3,Qt::Horizontal,trUtf8("Программа обучения"));
    industrialSecurityModel->setRelation(3,QSqlRelation("pbprogramma","pbprogrammaid","pbprogrammaname"));
    industrialSecurityModel->setHeaderData(4,Qt::Horizontal,trUtf8("Номер"));
    industrialSecurityModel->setHeaderData(5,Qt::Horizontal,trUtf8("Номер\nудостоверения"));
    industrialSecurityModel->setHeaderData(6,Qt::Horizontal,trUtf8("Дата\nудостоверения"));
    industrialSecurityModel->setHeaderData(8,Qt::Horizontal,trUtf8("Причина\nпроверки"));
    industrialSecurityModel->setRelation(8,QSqlRelation("prichinaobuch","prichinaobuchid","prichinaobuchname"));
    industrialSecurityModel->select();
    industrialSecurityWidget->setModel(industrialSecurityModel);
    industrialSecurityWidget->setColumnHidden(0,true);
    industrialSecurityWidget->setColumnHidden(1,true);
    industrialSecurityWidget->setColumnHidden(7,true);
    industrialSecurityWidget->setItemDelegateForColumn(2, new DateItemDelegate);
    industrialSecurityWidget->setItemDelegateForColumn(6, new DateItemDelegate);
    industrialSecurityWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    industrialSecurityWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    industrialSecurityWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    industrialSecurityWidget->setAlternatingRowColors(true);
    industrialSecurityWidget->resizeColumnsToContents();
    //industrialSecurityWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headIndustrialSecurity = industrialSecurityWidget->horizontalHeader();
    headIndustrialSecurity->setStretchLastSection(true);
    contextIndustrialSecurity();
    connect(industrialSecurityWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(industrialSecurity()));
    //*****************************************************
    //Fire Protection
    //*****************************************************
    fireProtectionWidget = new QTableView;
    fireProtectionModel = new QSqlRelationalTableModel;
    fireProtectionModel->setTable("ptm");
    fireProtectionModel->setFilter(filterP);
    fireProtectionModel->setHeaderData(2,Qt::Horizontal,trUtf8("Дата"));
    fireProtectionModel->setHeaderData(3,Qt::Horizontal,trUtf8("Номер"));
    fireProtectionModel->setHeaderData(4,Qt::Horizontal,trUtf8("Номер\nудостоверения"));
    fireProtectionModel->setHeaderData(5,Qt::Horizontal,trUtf8("Дата\nудостоверения"));
    fireProtectionModel->setHeaderData(7,Qt::Horizontal,trUtf8("Причина\nпроверки"));
    fireProtectionModel->setRelation(7,QSqlRelation("prichinaobuch","prichinaobuchid","prichinaobuchname"));
    fireProtectionModel->select();
    fireProtectionWidget->setModel(fireProtectionModel);
    fireProtectionWidget->setColumnHidden(0,true);
    fireProtectionWidget->setColumnHidden(1,true);
    fireProtectionWidget->setItemDelegateForColumn(2,new DateItemDelegate);
    fireProtectionWidget->setItemDelegateForColumn(5,new DateItemDelegate);
    fireProtectionWidget->setColumnHidden(6,true);
    fireProtectionWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    fireProtectionWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    fireProtectionWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fireProtectionWidget->setAlternatingRowColors(true);
    fireProtectionWidget->resizeColumnsToContents();
    //fireProtectionWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headFireProtection = fireProtectionWidget->horizontalHeader();
    headFireProtection->setStretchLastSection(true);
    contextPTM();
    connect(fireProtectionWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(ptm()));
    //*****************************************************
    //Electro Protection
    //*****************************************************
    electroProtWidget = new QTableView;
    electroProtModel = new QSqlRelationalTableModel;
    electroProtModel->setTable("electroprot");
    electroProtModel->setFilter(filterP);
    electroProtModel->setHeaderData(2,Qt::Horizontal,trUtf8("Дата"));
    electroProtModel->setHeaderData(3,Qt::Horizontal,trUtf8("Номер"));
    electroProtModel->setHeaderData(4,Qt::Horizontal,trUtf8("Номер\nудостоверения"));
    electroProtModel->setHeaderData(5,Qt::Horizontal,trUtf8("Дата\nудостоверения"));
    electroProtModel->setHeaderData(7,Qt::Horizontal,trUtf8("Причина\nпроверки"));
    electroProtModel->setRelation(7,QSqlRelation("prichinaobuch","prichinaobuchid","prichinaobuchname"));
    electroProtModel->select();
    electroProtWidget->setModel(electroProtModel);
    electroProtWidget->setColumnHidden(0,true);
    electroProtWidget->setColumnHidden(1,true);
    electroProtWidget->setItemDelegateForColumn(2,new DateItemDelegate);
    electroProtWidget->setItemDelegateForColumn(5,new DateItemDelegate);
    electroProtWidget->setColumnHidden(6,true);
    electroProtWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    electroProtWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    electroProtWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    electroProtWidget->setAlternatingRowColors(true);
    electroProtWidget->resizeColumnsToContents();
    //fireProtectionWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headElectroProtection = electroProtWidget->horizontalHeader();
    headElectroProtection->setStretchLastSection(true);
    contextElectroProm();
    connect(electroProtWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(elektroprom()));
    //*****************************************************
    //Physical
    //*****************************************************
    QTableView *physicalWidget = new QTableView;
    QSqlRelationalTableModel *physicalModel = new QSqlRelationalTableModel;
    physicalModel->setTable("physical");
    physicalModel->setFilter(filterP);
    physicalModel->setHeaderData(2,Qt::Horizontal,trUtf8("Number"));
    physicalModel->setHeaderData(3,Qt::Horizontal,trUtf8("Date"));
    physicalModel->setHeaderData(4,Qt::Horizontal,trUtf8("Passed"));
    physicalModel->setHeaderData(5,Qt::Horizontal,trUtf8("Fit"));
    physicalModel->setHeaderData(6,Qt::Horizontal,trUtf8("Date of passge"));
    physicalModel->setHeaderData(7,Qt::Horizontal,trUtf8("Nubber spravki"));
    physicalModel->select();
    physicalWidget->setModel(physicalModel);
    physicalWidget->setColumnHidden(0,true);
    physicalWidget->setItemDelegateForColumn(3,new DateItemDelegate);
    physicalWidget->setColumnHidden(1,true);
    physicalWidget->setItemDelegateForColumn(6,new DateItemDelegate);
    physicalWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    physicalWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    physicalWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    physicalWidget->setAlternatingRowColors(true);
    physicalWidget->resizeColumnsToContents();
    //physicalWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headPhysical = physicalWidget->horizontalHeader();
    headPhysical->setStretchLastSection(true);
    //*****************************************************
    //Overal
    //*****************************************************
    overalWidget = new QTableView;
    overalModel = new QSqlRelationalTableModel;
    overalModel->setTable("overal");
    overalModel->setFilter(filterP);
    overalModel->setHeaderData(2,Qt::Horizontal,trUtf8("Рост"));
    overalModel->setHeaderData(3,Qt::Horizontal,trUtf8("Головной убор"));
    overalModel->setHeaderData(4,Qt::Horizontal,trUtf8("Зимняя одежда"));
    overalModel->setHeaderData(5,Qt::Horizontal,trUtf8("Летняя одежда"));
    overalModel->setHeaderData(6,Qt::Horizontal,trUtf8("Зимняя обувь"));
    overalModel->setHeaderData(7,Qt::Horizontal,trUtf8("Летняя обувь"));
    overalModel->select();

    overalWidget->setModel(overalModel);
    overalWidget->setColumnHidden(0,true);
    overalWidget->setColumnHidden(1,true);
//    for(int i = 2;i < 11; ++i){
//        if(overalWidget->model()->data(overalWidget->model()->index(0,i)).toInt() == 0)
//            overalWidget->setColumnHidden(i,true);
//    }
    overalWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    overalWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    overalWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    overalWidget->setAlternatingRowColors(true);
    overalWidget->resizeColumnsToContents();
    //overalWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headOveral = overalWidget->horizontalHeader();
    headOveral->setStretchLastSection(true);
    connect(overalWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(overalRecord()));
    createContextMenu();

    //*****************************************************
    //Средства Индивидуальной Защиты
    //*****************************************************
    QLabel *labelNormaSiz = new QLabel(trUtf8("Норма СИЗ:"));
    editNormaSiz = new LineEdit;
    QToolButton *addPostSizButton = new QToolButton;
    addPostSizButton->setIcon(listPix);
    addPostSizButton->setToolTip(trUtf8("Просмотр списка справочника"));
    connect(addPostSizButton,SIGNAL(clicked()),this,SLOT(ListPostSizRecord()));
    addPostSizButton->setStyleSheet(styleSheetString);

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
    tableSizWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableSizWidget->setAlternatingRowColors(true);
    tableSizWidget->setColumnWidth(0,300);
    tableSizWidget->setColumnWidth(1,300);

    QHBoxLayout *stringLayout = new QHBoxLayout;
    stringLayout->addWidget(labelNormaSiz);
    stringLayout->addWidget(editNormaSiz);
    stringLayout->addWidget(addPostSizButton);

    QVBoxLayout *sizLayout = new QVBoxLayout;
    sizLayout->addLayout(stringLayout);
    sizLayout->addWidget(tableSizWidget);
    QWidget *sizWidget = new QWidget;
    sizWidget->setLayout(sizLayout);
    createContextSIZ();

    //*****************************************************
    //Udostovereniya Visota
    //*****************************************************
    QTableView *visWidget = new QTableView;
    QSqlRelationalTableModel *visModel = new QSqlRelationalTableModel;
    visModel->setTable("visota");
    visModel->setFilter(filterP);
    visModel->setHeaderData(1,Qt::Horizontal,trUtf8("Protokol Number"));
    visModel->setHeaderData(2,Qt::Horizontal,trUtf8("Protokol Date"));
    //    sizModel->setHeaderData(4,Qt::Horizontal,trUtf8("Data Polucheniya"));
    //    sizModel->setHeaderData(5,Qt::Horizontal,trUtf8("Srok Ispolzovaniya"));
    visModel->select();
    visWidget->setModel(visModel);
    visWidget->setColumnHidden(0,true);
    //    sizWidget->setColumnHidden(1,true);
    visWidget->setItemDelegateForColumn(4,new DateItemDelegate);
    visWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    visWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    visWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    visWidget->setAlternatingRowColors(true);
    visWidget->resizeColumnsToContents();
    //industrialSecurityWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *headVis = visWidget->horizontalHeader();
    headVis->setStretchLastSection(true);

    //*****************************************************

    newRecord = false;
    bool loadPhoto = false;
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT emp.employeename, "
                      "(SELECT org.organizationname FROM organization AS org WHERE org.organizationid = emp.organizationid), "
                      "(SELECT sub.subdivisionname FROM subdivision AS sub WHERE sub.subdivisionid = emp.subdivisionid), "
                      "(SELECT pos.postname FROM post AS pos WHERE pos.postid = emp.postid), "
                      "emp.datebirthday, "
                      "emp.tabnumber, "
                      "emp.kodkarty, "
                      "emp.withorganization, "
                      "emp.passportseriya, "
                      "emp.passportnumber,  "
                      "emp.passportissuedby, "
                      "emp.passportissueddate, "
                      "(SELECT grafik.grafikrabotname FROM grafikrabot AS grafik WHERE grafik.grafikrabotid = emp.grafikrabot), "
                      "emp.genderid, "
                      "oformlenie "
                      "FROM employee AS emp WHERE emp.employeeid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR!"),query.lastError().text());

        }
        editFIO->setText(query.value(0).toString());
        editOrganization->setText(query.value(1).toString());
        editSubdivision->setText(query.value(2).toString());
        editPost->setText(query.value(3).toString());
        editDateBirthday->setDate(query.value(4).toDate());
        editTabNumber->setText(query.value(5).toString());
        editKarta->setText(query.value(6).toString());
        editWithOrganization->setDate(query.value(7).toDate());
        editPassportSeriya->setText(query.value(8).toString());
        nameEditPassportNumber->setText(query.value(9).toString());
        nameEditPassportIssuedBy->setText(query.value(10).toString());
        nameEditPassportIssuedDate->setDate(query.value(11).toDate());
        nameEditGrafikRabot->setText(query.value(12).toString());
        editGender->setText(query.value(13).toString());
        if(query.value(14).toBool()){
            editOformlen->setChecked(true);
        }else{
            editOformlen->setChecked(false);
        }

        QString experience;
        QDate datePostupleniya = query.value(7).toDate();
        if(!datePostupleniya.isNull()){
            int years;
            int mounth;
            int days;
            int prefd = 0;
            if(QDate::currentDate().day() < datePostupleniya.day()){
                days = QDate::currentDate().addMonths(-1).daysInMonth() - datePostupleniya.day() + QDate::currentDate().day();
                prefd = 1;
            }else{
                days = QDate::currentDate().day() - datePostupleniya.day();
            }
            if(QDate::currentDate().month() < datePostupleniya.month()){
                mounth = 12 - datePostupleniya.month() + QDate::currentDate().month() - prefd;
            }else{
                mounth = QDate::currentDate().month() - datePostupleniya.month();
            }

            QDate d1 = datePostupleniya;
            QDate d2 = QDate::currentDate();
            if(QDate::currentDate().year() == datePostupleniya.year()){
                years = 0;
            }else if(d1.daysTo(d2) < 365){
                years = 0;
            }else{
                if(mounth == 0){
                    years = QDate::currentDate().year() - datePostupleniya.year();
                }else{
                    years = QDate::currentDate().year() - datePostupleniya.year() - 1;
                }
            }

            experience = QString::number(years);
            if(years == 1){
                experience += trUtf8(" год, ");
            }else if(years == 2 || years == 3 || years == 4){
                experience += trUtf8(" года, ");
            }else{
                experience += trUtf8(" лет, ");
            }
            experience += QString::number(mounth);
            if(mounth == 1){
                experience += trUtf8(" месяц, ");
            }else if(mounth == 2 || mounth == 3 || mounth == 4){
                experience += trUtf8(" месяца, ");
            }else{
                experience += trUtf8(" месяцев, ");
            }
            experience += QString::number(days);
            if(days == 1){
                experience += trUtf8(" день.");
            }else if(mounth == 2 || mounth == 3 || mounth == 4){
                experience += trUtf8(" дня.");
            }else{
                experience += trUtf8(" дней.");
            }
        }else{
            experience = "";
        }
        editExperience->setText(experience);

        QSqlQuery queryDriver;
        queryDriver.prepare("SELECT * FROM driver WHERE employeeid =  :id");
        queryDriver.bindValue(":id",indexTemp);
        queryDriver.exec();
        while(queryDriver.next()){
            nameEditDriverLicenseSeriya->setText(queryDriver.value(2).toString());
            nameEditDriverLicenseNumber->setText(queryDriver.value(3).toString());
            nameEditDriverLicenseDate->setDate(queryDriver.value(4).toDate());
            nameEditDriverLicenseIssuedBy->setText(queryDriver.value(5).toString());
            if(queryDriver.value(6).toBool())
                nameEditDriverA->setChecked(true);
            if(queryDriver.value(7).toBool())
                nameEditDriverB->setChecked(true);
            if(queryDriver.value(8).toBool())
                nameEditDriverC->setChecked(true);
            if(queryDriver.value(9).toBool())
                nameEditDriverD->setChecked(true);
            if(queryDriver.value(10).toBool())
                nameEditDriverE->setChecked(true);
        }
        QSqlQuery queryPhoto;
        queryPhoto.prepare("SELECT photoname FROM photo WHERE photoid = :id");
        queryPhoto.bindValue(":id",indexTemp);
        queryPhoto.exec();
        while(queryPhoto.next()){
            QByteArray imageByte = queryPhoto.value(0).toByteArray();
            pixMap.loadFromData(imageByte);
            QImage re = pixMap.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            editFoto->setMaximumSize(100,100);
            editFoto->setPixmap(QPixmap::fromImage(re));
            loadPhoto = true;
            QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
            settings.setValue("indexPhoto", indexTemp);

        }

        sizQuery();
    }else{
        editFIO->clear();
        editFIO->setText(QObject::trUtf8("Фамилия Имя Отчество"));
        editFIO->selectAll();

        newRecord = true;
        NumPrefix numPref;
        indexTemp = numPref.getPrefix("employee");
        editOformlen->setChecked(true);
    }

    tabWidget = new QTabWidget;
    tabWidget->addTab(sizWidget,trUtf8("СИЗ"));
    tabWidget->addTab(laborProtectionWidget,trUtf8("Охрана труда"));
    QSqlQuery queryPost;
    queryPost.prepare("SELECT (SELECT post.itr FROM post WHERE post.postid = employee.postid) FROM "
                      "employee WHERE employeeid = :employeeid");
    queryPost.bindValue(":employeeid",indexTemp);
    queryPost.exec();
    queryPost.next();
    if(queryPost.value(0).toBool()){
        tabWidget->addTab(industrialSecurityWidget,trUtf8("Промбезопасность"));
    }
    tabWidget->addTab(fireProtectionWidget,trUtf8("ПТМ"));
    tabWidget->addTab(electroProtWidget,trUtf8("Электро"));
    tabWidget->addTab(visWidget,trUtf8("Высота"));
    tabWidget->addTab(physicalWidget,trUtf8("Медицина"));
    tabWidget->addTab(overalWidget,trUtf8("Размеры"));
    tabWidget->addTab(kadryWidget,trUtf8("Кадры"));
    tabWidget->addTab(passportWidget,trUtf8("Паспорт"));
    tabWidget->addTab(familyWidget,trUtf8("Семья"));
    tabWidget->addTab(educationWidget,trUtf8("Образование"));
    tabWidget->addTab(addressWidget,trUtf8("Личная"));
    tabWidget->addTab(driverWidget,trUtf8("Водитель"));


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(fotoLayout);
    mainLayout->addWidget(tabWidget);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox);
    }
    if(!loadPhoto){
        editFoto->setText(trUtf8("Нет Фото"));
    }
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Сотрудник"));

    //editFIO->setFocus();
    setTabOrder(editFIO,editOrganization);
    setTabOrder(editOrganization,editSubdivision);
    setTabOrder(editSubdivision,editPost);
    setTabOrder(editPost,editDateBirthday);
    setTabOrder(editDateBirthday,editGender);
    setTabOrder(editGender,saveButton);
    //setTabOrder(saveButton,editFIO);

}

EmployeeForm::~EmployeeForm()
{

}

void EmployeeForm::photoRead()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString userName = settings.value("CurrentUser").toString();
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE userid = :id");
    query.bindValue(":id",userName);
    query.exec();
    query.next();
    if(query.value(0).toInt() == 1){
        QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
        QString userName = settings.value("CurrentUser").toString();
        QSqlQuery query;
        query.prepare("SELECT role FROM users WHERE userid = :id");
        query.bindValue(":id",userName);
        query.exec();
        query.next();
        if(query.value(0).toBool()){
            QString fileName = QFileDialog::getOpenFileName(this,trUtf8("Open file"),"*.jpg","Foto (*.jpg)");
            QFile file(fileName);
            if(file.open(QIODevice::ReadOnly)){
                newImage = true;
                if(file.size() > 300000){
                    QMessageBox::warning(this,trUtf8("Attention"),trUtf8("Size of file must be < 300 kByte"));
                    return;
                }
                pixMap.load(fileName);
                QImage re = pixMap.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                editFoto->setMaximumSize(100,100);
                editFoto->setPixmap(QPixmap::fromImage(re));
                QSqlQuery queryEx;
                queryEx.prepare("SELECT photoid FROM photo WHERE photoid = :id");
                queryEx.bindValue(":id",indexTemp);
                queryEx.exec();
                queryEx.next();
                if(queryEx.value(0).toString() == ""){
                    QSqlQuery query;
                    query.prepare("INSERT INTO photo ("
                                  "photoid, "
                                  "photoname) VALUES("
                                  ":photid, :photoname)"
                                  );
                    query.bindValue(":photid",indexTemp);
                    QByteArray imageByte = file.readAll();
                    query.bindValue(":photoname",imageByte);
                    query.exec();
                    if(!query.isActive()){
                        QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR!"),query.lastError().text());
                    }
                }else{
                    QSqlQuery query;
                    query.prepare("UPDATE photo SET photoname = :bod "
                                  "WHERE photoid = :empid");
                    query.bindValue(":empid",indexTemp);
                    QByteArray imageByte = file.readAll();
                    query.bindValue(":bod",imageByte);
                    query.exec();
                    if(!query.isActive()){
                        QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR!"),query.lastError().text(),QMessageBox::Yes);
                    }
                }
            }
        }
    }else{
        QMessageBox::warning(this,"Attention!!!","You don't add record, Seed Role!");
    }
}

void EmployeeForm::editRecord()
{
    if(checkFill()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }

        if(newRecord){
            QSqlQuery queryPer;
            queryPer.prepare("SELECT employeeid FROM employee "
                             "WHERE (employeename = :employeename AND "
                             "datebirthday = :datebirthday);");
            queryPer.bindValue(":employeename",editFIO->text());
            queryPer.bindValue(":datebirthday",editDateBirthday->date());
            queryPer.exec();
            queryPer.next();
            qDebug()<<queryPer.isValid();
            if(!queryPer.isValid()){
                QSqlQuery query;
                query.prepare("INSERT INTO employee ("
                              "employeeid, "
                              "employeename, "
                              "employeenameupper, "
                              "genderid, "
                              "organizationid, "
                              "subdivisionid, "
                              "postid, "
                              "datebirthday, "
                              "tabnumber, "
                              "kodkarty, "
                              "withorganization, "
                              "passportseriya, "
                              "passportnumber,  "
                              "passportissuedby, "
                              "passportissueddate, "
                              "grafikrabot, "
                              "obosoblid, "
                              "oformlenie"
                              ") VALUES("
                              ":employeeid, "
                              ":employeename, "
                              ":employeenameupper, "
                              ":genderid, "
                              ":orgid, "
                              ":subid, "
                              ":postid, "
                              ":datebirthday, "
                              ":tabnumber, "
                              ":kodkarty, "
                              ":withorganization, "
                              ":passportseriya, "
                              ":passportnumber,  "
                              ":passportissuedby, "
                              ":passportissueddate, "
                              "(SELECT grafikrabotid FROM grafikrabot WHERE grafikrabotname = :grafikrabot), "
                              "(SELECT obosoblid FROM obosobl WHERE obosoblname = :obosoblid), "
                              ":oformlenie);"
                              );
                query.bindValue(":employeeid",indexTemp);
                query.bindValue(":employeename",editFIO->text());
                query.bindValue(":employeenameupper",editFIO->text().toUpper());
                query.bindValue(":genderid",editGender->text());
                QSqlQuery queryOrg;
                queryOrg.prepare("SELECT organizationid FROM organization WHERE organizationname = :orgname");
                queryOrg.bindValue(":orgname",editOrganization->text());
                queryOrg.exec();
                queryOrg.next();
                query.bindValue(":orgid",queryOrg.value(0).toString());
                queryOrg.clear();
                queryOrg.prepare("SELECT subdivisionid FROM subdivision WHERE subdivisionname = :subname");
                queryOrg.bindValue(":subname",editSubdivision->text());
                queryOrg.exec();
                queryOrg.next();
                query.bindValue(":subid",queryOrg.value(0).toString());
                queryOrg.clear();
                queryOrg.prepare("SELECT postid FROM post WHERE postname = :postname");
                queryOrg.bindValue(":postname",editPost->text());
                queryOrg.exec();
                queryOrg.next();
                query.bindValue(":postid",queryOrg.value(0).toString());
                query.bindValue(":datebirthday",editDateBirthday->date());
                query.bindValue(":tabnumber",editTabNumber->text());
                query.bindValue(":kodkarty",editKarta->text());
                query.bindValue(":withorganization",editWithOrganization->date());
                query.bindValue(":passportseriya",editPassportSeriya->text());
                query.bindValue(":passportnumber",nameEditPassportNumber->text());
                query.bindValue(":passportissuedby",nameEditPassportIssuedBy->text());
                query.bindValue(":passportissueddate",nameEditPassportIssuedDate->date());
                query.bindValue(":grafikrabot",nameEditGrafikRabot->text());
                query.bindValue(":obosoblid",editObosobl->text());
                if(editOformlen->isChecked()){
                    query.bindValue(":oformlenie",1);
                }else{
                    query.bindValue(":oformlenie",0);
                }
                query.exec();
                if(!query.isActive()){
                    QMessageBox::warning(this,QObject::trUtf8("Ошибка Вставки в Базу данных!"),query.lastError().text());
                }
                line += "INSERT INTO employee ("
                        "employeeid, "
                        "employeename, "
                        "employeenameupper, "
                        "genderid, "
                        "organizationid, "
                        "subdivisionid, "
                        "postid, "
                        "datebirthday, "
                        "tabnumber, "
                        "kodkarty, "
                        "withorganization, "
                        "passportseriya, "
                        "passportnumber,  "
                        "passportissuedby, "
                        "passportissueddate, "
                        "grafikrabot, "
                        "obosoblid, "
                        "oformlenie"
                        ") VALUES('";
                line += indexTemp;
                line += "', '";
                line += editFIO->text().toUtf8();
                line += "', '";
                line += editFIO->text().toUpper().toUtf8();
                line += "', '";
                line += editGender->text().toUtf8();
                line += "', '";
                QSqlQuery queryTemp;
                queryTemp.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
                queryTemp.bindValue(":name",editOrganization->text());
                queryTemp.exec();
                queryTemp.next();
                line += queryTemp.value(0).toString().toUtf8();
                line += "', '";
                queryTemp.clear();
                queryTemp.prepare("SELECT subdivisionid FROM subdivision WHERE subdivisionname = :name");
                queryTemp.bindValue(":name",editSubdivision->text());
                queryTemp.exec();
                queryTemp.next();
                line += queryTemp.value(0).toString().toUtf8();
                line += "', '";
                queryTemp.clear();
                queryTemp.prepare("SELECT postid FROM post WHERE postname = :name");
                queryTemp.bindValue(":name",editPost->text());
                queryTemp.exec();
                queryTemp.next();
                line += queryTemp.value(0).toString().toUtf8();
                line += "', '";
                line += editDateBirthday->date().toString("yyyy-MM-dd");
                line += "', '";
                line += editTabNumber->text().toUtf8();
                line += "', '";
                line += editKarta->text().toUtf8();
                line += "', '";
                line += editWithOrganization->date().toString("yyyy-MM-dd");
                line += "', '";
                line += editPassportSeriya->text().toUtf8();
                line += "', '";
                line += nameEditPassportNumber->text().toUtf8();
                line += "', '";
                line += nameEditPassportIssuedBy->text().toUtf8();
                line += "', '";
                line += nameEditPassportIssuedDate->date().toString("yyyy-MM-dd");
                line += "', '";
                queryTemp.clear();
                queryTemp.prepare("SELECT grafikrabotid FROM grafikrabot WHERE grafikrabotame = :name");
                queryTemp.bindValue(":name",nameEditGrafikRabot->text().toUtf8());
                queryTemp.exec();
                queryTemp.next();
                line += queryTemp.value(0).toString().toUtf8();
                line += "', '";
                queryTemp.clear();
                queryTemp.prepare("SELECT obosoblid FROM obosobl WHERE obosoblname = :name");
                queryTemp.bindValue(":name",editObosobl->text().toUtf8());
                queryTemp.exec();
                queryTemp.next();
                line += queryTemp.value(0).toString().toUtf8();
                line += "', '";
                if(editOformlen->isChecked()){
                    line += "1";
                }else{
                    line += "0";
                }
                line += "')";
                line += "\r\n";
                stream<<line;
            }else{
                QMessageBox::warning(this,trUtf8("Внимание!"),trUtf8("Такой сотрудник в базе существует!!!"));
                return;
            }
        }else{
            QSqlQuery query;
            query.prepare("UPDATE employee SET employeename = :empname, employeenameupper = :employeenameupper, "
                          "genderid = :genderid, "
                          "organizationid = (SELECT organizationid FROM organization WHERE organizationname = :orgname), "
                          "subdivisionid = (SELECT subdivisionid FROM subdivision WHERE subdivisionname = :subname), "
                          "postid = (SELECT postid FROM post WHERE postname = :postname), "
                          "datebirthday = :datebirthday, "
                          "tabnumber = :tabnumber, "
                          "kodkarty = :kodkarty, "
                          "withorganization = :withorganization, "
                          "passportseriya = :passportseriya, "
                          "passportnumber = :passportnumber,  "
                          "passportissuedby = :passportissuedby, "
                          "passportissueddate = :passportissueddate, "
                          "grafikrabot = (SELECT grafikrabotid FROM grafikrabot WHERE grafikrabotname =:grafikrabot), "
                          "obosoblid = (SELECT obosoblid FROM obosobl WHERE obosoblname =:obosobl), "
                          "oformlenie = :oformlenie "
                          "WHERE employeeid = :empid");
            query.bindValue(":empid",indexTemp);
            query.bindValue(":empname",editFIO->text());
            query.bindValue(":employeenameupper",editFIO->text().toUpper());
            query.bindValue(":genderid",editGender->text());
            query.bindValue(":orgname",editOrganization->text());
            query.bindValue(":subname",editSubdivision->text());
            query.bindValue(":postname",editPost->text());
            query.bindValue(":datebirthday",editDateBirthday->date());
            query.bindValue(":tabnumber",editTabNumber->text());
            query.bindValue(":kodkarty",editKarta->text());
            query.bindValue(":withorganization",editWithOrganization->date());
            query.bindValue(":passportseriya",editPassportSeriya->text());
            query.bindValue(":passportnumber",nameEditPassportNumber->text());
            query.bindValue(":passportissuedby",nameEditPassportIssuedBy->text());
            query.bindValue(":passportissueddate",nameEditPassportIssuedDate->date());
            query.bindValue(":grafikrabot",nameEditGrafikRabot->text());
            query.bindValue(":obosobl",editObosobl->text());
            if(editOformlen->isChecked()){
                query.bindValue(":oformlenie",1);
            }else{
                query.bindValue(":oformlenie",0);
            }
            query.exec();
            if(!query.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("Ошибка Обновления в Базу данных!"),query.lastError().text());
            }
            line += "UPDATE employee SET employeename = '";
            line += editFIO->text().toUtf8();
            line += "', employeenameupper = '";
            line += editFIO->text().toUpper().toUtf8();
            line += "', genderid = '";
            line += editGender->text().toUtf8();
            line += "', organizationid = '";
            QSqlQuery queryTemp;
            queryTemp.prepare("SELECT organizationid FROM organization WHERE organizationname = :orgname");
            queryTemp.bindValue(":orgname",editOrganization->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', subdivisionid = '";
            queryTemp.clear();
            queryTemp.prepare("SELECT subdivisionid FROM subdivision WHERE subdivisionname = :subname");
            queryTemp.bindValue(":subname",editSubdivision->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', postid = '";
            queryTemp.clear();
            queryTemp.prepare("SELECT postid FROM post WHERE postname = :postname");
            queryTemp.bindValue(":postname",editPost->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', datebirthday = '";
            line += editDateBirthday->date().toString("yyyy-MM-dd");
            line += "', tabnumber = '";
            line += editTabNumber->text().toUtf8();
            line += "', kodkarty = '";
            line += editKarta->text().toUtf8();
            line += "', withorganization = '";
            line += editWithOrganization->date().toString("yyyy-MM-dd");
            line += "', passportseriya = '";
            line += editPassportSeriya->text().toUtf8();
            line += "', passportnumber = '";
            line += nameEditPassportNumber->text().toUtf8();
            line += "', passportissuedby = '";
            line += nameEditPassportIssuedBy->text().toUtf8();
            line += "', passportissueddate = '";
            line += nameEditPassportIssuedDate->date().toString("yyyy-MM-dd");
            line += "', grafikrabot = '";
            queryTemp.clear();
            queryTemp.prepare("SELECT grafikrabotid FROM grafikrabot WHERE grafikrabotname =:grafikrabot");
            queryTemp.bindValue(":subname",nameEditGrafikRabot->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            line += "', obosoblid = '";
            queryTemp.clear();
            queryTemp.prepare("SELECT obosoblid FROM obosobl WHERE obosoblname =:obosoblname");
            queryTemp.bindValue(":obosoblname",editObosobl->text());
            queryTemp.exec();
            queryTemp.next();
            line += queryTemp.value(0).toString().toUtf8();
            if(editOformlen->isChecked()){
                line += "', oformlenie = '1' ";
            }else{
                line += "', oformlenie = '0' ";
            }
            line += " WHERE employeeid = '";
            line += indexTemp;
            line += "'";
            line += "\r\n";
            stream<<line;
        }
        QSqlQuery queryDriverQes;
        queryDriverQes.prepare("SELECT * FROM driver WHERE driverid = :id;");
        queryDriverQes.bindValue(":id",indexTemp);
        queryDriverQes.exec();
        queryDriverQes.next();
        if(!queryDriverQes.isValid()){
            QSqlQuery queryDriver;
            queryDriver.prepare("INSERT INTO driver ("
                                "driverid, "
                                "employeeid, "
                                "licenseseriya, "
                                "licensenumber, "
                                "issueddate, "
                                "issuedby, "
                                "licsnsea,"
                                "licsnseb, "
                                "licsnsec, "
                                "licsnsed, "
                                "licsnsee"
                                ") VALUES("
                                ":iddriv, "
                                ":idemp, "
                                ":licenseseriya, "
                                ":licensenumber, "
                                ":issueddate, "
                                ":issuedby, "
                                ":licsnsea,"
                                ":licsnseb, "
                                ":licsnsec, "
                                ":licsnsed, "
                                ":licsnsee);"
                                );
            queryDriver.bindValue(":iddriv",indexTemp);
            queryDriver.bindValue(":idemp",indexTemp);
            queryDriver.bindValue(":licenseseriya",nameEditDriverLicenseSeriya->text().simplified());
            queryDriver.bindValue(":licensenumber",nameEditDriverLicenseNumber->text().simplified());
            queryDriver.bindValue(":issueddate",nameEditDriverLicenseDate->date());
            queryDriver.bindValue(":issuedby",nameEditDriverLicenseIssuedBy->text().simplified());
            queryDriver.bindValue(":licsnsea",nameEditDriverA->checkState());
            queryDriver.bindValue(":licsnseb",nameEditDriverB->checkState());
            queryDriver.bindValue(":licsnsec",nameEditDriverC->checkState());
            queryDriver.bindValue(":licsnsed",nameEditDriverD->checkState());
            queryDriver.bindValue(":licsnsee",nameEditDriverE->checkState());
            queryDriver.exec();
            if(!queryDriver.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR!"),queryDriver.lastError().text());
            }
        }else{
            QSqlQuery queryDriver;
            queryDriver.prepare("UPDATE driver SET "
                                "driverid = :iddriv, "
                                "licenseseriya = :licenseseriya, "
                                "licensenumber = :licensenumber, "
                                "issueddate = :issueddate, "
                                "issuedby = :issuedby, "
                                "licsnsea = :licsnsea, "
                                "licsnseb = :licsnseb, "
                                "licsnsec = :licsnsec, "
                                "licsnsed = :licsnsed, "
                                "licsnsee = :licsnsee "
                                "WHERE employeeid = :empid");
            queryDriver.bindValue(":iddriv",indexTemp);
            queryDriver.bindValue(":empid",indexTemp);
            queryDriver.bindValue(":licenseseriya",nameEditDriverLicenseSeriya->text().simplified());
            queryDriver.bindValue(":licensenumber",nameEditDriverLicenseNumber->text().simplified());
            queryDriver.bindValue(":issueddate",nameEditDriverLicenseDate->date());
            queryDriver.bindValue(":issuedby",nameEditDriverLicenseIssuedBy->text().simplified());
            queryDriver.bindValue(":licsnsea",nameEditDriverA->checkState());
            queryDriver.bindValue(":licsnseb",nameEditDriverB->checkState());
            queryDriver.bindValue(":licsnsec",nameEditDriverC->checkState());
            queryDriver.bindValue(":licsnsed",nameEditDriverD->checkState());
            queryDriver.bindValue(":licsnsee",nameEditDriverE->checkState());
            queryDriver.exec();
            if(!queryDriver.isActive()){
                QMessageBox::warning(this,QObject::trUtf8("DataBase ERROR!"),queryDriver.lastError().text());
            }
        }
        emit accept();
        close();
    }
}

void EmployeeForm::deleteEmployee()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    ForDelete forDelete(indexTemp,"employee",this);
    forDelete.deleteOnDefault();

    QSqlQuery query;
    query.prepare("DELETE FROM employee WHERE employeeid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();

    line += "DELETE FROM employee WHERE employeeid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void EmployeeForm::addOrgRecord()
{
    OrganizationForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT organizationname From organization WHERE organizationid = :orgid");
        query.bindValue(":orgid",orgForm.returnValue());
        query.exec();
        query.next();
        editOrganization->setText(query.value(0).toString());
    }
}

void EmployeeForm::seeOrgRecord()
{
    QSqlQuery query;
    query.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
    query.bindValue(":name",editOrganization->text());
    query.exec();
    while(query.next()){
        OrganizationForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void EmployeeForm::listOrgRecord()
{
    QSqlQuery query;
    query.prepare("SELECT organizationid FROM organization WHERE organizationname = :name");
    query.bindValue(":name",editOrganization->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"organization",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT organizationname FROM organization WHERE organizationid = :orgid");
        query.bindValue(":orgid",listTemp.returnValue());
        query.exec();
        query.next();
        editOrganization->setText(query.value(0).toString());
    }
}

void EmployeeForm::addSubRecord()
{
    SubDivisionForm subForm("",this,false);
    subForm.exec();
    if(subForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT subdivisionname FROM subdivision WHERE subdivisionid = :subid");
        query.bindValue(":subid",subForm.returnValue());
        query.exec();
        query.next();
        editSubdivision->setText(query.value(0).toString());
    }
}

void EmployeeForm::seeSubRecord()
{
    QSqlQuery query;
    query.prepare("SELECT subdivisionid FROM subdivision WHERE subdivisionname = :name");
    query.bindValue(":name",editSubdivision->text());
    query.exec();
    while(query.next()){
        SubDivisionForm subForm(query.value(0).toString(),this,true);
        subForm.exec();
    }
}

void EmployeeForm::listSubRecord()
{
    QSqlQuery query;
    query.prepare("SELECT subdivisionid FROM subdivision WHERE subdivisionnname = :name");
    query.bindValue(":name",editSubdivision->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"subdivision",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT subdivisionname FROM subdivision WHERE subdivisionid = :subid");
        query.bindValue(":subid",listTemp.returnValue());
        query.exec();
        query.next();
        editSubdivision->setText(query.value(0).toString());
    }
}

void EmployeeForm::addPostRecord()
{
    PostForm postForm("",this,false);
    postForm.exec();
    if(postForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT postname FROM post WHERE postid = :posid");
        query.bindValue(":posid",postForm.returnValue());
        query.exec();
        query.next();
        editPost->setText(query.value(0).toString());
    }
}

void EmployeeForm::seePostRecord()
{
    QSqlQuery query;
    query.prepare("SELECT postid FROM post WHERE postname = :name");
    query.bindValue(":name",editPost->text());
    query.exec();
    while(query.next()){
        PostForm postForm(query.value(0).toString(),this,true);
        postForm.exec();
    }
}

void EmployeeForm::listPostRecord()
{
    QSqlQuery query;
    query.prepare("SELECT postid FROM post WHERE postname = :name");
    query.bindValue(":name",editPost->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"post",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT postname FROM post WHERE postid = :postid");
        query.bindValue(":postid",listTemp.returnValue());
        query.exec();
        query.next();
        editPost->setText(query.value(0).toString());
    }
}

void EmployeeForm::done(int result)
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.remove("indexPhoto");
    settings.setValue("EmployeeForm", saveGeometry());

    QDialog::done(result);
}

void EmployeeForm::overalRecord()
{
    OveralForm overal(indexTemp,this,false) ;
    overal.exec();
    overalModel->select();
}

void EmployeeForm::createContextMenu()
{
    addAction = new QAction(trUtf8("Добавить"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(overalRecord()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(trUtf8("Редактировать"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(overalRecord()));

    overalWidget->addAction(addAction);
    //tableView->addAction(deleteAction);
    overalWidget->addAction(editAction);
    overalWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EmployeeForm::ListPostSizRecord()
{
    QSqlQuery query;
    query.prepare("SELECT postsizid FROM postsiz WHERE postsizname = :name");
    query.bindValue(":name",editNormaSiz->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"postsiz",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT postsizname FROM postsiz WHERE postsizid = :postsizid");
        query.bindValue(":postsizid",listTemp.returnValue());
        query.exec();
        query.next();
        editNormaSiz->setText(query.value(0).toString());
    }
}

bool EmployeeForm::checkFill()
{
    bool fill = true;
    if(editDateBirthday->date() > QDate::currentDate().addYears(-18)){
        editDateBirthday->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Что-то с датой рождения!"));
        editDateBirthday->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editFIO->text() == ""){
        editFIO->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editFIO->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editOrganization->text() == ""){
        editOrganization->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editOrganization->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }if(editSubdivision->text() == ""){
        editSubdivision->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editSubdivision->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }if(editPost->text() == ""){
        editPost->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editPost->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }if(editGender->text() == ""){
        tabWidget->setCurrentWidget(passportWidget);
        editGender->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editGender->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }

    return fill;
}

void EmployeeForm::createContextSIZ()
{
    QPixmap pixEdit(":/edit.png");
    QAction *editAction = new QAction(trUtf8("Выдача СИЗ (Открыть)"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(vidachaSIZ()));

    tableSizWidget->addAction(editAction);
    tableSizWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EmployeeForm::contextLaborProtection()
{
    QPixmap pixEdit(":/edit.png");
    QAction *editAction = new QAction(trUtf8("Протокол Охраны Труда (Открыть)"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(laborProtection()));

    laborProtectionWidget->addAction(editAction);
    laborProtectionWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EmployeeForm::contextIndustrialSecurity()
{
    QPixmap pixEdit(":/edit.png");
    QAction *editAction = new QAction(trUtf8("Протокол Промбезопасности (Открыть)"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(industrialSecurity()));

    industrialSecurityWidget->addAction(editAction);
    industrialSecurityWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EmployeeForm::contextPTM()
{
    QPixmap pixEdit(":/edit.png");
    QAction *editAction = new QAction(trUtf8("Протокол ПТМ (Открыть)"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(ptm()));

    fireProtectionWidget->addAction(editAction);
    fireProtectionWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EmployeeForm::contextElectroProm()
{
    QPixmap pixEdit(":/edit.png");
    QAction *editAction = new QAction(trUtf8("Протокол Электробезопасности (Открыть)"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(elektroprom()));

    electroProtWidget->addAction(editAction);
    electroProtWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EmployeeForm::sizQuery()
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
    queryPostId.bindValue(":postname",editPost->text());
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
        editNormaSiz->setText(queryPostSiz.value(1).toString());
        tableSizWidget->insertRow(rowCount);

        QSqlQuery queryBuh;
        queryBuh.prepare("SELECT * FROM siz WHERE (employeeid = :employeeid AND siznaimid = :siznaimid)");
        queryBuh.bindValue(":employeeid",indexTemp);
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
        tableSizWidget->item(rowCount,0)->setTextColor(col);

        QTableWidgetItem *itemHar = new QTableWidgetItem;
        tableSizWidget->setItem(rowCount,1,itemHar);
        tableSizWidget->item(rowCount,1)->setText(querySIZNorm.value(1).toString());
        tableSizWidget->item(rowCount,1)->setTextColor(col);

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

        ++rowCount;
    }
}

void EmployeeForm::vidachaSIZ()
{
    VidachaSIZForm openForm("",indexTemp, editPost->text(),this);
    openForm.exec();
    sizQuery();
}

void EmployeeForm::laborProtection()
{
    QModelIndex index = laborProtectionWidget->currentIndex();
    QSqlRecord record = laborProtectionModel->record(index.row());

    QString iDValue = record.value("laborprotectionid").toString();
    ProtocolLaborprotectionForm openForm(iDValue,indexTemp,this,false);
    openForm.exec();
    laborProtectionModel->select();
    //laborProtectionWidget->repaint();
}

void EmployeeForm::industrialSecurity()
{
    QModelIndex index = industrialSecurityWidget->currentIndex();
    QSqlRecord record = industrialSecurityModel->record(index.row());

    QString iDValue = record.value("industrialsecurityid").toString();
    IndustrialSecurityForm openForm(iDValue,indexTemp,this,false);
    openForm.exec();
    industrialSecurityModel->select();
    industrialSecurityWidget->repaint();
}

void EmployeeForm::ptm()
{
    QModelIndex index = fireProtectionWidget->currentIndex();
    QSqlRecord record = fireProtectionModel->record(index.row());

    QString iDValue = record.value("ptmid").toString();
    FireProtectionForm openForm(iDValue,indexTemp,this,false);
    openForm.exec();
    fireProtectionModel->select();
    fireProtectionWidget->repaint();
}

void EmployeeForm::elektroprom()
{
    QModelIndex index = electroProtWidget->currentIndex();
    QSqlRecord record = electroProtModel->record(index.row());

    QString iDValue = record.value("electroprotid").toString();
    ElectroBezForm openForm(iDValue,indexTemp,this,false);
    openForm.exec();
    electroProtModel->select();
    electroProtWidget->repaint();
}

void EmployeeForm::printUd()
{
    bool printU = true;
    QSqlQuery queryLP;
    queryLP.prepare("SELECT laborprotection.laborprotectiondate, laborprotection.laborprotectionid, "
                    "(SELECT otprogramma.otprogrammaname FROM otprogramma WHERE "
                    "laborprotection.otprogrammaid = otprogramma.otprogrammaid) "
                    "FROM laborprotection WHERE employeeid = :employeeid");
    queryLP.bindValue(":employeeid",indexTemp);
    queryLP.exec();
    queryLP.next();
    if(queryLP.value(0).toDate().addYears(1) < QDate::currentDate()){
        QMessageBox::warning(this,trUtf8("Внимание!"),trUtf8("Необходим протокол по Охране труда!"));
        tabWidget->setCurrentWidget(laborProtectionWidget);
        printU = false;
    }else{
        dateLP = queryLP.value(0).toDate().toString("dd.MM.yyyy");
        numberLP = queryLP.value(1).toString();
        programmaLP = queryLP.value(2).toString();
    }
    QSqlQuery queryEP;
    queryEP.prepare("SELECT electroprotdate, electroprotid FROM electroprot WHERE employeeid = :employeeid");
    queryEP.bindValue(":employeeid",indexTemp);
    queryEP.exec();
    queryEP.next();
    if(queryEP.value(0).toDate().addYears(1) < QDate::currentDate()){
        QMessageBox::warning(this,trUtf8("Внимание!"),trUtf8("Необходим протокол по Электробезопасности!"));
        tabWidget->setCurrentWidget(electroProtWidget);
        printU = false;
    }else{
        dateEP = queryEP.value(0).toDate().toString("dd.MM.yyyy");
        numberEP = queryEP.value(1).toString();
    }
    QSqlQuery queryPTM;
    queryPTM.prepare("SELECT ptmdate, ptmid FROM ptm WHERE employeeid = :employeeid");
    queryPTM.bindValue(":employeeid",indexTemp);
    queryPTM.exec();
    queryPTM.next();
    if(queryPTM.value(0).toDate().addYears(1) < QDate::currentDate()){
        QMessageBox::warning(this,trUtf8("Внимание!"),trUtf8("Необходим протокол по ПТМ!"));
        tabWidget->setCurrentWidget(fireProtectionWidget);
        printU = false;
    }else{
        datePTM = queryPTM.value(0).toDate().toString("dd.MM.yyyy");
        numberPTM = queryPTM.value(1).toString();
    }
    if(printU){
        QPrinter printer(QPrinter::HighResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Portrait);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
        QPrintPreviewDialog preview(&printer,this);
        connect(&preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(printUdostov(QPrinter*)));
        preview.showMaximized();
        preview.exec();
    }
}

void EmployeeForm::printUdostov(QPrinter *printer)
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QString numberUd;
    QDate dateUd;
    QSqlQuery queryU;
    queryU.prepare("SELECT * FROM udrab WHERE employeeid = :id");
    queryU.bindValue(":id",indexTemp);
    queryU.exec();
    queryU.next();
    if(!queryU.isValid()){
        NumPrefix numPrefix;
        numberUd = numPrefix.getPrefix("udrab");
        dateUd = QDate::currentDate();
        QSqlQuery queryUd;
        queryUd.prepare("INSERT INTO udrab (udrabid, udrabdate, employeeid) VALUES(:udrabid, :udrabdate, :employeeid)");
        queryUd.bindValue(":udrabid",numberUd);
        queryUd.bindValue(":udrabdate",dateUd);
        queryUd.bindValue(":employeeid",indexTemp);
        queryUd.exec();

        line += "INSERT INTO udrab (udrabid, udrabdate, employeeid) VALUES('";
        line += numberUd.toUtf8();
        line += "', '";
        line += dateUd.toString("yyyy-MM-dd");
        line += "', '";
        line += indexTemp.toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
    }else{
        numberUd = queryU.value(0).toString();
        dateUd = queryU.value(2).toDate();
    }

    QPainter painter(printer);
    QRect rect25(0,0,2160,100);
    painter.setFont(QFont("Times New Roman",10,QFont::Bold));
    QSqlQuery query;
    query.exec("SELECT * FROM nashafirma");
    query.next();
    painter.drawText(rect25,Qt::AlignCenter,query.value(0).toString());
    painter.drawLine(100,100,2160,100);

    QRect rect27(0,100,2160,100);
    painter.setFont(QFont("Times New Roman",5,QFont::Normal));
    painter.drawText(rect27,Qt::AlignHCenter | Qt::AlignTop,trUtf8("(наименование организации)"));

    QRect rectUd(0,150,2160,100);
    painter.setFont(QFont("Times New Roman",10,QFont::Bold));
    QString rr = trUtf8("УДОСТОВЕРЕНИЕ № ");
    rr += numberUd;
    painter.drawText(rectUd,Qt::AlignCenter,rr);

    QRect rectStrC(0,250,300,100);
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    painter.drawText(rectStrC,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Выдано:"));
    QRect rectStrD(300,250,2160,100);
    painter.setFont(QFont("Times New Roman",9,QFont::Bold));
    painter.drawText(rectStrD,Qt::AlignCenter | Qt::TextWordWrap,editFIO->text());
    painter.drawLine(270,350,2160,350);

    QRect rect1(270,350,1890,100);
    painter.setFont(QFont("Times New Roman",5,QFont::Normal));
    painter.drawText(rect1,Qt::AlignTop | Qt::AlignHCenter,trUtf8("(Ф.И.О.)"));
    QRect rect3(0,450,540,100);
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    painter.drawText(rect3,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Место работы:"));
    QRect rect4(540,450,1620,100);
    painter.drawText(rect4,Qt::AlignCenter | Qt::TextWordWrap, editSubdivision->text());

    QRect rect6(0,550,540,100);
    painter.drawText(rect6,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Должность:"));
    QRect rect7(540,550,1620,200);
    painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    painter.drawText(rect7,Qt::AlignCenter | Qt::TextWordWrap, editPost->text());

    QRect rect9(0,750,2160,250);
    QString val = trUtf8("Проведена проверка знаний требований охраны труда по программе обучения безопасным "
                  "методам и приемам труда для профессии: ");
    val += programmaLP;
    val += " в объеме 20 ч.";
    painter.setFont(QFont("Times New Roman",7,QFont::Normal));
    painter.drawText(rect9,Qt::AlignJustify |
                     Qt::TextWordWrap,val);

    QRect rect10(0,1000,2160,100);
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    QString protocolOT = trUtf8("Протокол по ОТ № ");
    protocolOT += numberLP;
    protocolOT += trUtf8(" от ");
    protocolOT += dateLP;
    protocolOT += trUtf8(" г.");
    painter.drawText(rect10,Qt::AlignLeft | Qt::AlignVCenter,protocolOT);

    QRect rect11(0,1100,2160,100);
    painter.drawText(rect11,Qt::AlignLeft | Qt::AlignVCenter,trUtf8("Группа по электробезопасности 2."));

    QRect rect12(0,1200,2160,100);
    //painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    QString protocolET = trUtf8("Протокол по ЭБ № ");
    protocolET += numberEP;
    protocolET += trUtf8(" от ");
    protocolET += dateEP;
    protocolET += trUtf8(" г.");
    painter.drawText(rect12,Qt::AlignLeft | Qt::AlignVCenter,protocolET);

    QRect rect13(0,1300,2160,100);
    //painter.setFont(QFont("Times New Roman",8,QFont::Normal));
    QString protocolPTM = trUtf8("Протокол по ПТМ № ");
    protocolPTM += numberPTM;
    protocolPTM += trUtf8(" от ");
    protocolPTM += datePTM;
    protocolPTM += trUtf8(" г.");
    painter.drawText(rect13,Qt::AlignLeft | Qt::AlignVCenter,protocolPTM);

    QRect rect30(0,1400,1100,100);
    painter.drawText(rect30,Qt::AlignCenter | Qt::AlignHCenter,trUtf8("М.П. Председатель комиссии:"));
    QRect rect31(1000,1400,1160,100);
    painter.drawText(rect31,Qt::AlignRight | Qt::AlignVCenter,trUtf8("С.В.Малиновский"));
    painter.drawLine(810,1500,2160,1500);
    QRect rect32(810,1500,1350,100);
    painter.setFont(QFont("Times New Roman",7,QFont::Normal));
    painter.drawText(rect32,Qt::AlignHCenter | Qt::AlignTop,trUtf8("(Ф.И.О., подпись)"));

    QRect rect36(0,1500,810,100);
    painter.setFont(QFont("Times New Roman",10,QFont::Normal));
    painter.drawText(rect36,Qt::AlignCenter,dateUd.toString("dd.MM.yyyy"));

    painter.drawLine(0,1600,4590,1600);
    painter.drawLine(2160,0,2160,1600);
}
