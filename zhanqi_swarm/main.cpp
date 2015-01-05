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



int work2() {
    int x = 0;
    QApplication a(x,0);


    qDebug()<<"number";
        int n;
    cin>>n;

    if (n<0) n =0;
    if (n>300) n = 300;
    int ss = DanmuConnection::getDanmuSeverList().size();
    if (n> ss *4) n = ss*4;
    DanmuConnection dca[1000];
    x=0;
    while(x<n) {


        qDebug()<<"No."<<x+1;
        DanmuConnection & dc = dca[x];
        dc.debugFlag = false;
        QString s;
        //if (x<total) s = nameList[x];
        //1396","uid":"359887","n
        // "113289","317");

       // dc.login(s, s, "359887","1396");
        dc.login(s, s, "113289","317");

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
    }





    return a.exec();
}


