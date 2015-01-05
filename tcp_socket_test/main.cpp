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


int work3();



int main(int  , char * [])
{
    work3();
    return 1;

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
