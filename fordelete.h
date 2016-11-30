#ifndef FORDELETE_H
#define FORDELETE_H

#include <QtWidgets>
#include <QDialog>

class ForDelete : public QDialog
{
    Q_OBJECT

public:
    ForDelete(QString, QString, QWidget *parent);

public slots:
    void deleteOnDefault();

private:
    QLabel *labelLabel;
    QString indexTemp, tabNameAuditee;

};

#endif // FORDELETE_H
