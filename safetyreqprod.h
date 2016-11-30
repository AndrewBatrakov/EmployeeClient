#ifndef SAFETYREQPROD_H
#define SAFETYREQPROD_H

#include <QtSql>
#include <QtWidgets>

class LineEdit;


class SafetyReqProd : public QDialog
{
    Q_OBJECT

public:
    SafetyReqProd(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void saveRecord();
    void cancelRecord();
    bool checkingFill();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void seeRecordOfTable();
    void addSecurityRecord();
    void seeSecurityRecord();
    void listSecurityRecord();
    void addSafetyReqRecord();
    void seeSafetyReqRecord();
    void listSafetyReqRecord();
    void addTestTaskRecord();
    void seeTestTaskRecord();
    void listTestTaskRecord();

    void readSettings();
    void writeSettings();

private:

    QLabel *labelSecurity;
    LineEdit *editSecurity;
    QLabel *labelSafetyReq;
    LineEdit *editSafetyReq;
    QLabel *labelTestTask;
    LineEdit *editCiphr;
    LineEdit *editTestTask;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QTableWidget *tableWidget;

    QString idSecurity, idSafetyReq, idTestTask;
    QString indexTemp;
    bool addIntoTable, updateRecord;
};

#endif // SAFETYREQPROD_H
