#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <cassert>
#include <QString>
#include <qDebug>
#include <QMap>
#include <QSet>
#include <QMessageBox>

#include "keywordset.h"
#include "authorset.h"
#include "sisthread.h"
#include "filecenter.h"
#include "util.h"
#include "mainwindow.h"
#include "networkutil.h"

QMap<QString, KeywordSet*> KeywordSet:: pool;
KeywordSet:: KeywordSet(const QString &a)
{

    keyword = a;
    (*this)["keyword"] = a;
    (*this)["tid"] = QString("");
    (*this)["page"] = QString("");
    (*this)["pid"] = QString("");
    (*this)["content"] = QString("");
    assert(pool.count(keyword) == 0);
    pool[keyword] = this;
    ifqualify = 0;
}

KeywordSet:: KeywordSet(const QJsonObject & jo): QJsonObject(jo)
{
    keyword = jo["keyword"].toString();
    tid = jo["tid"].toString();
    page = jo["page"].toString();
    pid = jo["pid"].toString();

    assert(pool.count(keyword) == 0);
    pool[keyword] = this;
    ifqualify = 0;
}
QList< QString >  KeywordSet::strange;
void KeywordSet:: checkKeyword(const QString & title, const QString & author)
{
    if (title.count(myTr("【")) != 1 || title.count(myTr("【")) != title.count(myTr("】")) ) {

        strange.push_back(title + " " + author);
    }
    int a1,a2;
    a1 = 0;
    a2 = 0;
    while(true) {
        a1 = title.indexOf(myTr("【"),a2);
        a2 = title.indexOf(myTr("】"),a1);
        if (a1<0) break;
        if (a2<0) break;
        QString k = title.mid(a1+1,a2-a1-1);
        if (k.size()<2) continue;
        if (pool.count(k)>0) continue;
        pool[k] = new KeywordSet(k);
    }
}



void KeywordSet::buildRefer()
{
    QList<SisThread*> v =SisThread::phrase.values(keyword);

    for (int i=0; i< v.size(); i++) {
        SisThread * st = v.at(i);
        sisthreads.push_back(st);
    }
    std::sort(sisthreads.begin(),sisthreads.end());
    auto x = std::unique(sisthreads.begin(), sisthreads.end());
    sisthreads.erase(x,sisthreads.end());
    for (int i=0; i<sisthreads.size(); i++) {
        sisthreads[i]->backReferKeyword.push_back(this);
    }
}

void KeywordSet::buildDepthRefer()
{
    for (int i=0; i<sisthreads.size(); i++) {
        SisThread * st = sisthreads.at(i);
        for (int j=0; j<st->backReferAuthor.size(); j++) {
            authors.push_back(st->backReferAuthor.at(j));
        }
    }
    std::sort(authors.begin(),authors.end());
    auto x = std::unique(authors.begin(), authors.end());
    authors.erase(x, authors.end());
}


void KeywordSet::buildAllRefer()
{
    for (auto i = pool.begin(); i!= pool.end(); i++) (*i)->buildRefer();
}

void KeywordSet::buildAllDepthRefer()
{
    for (auto i = pool.begin(); i!= pool.end(); i++) (*i)->buildDepthRefer();
}
void KeywordSet::model()
{
    QJsonArray ja;
    for (int i=0; i<sisthreads.size(); i++) {
        ja.push_back(sisthreads[i]->tid);
    }
    (*this)["tidset"] = ja;
    QJsonArray ja1;
    for (int i=0; i<authors.size(); i++) {
        ja1.push_back(authors[i]->author);
    }
    (*this)["authorset"] = ja1;
}

void KeywordSet::read(const QJsonArray & a)
{
    for (int i=0; i<a.size(); i++) {
        new KeywordSet(a.at(i).toObject());
    }
    QString s = "read KeywordSet from file,  size " + QString::number(a.size());
    MainWindow::outputInfor(s);
}

void KeywordSet::write(QJsonArray & a)
{
    qDebug()<<"pool size "<< pool.size();
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        auto x = *i;
        (x)->model();
       // if (x->authors.size()>2) qDebug()<<x->keyword<<' '<<x->authors.size();
        a.append(*x);
    }
}

