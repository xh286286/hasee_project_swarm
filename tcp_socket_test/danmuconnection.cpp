#include "danmuconnection.h"
#include "../share_library/httpfiledownloader.h"
#include "../share_library/zhanqiutil.h"
#include <QJsonDocument>
#include <QDebug>
#include <QTimer>
#include <cassert>
DanmuConnection::DanmuConnection(QObject *parent) :
    QObject(parent)
{
    hfd = NULL;
    ts = NULL;
    QObject::connect(&connectionTimeout, &QTimer::timeout,  this, &DanmuConnection::work1);
    keepaliveTimer.start(30000);
    QObject::connect(&keepaliveTimer, &QTimer::timeout,  this, &DanmuConnection::keepAlive);
    state = 0;
    debugFlag = false;
}


void DanmuConnection::login(QString name, QString pass, QString uid, QString id) {
    username  = name ;
    password = pass;
    this->uid = uid;
    this->id = id;

    work1();

}

void DanmuConnection::work1() {
        state = 1;
        if (hfd!=NULL) {
            hfd->disconnect();
            hfd->deleteLater();
            hfd = NULL;
        }
        if (ts!=NULL) {
            ts->disconnect();
            ts->deleteLater();
            ts = NULL;
        }
    hfd = new HttpFileDownloader();
    QUrl u("http://www.zhanqi.tv/api/auth/user.login");
    QByteArray ba= "account=" ;
    ba.append(username.toUtf8().toPercentEncoding());
    ba.append( "&password=");
    ba.append(password.toUtf8().toPercentEncoding());
    //ba = s.toUtf8().toPercentEncoding();
    if (debugFlag) qDebug()<<ba;
    QObject::connect(hfd, &HttpFileDownloader::downloadFinished, this, &DanmuConnection::getHttpInfo);
    QObject::connect(hfd, &HttpFileDownloader::downloadAbort, this, &DanmuConnection::dealError);

    hfd->postFromURL(u,ba, 10000);
}
void DanmuConnection::work2() {
        state = 2;
        if (debugFlag) qDebug()<<"state "<<state;
    QUrl u("http://www.zhanqi.tv/api/public/room.viewer");
    QByteArray ba= "uid=" ;
    ba.append(uid.toUtf8().toPercentEncoding());

    //ba = s.toUtf8().toPercentEncoding();
    if (debugFlag) qDebug()<<ba;

    hfd->postFromURL(u,ba, 10000);
}
void DanmuConnection::work3() {
    state = 3;
    if (debugFlag) qDebug()<<"state "<<state;
    if (ts!=NULL) {
        ts->disconnect();
        ts->deleteLater();
    }
    ts = new QTcpSocket;
    QObject::connect(ts, &QTcpSocket::connected, this, &DanmuConnection::getSocketInfo);
    QObject::connect(ts, &QTcpSocket::readyRead, this, &DanmuConnection::getSocketInfo);
    QObject::connect(ts, SIGNAL( error(QAbstractSocket::SocketError )), this, SLOT(socketError(QAbstractSocket::SocketError )));
    ts->connectToHost("112.124.114.222",15010);

    //182.92.104.225
    if (debugFlag) qDebug()<<"connect";
}
void DanmuConnection::work4() {
    state = 4;
    if (debugFlag) qDebug()<<"state "<<state;
    QJsonObject jo;
    jo["cmdid"] = "svrisokreq";
    writeSocketData(1, jo);

    jo["cmdid"] = "loginreq";
    jo["roomid"] = id.toInt();
    jo["nickname"] = loginJo["data"].toObject()["nickname"];
    jo["t"] = 0;
    jo["fhost"] = "";
    jo["gid"]  = roomviewerJo["data"].toObject()["gid"];
    jo["timestamp"]  = roomviewerJo["data"].toObject()["timestamp"];
    jo["uid"] = loginJo["data"].toObject()["uid"];
    jo["sid"]  = roomviewerJo["data"].toObject()["sid"];
    jo["r"] = 1;
    writeSocketData(2, jo);
}
bool DanmuConnection::postDanmu(QString s) {
    if (debugFlag) qDebug()<<state;
    if (state != 5) return false;
//    QJsonObject({"cmdid":"chatmessage","content":"测试2","level":15,"showmedal":1,"s
//    peakinroom":1,"style":null,"toid":0,"usexuanzi":0})

    QString m = ZhanQiUtil::encodeString( s);
    while( historyMessage.contains(m)) {
        m+='_';
    }
    historyMessage.insert(m);
    QJsonObject jo;
    jo["cmdid"] = "chatmessage";
    jo["content"] = m;
    jo["level"] = 15;
    jo["showmedal"] = 1;
    jo["speakinroom"] = 0;
    jo["toid"] = 0;
    jo["style"] = QJsonValue::Null;
    jo["usexuanzi"] = 0;
   if (debugFlag)  qDebug()<< "try";

    //QString s1 = "{\"level\":0,\"cmdid\":\"chatmessage\",\"style\":null,\"showmedal\":1,\"toid\":0,\"speakinroom\":0,\"content\":\"test312\",\"usexuanzi\":0}";
   // writeSocketString(2, s1);
    writeSocketData(2,jo);
    return true;
}

