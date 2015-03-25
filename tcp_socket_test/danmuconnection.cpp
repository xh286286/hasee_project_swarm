#include "danmuconnection.h"
#include "../share_library/httpfiledownloader.h"
#include "../share_library/zhanqiutil.h"
#include "../share_library/Util.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QTimer>
#include <cassert>
//#include <QCoreApplication>
DanmuConnection::DanmuConnection(QObject *parent) :
    QObject(parent)
{
    hfd = NULL;
    ts = NULL;
    QObject::connect(&connectionTimeout, &QTimer::timeout,  this, &DanmuConnection::work1);
    QObject::connect(&connectionTimeout, &QTimer::timeout,  this, &DanmuConnection::noinfoError);
    keepaliveTimer.start(30000);
    QObject::connect(&keepaliveTimer, &QTimer::timeout,  this, &DanmuConnection::keepAlive);
    state = 0;
    debugFlag = false;
    shutdownFlag = false;
    cookieLoginFlag = true;
    postDanmuTimer.start(3000);
    danmuServerSeed = -1;
    danmuServerPort  = 15010;
    QObject::connect(&postDanmuTimer, &QTimer::timeout,  this, &DanmuConnection::postDanmuWork);

    cookieMap["aabbeaabbe"] =
"gid=1640079128; _alit_=AQAAANFg83YgFw4A2h5kCg2x0J3S+W9y; zq_uid=359887; zq_authorization=b7bf2090db08cd0fd94cbd315a3bb298; zq_token=c0063c9ecf97aff18474e90c8c75500d; ZQ_GUID=2B44722E-CEB2-A6FD-4666-F9EB6FFAEF8C; ZQ_GUID_C=2B44722E-CEB2-A6FD-4666-F9EB6FFAEF8C; beGiftShield=1; guide_cookie_guest=15; open_left=0; guide_cookie_uid_359887=15; Hm_lvt_299cfc89fdba155674e083d478408f29=1426159378,1426520726,1426757724; Hm_lpvt_299cfc89fdba155674e083d478408f29=1427181392; PHPSESSID=8glqk9asag9gsq7jnn59vbj663";
    cookieMap["aabbdaabbd"] =
"gid=1785039725; Hm_lvt_269ef1b2c73c42f960858db3c319511a=1416812919,1416836236,1416908600,1416920217; bdshare_firstime=1418039018364; room_chat_notice_cookie=20150112; s5_ibox_cookie=125; guide_cookie_uid_359887=7; _um_uuid=89d73272f704483480a88b21304b3e89; zq_uid=299370; zq_authorization=3c8416228088b536536dd5ad764db1b5; zq_token=28464420ad524a45750bc98aa2277e60; ZQ_GUID=A8C20546-52C2-D354-AB08-FC2DB2EC6B6A; ZQ_GUID_C=A8C20546-52C2-D354-AB08-FC2DB2EC6B6A; beGiftShield=1; open_left=0; PHPSESSID=e8pgvnp7jnmamda5rge7ujju06; _alit_=AQAAAPqd1U059gsAtz0Oj4o2s3fVb69W; guide_cookie_guest=15; Hm_lvt_299cfc89fdba155674e083d478408f29=1427037768,1427099022,1427100566,1427120898; Hm_lpvt_299cfc89fdba155674e083d478408f29=1427186212; guide_cookie_uid_299370=15";
    cookieMap["aabbiaabbi"] =
"gid=1792594488; Hm_lvt_269ef1b2c73c42f960858db3c319511a=1415458663,1416058040,1416119511; room_chat_notice_cookie=20150107; s5_ibox_cookie=123; guide_cookie_uid_100305463=7; PHPSESSID=mrrg6a2q529t9j7cgmng859254; _um_uuid=a2a1e29b4f08492692c4399fb5c43da2; ZQ_GUID=FFDAC940-9A1A-FA0C-2C0F-7C391C27D2AB; ZQ_GUID_C=FFDAC940-9A1A-FA0C-2C0F-7C391C27D2AB; zq_uid=104410226; zq_authorization=796d349d8f1a147c4c966cd5fef79c6c; zq_token=75c8f6dc3da462776092276990dc1f52; _alit_=AQAAANBEimFfOAUAtz0Ojz/im8PygRpE; Hm_lvt_299cfc89fdba155674e083d478408f29=1426952787,1427037808,1427038868,1427121543; Hm_lpvt_299cfc89fdba155674e083d478408f29=1427181974; guide_cookie_uid_104410226=7";
    cookieMap["aabbfaabbf"] =
"gid=1728526389; room_chat_notice_cookie=20141109; Hm_lvt_269ef1b2c73c42f960858db3c319511a=1416211461,1416388899,1416475599,1416485378; PHPSESSID=2ksujk7oj2eef63qu0o20fhto2; open_left=1; zq_uid=100015917; zq_authorization=dfed1d5731833c635ffa8877900fc514; zq_token=54daab5df5c5ad2d69a5d5cc1904c7af; ZQ_GUID=F8767DDE-873E-D519-E861-5934D0200FFB; ZQ_GUID_C=F8767DDE-873E-D519-E861-5934D0200FFB; _alit_=AQAAAOJLFDK5dAQAbwg5il0qokQ323BV; guide_cookie_guest=15; guide_cookie_uid_100015917=15; Hm_lvt_299cfc89fdba155674e083d478408f29=1426766783,1426769857,1426778165,1427182086; Hm_lpvt_299cfc89fdba155674e083d478408f29=1427182119";

}


