#include <cassert>
#include <QDebug>
#include <QJsonDocument>
#include <QUrl>
#include <QTextStream>
#include <QCoreApplication>
#include "sisthread.h"
#include "authorset.h"
#include "keywordset.h"
#include "networkutil.h"
#include "util.h"
#include "mainwindow.h"

QMap<QString, SisThread * > SisThread::pool;
QList< SisThread * > SisThread::trashList;
QMultiMap<QString, SisThread *> SisThread::phrase;


bool sisThreadCompareByTid(SisThread *a, SisThread*b)
{
    int dif =a->tid.toInt() - b->tid.toInt();
    if (dif<0) return true;
    return false;

}

bool sisThreadCompare(SisThread * a, SisThread* b)
{
    static QMap<QString, int> * ss = nullptr;
    if (ss==nullptr) {
        ss = new QMap<QString, int>();
        QMap<QString, int> & ss1 = *ss;
        int a[17] = {322, 383, 359, 390, 403, 391,  454, 278, 387, 415, 366, 385, 368,  402,  307,  405, 371};
        for (int i=0; i<17; i++) {
            ss1[QString::number(a[i])] = 10;
        }
        ss1["322"] = 1;
        ss1["383"] = 1;
        ss1["359"] = 1;
        ss1["390"] = 2;
        ss1["403"] = 3;
        ss1["391"] = 4;
    }
    QMap<QString, int> & ss1 = *ss;
    //322 383 359 390

    int  x = ss1[a->forum];
    int  y = ss1[b->forum];
    if (x<y) return true;
    if (x>y) return false;

    if (x==1) {
        if (a->author < b->author) return true;
        if (a->author > b->author) return false;
    }
    int dif =a->tid.toInt() - b->tid.toInt();
    if (dif<0) return true;
    if (dif>0) return false;

    return (a->title < b->title);
}

void SisThread::addOne(const QString & s, SisThread *  o)
{
    int n = s.size();
    const int minsize = 1;
    for (int i=0; i<n; i++) {
        for (int j=i+minsize; j<n; j++) {
            phrase.insert(s.mid(i,j-i+1),o);
        }
    }
}

SisThread::SisThread()
{
    assert(false);
    qDebug()<<"error1111!!!!!!!!!!!";

}
void SisThread::init()
{
    if (false) {
        trashList.push_back(this);
        return;
    }
    title = full2half(title);
    pool [tid] = this;

    if (!filter()) return;

    addOne(title,this);

    //322 383 359 390
    if (forum == "322" || forum == "383" || forum == "359" || forum == "390" )
    {
        AuthorSet::addOneThread(author,this);
        KeywordSet::checkKeyword(title, author);
    }
}

SisThread::SisThread(const QJsonObject & jo): QJsonObject(jo)
{

    type = jo["type"].toString();
    title = jo["title"].toString();
    author = jo["author"].toString();
    tid = jo["tid"].toString();
    forum = jo["forum"].toString();
    date = QDate::fromString(jo["date"].toString(),"yyyy-M-d");

}

SisThread::SisThread(const QString & page, int ii, const QString & forum)
{

    this->forum = forum;
    (*this)["forum"] = forum;
    assert(forum!="");
    ii = page.indexOf("normalthread",ii);
    int last = page.indexOf("</tbody>",ii);
    if (last<0) {
        assert(false);
        qDebug()<<"error1111!!!!!!!!!!!";
    }

    int aa;
    QString s;
    s = getMidString(page,ii,"typeid=",">","<", &aa);
    type = s;
    (*this)["type"] = s;

    QString s1 = "viewthread.php";
    s = getMidString(page,ii,s1,"=","&", &aa);
    if (aa<0 || aa > last) {
        assert(false);
        qDebug()<<"error!!!!!!!!!!!";
    }
    else {
        tid= s;
        (*this)["tid"] = s;
        assert(s!="");
    }
    int d = page.indexOf("span",ii);
    s = getMidString(page,d,s1,">","<", &aa);
    if (aa<0 || aa > last) {
        assert(false);
        qDebug()<<"error!!!!!!!!!!!";
    }
    else {
        title= s;
        (*this)["title"] = s;
        assert(s!="");
    }

    QString s2 = "space.php";
    QString s3 = "<td class=\"author\">";
    int a1 = page.indexOf(s3,ii);
    int a2 = page.indexOf(s2,a1);
    int a3 = page.indexOf("<em>",a1);
    if (a1<a2 && a2<a3 ) {
        s = getMidString(page,ii,s2,">","<", &aa);
        if (aa<0 || aa > last) {
            assert(false);
            qDebug()<<"error!!!!!!!!!!!";
        }
        else {
            author= s;
            (*this)["author"] = s;
            assert(s!="");
        }
    }
    else {
        author= myTr("匿名");
        (*this)["author"] = author;
    }
    s = getMidString(page,ii,s3,"<em>","</em>", &aa);
    if (aa<0 || aa > last) {
        assert(false);
        qDebug()<<"error!!!!!!!!!!!";
    }
    else {
        date = QDate::fromString(s,"yyyy-MM-dd");
        (*this)["date"] = s;
        assert(s!="");
    }

}

