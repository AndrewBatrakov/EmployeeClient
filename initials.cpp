#include "initials.h"

Initials::Initials()
{

}

QString Initials::getInitials(QString FIO)
{
    QString familiya, name, nameN, otchestvo, otchestvoO;

    QStringList ee = FIO.split(" ");
    familiya = ee[0];
    name = ee[1];
    nameN = name.left(1);
    nameN += ".";
    otchestvo = ee[2];
    otchestvoO = otchestvo.left(1);
    otchestvoO += ".";

    familiya += " ";
    familiya += nameN;
    familiya += " ";
    familiya += otchestvoO;
    return familiya;
}
