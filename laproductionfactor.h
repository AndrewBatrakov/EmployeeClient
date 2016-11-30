#ifndef LAPRODUCTIONFACTOR_H
#define LAPRODUCTIONFACTOR_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"

class LaProductionFactor : public QDialog
{
    Q_OBJECT
public:
    LaProductionFactor(QString,QWidget *parent,bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelSanatoryNorms;
    LineEdit *editSanatoryNorms;
    QLabel *labelApproved;
    LineEdit *editApproved;
    QLabel *labelApprovedDate;
    LineEdit *editApprovedDate;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // LAPRODUCTIONFACTOR_H