bool DanmuConnection::viewRoom(QString room) {
    this->room = room;
    QString h = "http://zhanqi.tv/";

    h+= room;

    qDebug()<<room;

    while(true) {

        HttpFileDownloader hfd;
        bool ok = hfd.sycGetPageFromURL(QUrl(h));
        if (!ok) continue;
        QString re = hfd.getPage();


        if (re.size() <1000) continue;
        int x = re.indexOf("uid");

        if (x<0) {
            //QCoreApplication::processEvents();
            qDebug()<<"get room info failed !!!";
            continue;
        }
        int a,b,c,d;
        a=re.indexOf(":",x);
        b=re.indexOf(",",x);
        c=re.lastIndexOf(":",x);
        d=re.lastIndexOf(",",x);
        QString s1,s2;
        s1 = re.mid(a+2,b-a-3);
        s2 = re.mid(c+2,d-c-3);

        if (debugFlag) qDebug()<< "get room info " <<s1<<' '<<s2;
        uid = s1;
        id = s2;

        break;
    }
    return true;
}
void DanmuConnection::login(QString name, QString pass) {
    username  = name ;
    password = pass;

    work1();
}

void DanmuConnection::login(QString name, QString pass, QString uid, QString id) {
    username  = name ;
    password = pass;
    this->uid = uid;
    this->id = id;

    work1();

}
void DanmuConnection::reboot() {
    if (debugFlag) qDebug()<<"reboot";
    work1();
}

