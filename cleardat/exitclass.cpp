#include "exitclass.h"
#include <QMetaObject>

ExitClass::ExitClass()
{
    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
}
