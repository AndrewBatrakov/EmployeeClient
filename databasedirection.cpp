#include <QtGui>
#include "databasedirection.h"

DataBaseDirection::DataBaseDirection()
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
}

void DataBaseDirection::createDataBase(QString base)
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString  prefName= settings.value("numprefix").toString();
    if(prefName == ""){
        QMessageBox::warning(this,tr("Attention!"),tr("Insert Prefix Number!!!"));
    }

    int passed = 1;
    QString character;
    do{
        //QSqlDatabase db;
        if(passed == 1){
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("traficsafety.dat");
            character = "";
        }else{
            db = QSqlDatabase::addDatabase("QMYSQL");
            db.setDatabaseName("test");
            db.setHostName("91.102.219.74");
            db.setUserName("root");
            db.setPassword("Helga2210");
            db.open();

            QSqlQuery query(db);
            query.exec("CREATE DATABASE traficsafety;");
            db.database("test").close();
            db.setDatabaseName("traficsafety");
            db.setHostName("91.102.219.74");
            db.setUserName("root");
            db.setPassword("Helga2210");
            character = "CHARACTER SET \'utf8\'";
        }

        if(db.open()){

        QProgressDialog progress(this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle(QObject::tr("Traffic Safety"));
        progress.setLabelText(QObject::tr("Creating database..."));
        progress.setMinimum(0);
        progress.setMaximum(9);
        progress.setValue(1);
        qApp->processEvents();

        progress.setValue(1);
        qApp->processEvents();
        QString queryString = "CREATE TABLE organization ("
                "organizationid VARCHAR(12)";
        queryString += character;
        queryString += " PRIMARY KEY NOT NULL, "
                "organizationname VARCHAR(100)";
        queryString += character;
        queryString += " NOT NULL DEFAULT 'No Value');";
        QSqlQuery query;
        query.exec(queryString);
        progress.setValue(2);
        qApp->processEvents();
        query.exec("INSERT INTO organization (organizationid, organizationname) VALUES ('OWN000000001', 'ZPGS');");
        progress.setValue(4);
        qApp->processEvents();
        queryString = "CREATE TABLE subdivision (subdivisionid VARCHAR(12)";
        queryString += character;
        queryString += "PRIMARY KEY NOT NULL, subdivisionname VARCHAR(150)";
        queryString += character;
        queryString += " NOT NULL DEFAULT 'No Value', organizationid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL);";
        query.exec(queryString);
        progress.setValue(5);
        qApp->processEvents();
        query.exec("INSERT INTO subdivision (subdivisionid, subdivisionname, organizationid) VALUES ('OWN000000001','No Value','OWN000000001');");
        progress.setValue(6);
        qApp->processEvents();
        queryString = "CREATE TABLE post (postid VARCHAR(12)";
        queryString += character;
        queryString += " PRIMARY KEY NOT NULL, postname VARCHAR(150)";
        queryString += character;
        queryString += " NOT NULL DEFAULT 'No Value');";
        query.exec(queryString);
        progress.setValue(7);
        qApp->processEvents();
        query.exec("INSERT INTO post (postid, postname) VALUES ('OWN000000001','No Value');");
        progress.setValue(8);
        qApp->processEvents();
        queryString = "CREATE TABLE employee (employeeid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL PRIMARY KEY, employeename VARCHAR(100)";
        queryString += character;
        queryString += " NOT NULL, organizationid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, subdivisionid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, postid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, datebirthday DATE NOT NULL, tabnumber VARCHAR(10)";
        queryString += character;
        queryString += ", kodkarty VARCHAR(10)";
        queryString += character;
        queryString += ", experience VARCHAR(100)";
        queryString += character;
        queryString += ", withorganization DATE, passportseriya VARCHAR(50)";
        queryString += character;
        queryString += ", passportnumber VARCHAR(50)";
        queryString += character;
        queryString += ", passportissuedby VARCHAR(200)";
        queryString += character;
        queryString += ", passportissueddate DATE, grafikrabot VARCHAR(50)";
        queryString += character;
        queryString += ");";
        query.exec(queryString);
        progress.setValue(9);
        qApp->processEvents();
        queryString = "CREATE TABLE users (userid VARCHAR(10)";
        queryString += character;
        queryString += " NOT NULL, userpas VARCHAR(10)";
        queryString += character;
        queryString += " NOT NULL);";
        query.exec(queryString);
        query.exec("INSERT INTO users (userid, userpas) VALUES ('root','Helga2210');");
        progress.setValue(10);
        qApp->processEvents();
        queryString = "CREATE TABLE photo (photoid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL PRIMARY KEY, photoname BLOB NOT NULL);";
        query.exec(queryString);
        progress.setValue(11);
        qApp->processEvents();
        queryString = "CREATE TABLE education (educationid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL PRIMARY KEY, employeeid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, typeofeducation VARCHAR(50)";
        queryString += character;
        queryString += ", educationalinstitution VARCHAR(200)";
        queryString += character;
        queryString += ", specialty VARCHAR(100)";
        queryString += character;
        queryString += ", diplom VARCHAR(30)";
        queryString += character;
        queryString += ", year DATE, qualification VARCHAR(150)";
        queryString += character;
        queryString += ");";
        query.exec(queryString);
        progress.setValue(12);
        qApp->processEvents();
        queryString = "CREATE TABLE firesafety (firesafetyid VARCHAR(12)";
        queryString += character;
        queryString += " PRIMARY KEY NOT NULL, firesafetydate DATE NOT NULL, employeeid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, firesafetynumber VARCHAR(20)";
        queryString += character;
        queryString += ", firesafetyhour INTEGER, firesafetysostav VARCHAR(200)";
        queryString += character;
        queryString += ", firesafetyudnumber VARCHAR(20)";
        queryString += character;
        queryString += ", firesafetyuddate DATE);";
        query.exec(queryString);
        progress.setValue(13);
        qApp->processEvents();
        queryString = "CREATE TABLE industrialsecurity (industrialsecurityid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL PRIMARY KEY, industrialsecuritydate DATE NOT NULL, employeeid VARCHAR(12)";
        queryString += character;
        queryString += ", trainingprogramm VARCHAR(200)";
        queryString += character;
        queryString += ", industrialsecuritynumber VARCHAR(50)";
        queryString += ", obchasov integer);";
        query.exec(queryString);
        progress.setValue(14);
        qApp->processEvents();
        queryString = "CREATE TABLE laborprotection (laborprotectionid VARCHAR(12)";
        queryString += character;
        queryString += " PRIMARY KEY NOT NULL, laborprotectiondate DATE NOT NULL, "
                "employeeid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, trainingprogramm VARCHAR(200)";
        queryString += character;
        queryString += ", laborprotectionnumber VARCHAR(50)";
        queryString += ", laborprotectionudostoverenie VARCHAR(50)";
        queryString += character;
        queryString += ");";
        query.exec(queryString);
        progress.setValue(15);
        qApp->processEvents();
        queryString = "CREATE TABLE physical (physicalid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL PRIMARY KEY, employeeid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, numberdirection VARCHAR(50)";
        queryString += character;
        queryString += ", datedirection VARCHAR(200)";
        queryString += character;
        queryString += ", passed BOOLEAN, fit BOOLEAN, dateofpassage DATE, numberspravki VARCHAR(50)";
        queryString += character;
        queryString += ");";
        query.exec(queryString);
        progress.setValue(16);
        qApp->processEvents();
        queryString = "CREATE TABLE driver (driverid VARCHAR(12)";
        queryString += " NOT NULL PRIMARY KEY, employeeid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, licenseseriya VARCHAR(4)";
        queryString += character;
        queryString += ", licensenumber VARCHAR(6)";
        queryString += ", issueddate DATE, issuedby VARCHAR(150)";
        queryString += ", licsnsea BOOLEAN, licsnseb BOOLEAN, licsnsec BOOLEAN, licsnsed BOOLEAN, licsnsee BOOLEAN);";
        query.exec(queryString);
        progress.setValue(17);
        qApp->processEvents();
        queryString = "CREATE TABLE overal (overalid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL, employeeid VARCHAR(12)";
        queryString += " NOT NULL, growth INTEGER, winclothes INTEGER, sumclothes INTEGER, winshoes INTEGER, "
                "sumshoes INTEGER, respirator INTEGER, gasmask INTEGER, mittens INTEGER, gloves INTEGER);";
        query.exec(queryString);
        progress.setValue(18);
        qApp->processEvents();
        queryString = "CREATE TABLE personalinformation (personalinformationid VARCHAR(12)";
        queryString += character;
        queryString += " PRIMARY KEY NOT NULL, employeeid VARCHAR(12)";
        queryString += " NOT NULL, information VARCHAR(200)";
        queryString += character;
        queryString += ");";
        query.exec(queryString);
        progress.setValue(19);
        qApp->processEvents();
        queryString = "CREATE TABLE rodnie (rodnieid VARCHAR(12)";
        queryString += character;
        queryString += " NOT NULL PRIMARY KEY, employeeid VARCHAR(12)";
        queryString += " NOT NULL, stepenrodstva VARCHAR(50)";
        queryString += character;
        queryString += ", namerodstva VARCHAR(150)";
        queryString += character;
        queryString += ", birthrodstva DATE);";
        query.exec(queryString);
        progress.setValue(progress.maximum());
        qApp->processEvents();
    }else{
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
    }
        ++passed;
        db.close();
    }while(passed < 3);
}


bool DataBaseDirection::connectDataBase()
{
//    QSqlDatabase globalDataBaseConnection = QSqlDatabase::addDatabase("QMYSQL");
//    globalDataBaseConnection.setDatabaseName("traficsafety");
//    globalDataBaseConnection.setHostName("91.102.219.74");
//    globalDataBaseConnection.setUserName("root");
//    globalDataBaseConnection.setPassword("Helga2210");
//    if(!globalDataBaseConnection.open()){
//        QMessageBox::warning(0, QObject::tr("MySQL Database Error"),
//                             globalDataBaseConnection.lastError().text());
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("CE_SQLite.dat");
        if (!db.open()) {
            QMessageBox::warning(0, QObject::tr("SQLite Database Error"),
                                 db.lastError().text());
            return false;
        }else{
            return true;
        }
//    }else{
//        qDebug()<<"MySQL Ok";
//        return true;
//    }
}

void DataBaseDirection::cancelCangesDataBase()
{

}

void DataBaseDirection::closeDataBase()
{
    db.close();
    db.removeDatabase("QSQLITE");
    //QSqlDatabase::removeDatabase("QSQLITE");
}
