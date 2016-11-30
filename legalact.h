#ifndef LEGALACT_H
#define LEGALACT_H

#include <QtWidgets>

class LineEdit;

class LegalAct : public QDialog
{
    Q_OBJECT

public:
    LegalAct(QString, QWidget *, bool);
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

#endif // LEGALACT_H
