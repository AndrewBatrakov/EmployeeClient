#ifndef OBUCHENIELULFORM_H
#define OBUCHENIELULFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"
#include <QtPrintSupport>

class ObuchenieLulForm : public  QDialog
{
    Q_OBJECT

public:
    ObuchenieLulForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();
    void printTable();

private slots:
    void saveRecord();
    void cancelRecord();
    void editRecord();
    void excelExport();

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
    QLabel *labelDogovor;
    LineEdit *editDogovor;

    QLabel *labelDateDoc;
    QDateEdit *editDateDoc;
    QLabel *labelDateCor;
    QDateEdit *editDateCor;
    QLabel *labelDateObuch;
    QDateEdit *editDateObuch;
    QLabel *labelProtokolObuch;
    LineEdit *editProtocolObuch;
    QLabel *labelPrichina;
    LineEdit *editPrichina;
    QLabel *labelVidAtest;
    LineEdit *editVidAtest;

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

#endif // OBUCHENIEDOCFORM_H