void KeywordSet:: filterKeyword()
{
    QJsonDocument jd;
    FileCenter::load("keywordFilter.ini",jd);
    QJsonArray include, exclude;
    include = jd.object()["include"].toArray();
    exclude = jd.object()["exclude"].toArray();
    qDebug()<<jd.object();
    for (int i=0; i<exclude.size(); i++) {
        QString k = exclude.at(i).toString();
        k = full2half(k);
        if (pool.count(k)==0) continue;
        delete pool.take(k);
    }
    for (int i=0; i<include.size(); i++) {
        QString k = include.at(i).toString();
        k = full2half(k);
        if (pool.count(k)>0) continue;
        pool[k] = new KeywordSet(k);
    }
}

bool KeywordSet::qualify()
{
    if (ifqualify ==1) return true;
    if (ifqualify ==2) return false;
    int count = 0;
    for (int i=0; i< sisthreads.size(); i++) {
        QString forum =  sisthreads[i]->forum;
        if (forum == "322" || forum == "383" || forum == "359" || forum == "390" ) count ++;
    }

    if (count>= Util::minTopicThreads) ifqualify = 1; else ifqualify = 2;
    if (ifqualify == 1) return true; else return false;
}

bool KeywordSet::getPostLocation()
{
    tid = "5737719";
    QString message = keyword;
    message += "  working ";
    int count = 0;
    while (!NetworkUtil::sendreply(tid,message,&pid, &page)) {
        count++;
       // if (count>5) return false;
    }
    return true;
}
bool KeywordSet::getAllPostLocation()
{
    bool ok = true;
    for (auto i=pool.begin(); i!= pool.end(); i++) {
        if ((*i)->qualify()) ok = (*i) ->getPostLocation();
        if (!ok) return false;
    }
    return true;
}

bool KeywordSet::distributeAllPostLocation(QString tid,  QJsonArray ja)
{
    assert(qualifySize() *  2 <= ja.size());
    QMap<QString, KeywordSet * > pid2keyword;
    for (auto i=pool.begin(); i!= pool.end(); i++) {
        KeywordSet * x = *i;
        if (!(x->qualify())) {
            (*x)["tid"] = QString("");
            (*x)["pid"] = QString("");
            (*x)["page"] = QString("");
            (*x)["content"] = QString("");
            continue;
        }
        if (x->tid != tid) continue;
        pid2keyword[x->pid] = x;
    }
    QSet<KeywordSet*> existK;
    QSet<QString> existP;
    for (int i=0; i<ja.size()/2; i++) {
        QString p = ja[i*2].toString();
        if (pid2keyword.count(p)>0) {
            KeywordSet * x = pid2keyword[p];
            x->page = ja[i*2+1].toString();
            existK.insert(x);
            existP.insert(p);
        }
    }

    int index = 0;

    for (auto i=pool.begin(); i!= pool.end(); i++) {
        KeywordSet * x = *i;
        if (!(x->qualify())) continue;
        if (existK.contains(x)) continue;
        while( existP.contains( ja[index].toString())  ) index += 2;
        x->tid = tid;
        x->pid = ja[index].toString();
        index++;
        x->page = ja[index].toString();
        index++;
        (*x)["content"] = QString("");
    }
    if (index > ja.size()) {
        QMessageBox::warning(0,0,"not enough poster room");
    }

    return true;
}

