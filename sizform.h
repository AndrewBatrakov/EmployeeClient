#ifndef SIZFORM_H
#define SIZFORM_H

#include <QtWidgets>
#include "lineedit.h"

class SizForm : public QDialog
{
    Q_OBJECT
public:
    SizForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:

    QLabel *labelEmployee;
    LineEdit *editEmployee;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // SIZFORM_H
