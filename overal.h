#ifndef OVERAL_H
#define OVERAL_H

#include <QtWidgets>
#include "lineedit.h"

class OveralForm : public QDialog
{
    Q_OBJECT

public:
    OveralForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:

    QLabel *labelGrowth;
    LineEdit *editGrowth;
    QLabel *labelHeaddress;
    LineEdit *editHeaddress;
    QLabel *labelWinChlothes;
    LineEdit *editWinClothes;
    QLabel *labelSumClothes;
    LineEdit *editSumClothes;
    QLabel *labelRespirator;
    LineEdit *editRespirator;
    QLabel *labelGasmask;
    LineEdit *editGasmask;
    QLabel *labelWinShoes;
    LineEdit *editWinShoes;
    QLabel *labelSumShoes;
    LineEdit *editSumShoes;
    QLabel *labelMittens;
    LineEdit *editMittens;
    QLabel *labelGloves;
    LineEdit *editGloves;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp, idEmployee;
    QFile exchangeFile;
    bool newRecord;
};

#endif // OVERAL_H
