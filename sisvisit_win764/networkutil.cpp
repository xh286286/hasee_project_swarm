#include <cassert>
#include <QTextCodec>
#include <QCoreApplication>
#include <QTime>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "networkutil.h"
#include "mymutexlocker.h"
#include "mainwindow.h"
#include "filecenter.h"
#include "util.h"
NetworkUtil * NetworkUtil::network = nullptr;
bool NetworkUtil::offInternetDebugMode = false;
int NetworkUtil::baseTimeout = 30000;
int NetworkUtil::maxPostMesageSize = 200000;
NetworkUtil::NetworkUtil()
{
}

void NetworkUtil::initial()
{
    if (network!=nullptr) return;
    network = new NetworkUtil();
    network->nam = new QNetworkAccessManager();
}

QList<QString> NetworkUtil::getPidOfPage(QString html )
{
    static const QString sss2 = "<div class=\"mainbox viewthread\">";

    QList<QString> re;
    int index = 0;
    while(true) {
        index = html.indexOf(sss2,index + 1);
        if (index<0) break;
        auto s = getMidString(html,index,"id=\"pid","pid","\"");
        assert(s.size() >4 && s.size()<15);
        re.push_back(s);
    }
    return re;
}
QJsonArray NetworkUtil::getPosterOfPage(QString html )
{
    static const QString sss2 = "<div class=\"mainbox viewthread\">";
    QJsonArray re;
    int index = 0;
    while(true) {
        index = html.indexOf(sss2,index+1);
        if (index<0) break;
        QJsonObject x;

        int end1 = html.indexOf(sss2,index+1);
        int end2 = html.indexOf("</form>",index+1);
        if (end2>end1) end1 = end2;
        auto s = getMidString(html,index,"id=\"pid","pid","\"");
        x["pid"] = s;

        int ri;
        s = getMidString(html,index,"showMenu(this.id)",">","<", &ri);
        if (ri > end1) x["author"] = myTr("匿名");
        else x["author"] = s;

        s = getMidString(html,index,myTr("发表于 "),myTr("于 "),"&nbsp;");
        x["datetime"] = s;

        re.push_back(x);
    }

    return re;
}

QJsonArray NetworkUtil::getPosterList(QString tid, int n, QString jobfile)
{

    QJsonArray re;
    if (offInternetDebugMode) {
        for (int i=0; i<n; i++) {
            re.push_back(QJsonObject());
        }
        return re;
    }
    QString sss0 = "http://174.127.195.200/bbs/thread-" + tid + "-1-1.html";

    QString loadresult;
    loadresult= NetworkUtil::mustLoadPage(sss0);
    QString sss1 = getMidString(loadresult,0,"d","<em>&nbsp;","&nbsp;</em>");
    int total;
    if (sss1=="") {
        total = getPidOfPage(loadresult).size();
    }
    else total = sss1.toInt();
    if (n<0) n = total;
    if (total < n) n = total;

    if (jobfile!= "")
    {
        QJsonDocument jd;
        if (FileCenter::load(jobfile,jd) ) re = jd.array();
        if (re.size() == n) return re;
        while(re.size() % 10 !=0) re.pop_back();

    }

    int totalpage = (n-1)/10 +1;
    for (int page = 1; page<=totalpage; page++) {
        if (page * 10 <= re.size()) continue;
        qDebug()<< "get pid "<<page<<'/'<<totalpage;

        QString sss0 = "http://174.127.195.200/bbs/thread-" + tid + "-"+QString::number(page) + "-1.html";
        QString loadresult;

        loadresult = NetworkUtil::mustLoadPage(sss0);

        auto pp = getPosterOfPage(loadresult);
        for (int i=0; i<pp.size(); i++) {
            auto x = pp[i].toObject();
            x["page"] = QString::number(page);
            x["tid"] = tid;
            re.append(x);
        }

        if (jobfile!= "") FileCenter::save(jobfile , QJsonDocument(re));
    }

    return re;
}
QJsonArray NetworkUtil::getPosterListPid(QString tid, int n, QString jobfile)
{
    QJsonArray re;
    if (offInternetDebugMode) {
        for (int i=0; i<n; i++) {
            re.push_back(QString::number(i+12312312));
            re.push_back(QString::number(2));
        }
        return re;
    }
    QString sss0 = "http://174.127.195.200/bbs/thread-" + tid + "-1-1.html";
    QUrl url(sss0);
    QString loadresult;
    while(loadresult.size()<100) loadresult= sendquest(url);
    QString sss1 = getMidString(loadresult,0,"d","<em>&nbsp;","&nbsp;</em>");
    int total;
    if (sss1=="") {
        total = getPidOfPage(loadresult).size();
    }
    else total = sss1.toInt();

    while(total < n) {
        QString message = myTr("目录施工中 凑够20字！！！！！！！！！！！！！！！！！！！！ ") + QString::number(total+1);
        qDebug()<< "supplement " << total+1 << "total "<< n;
        QString page;
        while(! sendreply(tid,message,0,&page))  {}

        waitMillisec(31000);

        total++;
        int t = page.toInt() * 10 - 9;
        if (total < t) total = t;
    }

    if (jobfile!= "")
    {
        QJsonDocument jd;
        if (FileCenter::load(jobfile,jd) ) re = jd.array();
        if (re.size() == n) return re;
        while(re.size() % 20 !=0) re.pop_back();

    }

    int totalpage = (n-1)/10 +1;
    for (int page = 1; page<=totalpage; page++) {
        if (page * 20 <= re.size()) continue;
        qDebug()<< "get pid "<<page<<'/'<<totalpage;

        QString sss0 = "http://174.127.195.200/bbs/thread-" + tid + "-"+QString::number(page) + "-1.html";
        QUrl url(sss0);
        QString loadresult;

        int count = 0;
        int waittime = 15000;
        int timeout = baseTimeout;
        while(loadresult.size()<100) {
            count++;
            if (count >3) {
                qDebug()<<" wait for a while";
                waitMillisec(waittime);
                if (waittime <45000) {
                    waittime+=5000;
                    timeout+=5000;
                }
                count = 0;
            }
            loadresult= sendquest(url,0,0,timeout);

        }
        auto pp = getPidOfPage(loadresult);
        for (int i=0; i<pp.size(); i++) {
            re.append(pp[i]);
            re.append(QString::number(page));
        }

        if (jobfile!= "") FileCenter::save(jobfile , QJsonDocument(re));
    }

    return re;
}

