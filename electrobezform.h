#ifndef ELECTROBEZFORM_H
#define ELECTROBEZFORM_H

#include <QtWidgets>
#include "lineedit.h"
#include <QtPrintSupport>

class ElectroBezForm : public QDialog
{
    Q_OBJECT
public:
    ElectroBezForm(QString,QString,QWidget *parent,bool);
    void done(int);

public slots:
    void deleteRecord();
    QString returnValue() const {return indexTemp;};
    void printTable();

private slots:
    void printUdTable();
    void editRecord();
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

#endif // ELECTROBEZFORM_H
