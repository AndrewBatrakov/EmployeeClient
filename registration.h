#ifndef REGISRATION_H
#define REGISRATION_H

#include <QtGui>
#include "lineedit.h"

class Registration : public QDialog
{
    Q_OBJECT

public:
    Registration(QWidget *parent = 0);

private slots:
    bool checkInput();

private:
    QLabel *labelUser;
    QLabel *labelPassword;
    QLabel *labelHost;
    QLabel *labelBase;

    LineEdit *editUser;
    LineEdit *editPassword;
    LineEdit *editHost;
    LineEdit *editBase;

    QPushButton *okPushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;
};

#endif // REGISRATION_H
