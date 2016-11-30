#ifndef SECURITY_H
#define SECURITY_H

#include <QtWidgets>
#include <QtGui>

class LineEdit;

class Security : public QDialog
{
    Q_OBJECT

public:
    Security(QString, QWidget *, bool);
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

#endif // SECURITY_H