bool NetworkUtil::sendreply(QString tid, QString message,   QString * pid,   QString * pagenumber, QString subject,  int timeout)
{
    if (offInternetDebugMode) {
        static int ppp = 33333333;
        if (pid!=nullptr) *pid = QString::number(ppp++);
        if (pagenumber!= nullptr) *pagenumber= 1;
        return true;
    }
//    MyMutexLocker mml2(&network->hashcodemutex,10000);
//    if (!mml2.success()) {
//        MainWindow::outputInfor("hashcode 冲突");
//        return false;
//    }

    QString sss0 = "http://174.127.195.200/bbs/post.php?action=reply&fid=&tid=";
    sss0+= tid;
    sss0+= "&extra=&replysubmit=yes";
    QUrl url1(sss0);

    QList<QNetworkReply::RawHeaderPair> lhp;
    while(true){
        QByteArray postData;
        postData.append("formhash=");
        postData.append(network->hashcode);
        postData.append("&usesig=1&subject=");
        postData.append(toSisPE(subject));
        postData.append("&message=");
        postData.append(toSisPE(message));
        QString loadresult = sendquest(url1,&postData, &lhp,timeout);
        if (loadresult.indexOf(myTr("您的请求来路不正确或验证字串不符"))>=0) {
            MainWindow::outputInfor("您的请求来路不正确或验证字串不符 get hashcode");
            QUrl url0("http://174.127.195.201/bbs/logging.php?action=login");
            getHashCode(url0);
            if (network->hashcode == "error") {
                return false;
            }
        }
        else break;
    }
    for (int i=0; i<lhp.size(); i++) {
        if (lhp[i].first == "location") {
            QString p = getMidString(lhp[i].second,0,"pid=","=","&");
            if (pid!= nullptr) *pid = p;
            qDebug()<< "pid "<<p;
            p = getMidString(lhp[i].second,0,"page=","=","&");
            if (pagenumber!= nullptr) *pagenumber= p;
            qDebug()<< "pagenumber "<<p;
            MainWindow::outputInfor("send reply success");
            return true;
        }
    }
    return false;
}

QString NetworkUtil::mustLoadPage(QString u)
{
    QUrl url(u);
    QString loadresult;

    int count = 0;
    int waittime = 15000;
    int timeout = baseTimeout;
    while(loadresult.size()<100) {
        count++;
        if (count >3) {
            qDebug()<<" wait for a while";
            waitMillisec(waittime);
            if (waittime <45000) {
                waittime+=5000;
                timeout+=5000;
            }
            count = 0;
        }
        loadresult= sendquest(url,0,0,timeout);
    }
    return loadresult;
}

