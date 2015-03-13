#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTime t;
    QDate d;
    t.setHMS(19,54,0);
    QDateTime dt;
    dt.setMSecsSinceEpoch(
                1422338373285);
//1420105383682
    qDebug()<<dt;

    dt.setTime(t);

    qDebug()<<dt.toMSecsSinceEpoch();

    dt.setMSecsSinceEpoch(
                1422356462397   );

    qDebug()<<dt;


    dt.setMSecsSinceEpoch(
                1422117369678);

    qDebug()<<dt;

    dt.setMSecsSinceEpoch(1421667096796);

    qDebug()<<dt;


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
