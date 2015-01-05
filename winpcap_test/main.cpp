typedef  unsigned int u_int;
typedef  unsigned short u_short;
typedef  unsigned char u_char;


#include <QString>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <QList>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QImage>
#include <QTimer>

#include <QIcon>
#include <QJsonObject>
#include <QMessageBox>
using namespace std;

#include "../share_library/messagecenter.h"
#include "workerthread.h"
#include "windowsizesetting.h"
#include "danmuwindow.h"
#include "../share_library/Util.h"
#include "mymenu.h"
#include "autothankgift.h"

/* prototype of the packet handler */
void test1();
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);




    qDebug()<<1;
    AutoThankGift  atg;
    MessageCenter mc;

    if (mc.registerTitle("mayu danmu")) {

    }
    else {
        QMessageBox::critical(0, "error",  "The danmu assistant was running!"   );
        return 0;
    }

    QSystemTrayIcon systemTrayIcon;
    QIcon ico(QString(":/ico/scorpio3.ico"));

    systemTrayIcon.setIcon(ico);

    systemTrayIcon.setToolTip(myTr("麻由的弹幕小助手"));

    systemTrayIcon.show();
//    qDebug()<< QSystemTrayIcon::supportsMessages()<<endl;
//    b. showMessage("sadsf","asdfasdfasdf",QSystemTrayIcon::Information,10000);
    DanmuWindow danmu_win;

    QObject::connect(&danmu_win, &DanmuWindow::broadcastDanmu, &mc, &MessageCenter::broadcast);
    QObject::connect( &mc, &MessageCenter::sendMessage, &danmu_win, &DanmuWindow::postDanmuMessge);

    QObject::connect(&danmu_win, &DanmuWindow::informGift,  &atg, &AutoThankGift::dealPresent);
    QObject::connect(&atg, &AutoThankGift::postThankMessage,  &danmu_win, &DanmuWindow::postDanmuMessge  );

    WindowSizeSetting sizeSetting_win(0,& danmu_win);
    sizeSetting_win.show();
    sizeSetting_win.hide();


    danmu_win.show();
    qDebug()<<" geometry "<<danmu_win.geometry()<<endl;

    danmu_win.addOneDebugInfor(myTr("麻由的弹幕助手已启动"));
    waitMillisec(200);

    danmu_win.addOneDebugInfor(myTr("请在浏览器打开 战旗直播间"));

    waitMillisec(200);
    danmu_win.addOneDebugInfor(myTr("右击系统托盘 显示菜单"));
    waitMillisec(200);

    //danmu_win.addOneDebugInfor(myTr("检测模块启动"));


    MyMenu myMenu;
    myMenu.addAction(myTr("欢迎使用麻由的弹幕小助手"), &myMenu, SLOT(about()) );
    myMenu.addAction(myTr("QQ群 310439712"), &myMenu, SLOT(about()) );
    myMenu.addAction(myTr("设置弹幕显示区域"), &sizeSetting_win, SLOT(show()) );
    myMenu.addAction(myTr("显示刷新弹幕历史"), &danmu_win, SLOT(displayHistory()) );
    //myMenu.addAction(myTr("找回弹幕显示"), &myMenu, SLOT(hideAndShow()) );
    myMenu.addAction(myTr("重启监测模块"), &myMenu, SLOT(startDectecting()) );
    myMenu.addAction(myTr("停止监测模块"), &myMenu, SLOT(stopDectecting()) );
    myMenu.addAction(myTr("退出"), &myMenu, SLOT(endProgram()) );
    //myMenu.hide();

    systemTrayIcon.setContextMenu(&myMenu);


    //QObject::connect(&systemTrayIcon, &QSystemTrayIcon::activated, &sizeSetting_win, &WindowSizeSetting::activated);
    //QObject::connect(&systemTrayIcon, &QSystemTrayIcon::activated, &myMenu, &MyMenu::show);

    WorkerThread::startDetecting();

    QTimer t;
    {

        t.start(10000);
        QObject::connect(&t, &QTimer::timeout, [&]() {
            //qDebug()<<"test";
            QDate d = QDate::currentDate();
            if (d.month()!=3) {
                t.stop();;
                return;
            }
            if (d.day()!=18 && d.day()!=19) {
                t.stop();;
                return;
            }
            if (d.day()==18) t.start(60000);
            danmu_win.addOneDebugInfor(myTr("天蝎祝麻由生日快乐"));
            waitMillisec(200);

            danmu_win.addOneDebugInfor(myTr("天蝎祝麻由生日快乐"));

            waitMillisec(200);
            danmu_win.addOneDebugInfor(myTr("天蝎祝麻由生日快乐"));
            waitMillisec(200);
            t.stop();
        });

    }

    a.exec();

    qDebug()<<" successful exit";

    return 0;
}




void test1() {
    qDebug()<<1;
}
