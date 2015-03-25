#include <algorithm>

#include <QDateTime>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QtWidgets>
#include <QtNetwork>
#include <QNetworkRequest>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QMutexLocker>
#include <QNetworkReply>
#include <cassert>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include "mymutexlocker.h"
#include "networkutil.h"
#include "sisthread.h"
#include "filecenter.h"
#include "keywordset.h"
#include "authorset.h"
MainWindow * MainWindow::mainW = nullptr;
QString MainWindow::runninglog = "";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    inforWin = new QTextEdit(this);
    debugWin = new QPlainTextEdit();
    debugWin2 = new QPlainTextEdit();
    debugWin->setWindowTitle("receive window");
    debugWin2->setWindowTitle("post window");
    //inforWin->setDisabled(true);
    //debugWin->setDisabled(true);
    setCentralWidget(inforWin);
    QMenu *testMenu = menuBar()->addMenu(tr("&Tools"));
    QMenu *test2Menu = menuBar()->addMenu(tr("&Tools2"));
    test2Menu->addAction(myTr("set timeout"), this, SLOT(setTimeout()));
    test2Menu->addAction(tr("settings"), this, SLOT(inputdata()));


    testMenu->addAction(myTr("登陆"), this, SLOT(loginsis()));
    testMenu->addAction(myTr("load thread from internet"), this, SLOT(loadthreadfromeinternet()));
    testMenu->addAction(myTr("working"), this, SLOT(working()));
    testMenu->addAction(myTr("catalog"), this, SLOT(generateCatalog()));



#define DEBUG_MODE
#ifdef DEBUG_MODE
    testMenu->addAction(myTr("load thread from file and checking"), this, SLOT(loadthreadfromfileandchecking()));
    testMenu->addAction(myTr("fixPoster"), this, SLOT(fixPoster()));

    testMenu->addAction(myTr("test"), this, SLOT(test4()));
    testMenu->addAction(myTr("fanye"), this, SLOT(fanye()));
    testMenu->addAction(myTr("robfloor"), this, SLOT(robfloor()));
    testMenu->addAction(myTr("clear cookie"), this, SLOT(clearCookie()));
    testMenu->addAction(myTr("签到"), this, SLOT(registersis()));
    //connect(nam, SIGNAL(finished(QNetworkReply*)),  this, SLOT(replyFinished(QNetworkReply*)));

    test2Menu->addAction(myTr("toggle internet mode"), this, SLOT(togglenet()));
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::outputInfor(const char * ba)
{
    QString s = myTr(ba);
    QString s1 = mainW->inforWin->toHtml();
    if (s1.size()> 10000) s1 = s1.left(8000);
    s+=" <br/> ";
    runninglog = s+runninglog;
    mainW->inforWin->setHtml(s+s1);
}
void MainWindow::outputInfor(QString s)
{
    QString s1 = mainW->inforWin->toHtml();
    if (s1.size()> 10000) s1 = s1.left(8000);
    s+=" <br/> ";
    runninglog = s+runninglog;
    mainW->inforWin->setHtml(s+s1);
}
void MainWindow::showrecehtml(QString s, QList<QNetworkReply::RawHeaderPair> hl)
{

    QString s1;
    s1+= " header number  " + QString::number(hl.size()) +"\n\n";
    for (int i = 0; i < hl.size(); ++i) {
        s1+=QString(hl.at(i).first) + "\n";
        s1+=QString(hl.at(i).second) + "\n";
        s1+= "\n";
    }
    s1+="\n\n\n\n";
    s1+=s;
//#ifdef QT_NO_DEBUG
//    return;
//#else
//#endif


    mainW->debugWin->setPlainText(s1);
    mainW->debugWin->show();
}

void MainWindow::showPostInfor(QString s)
{


    mainW->debugWin2->setPlainText(s);
    mainW->debugWin2->show();
}

void clearAll() {
    for (auto i=SisThread::pool.begin(); i!= SisThread::pool.end(); i++) {
        delete *i;
    }
    SisThread::pool.clear();
    SisThread::phrase.clear();

    for (auto i=AuthorSet::pool.begin(); i!= AuthorSet::pool.end(); i++) {
        delete *i;
    }
    AuthorSet::pool.clear();

    for (auto i=KeywordSet::pool.begin(); i!= KeywordSet::pool.end(); i++) {
        delete *i;
    }
    KeywordSet::pool.clear();

}
QString MainWindow::loadTid(QString name)
{
    QJsonDocument jd;
    FileCenter::load("tidpid_setting.ini",jd);
    QString s = jd.object()[name].toString();
    MainWindow::outputInfor("load tid  " + s);
    if (s=="") {
        QMessageBox::warning(0,"","no tid!");
    }
    return s;
}

