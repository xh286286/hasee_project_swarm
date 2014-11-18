#include <QCoreApplication>
#include "../share_library/httpfiledownloader.h"

#include <QDebug>
#include <QDir>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString s =  "http://dlpic.cdn.zhanqi.tv/live/20141114/317_4vefy_2014-11-14-22-10-52_big.jpg";
    HttpFileDownloader hfd;

    hfd.getFileFromURL(QUrl(s),"test12.jpg",20000);
    s[60] = '3';
    QDir d= QDir::current();
    for (int hour = 21; hour <=22; hour++) {
        s[62] = '0' + hour/10 ;
        s[63] = '0' + hour%10;
        for (int minute = 0; minute <60; minute++) {
            s[65] = '0' + minute/10 ;
            s[66] = '0' + minute%10;
            for (int second = 0; second <60; second++) {
                s[68] = '0' + second/10 ;
                s[69] = '0' + second%10;

                hfd.getFileFromURL(QUrl(s), "now.jpg", 20000);
                while (hfd.downloadOK() == false) {
                    QCoreApplication::processEvents();
                }
                if (hfd.getFileLength() >1000) {
                     qDebug()<<s;
                    int base = 13000000;
                    base += hour*10000 + minute * 100 + second;
                    d.rename("now.jpg", QString ::number(base) + ".jpg");
                }
            }
        }
    }
    return a.exec();
}
