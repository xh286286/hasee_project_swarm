#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <cassert>
#include <QList>
#include <QMap>
#include <QSet>
#include <QMessageBox>

#include <algorithm>


#include "networkutil.h"
#include "authorset.h"
#include "keywordset.h"
#include "sisthread.h"
#include "mainwindow.h"
#include "util.h"
#include "filecenter.h"
QMap<QString, AuthorSet*> AuthorSet:: pool;
AuthorSet:: AuthorSet(const QString &a)
{
    author = a;
    (*this)["author"] = a;
    (*this)["tid"] = QString("");
    (*this)["page"] = QString("");
    (*this)["pid"] = QString("");
    (*this)["content"] = QString("");
    assert(pool.count(author) == 0);
    pool[author] = this;
    ifqualify = 0;
}

AuthorSet:: AuthorSet(const QJsonObject & jo): QJsonObject(jo)
{
    author = jo["author"].toString();
    tid = jo["tid"].toString();
    page = jo["page"].toString();
    pid = jo["pid"].toString();

    assert(pool.count(author) == 0);
    pool[author] = this;
    ifqualify = 0;
}

void AuthorSet::addOneThread(const QString & author, SisThread * st )
{
    if (pool.count(author) ==0) new AuthorSet(author);

    pool[author]->sisthreads.push_back(st);
}

void AuthorSet::buildRefer()
{
    if (author.size() >2) {
        QList<SisThread*> v =SisThread::phrase.values(author);
        for (int i=0; i< v.size(); i++) {
            SisThread * st = v.at(i);
            sisthreads.push_back(st);
        }
    }

    std::sort(sisthreads.begin(),sisthreads.end());
    auto x = std::unique(sisthreads.begin(), sisthreads.end());
    sisthreads.erase(x,sisthreads.end());

    for (int i=0; i<sisthreads.size(); i++) {
        sisthreads[i]->backReferAuthor.push_back(this);
    }
}
void AuthorSet::buildDepthRefer()
{
    for (int i=0; i<sisthreads.size(); i++) {
        SisThread * st = sisthreads.at(i);
        for (int j=0; j<st->backReferKeyword.size(); j++) {
            keywords.push_back(st->backReferKeyword.at(j));
        }
    }
    std::sort(keywords.begin(),keywords.end());
    auto x = std::unique(keywords.begin(), keywords.end());
    keywords.erase(x,keywords.end());
}

void AuthorSet::model()
{
    QJsonArray ja;
    for (int i=0; i<sisthreads.size(); i++) {
        ja.push_back(sisthreads[i]->tid);
    }
    (*this)["tidset"] = ja;
    QJsonArray ja1;
    for (int i=0; i<keywords.size(); i++) {
        ja1.push_back(keywords[i]->keyword);
    }
    (*this)["keywordset"] = ja1;
}

void AuthorSet::buildAllRefer()
{
    for (auto i = pool.begin(); i!= pool.end(); i++) (*i)->buildRefer();
}

void AuthorSet::buildAllDepthRefer()
{
    for (auto i = pool.begin(); i!= pool.end(); i++) (*i)->buildDepthRefer();
}


void AuthorSet::read(const QJsonArray & a)
{
    for (int i=0; i<a.size(); i++) {
        new AuthorSet(a.at(i).toObject());
    }
    QString s = "read AuthorSet from file,  size " + QString::number(a.size());
    MainWindow::outputInfor(s);
}

void AuthorSet::write(QJsonArray & a)
{
    qDebug()<<"pool size "<< pool.size();
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        auto x = *i;
        (x)->model();
      //  if (x->keywords.size()>2) qDebug()<<x->author<<' '<<x->keywords.size();
        a.append(*x);
    }
}

void AuthorSet::filterAuthor()
{
    QJsonDocument jd;
    FileCenter::load("authorFilter.ini",jd);
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
        pool[k] = new AuthorSet(k);
    }
}

bool AuthorSet::qualify()
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

bool AuthorSet::getPostLocation()
{
    tid = "5737718";
    QString message = author;
    message += "  working ";
    int count = 0;
    while (!NetworkUtil::sendreply(tid,message,&pid, &page)) {
        count++;
       // if (count>5) return false;
    }
    return true;
}

