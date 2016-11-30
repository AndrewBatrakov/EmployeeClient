#ifndef LANORMLINKFORM_H
#define LANORMLINKFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"

class LaNormLinkForm : public QDialog
{
    Q_OBJECT
public:
    LaNormLinkForm(QString,QWidget *parent,bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void addRecordTable();
    void deleteRecordTable();

private:

    QLabel *labelName;
    LineEdit *editName;

    QSqlTableModel *model;
    QTableView *tableView;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // LANORMLINKFORM_H
