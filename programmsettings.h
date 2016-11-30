#ifndef PROGRAMMSETTINGS_H
#define PROGRAMMSETTINGS_H

#include <QtWidgets>
#include "lineedit.h"

class ProgrammSettings : public QDialog
{
    Q_OBJECT
public:
    ProgrammSettings(QWidget *parent = 0);

public slots:
    void writeSettings();
    void readSettings();
    void fontSet();

private:
    QLabel *nameFont;
    QComboBox *editFont;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;
};

#endif // PROGRAMMSETTINGS_H
