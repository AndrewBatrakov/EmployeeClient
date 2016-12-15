#ifndef XMLIMPORT_H
#define XMLIMPORT_H

#include <QtWidgets>
#include <QtXml>

class XMLImport : public QDialog
{
    Q_OBJECT
public:
    XMLImport(QWidget *parent);

public slots:
    void updateDir();
    void updateBase();
    void traverseAllInformation(const QDomNode &node);

private slots:
    //QString convertStToDate(QString);
};

#endif // XMLIMPORT_H