bool NetworkUtil::mustSendreply(QString tid, QString subject, QString message,   QString * pid,   QString * pagenumber)
{
    int count = 0;
    int waittime = 15000;
    int timeout = NetworkUtil::baseTimeout;
    if (message.size() >10000) {
        timeout +=  (message.size() -10000) / 4.0;
        if (timeout >20000) timeout = 20000;
    }
    while (!NetworkUtil::sendreply(tid,message, pid,pagenumber, subject,timeout)) {
        count++;
        if (count>2) {
            qDebug()<< "too many error!, wait";
            if (waittime <41000) {
                waittime +=3000;
                timeout +=5000;
            }
            waitMillisec(waittime);
            count = 0;
        }
        qDebug()<<"post error, repost";
    }
    return true;
}


bool NetworkUtil::mustEditPost(QString tid, QString pid , QString subject, QString message)
{
    int count = 0;
    int waittime = 15000;
    int timeout = NetworkUtil::baseTimeout;
    if (message.size() >10000) {
        timeout +=  (message.size() -10000) / 4.0;
        if (timeout >20000) timeout = 20000;
    }
    while (!NetworkUtil::editpost(tid,pid,subject,message, timeout)) {
        count++;
        if (count>2) {
            qDebug()<< "too many error!, wait";
            if (waittime <41000) {
                waittime +=3000;
                timeout +=5000;
            }
            waitMillisec(waittime);
            count = 0;
        }
        qDebug()<<"post error, repost";
    }
    return true;
}

bool NetworkUtil::mustEditMainPost(QString tid, QString pid , QString type,  QString subject, QString message)
{
    int count = 0;
    int waittime = 15000;
    int timeout = NetworkUtil::baseTimeout;
    if (message.size() >10000) {
        timeout +=  (message.size() -10000) / 4.0;
        if (timeout >20000) timeout = 20000;
    }
    while (!NetworkUtil::editMainPost(tid,pid,type, subject,message, timeout)) {
        count++;
        if (count>2) {
            qDebug()<< "too many error!, wait";
            if (waittime <41000) {
                waittime +=3000;
                timeout +=5000;
            }
            waitMillisec(waittime);
            count = 0;
        }
        qDebug()<<"post error, repost";
    }
    return true;
}

bool NetworkUtil::editMainPost(QString tid, QString pid , QString type, QString subject, QString message , int timeout){
    if (offInternetDebugMode) {
        return true;
    }

    if (subject =="") {
        MainWindow::outputInfor("no subject");
        return false;
    }

//    MyMutexLocker mml2(&network->hashcodemutex,10000);
//    if (!mml2.success()) {
//        MainWindow::outputInfor("hashcode 冲突");
//        return false;
//    }

    QString sss0 = "http://174.127.195.200/bbs/post.php?action=edit&fid=&tid=";
    sss0+= tid;
    sss0+= "&pid=" + pid;
    sss0+= "&extra=&editsubmit=true";
    QUrl url1(sss0);

    QList<QNetworkReply::RawHeaderPair> lhp;

    QString loadresult;
    while(true){
        QByteArray postData;
        postData.append("formhash=");
        postData.append(network->hashcode);
        postData.append("&usesig=1&subject=");
        postData.append(toSisPE(subject));
        postData.append("&typeid=");
        postData.append(toSisPE(type));
        postData.append("&message=");
        postData.append(toSisPE(message));
        loadresult = sendquest(url1,&postData, &lhp, timeout);
        //qDebug()<<loadresult;
        if (loadresult.indexOf(myTr("您的请求来路不正确或验证字串不符"))>=0) {
            MainWindow::outputInfor("您的请求来路不正确或验证字串不符 get hashcode");
            QUrl url0("http://174.127.195.201/bbs/logging.php?action=login");
            getHashCode(url0);
            if (network->hashcode == "error") {
                return false;
            }
        }
        else break;
    }
    if (loadresult.indexOf(myTr("未定义操作")) >=0) {
        MainWindow::outputInfor("未定义操作   假设正确");
        qDebug()<<myTr("未定义操作   假设正确");
        FileCenter::outputLog(myTr("未定义操作   假设正确"));
        FileCenter::outputLog(tid);
        FileCenter::outputLog(pid);
        return true;
    }
    for (int i=0; i<lhp.size(); i++) {
        if (lhp[i].first == "location") {
//            QString p = getMidString(lhp[i].second,0,"pid=","=","&");
//            if (pid!= nullptr) *pid = p;
//            qDebug()<< "pid "<<p;
//            p = getMidString(lhp[i].second,0,"page=","=","&");
//            if (pagenumber!= nullptr) *pagenumber= p;
//            qDebug()<< "pagenumber "<<p;
            qDebug()<<"edit post success";
            return true;
        }
    }
    return false;

}

