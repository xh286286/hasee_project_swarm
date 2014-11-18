
/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "player.h"

#include "playercontrols.h"
#include "playlistmodel.h"
#include "histogramwidget.h"
#include "httpfiledownloader.h"
#include "Util.h"
#include "userbank.h"
#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QStatusBar>
#include <QTextCodec>
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;
QString Player::getMp3UrlFromBaidu(QString query)
{
    QString qh = "http://tingapi.ting.baidu.com/v1/restserver/ting?from=webapp_music&method=baidu.ting.search.catalogSug&format=json&callback=&query=";

    QByteArray ba = qh.toUtf8();
    ba.append(query.toUtf8().toPercentEncoding());
    QUrl u ( ba );
  //  qDebug()<<ba;
    bool ok  = downloader->sycGetPageFromURL(u, 5000);
    if (!ok) return "";
    //qDebug()<<downloader->getPage();

    QJsonObject a = QJsonDocument::fromJson(downloader->getPage()).object();


    QJsonArray b = a["song"].toArray();
    if (b.isEmpty()) return "";
    QString songids =b[0].toObject()["songid"].toString();

    if (songids=="") return "";
    QString idu = "http://ting.baidu.com/data/music/links?songIds=" + songids;

    QUrl u1(idu);
    ok  = downloader->sycGetPageFromURL(u1, 5000);
    if (!ok) return "";
    //qDebug()<<downloader->getPage();
    a = QJsonDocument::fromJson(downloader->getPage()).object();

    QJsonObject oo  = a["data"].toObject()["songList"].toArray()[0].toObject();
    QString re = oo["songLink"].toString();

    netTitle= oo["songName"].toString();
    netAuthor = oo["artistName"].toString();
    //qDebug()<<re;
    if (re=="") {
        qDebug()<<a;
    }

    return re;
}
QString Player::getMp3UrlFromXiami(QString query)
{

    QString qh = "http://www.xiami.com/web/search-songs?key=";

    QByteArray ba = qh.toUtf8();
    ba.append(query.toUtf8().toPercentEncoding());
    QUrl u ( ba );
  //  qDebug()<<ba;
    bool ok  = downloader->sycGetPageFromURL(u, 5000);
    if (!ok) return "";
    //qDebug()<<downloader->getPage();

    QJsonArray a = QJsonDocument::fromJson(downloader->getPage()).array();
    if (a.size()==0) return "";
    QJsonObject song = a[0].toObject();
    QString src = song["src"].toString();
    netAuthor = song["author"].toString();
    netTitle= song["title"].toString();
    if (src=="") qDebug()<<a;
    return src;
}
void Player::playNextSong()
{

    static bool running = false;
    if (running) return;
    running = true;
    statusBar->clearMessage();
    bool refreshOrderListFlag = false;
    if (acceptOrderButton->isChecked())
    while(danmuHistory.size()>0) {
        QString s =danmuHistory.back();
        refreshOrderListFlag = true;
        danmuHistory.pop_back();
        QJsonObject a = QJsonDocument::fromJson(s.toUtf8()).object();


        QString cm = a["content"].toString();
        //&quot;&apos;&amp;
        cm.replace("&quot;","\"");
        cm.replace("&apos;","'");
        cm.replace("&amp;","&");
        cm.replace("_"," ");
        cm.replace("+"," ");
        cm.replace(","," ");
        cm.replace("-"," ");
        QString f = a["fromname"].toString();
        if (f=="") continue;
        addHintInfo(myTr("尝试点歌 ") + f +" " + cm);

        QString us;
        QUrl url;



        us = getMp3UrlFromBaidu(cm.mid(2));
        url= QUrl(us);

        if (url.isValid()) {
            orderInfo = f+" "+cm;
            statusBar->showMessage(f+" "+cm);
            addHintInfo(myTr("百度音乐查找成功"));
            addHintInfo(netAuthor + " | " + netTitle);
            playlist1->insertMedia(0,url);

            playlist1->setCurrentIndex(0);
            player->setPlaylist(playlist1);
            player->play();
            player->metaDataChanged();
            refreshOrderList();

            running = false;
            return;
        }
        else {
            addHintInfo(myTr("百度音乐查找失败"));

        }

        us = getMp3UrlFromXiami(cm.mid(2));
        url= QUrl(us);


        if (url.isValid()) {
            orderInfo = f+" "+cm;
            statusBar->showMessage(f+" "+cm);
            addHintInfo(myTr("虾米音乐查找成功"));
            addHintInfo(netAuthor + " | " + netTitle);
            playlist1->insertMedia(0,url);

            playlist1->setCurrentIndex(0);
            player->setPlaylist(playlist1);
            player->play();

            player->metaDataChanged();
            refreshOrderList();
            running = false;

            return;
        }
        else {
            addHintInfo(myTr("虾米音乐查找失败"));

        }

    }
    if (refreshOrderListFlag) refreshOrderList();
    orderInfo = "";
    netAuthor = "";
    netTitle = "";
    int n = playlist2->mediaCount();
    if (n!=0) {
        addHintInfo(myTr("下一首播放本地歌曲"));
    }
    if (n==0) n = 10;
    int last = playlist2->currentIndex();
    int z;
    for (int i=0; i<10; i++) {
        z = int(rand()) % n;
        if (z!=last) break;
    }


    //qDebug()<<n<<' '<<z<<' '<<last;
    player->setPlaylist(playlist2);

    playlist2->setCurrentIndex(z);
    player->play();

    running = false;
}