void MainWindow::loadthreadfromeinternet()
{
    MyMutexLocker mml2("mainwindowtest1",1000);
    if (!mml2.success()) return;
    clearAll();
    //322 383 359 390
    //403
    //391
    //454 278 387 415 366 385 368  402  307  405
// add 371
    int a[17] = {322, 383, 359, 390, 403, 391,  454, 278, 387, 415, 366, 385, 368,  402,  307,  405, 371};
    for (int i=0; i<17; i++) {
        //if (a[i] == 359) continue;   // jiuwen  bucha
        QString s = "forum";
        s+=QString::number(a[i]);
        s+=".txt";
        QJsonDocument jd;
        FileCenter::load(s, jd);
        SisThread::read( jd.array());
        SisThread::getOneForum(QString::number(a[i]));
        QJsonArray aa;
        SisThread::write(aa);
        FileCenter::save(s, QJsonDocument(aa));
        clearAll();
    }
}
void MainWindow::loadThreadPreWork()
{
    clearAll();
    // load previous
    outputInfor("load keywordset");
    KeywordSet::load("k1.txt");
    outputInfor("load authorset");
    AuthorSet::load("k2.txt");

    int a[17] = {322, 383, 359, 390, 403, 391,  454, 278, 387, 415, 366, 385, 368,  402,  307,  405, 371};
    for (int i=0; i<4; i++) {
        QJsonDocument jd;
        QString s = "forum";
        s+=QString::number(a[i]);
        s+=".txt";
        MainWindow::outputInfor("loadfile");
        MainWindow::outputInfor(s);
        FileCenter::load(s,jd);
        SisThread::read(jd.array());
    }
    outputInfor("filter");
    KeywordSet::filterKeyword();
    AuthorSet::filterAuthor();


    outputInfor("build Refer");
    KeywordSet::buildAllRefer();
    AuthorSet::buildAllRefer();
    SisThread::phrase.clear();
//    std::sort(KeywordSet::strange.begin(), KeywordSet::strange.end());
//    for (int i=0; i<KeywordSet::strange.size(); i++) {
//        qDebug()<<KeywordSet::strange[i];
//    }

    for (int i=4; i<17; i++) {
        QJsonDocument jd;
        QString s = "forum";
        s+=QString::number(a[i]);
        s+=".txt";
        MainWindow::outputInfor("loadfile");
        MainWindow::outputInfor(s);
        FileCenter::load(s,jd);
        SisThread::read(jd.array());
        outputInfor("build Refer");
        KeywordSet::buildAllRefer();
        AuthorSet::buildAllRefer();
        SisThread::phrase.clear();
    }

    outputInfor("build Depth Refer");
    KeywordSet::buildAllDepthRefer();
    AuthorSet::buildAllDepthRefer();

    qDebug()<<"keyword final size "<< KeywordSet::qualifySize();
    qDebug()<<"Author final size "<< AuthorSet::qualifySize();
}

void MainWindow::fixPoster()
{
    MyMutexLocker mml2("mainwindow_fixposter",1000);
    if (!mml2.success()) return;

    loadThreadPreWork();

    QString input;
    input = QInputDialog::getText(0,"","author");
    if (input!= "") {
        auto px = AuthorSet::pool["cangshubao"];
        auto x = *px;

        x.tid = x["tid"].toString();
        x.pid = x["pid"].toString();
        x.page = x["page"].toString();
        //x["content"] = "";
        bool same;
        x.finalPost(same);
    }
    outputInfor("function over");
}

void MainWindow::loadthreadfromfileandchecking()
{
    MyMutexLocker mml2("mainwindowtest2",1000);
    if (!mml2.success()) return;
    //NetworkUtil::offInternetDebugMode = true;
    loadThreadPreWork();

    KeywordSet::checking();
    AuthorSet::checking();

    return;
}

