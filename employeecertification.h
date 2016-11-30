#ifndef EMPLOYEECERTIFICATION_H
#define EMPLOYEECERTIFICATION_H

#include <QtWidgets>
#include <QtSql>

class LineEdit;

class EmployeeCertification : public QDialog
{
    Q_OBJECT
public:
    EmployeeCertification(QString, QWidget *parent, bool);
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
    void addEmployeeRecord();
    void seeEmployeeRecord();
    void listEmployeeRecord();

    void readSettings();
    void writeSettings();

    void setEnter(QString);

private:
    QLabel *labelEmployee;
    LineEdit *editEmployee;

    QLabel *labelPeriodicity;
    LineEdit *editPeriodicity;

    QTableWidget *tableWidget;

    QString indexTemp;
    bool addIntoTable;

    QString idEmployee, empcertdateId;
};

#endif // EMPLOYEECERTIFICATION_H
