#ifndef REGISRATION_H
#define REGISRATION_H

#include <QtGui>
#include "lineedit.h"

class QLabel;
class LineEdit;
class QDialogButtonBox;

class Registration : public QDialog
{
    Q_OBJECT

public:
    Registration(QWidget *parent = 0);

private slots:
    bool checkInput();
    void createBase();

private:
    QLabel *labelUser;
    QLabel *labelPassword;
    QLabel *labelHost;
    QLabel *labelBase;

    LineEdit *editUser;
    LineEdit *editPassword;
    LineEdit *editHost;
    LineEdit *editBase;
    //QCheckBox *

    QPushButton *okPushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;
};

#endif // REGISRATION_H