void MainWindow::working()
{
    MyMutexLocker mml2("mainwindowtest3",1000);
    if (!mml2.success()) return;
    //NetworkUtil::offInternetDebugMode = true;
    if (NetworkUtil::offInternetDebugMode) outputInfor("offline mode!!!!");

    loadThreadPreWork();

    QString ktid, atid;
    ktid = loadTid("keywordtid");
    atid = loadTid("authortid");
    outputInfor("get poster list");
    int l1 = KeywordSet::qualifySize() + 20;
    if (l1%10 ==0) l1++;
    int l2 = AuthorSet::qualifySize() + 20;
    if (l2%10 ==0) l2++;
    auto a1 = NetworkUtil::getPosterListPid(ktid, l1, "keywordsetposterlist.txt");
    auto a2 = NetworkUtil::getPosterListPid(atid, l2, "authorsetposterlist.txt");
    a1.pop_front();
    a1.pop_front();
    a2.pop_front();
    a2.pop_front();

    outputInfor("ditrubute poster list");
    KeywordSet::distributeAllPostLocation(ktid,a1);
    AuthorSet::distributeAllPostLocation(atid,a2);


    outputInfor("finalAllPost");
    KeywordSet::finalAllPost();
    AuthorSet::finalAllPost();




    if (NetworkUtil::offInternetDebugMode) {
        KeywordSet::save("offline_k1.txt");
        AuthorSet::save("offline_k2.txt");
    }
    else {
        KeywordSet::save("k1.txt");
        AuthorSet::save("k2.txt");
    }

    outputInfor("function over");
}

QList<QString> divMessage(QString  mess) {
    qDebug()<<" div mess total size "<<mess.size();
    QList<QString> re;
    QString sec = "\n";
    const int maxSize = 30000;
    while(mess.size() > maxSize) {
        int index =0;
        while(true) {
            int nindex = mess.indexOf(sec,index + 1);
            if (nindex > maxSize) break;
            if (nindex<0) {
                goto end;
            }
            index = nindex;
        }
        re.push_back( mess.left(index) + myTr("\n未完接楼下"));
        qDebug()<<" div size "<<index;
        mess = mess.right(mess.size() - index - sec.size());
    }
    end:
    re.push_back(mess);
    qDebug()<<" div size "<<mess.size();
    qDebug()<<" div mess over";
    return re;
}

