#ifndef VENUE_H
#define VENUE_H

#include <QtWidgets>
#include <QtGui>

class LineEdit;

class Venue : public QDialog
{
    Q_OBJECT

public:
    Venue(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();

private:

    QLabel *labelName;
    LineEdit *editName;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
};

#endif // VENUE_H
