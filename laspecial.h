#ifndef LASOUNDLIMITPRO_H
#define LASOUNDLIMITPRO_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"

class LaSpecialForm : public QDialog
{
    Q_OBJECT
public:
    LaSpecialForm(QString,QWidget *parent,bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void addRecordTable();
    void editRecordTable();
    void deleteRecordTable();

private:
    QLabel *labelName;
    LineEdit *editName;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QSqlTableModel *model;
    QTableView *tableView;

    QString indexTemp;
};

#endif // LASOUNDLIMITPRO_H
