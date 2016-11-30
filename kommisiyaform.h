#ifndef KOMMISIYAFORM_H
#define KOMMISIYAFORM_H

#include <QtWidgets>
#include "lineedit.h"

class KommisiyaForm : public QDialog
{
    Q_OBJECT

public:
    KommisiyaForm(QString,QWidget *parent,bool);
    void done(int);

public slots:
    void deleteRecord();
    QString returnValue() const {return indexTemp;};

private slots:
    void editRecord();
    void addEmp1Record();
    void seeEmp1Record();
    void listEmp1Record();

    void readSettings();
    void writeSettings();
    bool checkFill();

private:
    QLabel *labelEmp1;
    LineEdit *editEmp1;
    QLabel *labelEmp2;
    LineEdit *editEmp2;
    QLabel *labelEmp3;
    LineEdit *editEmp3;
    QDialogButtonBox *buttonBox;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    QString indexTemp;
    bool newRecord;
    QFile exchangeFile;
};

#endif // KOMMISIYAFORM_H
