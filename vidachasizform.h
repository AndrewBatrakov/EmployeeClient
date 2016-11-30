#ifndef VIDACHASIZFORM_H
#define VIDACHASIZFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"
#include <QtPrintSupport>

class VidachaSIZForm : public  QDialog
{
    Q_OBJECT

public:
    VidachaSIZForm(QString, QString, QString, QWidget *);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void printTable();

private slots:
    void excelExport();

    void readSettings();
    void writeSettings();

    void print(QPrinter *);
    int printParagraph(QString, QPainter*, QRect*, int);
    void createTableSIZ();
    void createContextSIZ();
    void deleteRecordTable();
   // void recordTableChanged(QTableWidgetItem *);
    void editRecord();

private:
    QLabel *labelEmployee;
    LineEdit *editEmployee;
    LineEdit *editNormaSIZ;
    QLabel *labelNumber;
    LineEdit *editNumber;
    QLabel *labelDate;
    QDateEdit *editDate;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QString indexTemp, idEmployee, namePost;
    QTableWidget *tableSizWidget;
    bool addIntoTable, updateRecord;

    QFile exchangeFile;
    bool printDoc;
};


#endif // VIDACHASIZFORM_H
