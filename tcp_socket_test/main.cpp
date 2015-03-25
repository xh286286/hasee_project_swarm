#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QFile>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <algorithm>
#include <iostream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include "../tcp_socket_test/danmuconnection.h"
#include "../share_library/Util.h"
#include "../share_library/httpfiledownloader.h"
using std::cin;
using namespace std;


int work3();

void workget();

int gift();

int main(int  , char * [])
{

    work3();
    return 1;

}

int gift() {
    int x = 0;
    QApplication a(x,0);

    DanmuConnection::getDanmuSeverListFromNet();
//359887
//1396","uid":"359887
    DanmuConnection dc,dc1,dc2 ;
    dc.debugFlag = true;

    while(! dc.viewRoom("mayu"));
    dc.login("aabbdaabbd","aabbcaabbc");

    while(! dc1.viewRoom("mayu"));
    dc1.login("aabbeaabbe","aabbdaabbd");

    while(! dc2.viewRoom("mayu"));
    dc2.login("aabbiaabbi","aabbhaabbh");




    QTime t;
    t.start();
    while(t.elapsed() <30000) {

        QCoreApplication::processEvents();
    }

    for (int i=0; i<20; i++) {
        QTime t;
        t.start();
        while(t.elapsed()<300) {
            QCoreApplication::processEvents();
        }
        dc.sendGift(3);
        //dc.postDanmu("test " + QString::number(i+1));
        t.start();
        while(t.elapsed()<300) {
            QCoreApplication::processEvents();
        }
        dc1.sendGift(3);
        //dc1.postDanmu("test " + QString::number(i+1));
        t.start();
        while(t.elapsed()<300) {
            QCoreApplication::processEvents();
        }
        dc2.sendGift(3);
        //dc2.postDanmu("test " + QString::number(i+1));

    }
    return a.exec();

}

int work3() {
    int x = 0;
    QApplication a(x,0);

    DanmuConnection::getDanmuSeverListFromNet();

//1396","uid":"359887
    DanmuConnection dc,dc1,dc2,dc3,dc4,dc5;
    dc1.debugFlag = true;

    //QString room = "scorpio";
    QString room = "mayu";


    while(! dc.viewRoom(room));
    //dc.login("aabbdaabbd","aabbcaabbc");

   // dc.login("","");

   while(! dc1.viewRoom(room));
    dc1.login("aabbiaabbi","aabbhaabbh");

//    while(! dc2.viewRoom(room));
//    dc2.login("aabbiaabbi","aabbhaabbh");

//    while(! dc3.viewRoom(room));
//    dc3.login("aabbfaabbf","aabbeaabbe");

//    while(! dc4.viewRoom(room));
//    dc4.login("aabbgaabbg","aabbfaabbf");

//    while(! dc5.viewRoom(room));
//    dc5.login("aabbhaabbh","aabbgaabbg");



    QTimer  workcycle;

    workcycle.start(5000000);
    QString sss = myTr("这是我的部分ID");
    int count = 0;
    QObject::connect(&workcycle, &QTimer::timeout, [&](){
        return;
        count++;
        switch(count) {
        case 1:
            dc.postDanmu(sss);
            break;
        case 2:
            dc1.postDanmu(sss);
            break;
        case 3:
            dc2.postDanmu(sss);
            break;
        case 4:
            dc4.postDanmu(sss);
            break;
        case 5:
            dc5.postDanmu(sss);
            break;
        }


//        dc1.postDanmu(sss);
//        dc2.postDanmu(sss);
//      //  dc3.postDanmu(sss);
//        dc4.postDanmu(sss);
//        dc5.postDanmu(sss);
    });

//    while(true) {
//    QTime t;
//    t.start();
//    int count = 0;
//     x = 1;
//    while(true) {

//        if (t.elapsed() >30000)  {
//            dc.sendRoses();
//            dc1.sendRoses();
//            dc2.sendRoses();
//            dc3.sendRoses();
//            dc4.sendRoses();
//            dc5.sendRoses();
////            for (int i=0; i<0; i++) {
////                dc.sendGift(1);
////                dc.sendGift(3);
////                dc.sendGift(4);
////            }


//            break;
//           t.restart();
//        }
//        QCoreApplication::processEvents();
//    }
//    }
    return a.exec();

}
void workget() {
    int x = 0;
    QApplication a(x,0);

    QString base ="http://www.zhanqi.tv/api/static/game.lives/66/30-";
    QString end = ".json";

    QMap < QString , QJsonObject >  mapintjo;
    QJsonDocument jd;
    bool ok = loadJsonFile("lolmasterdata.json",jd);
    QJsonArray ja;
    ja = jd.array();

    QFile file("outdata.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))             return ;

    QTextStream out(&file);


    for (int i=0; i<ja.size(); i++) {
        QJsonObject jo;
        jo = ja[i].toObject();
        mapintjo[jo["uid"].toString()] = jo;
        out<<jo["gameusername"].toString()<< "   " <<jo["nickname"].toString()<< "   "<<jo["url"].toString()<<endl;
    }
    file.close();

    while(true) {
        for (int x = 1; x< 145; x++) {
            QString s = QString::number(x);
            QString u = base+s+end;
            qDebug()<<u;
            HttpFileDownloader hfd;
            bool ok = hfd.sycGetPageFromURL(QUrl(u));
            qDebug()<<ok;
            if (!ok) continue;
            QString re = hfd.getPage();
            QJsonDocument jd = QJsonDocument::fromJson(re.toUtf8());
            qDebug()<<jd.object()["data"].toObject()["rooms"].toArray().size();
            QJsonArray ja = jd.object()["data"].toObject()["rooms"].toArray();

            for (int i=0; i< ja.size(); i++) {
                QJsonObject jo = ja[i].toObject() ;
                QString nickname = jo["nickname"].toString();
                QString url = jo["url"].toString();
                QString id,uid;
                id = jo["id"].toString();
                uid = jo["uid"].toString();
                //qDebug()<<uid;
                if (mapintjo.contains(uid)  &&  mapintjo[uid]["gameusername"].toString()!="") continue;
                QString bb = "http://www.zhanqi.tv";
                HttpFileDownloader hfd;
                bool ok = hfd.sycGetPageFromURL(QUrl(bb+url));
                //qDebug()<<ok;
                if (!ok) continue;
                QString re = hfd.getPage();

                int x = re.indexOf("getLols5Nickname");
                int a = re.indexOf(myTr("战旗TV丶"),x);
                int b = re.indexOf("</span>",x);
                if (x<0 ) continue;
                if (b<0) {
                    qDebug()<<nickname;
                    continue;
                }
                QString gameusername;
                if (a<0) {
                    gameusername = re.mid(x+22, b-x-22);

                }
                else {
                     gameusername = re.mid(a,b-a);
                }
                qDebug()<<gameusername;

                QJsonObject so;
                so["nickname"] = nickname;
                so["url"] = url;
                so["id"] = id;
                so["uid"] = uid;
                so["gameusername"] = gameusername;
                mapintjo[uid] = so;
            }



            {
                QJsonArray ja;
                for (auto a = mapintjo.begin(); a!= mapintjo.end(); a++) {
                    ja.push_back(a.value());
                }
                bool ok = saveJsonFile("lolmasterdata.json", QJsonDocument(ja));
                qDebug()<<"save file !! " <<ok;
            }
        }

    }
    a.exec();
}
