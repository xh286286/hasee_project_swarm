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

int work2();



int main(int  , char * [])
{
    work2();
    return 1;

}
const int total = 63;



int work2() {
    int x = 0;
    QApplication a(x,0);

int ss = DanmuConnection::getDanmuSeverListFromNet().size();
    qDebug()<<"maximum robot number "<<ss*2;
    qDebug()<<"input robot number:";
        int n;
    cin>>n;

    if (n<0) n =0;


    if (n> ss *3) n = ss*3;
    DanmuConnection dca[1000];
    x=0;

    for (int i=1; i<2; i++) {
        for (int j=0; j<ss; j++) {

            qDebug()<<"No."<<x+1;

            DanmuConnection & dc = dca[x];
            dc.debugFlag = false;
            QString s;
            dc.danmuServerSeed = j;
            //dc.danmuServerPort = 15010 + i;
            dc.login(s, s, "113289","317");
            //dc.login(s, s, "359887","1396");
            QTime t;
            t.start();

            while(true) {

                if (t.elapsed() >5000)  {
                    break;
                }
                QCoreApplication::processEvents();
            }
            dc.debugFlag = false;

            x++;
            if (x>= n) goto end;
        }
    }

 end:




    return a.exec();
}


