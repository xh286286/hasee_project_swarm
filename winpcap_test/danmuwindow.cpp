#include <cassert>

#include <QDateTime>
#include <QPainter>
#include <QTimer>
#include <QMap>
#include <QSet>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>

#include <QTimer>

#include "../share_library/Util.h"
#include "danmuwindow.h"
#include "workerthread.h"
#include "danmu.h"



DanmuWindow * DanmuWindow::instance =  NULL;

DanmuWindow::DanmuWindow(QWidget *parent) :
    QWidget(parent)
{
    assert (instance == NULL);
    instance = this;

    flags = Qt::ToolTip;
    flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTransparentForInput;
    QWidget::setWindowFlags(flags);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowOpacity(0.6);

    QTimer * timer=new QTimer;
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(getMessage()));
    timer->start(100);

    test ="console text";

    ifDetectingFlag = true;

    c_fontColor = QColor(Qt::blue);
    c_fontSize = 10;
    c_rectHeight = 30;
    c_flowerChannel = 6;

    c_inTime = 300;
    c_lastTime = 10000;
    c_flowerLastTime = 1000;
    c_outTime = 300;

    c_fontColor = QColor(Qt::blue);
    c_backRectColor = QColor(Qt::gray);


}

void DanmuWindow::dealOneMessage(QJsonObject a)
{
    QString cmdid = a["cmdid"].toString();

    if (cmdid == "chatmessage") {
        QString s = a["content"].toString();
        //&quot;&apos;&amp;
        s.replace("&quot;","\"");
        s.replace("&apos;","'");
        s.replace("&amp;","&");
        test = s;
        QString f = a["fromname"].toString();
        if (f=="") return;
        addOneMessage(a["chatid"].toString(),f,s);

    }
    else if (cmdid == "useprop") {
        test = test+"\nflower";
        addOneFlower(a["hot"].toInt(), a["count"].toInt(), a["fromname"].toString());
    }
    else if (cmdid == "getuc"){
        test = a["cmdid"].toString();
    }
    else if (cmdid == "Gift.Use") {
        QJsonObject b = a["data"].toObject();
        //qDebug()<<b["anchorname"].toString()<<endl;
        //qDebug()<<b["anchorname"].toString().toUtf8().toPercentEncoding()<<endl;
        //qDebug()<<myTr("御园麻由mayu").toUtf8().toPercentEncoding()<<endl;
        //"/alyssasearrs"
        if (b["url"].toString() !="/mayu") return;
        addOnePresent(b["gid"].toInt(), b["count"].toInt(), b["nickname"].toString(), b["name"].toString());
    }
    else {
        //qDebug()<<a<<endl;
    }
}

void DanmuWindow::messageDecode(QString s)
{
    if ( historySet.contains(s)) {
        return;
    }
    else {
        historySet.insert(s);
        historyList.push_back(s);

        if (historyList.size()>2000) {
            historySet.remove(historyList.front());
            historyList.pop_front();
        }
    }

    QJsonObject a = QJsonDocument::fromJson(s.toUtf8()).object();
    qDebug()<<a;
    QString cmdid = a["cmdid"].toString();
    if (cmdid!="") {
        dealOneMessage(a);

        emit broadcastDanmu(s);
    }
    else {
        qDebug()<<s;
    }

}


void DanmuWindow::getMessage()
{
    static int count = 0;
    count++;
    if (count == 19) {
        if (danmuPool.size()==0){
            //hide();
           // show();
        }

         count = 0;
    }

    QString s ;
    s = WorkerThread::getFirstMessage();

    static qint64 lastTime =QDateTime::currentMSecsSinceEpoch();
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    if (!ifDetectingFlag) {
        lastTime = now;
        return;
    }

    if (s.size()<20) {
        if (now - lastTime > 60000 ) {
            lastTime = now + 10000;
            addOneDebugInfor(myTr("长时间未检测到战旗通讯"));
            WorkerThread::startDetecting();
        }
        return;
    }

    //qDebug()<<now-lastTime<<endl;

    lastTime = now;

    messageDecode(s);
    //test = s;
    //repaint();
}

void DanmuWindow::outputDebugInfor(QString s) {
    assert(instance!=NULL);
    instance->addOneDebugInfor(s);
}

void DanmuWindow::addOneDebugInfor(QString content)
{
    int count = 0;
    for (int i=0; i<danmuPool.size(); i++) {
        if (danmuPool[i]->chatFlag) count++;
    }
    Danmu * d = new Danmu(this);
    d ->chatFlag = true;
    d ->chatid = -1;
    d->name = myTr("系统消息");
    d->content = content;

    if (count ==0) {
        nowChatChannel = c_flowerChannel + 1;
    }
    else {
        nowChatChannel ++;
        if (nowChatChannel>=totalChannel-1) nowChatChannel = c_flowerChannel + 1;
    }
    d->channel = nowChatChannel;
    for (int i=0; i<danmuPool.size(); i++) {
        if (danmuPool[i]->channel == nowChatChannel) {
            danmuPool[i]->kill();
            d->delayFlag = true;
            break;
        }
    }
    d->init();
    danmuPool.push_back(d);
    updateLater();

}

