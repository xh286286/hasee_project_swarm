#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QFile>
#include <algorithm>

//#include "../tcp_socket_test/danmuconnection.h"

using namespace std;

void work1();
int work2();
int work3();
int main(int argc, char *argv[])
{
    qDebug()<<1;

    return 1;
    work2();


}


void work1() {
    QString s;
    s = "123456";
    QList <QString> sl;
    for (int i=0; i<720; i++) {
        QString a = s;
        bool ok = true;
        for (int i=0; i<6; i++) {
            if (a[i] == '1'+i) {
                ok = false;
                break;
            }
        }
        if (ok) sl.push_back(a);
        next_permutation(s.begin(), s.end());
    }

    random_shuffle(sl.begin(), sl.end());
    for (int i=0; i<sl.size(); i++) {
        qDebug()<<i+1<<"  " <<sl[i];
    }


}


int work3() {
    int x = 0;
    QApplication a(x,0);

   // DanmuConnection dc;




    return a.exec();
}
