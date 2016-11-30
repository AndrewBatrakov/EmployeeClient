#ifndef LACLASSFORM_H
#define LACLASSFORM_H

#include <QtWidgets/QDialog>
#include <QtGui>
#include "lineedit.h"

class LaClassForm : public QDialog
{
    Q_OBJECT
public:
    LaClassForm(QString,QWidget *parent,bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:

    QLabel *labelName;
    LineEdit *editName;


    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // LACLASSFORM_H
