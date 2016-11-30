#ifndef SROKNOSKIFORM_H
#define SROKNOSKIFORM_H

#include <QtWidgets>
#include <QtPrintSupport>

class SrokNoskiForm : public QDialog
{
    Q_OBJECT
public:
    SrokNoskiForm(QWidget *parent = 0);

public slots:
    void printTable();
    void reportView();
    void done(int result);

private slots:
    void print(QPrinter *);
    int printParagraph(QString, QPainter*, QRect*, int);
    void readSettings();
    void writeSettings();

private:
     QTableWidget *employeeView;
     QLabel *labelDate;
     QDateEdit *editDate;
};

#endif // SROKNOSKIFORM_H