void Player::getDanmu(QString s)
{
    danmuConnectionTimer.start(40000);
    QJsonObject a = QJsonDocument::fromJson(s.toUtf8()).object();

    QString host = myTr("御园麻由mayu");
    QString author = myTr("天蝎10000");
    QString cmdid = a["cmdid"].toString();
    if ( cmdid == "chatmessage" ) {
        QString f = a["fromname"].toString();
        if (f=="") return;

        int level = a["level"].toInt();
        if (level ==0 && f!=host && f!=author) return;

        QString cm = a["content"].toString();

        if (cm.indexOf(myTr("切歌"))==0) {
            if (userSwitchButton->isChecked() == false ) return;
            if (UserBank::leftMoney(f) >=5 || f==host ) {
                UserBank::withdrawMoney(f,5);
                statusBar->showMessage(f+myTr(" 切歌"),1000);
                addHintInfo(f+ myTr(" 切歌"));
                playNextSong();

            }
            return;
        }

        if (cm.indexOf(myTr("点歌"))!=0 && cm.indexOf(myTr("选歌"))!=0) {}
        else {
            addHintInfo(myTr("加入点歌队列 ") + f + " " + cm);
            danmuHistory.push_back(s);
            if (player->state() == QMediaPlayer::StoppedState ) {

                playNextSong();
            }
            else {
                refreshOrderList();
            }
        }

        if (cm.indexOf(myTr("抢歌"))!=0) return;
        addHintInfo(myTr("抢歌 ") + f + " " + cm);
        if (UserBank::leftMoney(f) >=10 || f==host ) {
            danmuHistory.push_back(s);
            UserBank::withdrawMoney(f,10);
            playNextSong();
        }
    }
    else if ( cmdid == "Gift.Use" ) {
        QJsonObject data = a["data"].toObject();
        if (data["url"].toString()!= "/mayu") return;
        int count = data["count"].toInt();
        int v = UserBank::evaluateGift(data["name"].toString());
        UserBank::saveMoney(data["nickname"].toString(), v*count);
    }

}

bool Player::loadMusicListFileNames()
{
    QJsonArray a;
    QJsonDocument d;
    bool ok = loadJsonFile("musicFileNames.json", d);
    a = d.array();

    if (!ok) {
        qDebug()<< " load music list file failed!!";
        return false;
    }
    else {
         qDebug()<< " load music list file succeed!!";
    }
    for (int i=0; i<a.size(); i++) {
        musicFileNames.push_back(a[i].toString());
    }
    addToPlaylist(musicFileNames);
    return true;
}

bool Player::saveMusicListFileNames()
{
    QJsonArray a;

    for (int i=0; i<musicFileNames.size(); i++) {
        a.push_back(musicFileNames[i]);
    }
    bool ok = saveJsonFile("musicFileNames.json", QJsonDocument(a));
    if (!ok) {
        qDebug()<< " save music list file failed!!";
        return false;
    }
    else {
         qDebug()<< " save music list file succeed!!";
         return true;
    }
}
void Player::setPlayInfo()
{
    QString s;
    if (netTitle=="") s = "playing : " + trackInfo  ;
    else s = "playing : " + netAuthor + " | " + netTitle ;
    s+=  "\n" + orderInfo;
    //qDebug()<<s;
    //static QTime t;
    //if (t.elapsed()<1000)
    static QFile aa("hint/playInfo.txt");
    aa.open(QFile::WriteOnly | QIODevice::Text);
    aa.write(s.toUtf8());
    aa.close();
}
void Player::refreshOrderList(){
    static QFile aa("hint/orderList.txt");
    aa.open(QFile::WriteOnly | QIODevice::Text);
    QString s;
    for (int i=0; i<danmuHistory.size(); i++) {
        QJsonObject a = QJsonDocument::fromJson(danmuHistory[i].toUtf8()).object();
        QString cm = a["content"].toString();
        QString name = a["fromname"].toString();
        s =name +" "+ cm + "\n" + s;
    }
    aa.write(s.toUtf8());
    aa.close();
}

void Player::addHintInfo(QString s)
{
//    qDebug()<<s;
    QByteArray ba;

    static QTextCodec *codec1 = QTextCodec::codecForName("GBK");
    static QTextCodec *codec2 = QTextCodec::codecForName("GB18030");
    static QTextCodec *codec3 = QTextCodec::codecForName("GB2312");
    if (codec1) {
        ba   = codec1->fromUnicode(s);
        cout<<ba.data()<<endl;
    }
    if (codec2) {
        ba   = codec2->fromUnicode(s);
        cout<<ba.data()<<endl;
    }
    if (codec3) {
        ba   = codec2->fromUnicode(s);
        cout<<ba.data()<<endl;
    }
    s+= "\n";


    static QStringList sl;
    sl.push_back(s);
    static QFile aa("hint/console.txt");

    static bool firstTimeFlag = true;
    if (firstTimeFlag) {
        firstTimeFlag = false;
        aa.open(QFile::WriteOnly | QIODevice::Text);
    }
    else {
        aa.open(QFile::Append | QIODevice::Text);
    }

    aa.write(s.toUtf8());
    aa.close();

}