QString KeywordSet::postString(QString format)
{
    format.replace("[keyword]",keyword);
    format.replace("[size]", QString::number(sisthreads.size()));
    format.replace("[refer]", "[bbs]viewthread.php?tid=" + tid +"&page="+page+ "#pid" + pid + "[/bbs]");

    //viewthread.php?tid=5660398&page=2#pid99352882
    //re += "[color=red]" + keyword + "[/color] | " + QString::number(sisthreads.size()) + "  [bbs]viewthread.php?tid=" + tid +"&page="+page+ "#pid" + pid + "[/bbs]";
    return format;
}
bool debugout;
bool KeywordSet::finalPost(bool & same)
{

    std::sort(sisthreads.begin(),sisthreads.end(),sisThreadCompareByTid);
    auto xxx = std::unique(sisthreads.begin(), sisthreads.end(), [](SisThread * x, SisThread * y) { return (x -> tid) ==  (y->tid); });
    sisthreads.erase(xxx,sisthreads.end());

    QString subject = keyword +myTr("  关键字合集");
    QString message;
    std::sort(sisthreads.begin(), sisthreads.end(), sisThreadCompare);
    int x = 0;
    QString lastAuthor;
    for (x=0; x<sisthreads.size(); x++) {
        auto st = sisthreads[x];
        QString & forum = st->forum;
        if (forum == "322" || forum == "383" || forum == "359" || forum == "390" ) {}
        else break;
        if (st->author!= lastAuthor) {
            lastAuthor = st->author;
            message += myTr("\n     作者: ") + lastAuthor + "\n";
        }
        message+= sisthreads[x]->postString() + '\n';
    }

    if (message.size()<NetworkUtil::maxPostMesageSize) {
        //message += myTr("\n\n 其他 \n");
        bool b_mulu = false, b_pinglun =false, b_xiazai=false;
        for (; x<sisthreads.size(); x++) {
            auto st = sisthreads[x];
            if (st->forum == "403") {
                if (!b_mulu) {
                    message += myTr("\n\n 目录 \n");
                    b_mulu = true;
                }
            }
            else if (st->forum == "391") {
                if (!b_pinglun) {
                    message += myTr("\n\n 评论 \n");
                    b_pinglun= true;
                }
            }
            else {
                if (!b_xiazai) {
                    message += myTr("\n\n 下载 \n");
                    b_xiazai= true;
                }
            }
            message+= st->postString() + '\n';
        }
    }

    int frontPart = message.size();
    if (authors.size()>0) {
        bool first = true;
        for (int i=0; i<authors.size(); i++) {
            if (authors[i]->pid =="") continue;
            if (first) {
                message += myTr("\n\n 相关作者链接 \n");
                first = false;
            }
            message+= authors[i]->postString() + '\n';
        }
    }

    if (message.left(frontPart) != (*this)["content"].toString().left(frontPart) ) {
        NetworkUtil::mustEditPost(tid,pid,subject,message);
        qDebug()<<tid<<' '<<pid<<' '<<page;
        static int difcount = 0;
        if (++difcount <= 2) {
            FileCenter::outputLog(message.left(frontPart));
            FileCenter::outputLog((*this)["content"].toString().left(frontPart));
        }
        same = false;
    }
    else {
        //MainWindow::outputInfor("same, jump.");
        same = true;
    }
    (*this)["content"] = message;
    (*this)["tid"] = tid;
    (*this)["page"] = page;
    (*this)["pid"] = pid;

    return true;
}
bool KeywordSet::finalAllPost()
{
    bool ok = true;
    int total = qualifySize();
    int count = 0;
    QTime timer;
    timer.start();
    for (auto i=pool.begin(); i!= pool.end(); i++) {
        auto x = *i;
        if (!x->qualify()) continue;
        if (timer.elapsed() >60000) {
            qDebug()<<"temp save";
            save("temp_k1.txt");
            timer.restart();
        }
        bool same;
        ok = x ->finalPost(same);
        QCoreApplication::processEvents();
        if (!ok) return false;
        ++count;
        if (!same) {
            qDebug()<<" final post "<<count<<"  total "<<total;
            MainWindow::outputInfor(QString("keyword final post ")+ QString::number(count)+" total "+  QString::number(total));
        }
    }
    save("temp_k1.txt");
    return true;
}


int KeywordSet::qualifySize()
{
    int count = 0;
    for (auto i=pool.begin(); i!= pool.end(); i++) {
        if ((*i)->qualify()) count++;
    }
    return count;
}


bool KeywordSet::save(QString file)
{
    QJsonArray b1;
    write(b1);
    return FileCenter::save(file , QJsonDocument(b1));
}

bool KeywordSet::load(QString file)
{
    QJsonDocument jd;
    bool ok;
    ok = FileCenter::load(file,jd);
    if (!ok) return false;

    FileCenter::load(file,jd);
    QJsonArray b1 = jd.array();
    for (int i=0; i<b1.size(); i++) {
        new KeywordSet(b1[i].toObject());
    }
    return true;
}