void DanmuConnection::work1() {



    state = 1;
     if (debugFlag) qDebug()<<"state "<<state;
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
    QObject::connect(hfd, &HttpFileDownloader::downloadFinished, this, &DanmuConnection::getHttpInfo);
    QObject::connect(hfd, &HttpFileDownloader::downloadAbort, this, &DanmuConnection::dealError);

    if (cookieLoginFlag) {
        work2();
        return;
    }

    QUrl u("http://www.zhanqi.tv/api/auth/user.login");
    QByteArray ba= "account=" ;
    ba.append(username.toUtf8().toPercentEncoding());
    ba.append( "&password=");
    ba.append(password.toUtf8().toPercentEncoding());
    //ba = s.toUtf8().toPercentEncoding();
    if (debugFlag) qDebug()<<ba;


    hfd->postFromURL(u,ba, 10000);
}
void DanmuConnection::work2() {
        state = 2;
        if (debugFlag) qDebug()<<"state "<<state;
    QUrl u("http://www.zhanqi.tv/api/public/room.viewer");
    QByteArray ba= "uid=" ;
    ba.append(uid.toUtf8().toPercentEncoding());
    //qDebug()<< " uid " << uid;
    //ba = s.toUtf8().toPercentEncoding();
    if (debugFlag) qDebug()<<ba;
    if ( cookieLoginFlag ) {
        //qDebug()<<"test";
        QNetworkRequest   req(u);

//Accept:*/*
        req.setRawHeader("Accept","*/*");
//Accept-Encoding:gzip, deflate
        req.setRawHeader("Accept-Encoding","gzip, deflate");
//Accept-Language:zh-CN,zh;q=0.8
        req.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
//Cache-Control:max-age=0
        req.setRawHeader("Cache-Control","max-age=0");
//Connection:keep-alive
        req.setRawHeader("Connection","keep-alive");

//Content-Type:application/x-www-form-urlencoded; charset=UTF-8
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");


//Host:www.zhanqi.tv
        req.setRawHeader("Host","www.zhanqi.tv");
//Origin:http://www.zhanqi.tv
        req.setRawHeader("Origin","http://www.zhanqi.tv");
//Referer:http://www.zhanqi.tv/seraph_xiaot
        req.setRawHeader("Referer","http://www.zhanqi.tv/scorpio");
//User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36
        req.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
//X-Requested-With:XMLHttpRequest
        req.setRawHeader("X-Requested-With","XMLHttpRequest");
        //qDebug()<<username;
       // qDebug()<<cookieMap[username];
        //req.setHeader(QNetworkRequest::CookieHeader, cookieMap[username]);
        req.setRawHeader("Cookie", cookieMap[username].toUtf8());

        hfd->postFromRequset(req,ba);
        //sycPostFromRequset

    }
    else {
        hfd->postFromURL(u,ba, 10000);
    }
    //
}
QStringList DanmuConnection::getDanmuSeverListFromNet() {

    QString h = "http://zhanqi.tv/mayu";

    int trycount = 4;
    while(true) {
        if (trycount  <= 0) {
            qDebug()<< " load danmu server list from net  failed!!!!!";
            return  QStringList();
        }
        qDebug()<<"get danmu server list from net ";
        trycount --;

        HttpFileDownloader hfd;
        bool ok = hfd.sycGetPageFromURL(QUrl(h));
        if (!ok) continue;
        QString re = hfd.getPage();

        if (re.size() <1000) continue;
        int x = re.indexOf("\"Servers\":\"");  //1;
        int y = re.indexOf("\"", x+30);


        if (x<0) {
            //QCoreApplication::processEvents();
            qDebug()<<"get danmu server list from net failed !!!";
            continue;
        }

        QString s1 ;
        s1 = re.mid(x+11, y-x-11);
        //qDebug()<< "get  danmu server base64  " <<s1 ;

        QByteArray s2 = QByteArray::fromBase64(s1.toLatin1());
        //qDebug()<< "get  danmu server json  " <<s2 ;


        QJsonDocument jd = QJsonDocument::fromJson(s2);
        //qDebug()<< "get  danmu server json object " <<jd ;

        QJsonObject jo = jd.object();
        QJsonArray ja = jo["list"].toArray();

        qDebug()<< "danmu server number "<< ja.size();

        if (ja.size()<5) {
            qDebug()<<"get not enough danmu server list from net !!!";
            return QStringList();
        }
        QStringList rel;
        QJsonArray ja1;
        for (int i=0; i<ja.size(); i++) {
            rel.push_back(ja[i].toObject()["ip"].toString());
            ja1.push_back(rel[i]);
        }

        jd = QJsonDocument(ja1);
        saveJsonFile("danmuServer.json",jd);

        return rel;
    }
    return QStringList();
}


