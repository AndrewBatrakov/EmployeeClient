#ifndef SIZPOSTFORM_H
#define SIZPOSTFORM_H

#include <QtWidgets>
#include "lineedit.h"
#include <QtSql>

class SizPostForm : public QDialog
{
    Q_OBJECT

public:
    SizPostForm(QString, QWidget *, bool);
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

    void addRecordOfTableSiz();
    void deleteRecordOfTableSiz();
    void editRecordOfTableSiz();

    void addRecordOfTablePost();
    void deleteRecordOfTablePost();
    void editRecordOfTablePost();

    bool checkingFill();
    void createContextMenuPost();
    void createContextMenuSiz();

    void updateSiz();

private:
    QLabel *labelPostSIZ;
    LineEdit *editPostSIZ;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QTableWidget *sizView;
    QTableWidget *postView;
    bool addIntoTable, updateRecord;
    QFile exchangeFile;

protected:
    void keyReleaseEvent(QKeyEvent *);
};

#endif // SIZPOSTFORM_H