void DanmuConnection::work5() {


}
void DanmuConnection::work6() {

}
void DanmuConnection::getSocketInfo() {
    if (state == 3) {

        work4();
        return;
    }
    connectionTimeout.start(40000);
    state = 5;
    QByteArray ba = ts->readAll();
    //qDebug()<<ba;
    cache.append(ba);
    int count = 0;
   // if (debugFlag) qDebug()<<"getSocketInfo";
    while(true) {
        if (cache.length()<=12) return;
        //assert((unsigned )( cache[0] ) ==  11*16+11 );
        int a = unsigned  (cache[7]);
        if (a<0) a+= 256;
        int b = unsigned  (cache[6]);
        if (b<0) b+= 256;
        int length =  a * 256 + b;
        //if (debugFlag) qDebug()<<length;
        //if (unsigned(cache[7])!=0) qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!";
        if (cache.length() < length + 12) return;

        QJsonDocument jd = QJsonDocument::fromJson(cache.mid(12, length));
        auto jo = jd.object();
        if (jo["cmdid"].toString() != "getuc") {
            infoPool.push_back(jo);
            count++;
            if (debugFlag) qDebug()<<cache.length()<<' '<<jo;
        }

        cache = cache.mid(12+length);
        //if (debugFlag) qDebug()<<12+length<< ' '<< cache.length();
    //    qDebug()<<cache.toPercentEncoding();

    }
    if (count>0) emit messReady();
    return;
}
void DanmuConnection::socketError(QAbstractSocket::SocketError se ){
    if (debugFlag) qDebug()<<"socket error restart " <<se;
    work1();
}

void DanmuConnection::getHttpInfo() {
    if (debugFlag) qDebug()<<"get http info";
    if (debugFlag) qDebug()<<hfd->getPage();
    if (state == 1) {
        loginJo =  QJsonDocument::fromJson(hfd->getPage()) .object();
        work2();
    }
    else if (state ==2) {
        roomviewerJo = QJsonDocument::fromJson(hfd->getPage()) .object();

        work3();
    }
}
void DanmuConnection::dealError() {
    state = 0;
    if (debugFlag) qDebug()<<hfd->getLastErrorMessage();
    work1();
}
int get16(const char * s) {
    int a,b;
    if (s[0] <='9') {
        a = s[0] - '0';
    }
    else {
        a = s[0]-'A'+ 10;
    }
    if (s[1] <='9') {
        b = s[1] - '0';
    }
    else {
        b = s[1]-'A'+ 10;
    }
    return a*16 + b;
}

void DanmuConnection::keepAlive() {
    //if (debugFlag) qDebug()<<state<<' ' <<" keepalive";
    if (state != 5) return  ;
    writeSocketData(3,QJsonObject());
}

void DanmuConnection::writeSocketString(int type, QString dd) {
    QByteArray ds;
    ds =  dd.toUtf8();

    char s[10000] =  "            ";

    for (int i=0; i<12; i++) s[i] = 0;

    s[0] = get16("BB");
    s[1] = get16("CC");
    s[6] = ds.length() % 256 ;
    s[7] = ds.length() / 256;
    if (type ==1) {
        s[10] = get16("E8");
        s[11] = get16("03");
    }
    else if (type ==2){
        s[10] = get16("10");
        s[11] = get16("27");
    }
    else {
        assert(false);
    }
    for (int i=0; i<ds.size(); i++) {
        s[12+i] = ds[i];
    }
    if (debugFlag) qDebug()<<ds;
    if (debugFlag) qDebug()<<ds.size();
    ts->write(s, ds.size()+12);
}

void DanmuConnection::writeSocketData(int type, QJsonObject jo)
{
    QByteArray ds;
    ds = QJsonDocument(jo).toJson(QJsonDocument::Compact);

    char s[10000] =  "            ";

    for (int i=0; i<12; i++) s[i] = 0;

    s[0] = get16("BB");
    s[1] = get16("CC");
    s[6] = ds.length() % 256 ;
    s[7] = ds.length() / 256;
    if (type ==1) {
        s[10] = get16("E8");
        s[11] = get16("03");
    }
    else if (type == 2) {
        s[10] = get16("10");
        s[11] = get16("27");
    }
    else if (type == 3) {
        s[10] = get16("59");
        s[11] = get16("27");
        ds.clear();
    }
    else {
        assert(false);
    }
    for (int i=0; i<ds.size(); i++) {
        s[12+i] = ds[i];
    }
    if (debugFlag) {
        if (type!=3) qDebug()<<ds<<' '<<ds.size();
    }
    ts->write(s, ds.size()+12);

}
