#ifndef ATTESTPBPOSTFORM_H
#define ATTESTPBPOSTFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"
#include <QtPrintSupport>

class AttestPBPostForm : public  QDialog
{
    Q_OBJECT

public:
    AttestPBPostForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void saveRecord();
    void cancelRecord();
    void editRecord();
    void shifrChange(QString);

    void readSettings();
    void writeSettings();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();

    bool checkingFill();
    void updateTable();
    void createContextMenu();

private:
    QLabel *labelShifr;
    LineEdit *editShifr;

    QLabel *labelTestZad;
    QTextEdit *editTestZad;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QString indexTemp;//, idPost, idPostSizTable;
    QTableWidget *postView;
    bool addIntoTable, updateRecord;

    QFile exchangeFile;
};

#endif // ATTESTPBPOSTFORM_H
