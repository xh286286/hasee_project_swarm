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
    static bool showflag = true;
    if (showflag) {

        DanmuWindow::instance->hide();
        showflag = false;
    }
    else {

        DanmuWindow::instance->show();
        showflag = true;
        DanmuWindow::instance->addOneDebugInfor(myTr("打开弹幕显示"));
    }
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