bool AuthorSet::getAllPostLocation()
{
    assert(false);
    bool ok = true;
    for (auto i=pool.begin(); i!= pool.end(); i++) {
        if ((*i)->qualify()) ok = (*i) ->getPostLocation();
        if (!ok) return false;
    }
    return true;
}
bool AuthorSet::distributeAllPostLocation(QString tid, QJsonArray ja)
{
    assert(qualifySize() *  2 <= ja.size());
    QMap<QString, AuthorSet *> pid2author;
    for (auto i=pool.begin(); i!= pool.end(); i++) {
        AuthorSet * x = *i;
        if (!(x->qualify())) {
            (*x)["tid"] = QString("");
            (*x)["pid"] = QString("");
            (*x)["page"] = QString("");
            (*x)["content"] = QString("");
            continue;
        }
        if (x->tid != tid) continue;
        pid2author[x->pid] = x;
    }
    QSet<AuthorSet * > existK;
    QSet<QString> existP;
    for (int i=0; i<ja.size()/2; i++) {
        QString p = ja[i*2].toString();
        if (pid2author.count(p)>0) {
            AuthorSet * x = pid2author[p];
            x->page = ja[i*2+1].toString();
            existK.insert(x);
            existP.insert(p);
        }
    }

    int index = 0;

    for (auto i=pool.begin(); i!= pool.end(); i++) {
        AuthorSet * x = *i;
        if (!(x->qualify())) continue;
        if (existK.contains(x)) continue;
        while( existP.contains( ja[index].toString()) ) index += 2;
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

QString AuthorSet::postString(QString format)
{
    format.replace("[author]",author);
    format.replace("[size]", QString::number(sisthreads.size()));
    format.replace("[refer]", "[bbs]viewthread.php?tid=" + tid +"&page="+page+ "#pid" + pid + "[/bbs]");
    //re += "[color=red]" + author + "[/color] | " + QString::number(sisthreads.size()) +  "  [bbs]viewthread.php?tid=" + tid +"&page="+page+ "#pid" + pid + "[/bbs]";
    return format;
}

bool AuthorSet::finalPost(bool & same)
{

    std::sort(sisthreads.begin(),sisthreads.end(),sisThreadCompareByTid);
    auto xxx = std::unique(sisthreads.begin(), sisthreads.end(),[](SisThread * x, SisThread * y) { return (x -> tid) ==  (y->tid); });
    sisthreads.erase(xxx,sisthreads.end());

    QString subject = myTr("作者合集: ")+author;
    QString message;
    std::sort(sisthreads.begin(), sisthreads.end(), sisThreadCompare);

    int x = 0;
    for (x=0; x<sisthreads.size(); x++) {
        QString & forum = sisthreads[x]->forum;
        if (forum == "322" || forum == "383" || forum == "359" || forum == "390" ) {}
        else break;
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
    if (keywords.size()>0) {
        bool first = true;
        for (int i=0; i<keywords.size(); i++) {
            if (keywords[i]->pid =="") continue;
            if (first) {
                message += myTr("\n\n 相关关键字链接 \n");
                first = false;
            }
            message+= keywords[i]->postString() + '\n';
        }
    }

    if (message.left(frontPart) != (*this)["content"].toString().left(frontPart) ) {
        NetworkUtil::mustEditPost(tid,pid,subject,message);
        qDebug()<<tid<<' '<<pid<<' '<<page;
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
bool AuthorSet::finalAllPost()
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
            save("temp_k2.txt");
            timer.restart();
        }
        bool same;
        ok = x ->finalPost(same);
        QCoreApplication::processEvents();
        if (!ok) return false;
        ++count;
        if (!same) {
            qDebug()<<" final post "<<count<<"  total "<<total;
            MainWindow::outputInfor(QString("author final post ")+ QString::number(count)+" total "+  QString::number(total));
        }
    }
    save("temp_k2.txt");
    return true;
}

int AuthorSet::qualifySize()
{
    int count = 0;
    for (auto i=pool.begin(); i!= pool.end(); i++) {
        if ((*i)->qualify()) count++;
    }
    return count;
}


bool AuthorSet::save(QString file)
{
    QJsonArray b1;
    write(b1);
    return FileCenter::save(file , QJsonDocument(b1));
}
bool AuthorSet::load(QString file)
{
    QJsonDocument jd;
    bool ok;
    ok = FileCenter::load(file,jd);
    if (!ok) return false;
    QJsonArray b1 = jd.array();
    for (int i=0; i<b1.size(); i++) {
        new AuthorSet(b1[i].toObject());
    }
    return true;
}

void AuthorSet::checking()
{
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        auto x = *i;
        if (x->keywords.size()>2) qDebug()<<x->author<<' '<<x->keywords.size();
    }
    qDebug()<<"\n\n\n";
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        AuthorSet * x = *i;
        int t = x->sisthreads.size();
        int count = 0;
        for (int i=0; i< x->keywords.size(); i++) {
            KeywordSet * k = x->keywords[i];
            for (int j=0; j<k->sisthreads.size(); j++) {
                if (k->sisthreads[j]->author == x->author) count++;
            }
        }
        if (t-count>=5) {
            qDebug()<<x->author<<"  miss   "<<t<<" "<<count;
        }
    }
    qDebug()<<"\n\n\n";
}

