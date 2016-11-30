#ifndef POSTSIZFORM_H
#define POSTSIZFORM_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"

class PostSizForm : public QDialog
{
    Q_OBJECT
public:
    PostSizForm(QString, QWidget *, bool);
    void done(int result);
    QString returnValue() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void saveRecord();
    void cancelRecord();
    void editRecord();

    void readSettings();
    void writeSettings();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();

    bool checkingFill();
    void updateTable();
    void createContextMenu();

private:
    QLabel *labelPostSIZ;
    LineEdit *editPostSIZ;

    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QString indexTemp, idPost, idPostSizTable;
    QTableWidget *postView;
    bool addIntoTable, updateRecord;

    QFile exchangeFile;
};

#endif // POSTSIZFORM_H
