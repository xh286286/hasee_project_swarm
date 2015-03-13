#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QFile>
#include <QTime>
#include <algorithm>
#include <iostream>

#include "../tcp_socket_test/danmuconnection.h"

#include "../share_library/httpfiledownloader.h"
using std::cin;
using namespace std;

void guaji();
int work2();
int work3();



void gua(DanmuConnection & dc, QString u, QString p, QString room);

int main(int  , char * [])
{
    guaji();
    return 1;

}
const int total = 63;
QString nameList[100]=
{"salary123",
"sale123",
"salesman123",
"salt123",
"salute123",
"sample123",
"sandwich123",
"sandy123",
"satellite123",
"satisfaction123",
"satisfactory123",
"satisfy123",
"sauce123",
"saucer123",
"sausage123",
"saving123",
"saw123",
"scale123",
"scan123",
"scar123",
"scarce123",
"scarcely123",
"scarf123",
"scatter123",
"scene123",
"scenery123",
"scent123",
"schedule123",
"scheme123",
"scholar123",
"scholarship123",
"scientific123",
"scientist123",
"scissors123",
"scope123",
"scorn123",
"scout123",
"scrape123",
"scratch123",
"scream123",
"screen123",
"screw123",
"seal123",
"seaman123",
"seaport123",
"secondary123",
"secondly123",
"secret123",
"secretary123",
"section123",
"secure123",
"security123",
"seed123",
"seek123",
"seize123",
"selection123",
"selfish123",
"sell123",
"seller123",
"semester123",
"semiconductor123",
"senate123",
"senior123" };
void gua(DanmuConnection & dc, QString u, QString p, QString room) {

    QString h = "http://zhanqi.tv/";

    h+= room;

    qDebug()<<room;

    while(true) {
        QTime t;
        t.start();

        while(true) {

            if (t.elapsed() >5000)  {
                break;
            }
            QCoreApplication::processEvents();
        }

        HttpFileDownloader hfd;
        bool ok = hfd.sycGetPageFromURL(QUrl(h));
        if (!ok) continue;
        QString re = hfd.getPage();


        if (re.size() <1000) continue;
        int x = re.indexOf("uid");

        if (x<0) continue;
        int a,b,c,d;
        a=re.indexOf(":",x);
        b=re.indexOf(",",x);
        c=re.lastIndexOf(":",x);
        d=re.lastIndexOf(",",x);
        QString s1,s2;
        s1 = re.mid(a+2,b-a-3);
        s2 = re.mid(c+2,d-c-3);

        qDebug()<<s1<<' '<<s2;

        dc.login(u,p,s1,s2);
        dc.debugFlag = false;
        break;
    }


    dc.debugFlag = false;
}
void guaji() {
    int x = 0;
    QApplication a(x,0);

    qDebug()<<1;
    DanmuConnection dca[100];
    QString ss1 [ 19] = {"mayu","kirito","sha77zhu","seraph_xiaot","chacha","icecreamrei","huamituan","alyssasearrs","1113430","2009","14689","suger","12474","mirai","LMM","12838","11249","laoshusjq","ana0727"};
    QString ss2 [ 12] = {"mayu","kirito","sha77zhu","seraph_xiaot","chacha","icecreamrei","huamituan","alyssasearrs","14689","suger","ana0727","scorpio"};
    QString ss3 [ 11] = {"mayu", "sha77zhu","seraph_xiaot","chacha","icecreamrei","huamituan","2009","14689","suger","ana0727","scorpio"};
    int count = 0;
    int i1,i2,i3;
    i1 = 0; i2 = 19+i1; i3 = 11+i2;
    for (int i=0; i<19 ; i++) {
        gua(dca[count],"aabbeaabbe", "aabbdaabbd", ss1[i]);
        count++;
    }
    for (int i=0; i<12 ; i++) {
        gua(dca[count],"aabbdaabbd", "aabbcaabbc", ss2[i]);
        count++;
    }
    for (int i=0; i<11 ; i++) {
        gua(dca[count],"aabbiaabbi", "aabbhaabbh", ss3[i]);
        count++;
    }
    qDebug()<<"finished";

//    QTimer  workcycle;

//    workcycle.start(30000);

//    QObject::connect(&workcycle, &QTimer::timeout, [&](){
//        dca[i1].sendRoses();
//        dca[i2].sendRoses();
//        dca[i3].sendRoses();
//    });


    a.exec();

}
int work2() {
    int x = 0;
    QApplication a(x,0);
    qDebug()<<"num";
        int n;
    cin>>n;

    if (n<0) n =0;
    if (n>300) n = 300;

    DanmuConnection dca[1000];
    x=0;
    while(x<n) {


        qDebug()<<"No."<<x+1;
        DanmuConnection & dc = dca[x];
        dc.debugFlag = false;
        QString s;
        if (x<total) s = nameList[x];
        dc.login(s, s, "113289","317");
        QTime t;
        t.start();

        while(true) {

            if (t.elapsed() >30000)  {
                break;
            }
            QCoreApplication::processEvents();
        }
        dc.debugFlag = false;

        x++;
    }





    return a.exec();
}


int work3() {
    int x = 0;
    QApplication a(x,0);
//1396","uid":"359887
    DanmuConnection dc;
    dc.debugFlag = true;
    dc.login("aabbeaabbe","aabbdaabbd", "359887","1396");
    QTime t;
    t.start();
    int count = 0;
     x = 1;
    while(true) {

        if (t.elapsed() >5000)  {
            count+=123123;
            //a = dc.postDanmu(QString ::number(count));
            //   QJsonObject({"action":1,"cmdid":"blockuser","ip":"111.8.57.138","msg":"111","nam
            //   e":"九千万亿电竞女神","type":1,"uid":100305463})

            x = 1-x;
           t.restart();
        }
        QCoreApplication::processEvents();
    }
    return a.exec();

}
