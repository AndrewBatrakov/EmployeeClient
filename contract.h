#ifndef CONTRACTFORM_H
#define CONTRACTFORM_H

#include <QtWidgets>

class LineEdit;

class ContractForm : public QDialog
{
    Q_OBJECT

public:
    ContractForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void addRecord();
    void seeRecord();
    void listRecord();

private:

    QLabel *labelNumber;
    LineEdit *editNumber;
    QLabel *labelDate;
    QDateEdit *editDate;
    QLabel *labelEmployee;
    LineEdit *editEmployee;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QPushButton *printPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // CONTRACTFORM_H
