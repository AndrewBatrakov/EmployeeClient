#ifndef NOMENFORM_H
#define NOMENFORM_H

#include <QtWidgets>
#include "lineedit.h"

class NomenForm : public QDialog
{
    Q_OBJECT

public:
    NomenForm(QString, QWidget *, bool);
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

#endif // NOMENFORM_H
