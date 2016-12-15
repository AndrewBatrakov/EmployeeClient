#ifndef INITIALS_H
#define INITIALS_H

#include <QtCore>

class Initials : public QObject
{
    Q_OBJECT
public:
    Initials();

public slots:
    QString getInitials(QString tableName);

};
#endif // INITIALS_H
