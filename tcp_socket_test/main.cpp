#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>

#include "danmuconnection.h"
int get16_2(const char * s) {
    int a,b;
    if (s[0] <='9') {
        a = s[0] - '0';
    }
    else {
        a = s[0]-'A'+ 10;
    }
    if (s[1] <='9') {
        b = s[1] - '0';
    }
    else {
        b = s[1]-'A'+ 10;
    }
    return a*16 + b;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if 1
    DanmuConnection dc;
    dc.login("aabbdaabbd","aabbcaabbc", "113289","317");

    return a.exec();
#endif
    QTcpSocket ts;
    ts.connectToHost("112.124.114.222",15010);
    ts.waitForConnected(10000);

    char s[35] =  "            {\"cmdid\":\"svrisokreq\"}";
    for (int i=0; i<12; i++) s[i] = 0;

    s[0] = get16_2("BB");
    s[1] = get16_2("CC");
    s[6] = get16_2("16");
    s[10] = get16_2("E8");
    s[11] = get16_2("03");
    for (int i=0; i<35; i++) {
        qDebug()<<int(s[i]);
    }
    ts.write(s,34);

    ts.waitForReadyRead(10000);

    QByteArray ba = ts.readAll();

    qDebug()<< ba.toPercentEncoding();

    return a.exec();
}
