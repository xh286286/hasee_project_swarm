#include <QCoreApplication>
#include <QFile>
#include <QMap>
#include <QSet>
#include <QVector>
#include <QRegularExpression>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include "../share_library/Util.h"
QTextCodec *codec1 = QTextCodec::codecForName("GBK");
QTextCodec *codec2 = QTextCodec::codecForName("GB18030");
QTextCodec *codec3 = QTextCodec::codecForName("GB2312");

void work1();

void work2();

void work3();

void work4();


QSet<QString>  processFile(QString filename);

QMap<QString, QString> map1,map2,map3;
QString order[ 300];


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    work1();
    work2();
    work3();
    work4();
    qDebug()<< processFile("D:/sql/ccb1/deletelogic.xml") ;
    return a.exec();
}



void work1() {
    QString filename = "D:/sql/tmp001.prn";

    QFile file(filename);
    qDebug()<<1;
    if (!file.open(QIODevice::ReadOnly  ) )
        return;
    qDebug()<<2;

    QString ss[2000];

    int count = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();

//        qDebug()<< codec1->toUnicode(line);
//        qDebug()<< codec2->toUnicode(line);
//        qDebug()<< codec3->toUnicode(line);

        ss[count] = codec1->toUnicode(line);
        count++;
        //if (count == 4) return;
    }
    qDebug()<<3;
    for (int i=1; i<=235; i++) {
        QTextStream ts(ss+i);
        QString a,b,c,d;
        ts>>a>>b>>c>>d;

        if (d!= "") {
            int x = d.indexOf(".jsf");
            int y = d.lastIndexOf("/",x);
            d = d.mid(y+1,x-y-1);
        }

        map1[c] = d;
     //   qDebug()<<c<<' '<<d;
    }
}

void work2() {
    QString filename = "D:/sql/work zhou simple.prn";

    QFile file(filename);
    qDebug()<<1;
    if (!file.open(QIODevice::ReadOnly  ) )
        return;
    qDebug()<<2;

    QString ss[2000];

    int count = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();

//        qDebug()<< codec1->toUnicode(line);
//        qDebug()<< codec2->toUnicode(line);
//        qDebug()<< codec3->toUnicode(line);

        ss[count] = codec1->toUnicode(line);
        count++;
        //if (count == 4) return;
    }
    qDebug()<<3;
    for (int i=1; i<=228; i++) {
        QTextStream ts(ss+i);
        QString  b,c,d;
        ts>> b>>c>>d;

        if (d!= "") {
            int x = d.indexOf(".jsp");

            d = d.mid(0, x );
            if (x<0) d ="";
        }

        map2[c] = d;
        map3[d] = QString::number(i);
        order[i] = c;
     //   qDebug()<<c<<' '<<d;
    }

    for (int i=1; i<=228; i++) {
        QString a = order[i];
        if (map1[a]=="") continue;\
        if (map2[a]=="") continue;
        if (map1[a] != map2[a]) qDebug()<<a<<' '<<map1[a]<<' '<<map2[a];
    }
}

QSet<QString>  processFile(QString filename) {
   // qDebug()<<0;
    QFile file(filename);
  //  qDebug()<<1;
    if (!file.open(QIODevice::ReadOnly  ) )
        return QSet<QString>();
   // qDebug()<<2;

    QString ss[2000];
    QString word[100000];
    QString word1[100000];
    QSet<QString> wordSet,wordSet1;

    int count = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();

//        qDebug()<< codec1->toUnicode(line);
//        qDebug()<< codec2->toUnicode(line);
//        qDebug()<< codec3->toUnicode(line);

        ss[count] = codec1->toUnicode(line);
        count++;
        //if (count == 4) return;
    }
    //qDebug()<<3;

    int acc = 0;

    for (int i=0; i<count; i++) {
        QTextStream ts(ss+i);
        QString s;

        while( true) {
            ts>>s;
            if (s=="") break;
            word[acc] = s;
            acc++;
        }
    }
   // qDebug()<<acc;

    for (int i=0; i<acc; i++) {
        QString a = word[i].toLower();

        if (a == "update" || a == "into" || a == "join") {
            QString b = word[i+1];
            if (b[0]=='(') continue;
            int x = b.indexOf(".");
            if (x>=0) {
                b = b.mid(x+1);
            }
            x = b.indexOf('(');
            if (x>=0) {
                b = b.mid(0,x);
            }
            wordSet.insert(b);
        }

    }



    wordSet.remove("(");
    wordSet.remove("");
    wordSet.remove("<isEqual");


    int acc1 = 0;
    for (int i=0; i<acc; i++) {
        QString a = word[i];

        static QRegularExpression re("[\\,\\.\\#\\(\\\\=\\>\\<\\/)]");
        a.replace(re," ");
        QTextStream ts(&a);

        QString s;
        while(true) {
            ts>>s;
            if (s=="") break;
            word1[acc1] = s;
            acc1++;
        }

    }
    for (int i=0; i<acc1; i++) {
        QString s = word1[i];

        if (s.indexOf('\"')>=0) continue;
        int x = -1;
        int count = 0;
        while(true) {
            x = s.indexOf('_',x+1);
            if (x<0) break;
            count++;
        }
        if (i!=acc1-1 && word[i+1] == "as") continue;
        if (s.indexOf("crd_trad")>=0) continue;
        if (s.indexOf("CRD_TRAD")>=0) continue;
        if (s[s.size()-1]=='_') continue;
        if (count>=3) {
            wordSet1.insert(s);
        }

    }
    wordSet1.remove("SHOP_ORG_CODE_NUM");

    return wordSet1.unite(wordSet);
}

void work3() {
    QFile file("out.csv");
    file.open(QIODevice::WriteOnly);
    QTextStream ts(&file);
    for (int i=1; i<=228;i++) {
        QString f = order[i];

        QString t = map1[f];
        if (t=="") t = map2[f];
        if (t=="") {
            ts<<i<<','<<endl;
            continue;
        }
        QString filename = "D:/sql/ccb1/" + t +".xml";
        //qDebug()<<"process "<<i <<' '<< filename;
        QSet<QString>  g1,g2 ;
        g1 = processFile(filename);
        //qDebug()<<g1;
        if (map3[t] != "") {
            QString filename = "D:/sql/xxx/sql (" + map3[t]  +").txt";
            //qDebug()<<"process "<<i <<' '<< filename;
            g2 = processFile(filename);
            if (g1!=g2 &&g1.size()!=0 && g2.size()!=0) {
                qDebug()<<g1;
                qDebug()<<g2;
                qDebug()<<"===========";
            }
            //qDebug()<<g2;
            g1 = g1.unite(g2);
        }
        auto ff = g1.begin();
        if (g1.size()==0) {
            ts<<i<<','<<endl;
            continue;
        }
        for (int j=0; j<g1.size(); j++) {
            if (j==0) ts<< i;
            ts<<','<< *ff<<endl;
            ff++;
        }

    }

}

void work4() {


}
