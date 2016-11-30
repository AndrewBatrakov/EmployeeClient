#ifndef PHOTOLOAD_H
#define PHOTOLOAD_H

#include <QtWidgets/QDialog>

class QLabel;
class LineEdit;
class QDialogButtonBox;
class QComboBox;
class QDateEdit;
class QTableView;

class PhotoLoad : public QDialog
{
    Q_OBJECT
public:
    PhotoLoad(QWidget *parent);

public slots:
    //void traverseNode(const QDomNode &node);
    void importPhotoProcedure();

private:

    int fileCount;

    QPushButton *importPushButton;
};

#endif // PHOTOLOAD_H
