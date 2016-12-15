#include "exchangefile.h"
#include "putfile.h"

ExchangeFile::ExchangeFile()
{

}

void ExchangeFile::toServer()
{
    exFile.setFileName("Message.txt");
    if(exFile.size() == 0){
        exFile.remove();
        return;
    }else{
        exFile.close();
    }
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    QString fileName = settings.value("numprefix").toString();
    fileName += "_SRV.txt";

    qDebug()<<exFile.rename(fileName);
    exFile.close();

    qDebug()<<exFile.isOpen()<<" - "<<exFile.fileName()<<fileName;

    QString fN = exFile.fileName();

    PutFile putFile;
    putFile.putFile(fN);

    PutFile putFtp1;
    QString nullFileName = "Null.txt";
    //nullFileName += fN;
    QFile nullFile;
    nullFile.setFileName(nullFileName);
    nullFile.open(QIODevice::WriteOnly);
    QByteArray rr = "22\n10";
    nullFile.write(rr);
    nullFile.close();
    putFtp1.putFile(nullFileName);
    nullFile.remove();

    QFile file;

    file.setFileName("null.txt");
    file.remove();
}