QStringList DanmuConnection::getDanmuSeverList() {
    static bool initFlag = false;
    static QStringList danmuSeverList;

    if (!initFlag) {
        initFlag = true;
        QJsonDocument jd;
        loadJsonFile("danmuServer.json",jd);

        QJsonArray ja = jd.array();
        qDebug()<<ja;
        QSet<QString > pool;
        for (int i=0; i<ja.size(); i++) {
            QString s = ja[i].toString();
            if (pool.contains(s)) continue;
            pool.insert(s);
            danmuSeverList.push_back(s);
        }
        if (danmuSeverList.size() ==0) danmuSeverList.push_back("115.29.169.228");
        qDebug()<<danmuSeverList;
    }
    return danmuSeverList;
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

    static QStringList danmuSeverList = getDanmuSeverList();

    static int danmuSeverIndex = 0;

    int index;

    if (danmuServerSeed<0) {
        index = danmuSeverIndex;
        danmuSeverIndex++;
    }
    else {
        index = danmuServerSeed;
    }

    QString severName = danmuSeverList[ index % danmuSeverList.size() ];

    ts->connectToHost(severName ,danmuServerPort);
    qDebug()<<username << " " << room<< " " <<uid << " " <<"connect to "<<severName <<" "<< danmuServerPort<<" "<< QTime::currentTime().toString();
    //182.92.104.225
    if (debugFlag) qDebug()<<"connect  danmu sever!!";
}
void DanmuConnection::work4() {
    state = 4;
    if (debugFlag) qDebug()<<"state "<<state;
    QJsonObject jo;
    jo["cmdid"] = QString("svrisokreq");
    writeSocketData(1, jo);

    jo["cmdid"] = QString("loginreq");
    jo["roomid"] = id.toInt();
    //jo["nickname"] = loginJo["data"].toObject()["nickname"];
    jo["nickname"] = username;
    jo["t"] = 0;
    jo["fhost"] = QString("");
    jo["gid"]  = roomviewerJo["data"].toObject()["gid"];
    jo["timestamp"]  = roomviewerJo["data"].toObject()["timestamp"];
    //jo["uid"] = loginJo["data"].toObject()["uid"];
    jo["uid"] =  roomviewerJo["data"].toObject()["uid"];
    jo["sid"]  = roomviewerJo["data"].toObject()["sid"];
    jo["r"] = 1;
    writeSocketData(2, jo);
}

void DanmuConnection::postDanmuWork() {
    //if (debugFlag) qDebug()<<state;
    if (state != 5) return;

//    QJsonObject({"cmdid":"chatmessage","content":"测试2","level":15,"showmedal":1,"s
//    peakinroom":1,"style":null,"toid":0,"usexuanzi":0})
    if (danmuPool.size() ==0) return;
    QString s = danmuPool.front();
    danmuPool.pop_front();
    QString m = ZhanQiUtil::encodeString( s);
//    while( historyMessage.contains(m)) {
//        m+='_';
//    }
    historyMessage.insert(m);
    QJsonObject jo;
    jo["cmdid"] = QString("chatmessage");
    jo["content"] = m;
    jo["level"] = 0;
    jo["showmedal"] = 1;
    jo["speakinroom"] = 0;
    jo["fromid"] = roomviewerJo["data"].toObject()["gid"];
    jo["toid"] = 0;
    jo["style"] = QJsonValue::Null;
    jo["usexuanzi"] = 0;
   if (debugFlag)  qDebug()<< "try";

    //QString s1 = "{\"level\":0,\"cmdid\":\"chatmessage\",\"style\":null,\"showmedal\":1,\"toid\":0,\"speakinroom\":0,\"content\":\"test312\",\"usexuanzi\":0}";
   // writeSocketString(2, s1);
    writeSocketData(2,jo);


}
void DanmuConnection::sendRoses() {
    if (state != 5) {
        qDebug()<< "error  action while not state 5 !!!";
        return;
    }
//    71   QJsonObject({"cmdid":"rosebro","cnt":6,"name":"天蝎10000","rank":47})
//    QJsonObject({"cmdid":"rosebro","cnt":6,"name":"天蝎10000","rank":47})
    //QJsonObject({"cmdid":"roseuse","code":0,"message":""})
    QJsonObject jo;
    jo["cmdid"] = QString("roseuse");
    jo["code"] = 0;
    jo["message"] = QString( myTr(""));


   if (debugFlag)  qDebug()<< "try";


    writeSocketData(2,jo);
}

