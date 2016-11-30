#ifndef SIZNAIMFORM_H
#define SIZNAIMFORM_H

#include <QtWidgets>
#include "lineedit.h"

class SizNaimForm : public QDialog
{
    Q_OBJECT

public:
    SizNaimForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

private:
    QLabel *labelName;
    LineEdit *editName;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QFile exchangeFile;
};

#endif // SIZNAIMFORM_H
