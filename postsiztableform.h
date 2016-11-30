#ifndef POSTSIZTABLEFORM_H
#define POSTSIZTABLEFORM_H

#include <QtWidgets>
#include "lineedit.h"
#include <QtSql>

class PostSizTableForm : public QDialog
{
    Q_OBJECT

public:
    PostSizTableForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void saveRecord();
    void cancelRecord();
    void editRecord();

    void readSettings();
    void writeSettings();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();

    bool checkingFill();

private:
    QLabel *labelPostSIZ;
    LineEdit *editPostSIZ;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp, idPost, idPostSizTable;
    QTableWidget *sizView;
    bool addIntoTable, updateRecord;
    QFile exchangeFile;
};

#endif // POSTSIZTABLEFORM_H
