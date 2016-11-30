#ifndef ALLEMPLOYEESIZ_H
#define ALLEMPLOYEESIZ_H

#include <QtWidgets>
#include <QtPrintSupport>

class AllEmployeeSiz : public QDialog
{
    Q_OBJECT
public:
    AllEmployeeSiz(QWidget *);
    void done(int result);

public slots:
    void printPreview();
    void reportView();

private slots:
    void print(QPrinter *);
    int printParagraph(QString, QPainter*, QRect*, int);
    void readSettings();
    void writeSettings();

private:
    QTableView *employeeView;
    QLabel *labelDate;
};

#endif // ALLEMPLOYEESIZ_H
