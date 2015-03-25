#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QList>
#include "../share_library/httpfiledownloader.h"


QList<qint64> ls;

QDateTime  int64todatetime( qint64 i64) {
    QDateTime dt;
    dt.setMSecsSinceEpoch(
                i64);
    return dt;
}
QString target = "34832_UuaK5";
QString base = "http://dlhls.cdn.zhanqi.tv/zqlive/";
QString start = "2015-3-23 18:20:03";
QString end =  "2015-3-23 18:33:03";
QString result;
void getListFile() {


    int now = int(QDateTime::currentDateTime().toMSecsSinceEpoch() /1000);


    QString tail = ".m3u8?starttime=1409939688-endtime=" + QString::number(now);

    QString qh = base + target +tail;
    qDebug()<<qh;
    HttpFileDownloader  hfd ;

    hfd.sycGetPageFromURL(QUrl(qh));

    QString s1, s = hfd.getPage();

    qDebug()<<s;

    QTextStream ts(&s);
    ts>>s1>>s1>>s1>>s1;

    qDebug()<<s1;


    qh = base + s1;

    hfd.sycGetPageFromURL(QUrl(qh));

    result = hfd.getPage();

}

void loadFromFile() {
    qDebug()<<"loadFromFile begin";
//    QFile file("input.txt");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))             return;
    QTextStream ts( &result);
    while( true) {
        QString s;
        ts>>s;
        if (s=="") break;
        int x = s.indexOf(".ts");
        if (x>=0) {
            s = s.mid(0,x);
            ls.push_back( s.toLongLong());
        }

    }
    qDebug()<<ls.size();
    qDebug()<<"loadFromFile end";
}
void showList() {
    qDebug()<<"showList begin";
    qint64 last = 0;
    if (ls.size()==0) return;
    for (int i=0; i<ls.size(); i++) {
        if (i==0) {
            qDebug()<< int64todatetime(ls[i]);
            continue;
        }
        if (ls[i] - ls[i-1] > 300000) {
            qDebug()<<  int64todatetime(ls[i-1]);
            qDebug()<<"num "<< i-last<<" freq "<< (ls[i-1] - ls[last]) / (i-last);
            last = i;
            qDebug()<<"-----------------";
            qDebug()<<  int64todatetime(ls[i]);
        }

    }
    qDebug()<< int64todatetime(ls.back());
    qDebug()<<"num "<< ls.size()-last<<" freq "<< (ls[ls.size()-1] - ls[last]) / (ls.size()-last);
    qDebug()<<"showList end";
}

void filterOut(QDateTime a, QDateTime b) {
    QFile file("output.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))             return;
    QTextStream ts( &file);
    qint64 a1,b1;
    a1 = a.toMSecsSinceEpoch();
    b1 = b.toMSecsSinceEpoch();
    for (int i=0; i<ls.size(); i++) {
        if (ls[i]>=a1 && ls[i]<=b1) {
            ts<<"http://dlhls.cdn.zhanqi.tv/zqlive/317_4vefy_1024/"<<  qint64(ls[i])<<".ts"<<endl;
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTime t;
    QDate d;
    t.setHMS(19,54,0);
    QDateTime dt;
    dt.setMSecsSinceEpoch(
                1427106181207);
//1420105383682
    qDebug()<<dt;

    dt.setTime(t);

    qDebug()<<dt.toMSecsSinceEpoch();

    dt.setMSecsSinceEpoch(
                1427121144975   );

    qDebug()<<dt;


    dt.setMSecsSinceEpoch(
                1422117369678);

    qDebug()<<dt;

    dt.setMSecsSinceEpoch(1421667096796);

    qDebug()<<dt;


    getListFile();

    loadFromFile();

    showList();
    QDateTime ta,tb;
    ta = QDateTime::fromString(start,"yyyy-M-d hh:mm:ss");
    tb = QDateTime::fromString(end,"yyyy-M-d hh:mm:ss");
    filterOut(ta, tb);

    return 0;

    QFile file("x.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))             return 0;

    QTextStream out(&file);
    d.setDate(2015,1,29);
    for (int m = 0; m<60; m++)
        for (int s = 0; s<60; s++) {
            t.setHMS(1,m,s);

            dt.setDate(d);
            dt.setTime(t);

            out << "http://dlpic.cdn.zhanqi.tv//live/20150129/317_4vefy_" << dt.toString("yyyy-MM-dd-hh-mm-ss") <<".jpg"<<endl;

        }

    qDebug()<<1<<endl;


    return a.exec();
}
//{"mayu","kirito","sha77zhu","seraph_xiaot","chacha","icecreamrei","huamituan","alyssasearrs","1113430","2009","14689","1114920","12474","mirai","LMM","12838","11249","laoshusjq","ana0727"};
