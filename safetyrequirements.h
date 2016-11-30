#ifndef SAFETYREQUIREMENTS_H
#define SAFETYREQUIREMENTS_H

#include <QtWidgets>
#include <QtGui>

class LineEdit;

class SafetyRequirements : public QDialog
{
    Q_OBJECT

public:
    SafetyRequirements(QString, QWidget *, bool);
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

#endif // SAFETYREQUIREMENTS_H
