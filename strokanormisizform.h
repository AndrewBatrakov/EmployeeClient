#ifndef STROKANORMISIZFORM_H
#define STROKANORMISIZFORM_H

#include <QtWidgets>
#include "lineedit.h"

class StrokaNormiSizForm : public QDialog
{
    Q_OBJECT

public:
    StrokaNormiSizForm(QString, QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();
    bool checkingFill();

private:
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelHar;
    LineEdit *editHar;
    QLabel *labelNumber;
    LineEdit *editNumber;
    QLabel *labelSrok;
    LineEdit *editSrok;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp, idPostSiz, idSizNaim, idHar;
    QFile exchangeFile;
};
#endif // STROKANORMISIZFORM_H
