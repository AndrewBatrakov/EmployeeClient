#ifndef GRAFIKPROMBEZOP_H
#define GRAFIKPROMBEZOP_H

#include <QtWidgets>
#include <QtPrintSupport>
#include <QtSql>
#include "lineedit.h"

class GrafiKPromBezop : public QDialog
{
    Q_OBJECT
public:
    GrafiKPromBezop(QWidget *parent);
    
private slots:
    void print(QPrinter *);
    void printPreview();
    int printParagraph(QString, QPainter*, QRect*, int);

    void seeSubRecord();
    void listSubRecord();
    void seeTestTaskRecord();
    void listTestTaskReccord();
    
private:
    QLabel *labelSMU;
    LineEdit *editSMU;
    QLabel *labelTestTask;
    LineEdit *editTestTask;
};

#endif // GRAFIKPROMBEZOP_H
