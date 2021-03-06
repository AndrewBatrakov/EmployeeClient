#ifndef EMPLOYEEFORM_H
#define EMPLOYEEFORM_H

#include <QtWidgets>
#include "lineedit.h"
#include "photolabel.h"
#include <QtSql>
#include <QtPrintSupport>

class PhotoLabel;
class LineEdit;

class EmployeeForm : public QDialog
{
    Q_OBJECT
public:
    EmployeeForm(QString,QWidget *parent,bool);
    void done(int);
    ~EmployeeForm();

private slots:
    void editRecord();
    void photoRead();
    void addOrgRecord();
    void seeOrgRecord();
    void listOrgRecord();
    void addSubRecord();
    void seeSubRecord();
    void listSubRecord();
    void addPostRecord();
    void seePostRecord();
    void listPostRecord();
    void overalRecord();
    void createContextMenu();
    void ListPostSizRecord();
    void printUd();
    void printUdostov(QPrinter *);

    bool checkFill();

    void sizQuery();
    void createContextSIZ();
    void vidachaSIZ();
    void contextLaborProtection();
    void laborProtection();
    void contextIndustrialSecurity();
    void industrialSecurity();
    void ptm();
    void contextPTM();
    void elektroprom();
    void contextElectroProm();

public slots:
    void deleteEmployee();
    QString returnValue() const {return indexTemp;};

private:
    QLabel *labelFoto;
    PhotoLabel *editFoto;
    QPushButton *paintButton;
    QImage pixMap;

    QLabel *labelFIO;
    LineEdit *editFIO;
    QLabel *labelOrganization;
    LineEdit *editOrganization;
    QLabel *labelSubdivision;
    LineEdit *editSubdivision;
    QLabel *labelPost;
    LineEdit *editPost;
    LineEdit *editNormaSiz;
    QLabel *labelDateBirthday;
    QDateEdit *editDateBirthday;

    QLabel *labelTabNumber;
    LineEdit *editTabNumber;
    QLabel *labelExperience;
    LineEdit *editExperience;
    QLabel *labelWithOrganization;
    QDateEdit *editWithOrganization;
    QLabel *labelKarta;
    LineEdit *editKarta;
    QLabel *labelEducation;
    LineEdit *editEducation;
    QLabel *labelObosobl;
    LineEdit *editObosobl;
    QLabel *labelOformlen;
    QCheckBox *editOformlen;

    QLabel *nameLabelDriverLicenseSeriya;
    LineEdit *nameEditDriverLicenseSeriya;
    QLabel *nameLabelDriverLicenseNumber;
    LineEdit *nameEditDriverLicenseNumber;
    QLabel *nameLabelDriverLicenseDate;
    QDateEdit *nameEditDriverLicenseDate;
    QLabel *nameLabelDriverLicenseIssuedBy;
    LineEdit *nameEditDriverLicenseIssuedBy;
    QLabel *nameLabelDriverA;
    QCheckBox *nameEditDriverA;
    QLabel *nameLabelDriverB;
    QCheckBox *nameEditDriverB;
    QLabel *nameLabelDriverC;
    QCheckBox *nameEditDriverC;
    QLabel *nameLabelDriverD;
    QCheckBox *nameEditDriverD;
    QLabel *nameLabelDriverE;
    QCheckBox *nameEditDriverE;

    QLabel *labelPassportSeriya;
    LineEdit *editPassportSeriya;
    QLabel *nameLabelPassportNumber;
    LineEdit * nameEditPassportNumber;
    QLabel *nameLabelPassportIssuedBy;
    LineEdit *nameEditPassportIssuedBy;
    QLabel *nameLabelPassportIssuedDate;
    QDateEdit *nameEditPassportIssuedDate;
    QLabel *labelGender;
    LineEdit *editGender;

    QLabel *nameLabelGrafikRabot;
    LineEdit * nameEditGrafikRabot;

    QLabel *nameLabelDateLaborProtection;
    QDateEdit *nameEditDateLaborProtection;
    QLabel *nameLabelTraningProgramm;
    LineEdit *nameEditTraningProgramm;
    QLabel *nameLabelNumberLaborProtection;
    LineEdit *nameEditNumberLaborProtection;
    QLabel *nameLabelUdLaborProtection;
    LineEdit *nameEditUdLaborProtection;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    bool newRecord;
    bool newImage;

    QAction *deleteAction;

    QTableView *overalWidget;
    QSqlRelationalTableModel *overalModel;
    QTableWidget *tableSizWidget;
    QWidget *passportWidget;
    QTableView *laborProtectionWidget;
    QSqlRelationalTableModel *laborProtectionModel;
    QTableView *industrialSecurityWidget;
    QSqlRelationalTableModel *industrialSecurityModel;
    QTableView *fireProtectionWidget;
    QSqlRelationalTableModel *fireProtectionModel;
    QTableView *electroProtWidget;
    QSqlRelationalTableModel *electroProtModel;

    QTabWidget *tabWidget;
    QAction *addAction;
    QAction *editAction;
    QFile exchangeFile;

    QString numberLP, dateLP, programmaLP, numberPTM, datePTM, numberEP, dateEP;
};

#endif // EMPLOYEEFORM_H
