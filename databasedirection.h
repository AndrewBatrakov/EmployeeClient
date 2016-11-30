#ifndef DATABASEDIRECTION_H
#define DATABASEDIRECTION_H

#include <QtSql>
#include <QtWidgets>

class DataBaseDirection : public QDialog
{
    Q_OBJECT
public:
    DataBaseDirection();

public:
    void createDataBase(QString base);
    bool connectDataBase();
    void cancelCangesDataBase();
    void closeDataBase();

private:
    QSqlDatabase db;
};
#endif // DATABASEDIRECTION_H