QList<SisThread *> SisThread::getOneForum(const QString & forum)
{
    QString sss = "http://174.127.195.201/bbs/forumdisplay.php?fid=";
    sss+= forum;
    sss+= "&filter=0&orderby=dateline&ascdesc=DESC";
    QUrl url(sss);
    QString page;
    while(page.size()<100) page = NetworkUtil::sendquest(url);

    // get page number;
    int start = page.indexOf("class=\"pages_btns");
    int end = page.indexOf("/div",start);
    int maxPage = 1;
    while (true) {
        QString s = getMidString(page,start,"page=","=","\"",&start);
        if (start >end) break;
        int t = s.toInt();
        if (t>maxPage) maxPage = t;
    }
    QString output;
    QTextStream ts(&output);
    ts<< "load forum "<<forum<<"   page  "<<maxPage<<endl;
    qDebug()<<output;
    MainWindow::outputInfor(output);

    sss+="&page=";
    int lastPageSize = 0;
    QList<SisThread *> re;
    for (int i=1; i<=maxPage; i++) {
        QUrl url(sss+ QString::number(i));
        QString page;
        while(page.size()<100) page = NetworkUtil::sendquest(url);
        auto x = getOnePage(page,forum);

        output = "";
        ts<<"forum "<<forum<<" maxpage "<<maxPage<< " page "<<i<<"  total  "<<re.size()<<endl;
        qDebug()<<output;
        MainWindow::outputInfor(output);

        int count = 0;
        for (int i=0; i<x.size(); i++) {
            if ( pool.count(x[i]->tid) > 0) count++;
            x[i]->init();
        }
        qDebug()<< "total get "<<x.size()<< "   same "<<count;
        if (x.size() == lastPageSize && x.size()!=0 && count == x.size()) {
            MainWindow::outputInfor(" reach final desti  ");
            break;
        }
        re.append(x);
        lastPageSize = x.size();
        //re.append();
    }
    qDebug()<<"load forum over";
    return re;
}

QList<SisThread *> SisThread::getOnePage(const QString & page, QString  forum)
{
    QList<SisThread *> re;
    QString s = "normalthread";

    int index = page.indexOf("threadlist");
    assert(index>0);
    while(true) {
        index = page.indexOf(s,index);
        if (index<0) break;
        re.push_back( new SisThread(page,index,forum)  );
        //QString s0 = QJsonDocument(QJsonObject(*re.back())).toJson();
        //qDebug()<<s0;
        index +=10;
    }
    return re;
}
bool SisThread::filter()
{
    if ( type == myTr("版务")) return false;
    if ( type == myTr("版 务")) return false;
    if ( type == myTr("求文找书")) return false;
    if ( type == myTr("求助")) return false;
    if ( title.indexOf(myTr("欢迎色城")) >=0) return false;
    return true;
}

void SisThread::read(const QJsonArray & a)
{
    for (int i=0; i<a.size(); i++) {
        auto x = new SisThread(a.at(i).toObject());
        x ->init();
        if (i%100==0) QCoreApplication::processEvents();
    }
    QString s = "read thread from file,  size " + QString::number(a.size());
    MainWindow::outputInfor(s);
}

void SisThread::write(QJsonArray & a)
{
    qDebug()<<"pool size "<< pool.size();
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        a.append(*(*i));
    }
}
QString SisThread::postString(QString format)
{
    format.replace("[type]", type);
    format.replace("[date]", date.toString("yyyy-MM-dd "));
    format.replace("[title]", title);
    format.replace("[refer]", "[bbs]thread-" + tid + "-1-1.html[/bbs]");
    format.replace("[author]", author);
    return format;
}
