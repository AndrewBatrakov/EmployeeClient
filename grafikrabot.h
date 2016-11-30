#ifndef GRAFIKRABOT_H
#define GRAFIKRABOT_H

#include <QtWidgets>

class GrafikRabotForm : public QDialog
{
    Q_OBJECT

public:
    GrafikRabotForm(QString,QWidget *parent,bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

private:
    QString indexTemp;
};

#endif // GRAFIKRABOT_H
