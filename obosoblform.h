#ifndef OBOSOBLFORM_H
#define OBOSOBLFORM_H

#include <QtWidgets>
#include "lineedit.h"

class ObosoblForm : public QDialog
{
    Q_OBJECT

public:
    ObosoblForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:
    QLabel *labelName;
    LineEdit *editName;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};
#endif // OBOSOBLFORM_H