bool NetworkUtil::editpost(QString tid, QString pid, QString subject, QString message , int timeout)
{
    if (offInternetDebugMode) {
        return true;
    }


//    MyMutexLocker mml2(&network->hashcodemutex,10000);
//    if (!mml2.success()) {
//        MainWindow::outputInfor("hashcode 冲突");
//        return false;
//    }

    QString sss0 = "http://174.127.195.200/bbs/post.php?action=edit&fid=&tid=";
    sss0+= tid;
    sss0+= "&pid=" + pid;
    sss0+= "&extra=&editsubmit=true";
    QUrl url1(sss0);

    QList<QNetworkReply::RawHeaderPair> lhp;

    QString loadresult;
    while(true){
        QByteArray postData;
        postData.append("formhash=");
        postData.append(network->hashcode);
        postData.append("&usesig=1&subject=");
        postData.append(toSisPE(subject));
        postData.append("&message=");
        postData.append(toSisPE(message));
        loadresult = sendquest(url1,&postData, &lhp, timeout);
        //qDebug()<<loadresult;
        if (loadresult.indexOf(myTr("您的请求来路不正确或验证字串不符"))>=0) {
            MainWindow::outputInfor("您的请求来路不正确或验证字串不符 get hashcode");
            QUrl url0("http://174.127.195.201/bbs/logging.php?action=login");
            getHashCode(url0);
            if (network->hashcode == "error") {
                return false;
            }
        }
        else break;
    }
    if (loadresult.indexOf(myTr("未定义操作")) >=0) {
        MainWindow::outputInfor("未定义操作   假设正确");
        qDebug()<<myTr("未定义操作   假设正确");
        FileCenter::outputLog(myTr("未定义操作   假设正确"));
        FileCenter::outputLog(tid);
        FileCenter::outputLog(pid);
        return true;
    }
    for (int i=0; i<lhp.size(); i++) {
        if (lhp[i].first == "location") {
//            QString p = getMidString(lhp[i].second,0,"pid=","=","&");
//            if (pid!= nullptr) *pid = p;
//            qDebug()<< "pid "<<p;
//            p = getMidString(lhp[i].second,0,"page=","=","&");
//            if (pagenumber!= nullptr) *pagenumber= p;
//            qDebug()<< "pagenumber "<<p;
            qDebug()<<"edit post success";
            return true;
        }
    }
    return false;
}

QString NetworkUtil::getHashCode(const QUrl & url,QString * page)
{
    assert(!offInternetDebugMode);
        MyMutexLocker mml2(&network->hashcodemutex,10000);
        if (!mml2.success()) {
            MainWindow::outputInfor("hashcode 冲突");
            return network->hashcode;
        }

    QString fff;
    if (page == nullptr) {page = &fff;}
    *page = sendquest(url);
    QString loadresult = *page;
    int a1 = (*page).indexOf("formhash=");
    if (a1<0) {
        MainWindow::outputInfor( ("no hashcode 无法访问指定页面,请检查网络, 或者检查IP地址 "));
        network->hashcode = "";
        return "";
    }

    a1+=9;
    network->hashcode = loadresult.mid(a1,8);
    MainWindow::outputInfor("hashcode ");
    //outputInfor(hashcode.toLocal8Bit().data());
    MainWindow::outputInfor(network->hashcode);
    return network->hashcode;
}

