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
    int x = 0;
    QApplication a(x,0);
    qDebug()<<1;
    QString dir1 = QFileDialog::getExistingDirectory(0,  "Open Directory" ,
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    qDebug()<<dir1;
    QDir::setCurrent(dir1);
    QDir dir;
    dir.setPath(dir1);
    auto fl = dir.entryInfoList();

    for (int i=0; i<fl.size(); i++) {
        QFileInfo a = fl[i];
        qDebug()<< a.baseName()<<' '<<a.created()<<' '<<a.lastModified();
        if (a.isFile()) {} else continue;
        if (a.suffix() != "flv" && a.suffix()!="") continue;
        qDebug()<<a.filePath();
        QFile x(a.filePath());
        x.rename("mayu " + a.created().toString("yyyy-MM-dd HH-mm ") + a.lastModified().toString("HH-mm") + ".flv");
    }

    return a.exec();

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
int work2() {
    return 1;
}


int work3() {
    int x = 0;
    QApplication a(x,0);

   // DanmuConnection dc;




    return a.exec();
}
