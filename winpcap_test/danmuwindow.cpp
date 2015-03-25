#include <cassert>
#undef min
#include <QDateTime>
#include <QPainter>
#include <QTimer>
#include <QMap>
#include <QSet>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDir>
#include <QTimer>
#include "myplaintextedit.h"
#include <QTextCursor>
#include "../share_library/Util.h"
#include "../share_library/zhanqiutil.h"
#include "danmuwindow.h"
#include "workerthread.h"
#include "danmu.h"



DanmuWindow * DanmuWindow::instance =  NULL;

DanmuWindow::DanmuWindow(QWidget *parent) :
    QWidget(parent)
{
    assert (instance == NULL);
    instance = this;

    historyTextEdit = new MyPlainTextEdit();
    historyTextEdit->hide();
    historyTextEdit->setWindowTitle(myTr("弹幕历史"));
    flags = Qt::ToolTip;
    flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTransparentForInput;
    QWidget::setWindowFlags(flags);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowOpacity(0.7);

    QTimer * timer=new QTimer;
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(getMessage()));
    connect(timer,SIGNAL(timeout()),this,SLOT(getMessage2()));
    timer->start(100);

    test ="console text";

    ifDetectingFlag = true;

    c_fontColor = QColor(Qt::blue);
    c_fontSize = 10;
    c_rectHeight = 30;
    c_flowerChannel = 6;

    c_moveChangeFlag = true;
    c_alphaChangeFlag = true;

    c_inTime = 300;
    c_lastTime = 12000;
    c_flowerLastTime = 1000;
    c_outTime = 300;

    c_fontColor = QColor(Qt::blue);
    c_backRectColor = QColor(Qt::white);


    QString uid,id,user,password;

    uid = getGlobalParameterString("room_uid","113289");
    id = getGlobalParameterString("room_id","317");
    user = getGlobalParameterString("secretary_user","aabbfaabbf");
    password = getGlobalParameterString("secretary_password","aabbeaabbe");


//    uid = "100007714";
//    id = "1378";
//    user = "";
//    password = "";


    danmuConnectionPool.push_back( new DanmuConnection);
    danmuConnectionPool[0]->login(user,password, uid,id);

    // reboot 10min
    QTimer * timer1 = new QTimer;
    connect(timer1, &QTimer::timeout, danmuConnectionPool[0], &DanmuConnection::reboot);
    timer1->start(600000);


    danmuConnectionPool.push_back( new DanmuConnection);
    danmuConnectionPool[1]->login("","", uid,id);

    danmuConnectionPool.push_back( new DanmuConnection);
    danmuConnectionPool[2]->login("","", uid,id);

    danmuConnectionPool[0]->debugFlag = true;

//    for (int i=0; i<8; i++) {
//        int t  = i;
//        QString f="tianxie1";
//        for (int j = 0; j<3; j++) {
//            int x = t%2;
//            t/=2;
//            if (x==1) {
//                f+='0';
//            }
//            else {
//                f+='O';
//            }

//        }
//        qDebug()<<f;
//        danmuConnectionPool.push_back( new DanmuConnection);
//        danmuConnectionPool.back()->login(f,f, uid,id);
//    }
//    for (int i=0; i<16; i++) {
//        int t  = i;
//        QString f="tianxie1";
//        for (int j = 0; j<4; j++) {
//            int x = t%2;
//            t/=2;
//            if (x==1) {
//                f+='0';
//            }
//            else {
//                f+='O';
//            }

//        }
//        qDebug()<<f;
//        danmuConnectionPool.push_back( new DanmuConnection);
//        danmuConnectionPool.back()->login(f,f, uid,id);
//    }

    autohideandshowTimer.start(10000);
    QObject::connect(&autohideandshowTimer, &QTimer::timeout, this, &DanmuWindow::hideandshow);
}
DanmuWindow:: ~DanmuWindow() {
    delete historyTextEdit;
}

bool DanmuWindow::checkDuplicated(QString a) {

    if ( historySet.contains(a)) {

        return false;
    }
    else {
        historySet.insert(a);
        historyList.push_back(a);

        if (historyList.size()>1000) {
            historySet.remove(historyList.front());
            historyList.pop_front();
        }

        return true;
    }

}
void DanmuWindow::hideandshow() {
    if (this->isVisible()) {
    hide();
    show();
    }
}
void DanmuWindow::postDanmuMessge(QString s) {
    danmuConnectionPool[0]->postDanmu(s);
}

