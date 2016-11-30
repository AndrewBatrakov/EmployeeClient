#ifndef EMPCERTDATE_H
#define EMPCERTDATE_H

#include <QtWidgets>
#include "lineedit.h"

class EmpCertDate : public QDialog
{
    Q_OBJECT

public:
    EmpCertDate(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void listTestTask();
    bool checkingFill();

private:
    QLabel *labelFIO;
    LineEdit *editFIO;
    QLabel *labelProtocolDate;
    QDateEdit *editProtocolDate;
    QLabel *labelProtocolNumber;
    LineEdit *editProtocolNumber;
    QLabel *labelName;
    QLabel *editName;
    QLabel *labelCipher;
    LineEdit *editCipher;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QString employeeId, empcertId, testtaskId;
};

#endif // EMPCERTDATE_H
