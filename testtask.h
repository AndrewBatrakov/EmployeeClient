#ifndef TESTTASK_H
#define TESTTASK_H

#include <QtWidgets>
#include <QtGui>

class LineEdit;

class TestTask : public QDialog
{
    Q_OBJECT

public:
    TestTask(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:

    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelCipher;
    LineEdit *editCipher;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // TESTTASK_H