QString NetworkUtil::sendquest(const QUrl & url, QByteArray * data, QList<QNetworkReply::RawHeaderPair> * headerpair,  int timeout)
{
    assert(!offInternetDebugMode);
    if (timeout<baseTimeout) timeout = baseTimeout;
    QUrl xu = url;
    QUrl temp(Util::host);
    xu.setHost(temp.host(),QUrl::TolerantMode);
    QString loadresult = "";
    network->errorcode = QNetworkReply::NoError;

    static int stamp = 10000;
    stamp++;
    qDebug()<<xu;
    if (data!=0) qDebug()<<" message size " << data->size();
    qDebug()<<"waittime "<<timeout;
    QString inf = QString::number(stamp) + '\n';
    inf += QDateTime::currentDateTime().toString() + '\n';
    inf += xu.toString();
    if (data!=0) inf+= "\n\n\n" + *data;
    MainWindow::showPostInfor(inf);


    QNetworkRequest r(xu);

    QNetworkReply * a;
    {
        static QMutex nammutex;
        QMutexLocker lock(&nammutex);
        if (data==0) a = network->nam->get(r);
        else {
            r.setRawHeader("content-type","application/x-www-form-urlencoded");
            a = network->nam->post(r, *data);
        }
    }
    QTime t;
    t.start();
    //int timeout = 10000;
    while(a->isRunning()) {
        QCoreApplication::processEvents();
        if (t.elapsed()>timeout) {
            qDebug()<<"timeout";
            return "timeout";
        }
    }

    if (headerpair != nullptr) *headerpair = a->rawHeaderPairs();

    qDebug()<<"rece   time spend " << t.elapsed();
    QByteArray s = a->readAll();
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");

    loadresult = gbk->toUnicode(s);

    if (a->error() != QNetworkReply::NoError) {
        loadresult = "error code " + QString::number( a->error());
        qDebug()<<loadresult;
        MainWindow::outputInfor("无法访问网络");
        if (t.elapsed()<100) {
            waitMillisec(1000);
        }
    }
    QString inf1 = QString::number(stamp) + '\n';
    inf1 += QDateTime::currentDateTime().toString() + '\n';
    MainWindow::showrecehtml(inf1 + loadresult, a->rawHeaderPairs() );
    //qDebug()<<network->loadresult<<endl;

    a->deleteLater();
    return loadresult;
}

void NetworkUtil::replyFinished(QNetworkReply* a)
{
    qDebug()<<"rece signal";
    QByteArray s = a->readAll();
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");

    QString loadresult = gbk->toUnicode(s);

    qDebug()<<loadresult<<endl;

    a->deleteLater();
}

void NetworkUtil::visiterror(QNetworkReply::NetworkError code)
{
    network->errorcode = code;
    qDebug()<<"visit error" << code;
}
void NetworkUtil::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QString s;
    s = "download progress "+ QString::number(bytesReceived) + " " + QString::number(bytesTotal);
    MainWindow::outputInfor(s.toLocal8Bit().data());
}

void NetworkUtil::loginsis()
{
    static QMutex loginmutex;
    MyMutexLocker mml1(&loginmutex);
    if (!mml1.success()) return;

//    MyMutexLocker mml2(&network->hashcodemutex,10000);
//    if (!mml2.success()) return;

    QString loadresult ;
    MainWindow::outputInfor(("开始登陆"));
    QUrl url("http://174.127.195.200/bbs/logging.php?action=login");
    network->hashcode = getHashCode(url,&loadresult);
    int a0 = loadresult.indexOf(myTr("现在将转入登录前页面"));
    if (a0>=0) {
        MainWindow::outputInfor( ("已经登陆,请勿重复登陆"));
        return;
    }
    if (network->hashcode=="") {
        return;
    }

    QByteArray postData;
    postData.append("formhash=");
    postData.append(network->hashcode);
    postData.append("&referer=http%3A%2F%2F174.127.195.200%2Fbbs%2Fforum-403-1.html&loginfield=username&username=");
    postData.append(toSisPE(Util::username));
    postData.append("&password=");
    postData.append(toSisPE(Util::userpass));
    postData.append("&questionid=");
    postData.append(toSisPE(Util::loginquestionid));
    postData.append("&answer=");
    postData.append(toSisPE(Util::loginanswer));
    postData.append("&cookietime=");
    postData.append("315360000");
    postData.append("&loginmode=");
    postData.append("&styleid=13");
    postData.append("&loginsubmit=true");

    qDebug()<<postData;

    loadresult = sendquest(url,&postData);

    int a1 = loadresult.indexOf("uid=");
    if (a1>=0) {
        int a2 = loadresult.indexOf("\"",a1);
        network->uid = loadresult.mid(a1+4,a2-a1-4);
        MainWindow::outputInfor("uid " );
        //outputInfor(uid.toLocal8Bit().data());
        MainWindow::outputInfor(network->uid);
        MainWindow::outputInfor(("登陆成功"));
    }
    else {
        int a2 = loadresult.indexOf(myTr("次错误尝试"));
        if (a2>=0) {
            MainWindow::outputInfor( ("累计 5 次错误尝试，15 分钟内您将不能登录论坛"));
        }
        else {
            MainWindow::outputInfor( ("登陆失败,请检查用户配置,重新设置,删除save.ini文件重新输入"));
        }
    }

}

void NetworkUtil::clearCookies() {
    QUrl temp(Util::host);
    auto cookielist = network->nam ->cookieJar()->cookiesForUrl(temp);
    for (auto i= cookielist.begin(); i!= cookielist.end(); i++) {
        bool ok= network->nam->cookieJar()->deleteCookie(*i);
        qDebug() <<"delete cookie "<<ok;
    }
}