void DanmuConnection::sendGift(int pid, int count) {
//    顶 3
//    QJsonObject({"cmdid":"Gift.Use","count":1,"pid":3})
//    溜 1
//    锅 4
    if (state != 5) {
        qDebug()<< "error  action while not state 5 !!!";
        return;
    }
    if (pid !=1 && pid != 3 && pid!=4) {
        qDebug()<<"wrong gift code";
        return;
    }
    QJsonObject jo;
    jo["cmdid"] = QString("Gift.Use");
    jo["count"] = count;
    jo["pid"] =pid;

    jo["uid"] = uid;
   if (debugFlag)  qDebug()<< "try";


    writeSocketData(2,jo);

}

void DanmuConnection::blockUser(QString name, int uid, int action) {
    //if (debugFlag) qDebug()<<state;
    if (state != 5) {
        qDebug()<< "error  action while not state 5 !!!";
        return;
    }
 //   QJsonObject({"action":1,"cmdid":"blockuser","ip":"111.8.57.138","msg":"111","nam
 //   e":"九千万亿电竞女神","type":1,"uid":100305463})

    QJsonObject jo;
    jo["cmdid"] = QString("blockuser");
    jo["action"] = action;
    jo["ip"] = QString("111.8.57.138");
    jo["msg"] = QString("222");
    jo["name"] = name;
    jo["type"] = 1;
    jo["uid"] = uid;
   if (debugFlag)  qDebug()<< "try";


    writeSocketData(2,jo);
}

bool DanmuConnection::postDanmu(QString s) {
    danmuPool.push_back(s);
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
    connectionTimeout.start(100000);
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
        else {
           if (debugFlag) qDebug()<<cache.length()<<' '<<jo<<' '<<QTime::currentTime();
           // keepAlive();
        }

        cache = cache.mid(12+length);
        //if (debugFlag) qDebug()<<12+length<< ' '<< cache.length();
    //    qDebug()<<cache.toPercentEncoding();

    }
    if (count>0) emit messReady();
    return;
}
void DanmuConnection::socketError(QAbstractSocket::SocketError se ){
    //if (debugFlag)
    qDebug()<<username<<" " <<room<<" "<<uid   <<" socket error restart " <<se;
    work1();
}

void DanmuConnection::getHttpInfo() {
    if (debugFlag) qDebug()<<"get http info";
    if (debugFlag) qDebug()<<hfd->getPage();
    if (debugFlag) {
       // qDebug()<<hfd->getReply()->rawHeaderPairs();
    }
    if (state == 1) {
        loginJo =  QJsonDocument::fromJson(hfd->getPage()) .object();

        if (password != "" ) {
            if (loginJo["code"].isNull() || loginJo["code"].toInt() !=0)     {

                work1();
            }
        }

        work2();
    }
    else if (state ==2) {
        roomviewerJo = QJsonDocument::fromJson(hfd->getPage()) .object();

        if ( roomviewerJo["code"].isNull() ||  roomviewerJo["code"].toInt() !=0) {
            work1();
        }
        work3();
    }
}
void DanmuConnection::dealError() {
    state = 0;
    if (debugFlag) qDebug()<<hfd->getLastErrorMessage();
    work1();
}

void DanmuConnection::noinfoError()
{
    qDebug()<<username<<" " <<room<<" "<<uid   <<"  communication timeout ";
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
    if (state != 5) return  ;

    //if (debugFlag) qDebug()<<state<<' ' <<" keepalive";
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
        s[6] = 0;
        s[7] = 0;
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
