#ifndef HARFORM_H
#define HARFORM_H

#include <QtWidgets>
#include "lineedit.h"

class HarForm : public QDialog
{
    Q_OBJECT

public:
    HarForm(QString, QWidget *, bool);
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


#endif // HARFORM_H
