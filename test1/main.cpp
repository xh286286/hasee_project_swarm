﻿

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QStyleFactory>
#include <QIcon>

#include <ctime>
#include <QDate>
#include <QDir>
#include "mydialog.h"
#include "messagecenter.h"
#include "player.h"
#include "httpfiledownloader.h"
#include "userbank.h"
#include "CHotKeyEventFilter.hpp"
#include "Util.h"

#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;


int main(int argc, char *argv[])
{
    srand(time(0));

#ifdef Q_WS_MAEMO_6
    //Meego graphics system conflicts with xvideo during fullscreen transition
    QApplication::setGraphicsSystem("raster");
#endif
    QApplication app(argc, argv);

//    QString s = myTr("水电费似懂非懂发");
//        while(true) {
//            qDebug()<<s.toUtf8().toPercentEncoding();
//        qDebug()<<s;
//        cout<<s.toUtf8().data()<<endl;
//        }


    QApplication::setStyle(QStyleFactory::create("Fusion"));
    qDebug()<<QStyleFactory::keys();
   // HttpFileDownloader hfd;
    //hfd.getFileFromURL(QUrl("http://yinyueshiting.baidu.com/data2/music/121523379/14385500158400128.mp3?xcode=c6e50f79d04aaac4b89e9d84f827b20882c86357ec80af0a&song_id=14385500"),"t.mp3");
    //hfd.getFileFromURL(QUrl("http://www.acfun.tv/a/ac1515425"),"t.txt");

    UserBank::load();
    {
        QDir d;
        d.mkdir("hint");
    }

    MessageCenter mc;
    CHotKeyEventFilter hk(mc.winId());
    app.installNativeEventFilter(&hk);



    if (mc.registerTitle("mayu player")) {
        if (mc.addOutput("mayu danmu") ) {

        }
        else {

            //QMessageBox::warning(0, "warning",  "Danmu assistant is not running!"   );
            qDebug()<<"Danmu assistant is not running!"  ;
           // return 0;
        }
    }
    else {
        QMessageBox::critical(0, "error",  "The media player was running!"   );
        return 0;
    }

   // mc.registerHotkey(MOD_CONTROL, VK_OEM_PLUS);


    Player player;
    QIcon ico(QString(":/ico/scorpio3.ico"));
    player.setWindowIcon(ico);
   // QObject::connect(& (mc), &MessageCenter::globalHotkey, [&](UINT, UINT){  player.playNextSong(); qDebug()<<"hotkey play next song";   });
    QObject::connect(&hk, &CHotKeyEventFilter::HotKeyTrigger,[&]( ){  player.playNextSong(); qDebug()<<"hotkey play next song";   } );

    player.danmuConnectionTimer.start(20000);

    QObject::connect(& (player.danmuConnectionTimer), &QTimer::timeout, [&](){
        mc.addOutput("mayu danmu");
        //qDebug()<<"reconnect danmu";
        player.danmuConnectionTimer.start(20000);
    });

    player.loadMusicListFileNames();

    QObject::connect(&mc, &MessageCenter::sendMessage, &player, &Player::getDanmu);

#if defined(Q_WS_SIMULATOR)
    player.setAttribute(Qt::WA_LockLandscapeOrientation);
    player.showMaximized();
#else
    player.show();
#endif
    QTimer t;
    {

        t.start(10000);
        QObject::connect(&t, &QTimer::timeout, [&]() {
             qDebug()<<"test";
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
            qDebug()<<"test";
            QString s = myTr("{\"chatid\":\"8526706110\",\"cmdid\":\"chatmessage\",\"content\":\"点歌祝你生日快乐\",\"fromid\":1709988274,\"fromname\":\"天蝎1000\",\"fromuid\":359887,\"intotallist\":0,\"inweeklist\":0,\"ip\":\"101.254.181.34\",\"level\":0,\"permission\":30,\"rank\":\"primary\",\"showmedal\":1,\"speakinroom\":1,\"style\":null,\"time\":\"20:49\",\"toid\":0,\"usexuanzi\":0}");
            player.danmuHistory.push_back(s);

            player.playNextSong();
            t.stop();
        });

    }



    int re = app.exec();

    UserBank::save();
    player.saveMusicListFileNames();

    mc.unregisterHotkey();

    return re;


    QApplication aaa(argc, argv);
/*
    MessageCenter a;

    bool b = a.registerTitle("aaaa");
    a.show();
    a.hide();
    qDebug()<<b;
    if (b) {
        while(true) {
            a.broadcast("asdfadsf");
            aaa.processEvents();
        }

    }
    else {

        b = a.registerTitle("bbbb");
        a.show();
        a.hide();
        a.addOutput("aaaa");

    }
    */

    return aaa.exec();
}
