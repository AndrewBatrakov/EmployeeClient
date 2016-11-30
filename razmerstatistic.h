#ifndef RAZMERSTATISTIC_H
#define RAZMERSTATISTIC_H

#include <QtWidgets>
#include <QtPrintSupport>

class RazmerStatistic : public QDialog
{
    Q_OBJECT
public:
    RazmerStatistic(QWidget *parent = 0);
    void printPreview();


private slots:
    void print(QPrinter *);
    int printParagraph(QString, QPainter*, QRect*, int);
};

#endif // RAZMERSTATISTIC_H
