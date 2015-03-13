#ifndef MYMUTEXLOCKER_H
#define MYMUTEXLOCKER_H

#include <QString>

class QMutex;
class MyMutexLocker
{
public:
    MyMutexLocker(QMutex *, int = 0);
    MyMutexLocker(QString s, int = 0);
    ~MyMutexLocker();
    bool success();


protected:
    QMutex * m;
    bool ok;
    QString name;
};

#endif // MYMUTEXLOCKER_H
