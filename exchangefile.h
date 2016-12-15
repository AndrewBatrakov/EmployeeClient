#ifndef EXCHANGEFILE_H
#define EXCHANGEFILE_H

#include <QtCore>

class ExchangeFile : public QObject
{
    Q_OBJECT

public:
    ExchangeFile();

public slots:
    void toServer();

private:
    QFile exFile;
};

#endif // EXCHANGEFILE_H
