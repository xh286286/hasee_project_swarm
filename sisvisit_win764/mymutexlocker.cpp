#include "mymutexlocker.h"
#include "mainwindow.h"

#include <QMutex>
#include <QCoreApplication>
#include <QTime>
#include <QMap>

QMap<QString, QMutex *>mapSM;
MyMutexLocker::MyMutexLocker(QMutex * m0, int timeout)
{
    m= m0;

    ok = m->tryLock();
    if (!ok) qWarning("wait for unlocking");
    QTime t;
    t.start();
    while(t.elapsed()<timeout && !ok) {
        QCoreApplication::processEvents();
        ok = m->tryLock();
    }
}
MyMutexLocker::MyMutexLocker(QString s, int timeout ):name(s)
{
    MainWindow::outputInfor(s);
    if (mapSM[s] == nullptr) {
        mapSM[s] = new QMutex;
    }
    m = mapSM[s];

    ok = m->tryLock();
    if (!ok) qWarning("wait for unlocking");
    QTime t;
    t.start();
    while(t.elapsed()<timeout && !ok) {
        QCoreApplication::processEvents();
        ok = m->tryLock();
    }
}

MyMutexLocker::~MyMutexLocker()
{
    if (ok) m->unlock();
    if (name!="") MainWindow::outputInfor("unlock " + name);

}
bool MyMutexLocker::success()
{
    return ok;
}
