#include <QCoreApplication>
#include <QDateTime>

#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDateTime dt;
    dt.setMSecsSinceEpoch(

                1420191223743);
//1420105383682
    qDebug()<<dt;


    dt.setMSecsSinceEpoch(
                1420197867085);

    qDebug()<<dt;


    dt.setMSecsSinceEpoch(
                1420103504342);

    qDebug()<<dt;

    dt.setMSecsSinceEpoch(1420104499629);

    qDebug()<<dt;
    return a.exec();
}
//{"mayu","kirito","sha77zhu","seraph_xiaot","chacha","icecreamrei","huamituan","alyssasearrs","1113430","2009","14689","1114920","12474","mirai","LMM","12838","11249","laoshusjq","ana0727"};
