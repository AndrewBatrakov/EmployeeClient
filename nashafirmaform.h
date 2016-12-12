#ifndef NASHAFIRMAFORM_H
#define NASHAFIRMAFORM_H

#include <QtWidgets>
#include "lineedit.h"

class NashaFirmaForm : public QDialog
{
    Q_OBJECT

public:
    NashaFirmaForm(QWidget *);
    void done(int result);

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

private:
    QLabel *labelName;
    LineEdit *editName;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    bool newRecord;
    QFile exchangeFile;
};

#endif // NASHAFIRMAFORM_H