void MainWindow::generateCatalog()
{
    MyMutexLocker mml2("mainwindowgeneratecatalog",1000);
    if (!mml2.success()) return;
    //NetworkUtil::offInternetDebugMode = true;

    loadThreadPreWork();


    QString message, subject;

    outputInfor("catalog");
    FileCenter::outputLog("catalog");
    QString allcata = loadTid("catalogtid");
    QString ktid, atid;
    ktid = loadTid("keywordtid");
    atid = loadTid("authortid");

    qDebug()<<" post cata";
    QList<QString> re;
    QList<QString> sub;
    int count;
    message = KeywordSet::allPostString(keywordSetCmp2, subject);
    re.append(divMessage(message));
    count = 0;
    while(sub.size()< re.size()) {
        sub.push_back( subject + QString::number(++count));
    }
    message = AuthorSet::allPostString(authorSetCmp2, subject);
    re.append(divMessage(message));
    count = 0;
    while(sub.size()< re.size()) {
        sub.push_back( subject + QString::number(++count));
    }
    message = KeywordSet::allPostStringByType(subject);
    re.append(divMessage(message));
    count = 0;
    while(sub.size()< re.size()) {
        sub.push_back( subject + QString::number(++count));
    }
    message = AuthorSet::allPostStringByType(subject);
    re.append(divMessage(message));
    count = 0;
    while(sub.size()< re.size()) {
        sub.push_back( subject + QString::number(++count));
    }
    //    message = KeywordSet::allPostString(keywordSetCmp1);
    //    message = AuthorSet::allPostString(authorSetCmp1);

    qDebug()<<" post cata :" << re.size();

    qDebug()<<" get cata pid";
    auto aa = NetworkUtil::getPosterListPid(allcata, re.size() + 1, "totalposterlist.txt");

    QString mainp = aa[0].toString();
    aa.pop_front();
    aa.pop_front();

    for (int i=0; i<re.size(); i++) {
        MainWindow::outputInfor(QString("cata ")+ QString::number(i+1) + " / " + QString::number(re.size()));
        //NetworkUtil::mustEditPost(allcata, aa[i * 2].toString(), "", "network wrong");
        NetworkUtil::mustEditPost(allcata, aa[i * 2].toString(), sub[i], re[i]);
    }
    QString type = "202";
    message = "";
    subject = myTr("【色城原创作品目录】 总索引 (截止到 [date])");
    message += myTr("[pp=【管理员温馨提示】]\n");
    message += myTr("　　本目录楼层链接，要用 每页帖数10楼（使用默认）\n");
    message += myTr("\n");
    message += myTr("　　若出现楼层链接 跳转楼层不正确的情况，请修改以下：\n");
    message += myTr("　　【控制面板】--【论坛选项】--【每帖页数】--【使用默认】\n");
    message += myTr("\n");
    message += myTr("     目录跳转有时候会差1页， 跳转出错请前后翻一页寻找[/pp]\n");
    message += myTr("色城原创作品 总索引 [bbs]thread-[alltid]-1-1.html[/bbs]\n");
    message += myTr("关键字索引 [bbs]thread-[keywordtid]-1-1.html[/bbs]\n");
    message += myTr("作者索引 [bbs]thread-[authortid]-1-1.html[/bbs]\n");
    message += myTr("\n");
    message += myTr("有建议和疑问，错漏请在总索引[bbs]thread-[alltid]-1-1.html[/bbs]跟帖提出，认真回复奖励 1~10金。\n\n");

    for (int i=0; i<re.size(); i++) {
        QString t = " [bbs]viewthread.php?tid=[alltid]&page=[page]#pid[pid][/bbs]\n";
        t.replace("[page]",aa[i * 2 + 1].toString());
        t.replace("[pid]",aa[i * 2].toString());
        message+= sub[i] + t;
    }

    subject.replace("[date]", QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss"));
    message.replace("[alltid]",allcata);
    message.replace("[keywordtid]",ktid);
    message.replace("[authortid]",atid);

    NetworkUtil::mustEditMainPost(allcata,mainp,type,subject,message);
}
void fenbanmulu() {
    clearAll();
    int forum = QInputDialog::getInt(0,"forum","forum11");
    QJsonDocument jd;
    QString s = "forum";
    s+=QString::number(forum);
    s+=".txt";
    MainWindow::outputInfor("loadfile");
    MainWindow::outputInfor(s);
    FileCenter::load(s,jd);
    SisThread::read(jd.array());

    QString t = QInputDialog::getText(0,"subject","sss");
    QString subject = t;

    auto xx = SisThread::pool;
    QString message;
    for (auto i = xx.begin(); i!= xx.end(); i++) {
        message += (*i)->postString("[date] [type] [title] [refer]") +'\n';
    }
    NetworkUtil::mustSendreply("5752783",subject,message);

}
void zuozhequsuoyin() {
    clearAll();
    int forum = 322;
    QJsonDocument jd;
    QString s = "forum";
    s+=QString::number(forum);
    s+=".txt";
    MainWindow::outputInfor("loadfile");
    MainWindow::outputInfor(s);
    FileCenter::load(s,jd);
    SisThread::read(jd.array());


    auto xx = SisThread::pool;
    QSet<QString> authors;
    for (auto i = xx.begin(); i!= xx.end(); i++) {
        authors.insert( (*i)->author);
    }
    MainWindow::outputInfor("load authorset");
    AuthorSet::load("k2.txt");

    auto yy = AuthorSet::pool;
    QString message;
    for (auto i = yy.begin(); i!= yy.end(); i++) {
        if (authors.contains((*i)->author))
            if ((**i)["content"].toString() != "")
        message += (*i) -> postString("[color=red][author][/color] | [size] ([refer])\n");
    }
    QString subject = "";
    FileCenter::outputLog(message);
}
void tongjitiezi() {
    // qiandao
    QString qiandaoTid = QInputDialog::getText(0,"","qian dao tid");
    auto x2 = NetworkUtil::getPosterList(qiandaoTid,-1, qiandaoTid + "poster.txt");
    qDebug()<<x2;

    QFile file("qiandao.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))             return ;

    QTextStream out(&file);



    QMap<QString, QMap<int, int> > mmsm;
    for (int i=0; i<x2.size(); i++) {
        auto x = x2[i].toObject();
        auto author = x["author"].toString();
        auto datetime = QDateTime::fromString(x["datetime"].toString(), "yyyy-M-d hh:mm");
        mmsm[author][datetime.date().day()] ++ ;
    }
    QString ss;
    for (int i=0; i<=31; i++) {
        ss += "[td]" + QString::number(i) + "[/td]";
    }
    ss += "[td]total[/td][td]time[/td]";
    ss = "[table=98%,white][tr=skyblue]" + ss + "[/tr]";
    for (auto i = mmsm.begin(); i!= mmsm.end(); i++) {
        QString row;
        auto a = i.key();
        auto b = *i;
        row += "[td]" + a + "[/td]";
        int total = 0;
        for (int j=1; j<=31; j++) {
            int f = b[j];
            if (f==0) {
                row += "[td][color=red]" + QString::number(f) + "[/color][/td]";
            }
            else {
                total++;
                row += "[td][color=blue]" + QString::number(f) + "[/color][/td]";
            }
        }
        row+= "[td]"+  QString::number(total) + "[/td]";
        // 获取在线时间
        MainWindow::outputInfor("获取在线时间");
        QString s = "http://174.127.195.200/bbs/space.php?action=viewpro&username=";
        s+= toSisPE(a);
//        QUrl url(s);
  //      loadresult = NetworkUtil::sendquest(url,0,0,NetworkUtil::baseTimeout);
        QString loadresult = NetworkUtil::mustLoadPage(s);
        if (loadresult == "error") {
            row += "[td]N/A[/td]";
            ss+= "[tr]"+row+"[/tr]";
            MainWindow::outputInfor(" 无法访问,可能原因:未登录 ");
            continue;
        }
        int a1;
        QString time1 = getMidString(loadresult,0,"bold",">","<",&a1);
        QString time2 = getMidString(loadresult,a1,"bold",">","<",&a1);

        QString ttt = myTr("总计在线 ")+time1+myTr(" 小时, 本月在线 ")+time2+myTr(" 小时");
        if (time1<0 || ttt.size()>100) {
            row += "[td]N/A[/td]";
            ss+= "[tr]"+row+"[/tr]";
            MainWindow::outputInfor(" 无法访问,可能原因:未登录 ");
            continue;
        }
        MainWindow::outputInfor(ttt);
        row += "[td]" + time2 + "[/td]";
        ss+= "[tr]"+row+"[/tr]";
    }
    ss+= "[/table]";

    out<<ss;
    qDebug()<<ss;

    return;
    auto x1 = NetworkUtil::getPosterList("5735553",-1,"5735553poster.txt");
    qDebug()<<x1;


    QMap<QString, int> mmsi;
    for (int i=0; i<x1.size(); i++) {
        auto x = x1[i].toObject();
        mmsi[x["author"].toString()] ++;
    }
    for (auto i = mmsi.begin(); i!= mmsi.end(); i++) {
        qDebug()<<i.key()<<' '<<*i;
    }


}
void fasongpost() {
    QString hash = NetworkUtil::getHashCode(QUrl("http://174.127.195.201/bbs/forum-403-1.html"));
    QByteArray data = "ratesubmit=yes&score1=1&score2=0&score3=0&score6=0&reason=1&tid=5660398&pid=99522955&page=1&formhash=";
    data.append(hash);
    NetworkUtil::sendquest(QUrl("http://174.127.195.201/bbs/misc.php?action=rate&inajax=1"), &data);

}
void cangjingetongji() {
    qDebug()<< "tong ji cangjingge";
       {
        int a = 405;
        QJsonDocument jd;
        QString s = "forum";
        s+=QString::number(a);
        s+=".txt";
        MainWindow::outputInfor("loadfile");
        MainWindow::outputInfor(s);
        FileCenter::load(s,jd);
        SisThread::read(jd.array());
    }
    auto x = SisThread::pool.keys();
    QString s;
    for (int i=0; i< x.size(); i++) {
        auto y = SisThread::pool[x[i]];
        s+=y->postString();
        s+="\n";
    }

    QFile file("tongji123.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))             return ;

    QTextStream out(&file);

    out << s;
    qDebug()<< "end tong ji";
}

void MainWindow::test4()
{
    MyMutexLocker mml2("mainwindowtest4",1000);
    if (!mml2.success()) return;
    //tongjitiezi();
    cangjingetongji();
 }
void MainWindow::robfloor()
{
    MyMutexLocker mml2("mainwindowrobfloor",1000);
    if (!mml2.success()) return;
    static int aaa = 0;
    while (true) {
        const char * ss[4] = {"4098214"};

            waitMillisec(500);
            QString sss0 = "http://174.127.195.200/bbs/thread-";
            sss0+= ss[0];
            sss0+= "-10000-1.html";
            QUrl url(sss0);
            QString loadresult;
            while(loadresult.size()<100) loadresult= NetworkUtil:: sendquest(url);
            QString sss1 = getMidString(loadresult,0,"d","<em>&nbsp;","&nbsp;</em>");
            int total;
            total = sss1.toInt();
            qDebug()<<total;
            outputInfor(QString::number(total));
            if (total >= 8886 ) {
                QString message = myTr("我要抢8888楼");
                qDebug()<< "fanye " << ++aaa;
                NetworkUtil::mustSendreply(ss[0],"",message);
                message = myTr("8888楼是我的");
                qDebug()<< "fanye " << ++aaa;
                NetworkUtil::mustSendreply(ss[0],"",message);
                return;
            }
    }
}

void MainWindow::fanye()
{
    MyMutexLocker mml2("mainwindowfanye",1000);
    if (!mml2.success()) return;
    static int aaa = 0;
    while (true) {
        const char * ss[4] = {"4098214","4173362","4726825","4438133"};
        for (int i=0; i<4; i++) {
            waitMillisec(500);
            QString sss0 = "http://174.127.195.200/bbs/thread-";
            sss0+= ss[i];
            sss0+= "-10000-1.html";
            QUrl url(sss0);
            QString loadresult;
            while(loadresult.size()<100) loadresult= NetworkUtil:: sendquest(url);
            QString sss1 = getMidString(loadresult,0,"d","<em>&nbsp;","&nbsp;</em>");
            int total;
            total = sss1.toInt();
            qDebug()<<total;
            outputInfor(QString::number(total));
            if (total>100 && total % 10 ==0) {
                QString message = myTr("全自动翻页试用中 ") + QString::number(total+1);
                qDebug()<< "fanye " << ++aaa;
                while(! NetworkUtil:: sendreply(ss[i],message))  { QCoreApplication::processEvents();  }
            }

        }
    }

}

void MainWindow::test6()
{

}
void MainWindow::clearCookie()
{
    outputInfor("clear cookies");
    NetworkUtil::clearCookies();
}

void MainWindow::loginsis()
{
    NetworkUtil::loginsis();
}

void MainWindow::inputdata()
{
    Util::inputData();
}
void MainWindow::togglenet()
{
    NetworkUtil::offInternetDebugMode = ! NetworkUtil::offInternetDebugMode;
    if (NetworkUtil::offInternetDebugMode) {
        outputInfor("offline");
    }
    else {
        outputInfor("online");
    }
}
void MainWindow::setTimeout()
{
    NetworkUtil::baseTimeout = QInputDialog::getInt(0,"get time out","get time out",NetworkUtil::baseTimeout);
}

void MainWindow::registersis()
{
    static QMutex regmutex;
    MyMutexLocker mml1(&regmutex);
    if (!mml1.success()) return;

    QString loadresult ;

    // get  time and place
    outputInfor("获取在线时间以及版面");
    QString s = "http://174.127.195.200/bbs/space.php?action=viewpro&username=";
    s+= toSisPE(Util::username);
    QUrl url(s);
    loadresult = NetworkUtil::sendquest(url,0,0,NetworkUtil::baseTimeout);
    if (loadresult == "error") {
        return;
    }
    int a1,a2;
    QString time1 = getMidString(loadresult,0,"bold",">","<",&a1);
    QString time2 = getMidString(loadresult,a1,"bold",">","<",&a1);

    QString ttt = myTr("总计在线 ")+time1+myTr(" 小时, 本月在线 ")+time2+myTr(" 小时");
    if (time1<0 || ttt.size()>100) {
        outputInfor(" 无法访问,可能原因:未登录 ");
        return;
    }
    outputInfor(ttt);

    a1 = loadresult.indexOf(myTr("版主"));
    a2 = loadresult.indexOf(myTr("勋章"));
    QString bbb;
    while(true) {
        QString b = getMidString(loadresult,a1,"html",">","<", &a1);
        if (a1<0 || a1 >a2) break;
        if (bbb!="")  bbb+=",";
        bbb+= b;
    }
    outputInfor(bbb);

    QString mes ="ID:" + Util::username +"\n";
    mes += myTr("日期:") +QDateTime::currentDateTime().date().toString("yyyy-MM-dd") +"\n";
    mes += myTr("版区：") + bbb +"\n";
    mes += myTr("在线时间：") + ttt +"\n";

    if (NetworkUtil::sendreply(Util::regpage,mes,0,0,"",NetworkUtil::baseTimeout)) {
        outputInfor("签到成功");
    }
}


void MainWindow::closeEvent(QCloseEvent *)
{
    Util::writeData();
    FileCenter::save("runninglog.txt",runninglog);
    //QCoreApplication::exit(0);
    exit(0);
}
//MainWindow *MainWindow::getMainW() const
//{
//    return mainW;
//}

void MainWindow::setMainW(MainWindow *value)
{
    mainW = value;
}

