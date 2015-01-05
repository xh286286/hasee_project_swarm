#include <QJsonObject>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "windowsizesetting.h"
#include "../share_library/Util.h"
#include "danmuwindow.h"
#include <QDebug>
#include <QCloseEvent>
WindowSizeSetting::WindowSizeSetting(QWidget *parent, QWidget * t) :
    QWidget(parent), target(t)
{

    setWindowTitle(myTr("麻由的弹幕小助手 窗口设置"));
    QJsonDocument jd;
    if (loadJsonFile("windowSettingJson.txt", jd)) {

        QJsonObject f = jd.object();
        qDebug()<<f<<endl;
        setGeometry(f["x"].toInt(), f["y"].toInt(),  f["w"].toInt() , f["h"].toInt());
        target->setGeometry(geometry());
    }

}
void WindowSizeSetting::saveSetting() {
    QJsonObject f;
    QRectF r = geometry();
    f["x"] = r.x();
    f["y"] = r.y();
    f["w"] = r.width();
    f["h"] = r.height();
    saveJsonFile("windowSettingJson.txt", QJsonDocument(f));
}
void WindowSizeSetting::focusOutEvent(QFocusEvent *  )
{
//    hide();
//    saveSetting();
}

void WindowSizeSetting::resizeEvent(QResizeEvent *)
{
    target->setGeometry(geometry());

}
void WindowSizeSetting::moveEvent(QMoveEvent *)
{
    target->setGeometry(geometry());
}


void WindowSizeSetting::activated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug()<< " setting window size"<<endl;
    if (reason == QSystemTrayIcon::DoubleClick) {
        show();
    }
    else if (reason == QSystemTrayIcon::Context) {
        exit(0);
    }
}

void WindowSizeSetting::closeEvent(QCloseEvent *e) {
    e->ignore();
    //reject();
    hide();
    saveSetting();
}
