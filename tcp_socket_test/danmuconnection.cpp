#include "danmuconnection.h"
#include "httpfiledownloader.h"

#include <QJsonDocument>
#include <QDebug>

#include <cassert>
DanmuConnection::DanmuConnection(QObject *parent) :
    QObject(parent)
{
    hfd = NULL;
    ts = NULL;
}


void DanmuConnection::login(QString name, QString pass, QString uid, QString id) {
    username  = name ;
    password = pass;
    this->uid = uid;
    this->id = id;
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
    work1();

}

void DanmuConnection::work1() {
        state = 1;
    if (hfd!=NULL) {
        hfd->disconnect();
        hfd->deleteLater();
    }
    hfd = new HttpFileDownloader();
    QUrl u("http://www.zhanqi.tv/api/auth/user.login");
    QByteArray ba= "account=" ;
    ba.append(username.toUtf8().toPercentEncoding());
    ba.append( "&password=");
    ba.append(password.toUtf8().toPercentEncoding());
    //ba = s.toUtf8().toPercentEncoding();
    qDebug()<<ba;
    QObject::connect(hfd, &HttpFileDownloader::downloadFinished, this, &DanmuConnection::getHttpInfo);
    QObject::connect(hfd, &HttpFileDownloader::downloadAbort, this, &DanmuConnection::dealError);

    hfd->postFromURL(u,ba, 10000);
}
void DanmuConnection::work2() {
        state = 2;
        qDebug()<<"state "<<state;
    QUrl u("http://www.zhanqi.tv/api/public/room.viewer");
    QByteArray ba= "uid=" ;
    ba.append(uid.toUtf8().toPercentEncoding());

    //ba = s.toUtf8().toPercentEncoding();
    qDebug()<<ba;

    hfd->postFromURL(u,ba, 10000);
}
void DanmuConnection::work3() {
    state = 3;
    qDebug()<<"state "<<state;
    if (ts!=NULL) {
        ts->disconnect();
        ts->deleteLater();
    }
    ts = new QTcpSocket;
    QObject::connect(ts, &QTcpSocket::connected, this, &DanmuConnection::getSocketInfo);
    QObject::connect(ts, &QTcpSocket::readyRead, this, &DanmuConnection::getSocketInfo);
    QObject::connect(ts, SIGNAL( error(QAbstractSocket::SocketError )), this, SLOT(socketError(QAbstractSocket::SocketError )));
    ts->connectToHost("112.124.114.222",15010);
    qDebug()<<"connect";
}
void DanmuConnection::work4() {
    state = 4;
    qDebug()<<"state "<<state;
    QJsonObject jo;
    jo["cmdid"] = "svrisokreq";
    writeSocketData(1, jo);

}
void DanmuConnection::work42() {
    state = 42;
    qDebug()<<"state "<<state;
    QJsonObject jo;
    jo["cmdid"] = "svrisokreq";
    writeSocketData(1, jo);

}
void DanmuConnection::work5() {
    state = 5;
    QJsonObject jo;
qDebug()<<"state "<<state;


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
void DanmuConnection::work6() {
    state = 6;
    qDebug()<<"state "<<state;
    QJsonObject jo;
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
    writeSocketData(1, jo);
}
void DanmuConnection::getSocketInfo() {
    if (state == 3) {

        work4();
        return;
    }
    QByteArray ba = ts->readAll();
    //qDebug()<<ba;
    cache.append(ba);
    if (cache.length()<=10) return;
    //assert((unsigned )( cache[0] ) ==  11*16+11 );
    int length = unsigned (cache[6]);
    if (cache.length() < length + 12) return;

    QJsonDocument jd = QJsonDocument::fromJson(cache.mid(12, length));
    qDebug()<<jd;
    cache = cache.mid(12+length);
    qDebug()<<cache;
    if (jd.object()["cmdid"].toString() == "getuc") {
        return;
    }
    if (state ==4) {


        work42();
    }
    else if (state== 42) {
        work5();
    }
    else if (state == 5) {
        work6();
    }
}
void DanmuConnection::socketError(QAbstractSocket::SocketError se){

}

void DanmuConnection::getHttpInfo() {
    qDebug()<<"get http info";
    qDebug()<<hfd->getPage();
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
    qDebug()<<hfd->getLastErrorMessage();
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

void DanmuConnection::writeSocketData(int type, QJsonObject jo)
{
    QByteArray ds;
    ds = QJsonDocument(jo).toJson(QJsonDocument::Compact);

    char s[10000] =  "            ";

    for (int i=0; i<12; i++) s[i] = 0;

    s[0] = get16("BB");
    s[1] = get16("CC");
    s[6] = ds.length();
    if (type ==1) {
        s[10] = get16("E8");
        s[11] = get16("03");
    }
    else {
        s[10] = get16("10");
        s[11] = get16("27");
    }
    for (int i=0; i<ds.size(); i++) {
        s[12+i] = ds[i];
    }
    qDebug()<<ds;
    qDebug()<<ds.size();
    ts->write(s, ds.size()+12);

}
