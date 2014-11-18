#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>

#define HAVE_REMOTE
#include "pcap.h"

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(QObject * parent = 0): QThread(parent) {
        jobFlag = 3;
    }

protected:
    void run();

    void stopWork(); //1
    void startWork(); //2
    void work();   //3
    int jobFlag;

public:
    static bool isWorking();
    static bool startDetecting();
    static bool stopDetecting();
    static QString getFirstMessage();
public:
    void debug() {
        run();
    }
    pcap_if_t *devs;
};

#endif // WORKERTHREAD_H