void DanmuWindow::dealOneMessage(QJsonObject a, QString sss)
{

    //qDebug()<<a;
    QString cmdid = a["cmdid"].toString();

    if (cmdid == "chatmessage") {
        if (danmuPool.size()==0){
            hideandshow();
        }
        //qDebug()<<a;
        if (!checkDuplicated(a["chatid"].toString() + "chatid" )) return;
        QString s = a["content"].toString();
        //&quot;&apos;&amp;
        s.replace("&quot;","\"");
        s.replace("&apos;","'");
        s.replace("&amp;","&");
        test = s;
        QString f = a["fromname"].toString();
        if (f=="") return;

        if (!checkDuplicated( s + " " + f)) return;

        auto black = ZhanQiUtil::getBlackMap(a);
        if (getRandomInt() % 100 < black["danmumiss"]) return;
        emit broadcastDanmu(sss);
        addOneMessage(a["chatid"].toString(),f,s);
        addDanmuHistory("["+f+"]:"+s);
    }
    else if (cmdid == "useprop") {
        test = test+"\nflower";
        addOneFlower(a["hot"].toInt(), a["count"].toInt(), a["fromname"].toString());
    }
    else if (cmdid == "getuc"){
        test = a["cmdid"].toString();
    }
    else if (cmdid == "Gift.Use") {
        //qDebug()<<a;
        QJsonObject b = a["data"].toObject();
        //qDebug()<<b["anchorname"].toString()<<endl;
        //qDebug()<<b["anchorname"].toString().toUtf8().toPercentEncoding()<<endl;
        //qDebug()<<myTr("御园麻由mayu").toUtf8().toPercentEncoding()<<endl;
        //"/alyssasearrs"   /huamituan
        if (b["url"].toString() !="/mayu") return;

        if (!checkDuplicated(b["curexp"].toString() + "curexp" )) return;
        emit broadcastDanmu(sss);
        QString fromname =  b["nickname"].toString();
        if (fromname=="") return;
        QString giftname = b["name"].toString();
        int count = b["count"].toInt();
        addOnePresent(b["gid"].toInt(), count, fromname ,giftname );
        emit informGift(a);
        addDanmuHistory("["+fromname+"] send "+ QString::number(count) + giftname);
    }
    else if (cmdid == "notefanslevel") {
        //86   QJsonObject({"cmdid":"notefanslevel","data":{"fanslevel":15,"fansname":"天蝎1000"}})
        //qDebug()<< "notefans";
        auto data = a["data"].toObject();
        QString name = data["fansname"].toString();
        if (!checkDuplicated(name + " fans" )) return;
        addOneDebugInfor(data["fansname"].toString() + myTr(" 进入了直播间"));
    }
    else if (cmdid == "rosebro") {
        //QJsonObject({"cmdid":"rosebro","cnt":6,"name":"天蝎10000","rank":47})
        //qDebug()<<a;
        QString name = a["name"].toString();

        static QMap < QString , qint64> lastSend;

        qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
        qint64 last = lastSend[name];

        lastSend[name] = now;
        qDebug()<<now<<' '<<last;
        if (now - last < 20*60*1000) return;

        emit broadcastDanmu(sss);
        QString fromname =  name;
        if (fromname=="") return;
        QString giftname = myTr("玫瑰");
        int count = 6;
        addOnePresent(now, count, fromname ,giftname );
        emit informGift(a);
        addDanmuHistory("["+fromname+"] send "+ QString::number(count) + giftname);
    }
    else {
        //qDebug()<<a<<endl;
    }
}

void DanmuWindow::messageDecode(QString s)
{


    QJsonObject a = QJsonDocument::fromJson(s.toUtf8()).object();
    //qDebug()<<a;
    QString cmdid = a["cmdid"].toString();
    if (cmdid!="") {
        dealOneMessage(a,s);


    }
    else {
        //qDebug()<<s;
    }

}
void DanmuWindow::getMessage2() {
    auto a = danmuConnectionPool[0]->getALLMess();

    for (int i=0; i<a.size(); i++) {
        QJsonDocument jd(a[i]);

        dealOneMessage (a[i], jd.toJson(QJsonDocument::Compact));
    }


}


void DanmuWindow::getMessage()
{

    static int count = 0;
    count++;
    if (count == 19) {


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
            addOneDebugInfor(myTr("如有需要，请手动重启监测模块"));
            //WorkerThread::startDetecting();
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
        if (danmuPool[i]->chatid == chatid) {
            qDebug()<<"impossible";
            assert(false);
            return;
        }
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
            if (danmuPool[i]->gift == gname) {
                count+= danmuPool[i]->inc;
            }
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
    bool display = true;
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
        t->start(100);
    }
    if (t->isActive()) return;
    else t->start(100);
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
    //qDebug()<<"bbb1";
    clearDead();
    //qDebug()<<"eee1";
    paintingTime = QDateTime::currentMSecsSinceEpoch();;

    QPainter painter(this);

    painter.setPen(c_fontColor);
    painter.setBrush(c_backRectColor);
    painter.setFont(QFont("Arial", c_fontSize));

//    painter.drawText(rect(), Qt::AlignCenter, test);

    if (danmuPool.size()==0) return;
    //qDebug()<<"bbb2";
    for (int i=0; i<danmuPool.size(); i++) {
        danmuPool[i]->paint(painter);
    }
    //qDebug()<<"eee2";
   // qDebug()<<danmuPool.size();
    updateLater();
}


void DanmuWindow::addDanmuHistory(QString s) {
    readHistoryList.push_back(s);
}

void DanmuWindow::displayHistory() {
    QString s;
    for (int i=0; i<readHistoryList.size(); i++) {
        s+= QString::number(i+1) + " " + readHistoryList[i] + "\n";
    }
    historyTextEdit->setPlainText(s);
    auto c = historyTextEdit->textCursor();
    c.movePosition( QTextCursor::End);
    historyTextEdit ->setTextCursor(c);
    historyTextEdit->show();
}
