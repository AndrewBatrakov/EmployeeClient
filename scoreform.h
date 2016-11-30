#ifndef SCOREFORM_H
#define SCOREFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"
#include <QtPrintSupport>

class ScoreForm : public QDialog
{
    Q_OBJECT

public:
    ScoreForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();
    void printTable();

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
    void updateTable();
    void createContextMenu();

    void print(QPrinter *);
    int printParagraph(QString, QPainter*, QRect*, int);

private:
    QLabel *labelNumber;
    LineEdit *editNumber;
    QLabel *labelScore;
    LineEdit *editScore;
    QLabel *labelDateDoc;
    QDateEdit *editDateDoc;
    QLabel *labelOrganization;
    LineEdit *editOrganization;
    QLabel *labelPaid;
    QCheckBox *editPaid;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QString indexTemp;//, idPost, idPostSizTable;
    QTableWidget *employeeView;
    bool addIntoTable, updateRecord;

    QFile exchangeFile;
};


#endif // SCOREFORM_H
