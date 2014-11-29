#include "autothankgift.h"
#include "../share_library/Util.h"
#include "../share_library/zhanqiutil.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QTime>
#include <QJsonArray>
#include <cstdlib>
#include <ctime>
#include <QList>
#include <QDebug>
#include <cassert>
AutoThankGift::AutoThankGift(QObject *parent) :
    QObject(parent)
{
    eventTimer.start( 3000);
    QObject::connect(&eventTimer, &QTimer::timeout, this, &AutoThankGift::work);
    QJsonDocument jd;
    loadJsonFile("thank.json",jd);
    QJsonArray ja = jd.array();
    for (int i=1; i<ja.size(); i++) {
        thankWord.push_back( ja[i].toString());
    }
    qDebug()<<"!!!!!!!!!!!"<<ja;
}

//raw string   "{"cmdid":"Gift.Use","data":{"action":"","anchorname":"DD  ","classifier":"","count":38,"curexp":"38","desc":"我顶！~","gameId":"0","gid":1836156999,"icon":"http://dlpic.cdn.zhanqi.tv/uploads/2014/09/gifticon-2014091708055010360.png","id":"3","image":"http://dlpic.cdn.zhanqi.tv/uploads/2014/09/giftspic-2014091708055050897.gif","leftexp":42,"level":2,"name":"顶","nextexp":80,"nickname":"围观大神","object":566711,"parser":"common","pid":3,"rank":"primary","roomid":737,"show":"1","status":"1","tid":"1","uid":142456}}


void AutoThankGift::dealPresent(QJsonObject jo) {
    assert( jo["cmdid"].toString() == "Gift.Use");
    QJsonObject data = jo["data"].toObject();
    QString nickname = data["nickname"].toString();
    QString gift = data["name"].toString();
    int count = data["count"].toInt();

    giftHistory[nickname][gift]+= count;

    lastGiftTime[nickname] = QTime::currentTime().msecsSinceStartOfDay();

}

void AutoThankGift::work() {
    int time = QTime::currentTime().msecsSinceStartOfDay();
    QString thankName ;

    const int Thank_time = 5000;

    for (auto i = giftHistory.begin(); i!=giftHistory.end(); i++) {
        QString name = i.key();
        if (time - lastGiftTime[name] > Thank_time)  {
            thankName = name;
            break;
        }
    }
    if (thankName =="") return;

    QMap< QString , int>  gift = giftHistory[thankName];
    giftHistory.remove(thankName);


    int n = getRandomInt()% thankWord.size() ;

    QString word;
    if (thankWord .size() == 0) {
        //word = myTr("thanks_for_the_gift");
        word = myTr("谢谢[name]送的[gift]。");
    }
    else {
           word = thankWord[n];
    }

    QString g;

    for (auto i= gift.begin(); i!=gift.end(); i++) {
        g+= QString::number(i.value());
        g+= myTr("个");
        g+= i.key();
    }
    word.replace("[name]", thankName);
    word.replace("[gift]", g);
    word = ZhanQiUtil::encodeString(word);
    emit postThankMessage(word);
}
