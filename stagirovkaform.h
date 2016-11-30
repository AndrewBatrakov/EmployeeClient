#ifndef STAGIROVKA_H
#define STAGIROVKA_H

#include <QtWidgets>
#include <QtPrintSupport>
#include "lineedit.h"

class StagirovkaForm : public QDialog
{
    Q_OBJECT

public:
    StagirovkaForm(QString,QWidget *parent,bool);
    void done(int);

public slots:
    void deleteRecord();
    QString returnValue() const {return indexTemp;};
    void printTable();

private slots:
    void editRecord();
    void addEmpRecord();
    void seeEmpRecord();
    void listEmpRecord();
    void addOtvRecord();
    void seeOtvRecord();
    void listOtvRecord();

    void readSettings();
    void writeSettings();

    void print(QPrinter *);
    int printParagraph(QString, QPainter*, QRect*, int);

private:
    QLabel *labelNumber;
    LineEdit *editNumber;
    QLabel *labelDate;
    QDateEdit *editDate;
    QLabel *labelEmployee;
    LineEdit *editEmployee;
    QLabel *labelStartDate;
    QDateEdit *editStartDate;
    QLabel *labelEndDate;
    QDateEdit *editEndDate;
    QLabel *labelDay;
    LineEdit *editDay;
    QLabel *labelObyom;
    LineEdit *editObyom;
    QLabel *labelOtvetstv;
    LineEdit *editOtvetstv;

    QDialogButtonBox *buttonBox;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    QString indexTemp;
    bool newRecord;
    QFile exchangeFile;
};

#endif // STAGIROVKA_H
