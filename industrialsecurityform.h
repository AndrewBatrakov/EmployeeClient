#ifndef INDUSTRIALSECURITYFORM_H
#define INDUSTRIALSECURITYFORM_H

#include <QtWidgets>
#include "lineedit.h"
#include <QtPrintSupport>

class IndustrialSecurityForm : public QDialog
{
    Q_OBJECT
public:
    IndustrialSecurityForm(QString,QString,QWidget *parent,bool);
    void done(int);

public slots:
    void deleteRecord();
    QString returnValue() const {return indexTemp;};

private slots:
    void printTable();
    void printUdTable();
    void editRecord();
//    void addEmpRecord();
//    void seeEmpRecord();
//    void listEmpRecord();
    void addProgRecord();
    void seeProgRecord();
    void listProgRecord();
    void addKomRecord();
    void seeKomRecord();
    void listKomRecord();
    bool checkFill();

    void readSettings();
    void writeSettings();

    void print(QPrinter *);
    void printUd(QPrinter *);
    int printParagraph(QString, QPainter*, QRect*, int);

private:
    QLabel *labelNumber;
    LineEdit *editNumber;
    QLabel *labelDate;
    QDateEdit *editDate;
    QLabel *labelEmployee;
    LineEdit *editEmployee;
    QLabel *labelSub;
    LineEdit *editSub;
    QLabel *labelPost;
    LineEdit *editPost;
    QLabel *labelPrichina;
    LineEdit *editPrichina;
    QLabel *labelProgObuch;
    LineEdit *editProgObuch;
    QLabel *labelEmp1;
    LineEdit *editEmp1;
    QLabel *labelEmp2;
    LineEdit *editEmp2;
    QLabel *labelEmp3;
    LineEdit *editEmp3;

    QDialogButtonBox *buttonBox;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    QString indexTemp, idEmployee;
    bool newRecord;
    QFile exchangeFile;

};
#endif // INDUSTRIALSECURITYFORM_H
