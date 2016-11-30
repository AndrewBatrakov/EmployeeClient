#ifndef IMPORTXMLTABLE_H
#define IMPORTXMLTABLE_H

#include <QtWidgets>
#include <QtXml>

class ImportXmlTable : public QDialog
{
    Q_OBJECT
public:
    ImportXmlTable(QWidget *);

public slots:
    void importXMLTable();
    void traverse(const QDomNode &node);

};

#endif // IMPORTXMLTABLE_H
