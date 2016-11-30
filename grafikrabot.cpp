#include <QtGui>
#include <QtSql>

#include "grafikrabot.h"

GrafikRabotForm::GrafikRabotForm(QString, QWidget *parent, bool) :
    QDialog(parent)
{
}

void GrafikRabotForm::done(int result)
{
    QDialog::done(result);
}