void KeywordSet::checking()
{
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        auto x = *i;
        if (x->authors.size()>2) qDebug()<<x->keyword<<' '<<x->authors.size();
    }
    qDebug()<<"\n\n\n";
}



QDate KeywordSet::lastPostTime()
{
    if (lastPost.isNull()) {
        lastPost.setDate(1970,1,1);
        for (auto i = sisthreads.begin(); i!= sisthreads.end(); i++) {
            auto x = (*i)->date;
            if (x > lastPost) lastPost = x;
        }
    }
    assert(lastPost.isValid());
    return lastPost;
}

bool keywordSetCmp1(KeywordSet* a, KeywordSet* b)
{
    return  a->keyword< b->keyword;
}

bool keywordSetCmp2(KeywordSet* a, KeywordSet*b)
{
    if (a->lastPostTime() != b->lastPostTime()) return a->lastPostTime() > b->lastPostTime();
    return  a->keyword < b->keyword;
}


QString KeywordSet::allPostString(bool cmp(KeywordSet *, KeywordSet *), QString & subject)
{
    QList<KeywordSet*> la;
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        la.push_back(*i);
    }
    std::sort(la.begin(), la.end(),cmp);
    QString message;
    message = myTr("关键字合集索引 \n");

    bool outtime = false;

    if (cmp == keywordSetCmp2) {
        message += myTr("按更新时间倒序 \n");
        outtime = true;
    }
    else if (cmp == keywordSetCmp1) {
        message += myTr("按词典序 \n");
    }
    subject = message;
    subject.replace('\n',"");
    int lastMonth = 13;
    for (auto i = la.begin(); i!= la.end(); i++) {
        if (!(*i) -> qualify()) continue;
        int m = (*i)->lastPostTime().month();
        if (outtime && m != lastMonth) {
            lastMonth = m;
            message += "\n";
            message += QString::number((*i)->lastPostTime().year());
            message += "-"+QString::number(m)+" \n";
        }
        message += (*i) -> postString("[color=red][keyword][/color] | [size] ([refer])\n");
    }
    //FileCenter::outputLog(message);
    return message;
}

QString KeywordSet::allPostStringByType(QString & subject)
{
    const char * types[16] = {"乱伦", "人妻", "农村", "凌辱", "历史", "古典武侠", "另类", "同人", "女友", "春色群交", "校园", "武侠", "玄幻", "绿母", "都市", "肥水自耕"};

    QMap<QString, QList<KeywordSet *> >  table;
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        KeywordSet * x = *i;
        if (!x->qualify()) continue;
        auto z = x->sisthreads;
        for (int j=0; j<z.size(); j++) {
            SisThread * t = z[j];
            table[t->type].push_back(x);
        }
    }
    const char * alltypes[100] = {"中篇","中篇全本","乱伦","交流","人妻","作品合集","作者合集","全部完成","其它","农村","凌辱","历史","原创典藏","原创杂志","古典武侠","另类",
   "合集","同人","女友","心语小楼","成　人","成人原创","成人转帖","春色群交","有声合集","有声完成","有声连载","未删节全本","校园","武侠","河图实封",
   "玄幻","目前最全","短篇","系列合集","绿母","转帖杂志","连载更新","都市","长篇","长篇全本","非成人","非成人原创", "肥水自耕" };
    for (auto i = table.begin(); i!=table.end(); i++) {
        if (i.key() == "") continue;
        bool dif = true;
        for (int j=0; j<44; j++) {
            if (i.key() == myTr(alltypes[j])) dif = false;
        }

        if (dif) FileCenter::outputLog( " new type " + i.key());
    }
    QString message;
    message = myTr("关键字分类索引 \n");
    subject = message;
    subject.replace('\n',"");
    for (int i=0; i<16; i++) {
        QString type = myTr(types[i]);
        if ( type == "") continue;
        message += "\n" + type + "\n";

        auto z =  table[type];
        std::sort(z.begin(),z.end(),keywordSetCmp1);
        auto ii = std::unique(z.begin(),z.end());
        z.erase(ii, z.end());
        for (int j=0; j<z.size(); j++) {
             message += z[j] -> postString("[color=red][keyword][/color] | [size] ([refer])\n") ;
        }
    }
    return message;
}