void DanmuWindow::addOneMessage(QString chatid, QString name, QString content)
{
//    QMap<int, Danmu* > mIntDanmu;
    int count = 0;
    for (int i=0; i<danmuPool.size(); i++) {
        if (danmuPool[i]->chatid == chatid) return;
        if (danmuPool[i]->chatFlag) count++;
    }
    Danmu * d = new Danmu(this);
    d ->chatFlag = true;
    d ->chatid = chatid;
    d->name = name;
    d->content = content;

    if (count ==0) {
        nowChatChannel = c_flowerChannel + 1;
    }
    else {
        nowChatChannel ++;
        if (nowChatChannel>=totalChannel-1) nowChatChannel = c_flowerChannel + 1;
    }
    d->channel = nowChatChannel;
    for (int i=0; i<danmuPool.size(); i++) {
        if (danmuPool[i]->channel == nowChatChannel) {
            danmuPool[i]->kill();
            d->delayFlag = true;
            break;
        }
    }
    d->init();
    danmuPool.push_back(d);
    updateLater();
}

void DanmuWindow::addOnePresent(int gid, int count, QString uname, QString gname)
{
  //  QMap<int, Danmu* > mIntDanmu;
    static QSet<int> pastPool;
//    if (pastPool.contains(gid)) return;
    pastPool.insert(gid);
    int c= 0;
    for (int i=0; i<danmuPool.size(); i++) {
        if (danmuPool[i]->presentFlag == false) continue;
        if (danmuPool[i]->name == uname) {
            danmuPool[i]->gift = gname;
            danmuPool[i]->refresh(count);
            //flowerRank[name] += inc;
            updateLater();
            return;
        }
        c++;
    }

    Danmu * d = new Danmu(this);
    d ->presentFlag = true;
    d->inc = count;
    d->name = uname;
    d->gift = gname;
    if (c==0) {
        nowFlowerChannel= 0;
    }
    else {
        nowFlowerChannel ++;
        if (nowFlowerChannel>=c_flowerChannel) nowFlowerChannel = 0;
    }

    d->channel = nowFlowerChannel;
    bool display;
    for (int x =0; x<c_flowerChannel; x++, d->channel++) {
        if (d->channel == c_flowerChannel) d->channel = 0;
        display = true;
        for (int i=0; i<danmuPool.size(); i++) {
            if (danmuPool[i]->channel == nowFlowerChannel) {
                if (danmuPool[i]->smaller(d)) {
                    danmuPool[i]->kill();
                    d->delayFlag = true;
                    display = true;
                }
                else {
                    display = false;
                }
                break;
            }
        }
        if (display) break;
    }
    if (!display) {
        delete d;
        return;
    }
    d->init();
    danmuPool.push_back(d);
    updateLater();
}


void DanmuWindow::addOneFlower(int hot, int inc, QString name)
{
  //  QMap<int, Danmu* > mIntDanmu;
    int count = 0;
    for (int i=0; i<danmuPool.size(); i++) {
        if (danmuPool[i]->hot == hot) return;
        if (danmuPool[i]->flowerFlag == false) continue;
        if (danmuPool[i]->name == name) {
            danmuPool[i]->refresh(inc);
            flowerRank[name] += inc;
            updateLater();
            return;
        }
        count++;
    }

    Danmu * d = new Danmu(this);
    d ->flowerFlag = true;
    d ->hot= hot;
    d->inc = inc;
    d->total = flowerRank[name];
    d->name = name;
    flowerRank[name] += inc;

    if (count ==0) {
        nowFlowerChannel= 0;
    }
    else {
        nowFlowerChannel ++;
        if (nowFlowerChannel>=c_flowerChannel) nowFlowerChannel = 0;
    }
    d->channel = nowFlowerChannel;
    for (int i=0; i<danmuPool.size(); i++) {
        if (danmuPool[i]->channel == nowFlowerChannel) {
            danmuPool[i]->kill();
            d->delayFlag = true;
            break;
        }
    }
    d->init();
    danmuPool.push_back(d);
    updateLater();
}
void DanmuWindow::resizeEvent(QResizeEvent *)
{
    totalChannel = height() / c_rectHeight;
    qDebug()<<" geometry "<<geometry()<<endl;
}
void DanmuWindow::updateLater() {
    static QTimer * t = NULL;

    if (t == NULL) {
        t = new QTimer;
        connect(t, SIGNAL(timeout()), this, SLOT(update()));
        t->start(30);
    }
    if (t->isActive()) return;
    else t->start(30);
}

void DanmuWindow::clearDead()
{
    QList<Danmu *> old = danmuPool;
    danmuPool.clear();
    for (int i=0; i<old.size(); i++) {
        if (old[i]->dead()) continue;
        danmuPool.push_back(old[i]);
    }
}

void DanmuWindow::paintEvent(QPaintEvent *)
{
    clearDead();

    paintingTime = QDateTime::currentMSecsSinceEpoch();;

    QPainter painter(this);

    painter.setPen(c_fontColor);
    painter.setBrush(c_backRectColor);
    painter.setFont(QFont("Arial", c_fontSize));

//    painter.drawText(rect(), Qt::AlignCenter, test);

    if (danmuPool.size()==0) return;

    for (int i=0; i<danmuPool.size(); i++) {
        danmuPool[i]->paint(painter);
    }
   // qDebug()<<danmuPool.size();
    updateLater();
}
