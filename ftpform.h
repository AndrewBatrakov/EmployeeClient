#ifndef FTPFORM_H
#define FTPFORM_H

#include <QtWidgets>
#include "lineedit.h"

class FtpForm : public QDialog
{
    Q_OBJECT

public:
    FtpForm(QWidget *);
    void done(int result);

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

private:
    QLabel *labelAddress;
    LineEdit *editAddress;
    QLabel *labelLogin;
    LineEdit *editLogin;
    QLabel *labelPassword;
    LineEdit *editPassword;
    QLabel *labelCatalog;
    LineEdit *editCatalog;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

};

#endif // FTPFORM_H
