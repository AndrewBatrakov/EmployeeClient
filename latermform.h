#ifndef LATERMFORM_H
#define LATERMFORM_H

#include <QtWidgets/QDialog>
#include <QtGui>
#include "lineedit.h"

class LaTermForm : public QDialog
{
    Q_OBJECT
public:
    LaTermForm(QString,QWidget *parent,bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelDescription;
    LineEdit *editDescription;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};
#endif // LATERMFORM_H
