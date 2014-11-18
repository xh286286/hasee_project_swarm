#include <QCoreApplication>
#include "mymenu.h"
#include "workerthread.h"
#include "danmuwindow.h"
#include "../share_library/Util.h"
MyMenu::MyMenu()
{
}

void MyMenu::about() {

}

void MyMenu::endProgram() {
    WorkerThread::stopDetecting();
    while(WorkerThread::isWorking()) waitMillisec(100);
    QCoreApplication::exit();
}
void MyMenu::hideAndShow()
{
    DanmuWindow::instance->hide();
    DanmuWindow::instance->show();
}

void MyMenu::startDectecting()
{
    WorkerThread::startDetecting();
    DanmuWindow::instance->ifDetectingFlag = true;
}

void MyMenu::stopDectecting()
{
    WorkerThread::stopDetecting();
    DanmuWindow::instance->ifDetectingFlag = false;
}
