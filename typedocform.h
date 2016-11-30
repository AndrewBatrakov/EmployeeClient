#ifndef TYPEDOCFORM_H
#define TYPEDOCFORM_H

#include <QtWidgets>
#include "lineedit.h"

class TypeDocForm : public QDialog
{
    Q_OBJECT

public:
    TypeDocForm(QString, QWidget *, bool);
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
#endif // TYPEDOCFORM_H
