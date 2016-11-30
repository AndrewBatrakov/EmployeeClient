#ifndef SQLFORM_H
#define SQLFORM_H

#include <QtWidgets>
#include "lineedit.h"

class SQLForm : public QDialog
{
    Q_OBJECT

public:
    SQLForm(QWidget *);
    void done(int result);

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

private:
    QLabel *labelSQL;
    QTextEdit *editSQL;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QFile exchangeFile;
};

#endif // SQLFORM_H
