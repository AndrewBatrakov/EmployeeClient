#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include <QtWidgets>
#include "lineedit.h"

class SubDivisionForm : public QDialog
{
    Q_OBJECT

public:
    SubDivisionForm(QString,QWidget *parent,bool);
    void done(int);
    
public slots:
    void deleteRecord();
    QString returnValue() const {return indexTemp;};

private slots:
    void editRecord();
    void addOrgRecord();
    void seeOrgRecord();
    void listOrgRecord();

    void readSettings();
    void writeSettings();
    bool checkFill();

private:
    QLabel *labelSubdivision;
    LineEdit *editSubdivision;
    QLabel *labelOrganization;
    LineEdit *editOrganization;
    QDialogButtonBox *buttonBox;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    QString indexTemp;
    bool newRecord;
    QFile exchangeFile;
};

#endif // SUBDIVISION_H
