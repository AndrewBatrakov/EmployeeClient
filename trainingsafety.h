#ifndef TRAININGSAFETY_H
#define TRAININGSAFETY_H

#include <QtSql>
#include <QtWidgets>
#include <QtPrintSupport>

class LineEdit;


class TrainingSafety : public QDialog
{
    Q_OBJECT

public:
    TrainingSafety(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void saveRecord();
    void cancelRecord();
    bool checkingFill();

    void addRecordOfTable();
    void editRecordOfTable();
    void deleteRecordOfTable();
    void addVenueRecord();
    void seeVenueRecord();
    void listVenueRecord();

    void readSettings();
    void writeSettings();

    void printTS(QPrinter *);
    void printPreviewTS();
    int printParagraph(QString, QPainter*, QRect*, int);
    //int paintWord(QPainter *, int, int, const QString &word,int, int, int);

private:
    QLabel *labelNumberDoc;
    QLabel *editNumberDoc;
    QLabel *labelDateDoc;
    QDateTimeEdit *editDateDoc;
    QLabel *labelVenue;
    LineEdit *editVenue;
    QLabel *labelVenueDate;
    QDateEdit *editVenueDate;
    QLabel *labelProtokol;
    LineEdit *editProtokol;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QTableWidget *tableWidget;

    QString idVenue;
    QString indexTemp;
    bool addIntoTable, updateRecord;

    QRect *externalRect;
};

#endif // TRAININGSAFETY_H
