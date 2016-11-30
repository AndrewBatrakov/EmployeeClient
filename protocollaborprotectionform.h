#ifndef PROTOCOLLABORPROTECTIONFORM_H
#define PROTOCOLLABORPROTECTIONFORM_H

#include <QtWidgets>
#include "lineedit.h"
#include <QtPrintSupport>

class ProtocolLaborprotectionForm : public QDialog
{
    Q_OBJECT
public:
    ProtocolLaborprotectionForm(QString,QString,QWidget *parent,bool);
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
    void addProgRecord();
    void seeProgRecord();
    void listProgRecord();
    void addKomRecord();
    void seeKomRecord();
    void listKomRecord();

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
    QLabel *labelSub;
    LineEdit *editSub;
    QLabel *labelPost;
    LineEdit *editPost;
    QLabel *labelProgObuch;
    LineEdit *editProgObuch;
    QLabel *labelKommis;
    LineEdit *editKommis;
    //QLabel *labelOtvetstv;
    //LineEdit *editOtvetstv;

    QDialogButtonBox *buttonBox;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    QString indexTemp, idEmployee;
    bool newRecord;
    QFile exchangeFile;

};

#endif // PROTOCOLLABORPROTECTIONFORM_H
