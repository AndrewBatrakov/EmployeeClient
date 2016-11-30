#ifndef PHOTOLABEL_H
#define PHOTOLABEL_H

#include <QtWidgets/QLabel>

class PhotoLabel : public QLabel
{
    Q_OBJECT
public:
    PhotoLabel(QWidget *parent = 0);
    
signals:
    void clicked();

public slots:
    
protected:
    void mouseReleaseEvent(QMouseEvent *ev);

};

#endif // PHOTOLABEL_H