QDate AuthorSet::lastPostTime()
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

bool authorSetCmp1(AuthorSet * a, AuthorSet * b)
{
    return  a->author < b->author;
}

bool authorSetCmp2(AuthorSet *a, AuthorSet *b)
{
    if (a->lastPostTime() != b->lastPostTime()) return a->lastPostTime() > b->lastPostTime();
    return  a->author < b->author;
}


QString AuthorSet::allPostString(bool authorSetCmp(AuthorSet *, AuthorSet *), QString & subject)
{
    QList<AuthorSet*> la;
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        la.push_back(*i);
    }
    std::sort(la.begin(), la.end(),authorSetCmp);
    QString message;
    message = myTr("作者合集索引 \n");

    bool outtime = false;

    if (authorSetCmp == authorSetCmp2) {
        message += myTr("按更新时间倒序 \n");
        outtime = true;
    }
    else if (authorSetCmp == authorSetCmp1) {
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
        message += (*i) -> postString("[color=red][author][/color] | [size] ([refer])\n");
    }
    //FileCenter::outputLog(message);
    return message;
}


QString AuthorSet::allPostStringByType(QString & subject)
{
    const char * types[16] = {"乱伦", "人妻", "农村", "凌辱", "历史", "古典武侠", "另类", "同人", "女友", "春色群交", "校园", "武侠", "玄幻", "绿母", "都市", "肥水自耕"};
    QMap<QString, QList<AuthorSet *> >  table;
    for (auto i = pool.begin(); i!= pool.end(); i++) {
        AuthorSet * x = *i;
        if (!x->qualify()) continue;
        auto z = x->sisthreads;
        for (int j=0; j<z.size(); j++) {
            SisThread * t = z[j];
            table[t->type].push_back(x);
        }
    }
    //"中篇","中篇全本","乱伦","交流","人妻","作品合集","作者合集","全部完成","其它","农村","凌辱","历史","原创典藏","原创杂志","古典武侠","另类",
   //"合集","同人","女友","心语小楼","成　人","成人原创","成人转帖","春色群交","有声合集","有声完成","有声连载","未删节全本","校园","武侠","河图实封",
  // "玄幻","目前最全","短篇","系列合集","绿母","转帖杂志","连载更新","都市","长篇","长篇全本","非成人","非成人原创",
//    for (auto i = table.begin(); i!=table.end(); i++) {
//        qDebug()<< i.key();
//    }
    QString message;
    message = myTr("作者分类索引 \n");
    subject = message;
    subject.replace('\n',"");
    for (int i=0; i<16; i++) {
        QString type = myTr(types[i]);
        if ( type == "") continue;
        message += "\n" + type + "\n";

        auto z =  table[type];
        std::sort(z.begin(),z.end(),authorSetCmp1);
        auto ii = std::unique(z.begin(),z.end());
        z.erase(ii, z.end());
        for (int j=0; j<z.size(); j++) {
             message += z[j] -> postString("[color=red][author][/color] | [size] ([refer])\n");
        }
    }
    return message;
}
