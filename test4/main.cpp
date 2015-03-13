#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QFile>
#include <algorithm>

//#include "../tcp_socket_test/danmuconnection.h"

using namespace std;

int main(int argc, char *argv[])
{
    int x = 0;
    QApplication a(x,0);

    qDebug()<<QDir::current().absolutePath();
    qDebug()<<argc;
    QString dir1;
    QFile file("test4.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))             return 1;

    QTextStream out(&file);
    if (argc ==1 ) {
        dir1= QFileDialog::getExistingDirectory(0,  "Open Directory" ,
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    }
    else if (argc ==2) {
        dir1 = QDir::current().absolutePath();
    }
    qDebug()<<dir1;
    if (dir1=="") return 1;
    QDir::setCurrent(dir1);
    QDir dir;
    dir.setPath(dir1);
    auto fl = dir.entryInfoList();
    int count = 0;
    for (int i=0; i<fl.size(); i++) {
        QFileInfo a = fl[i];
        qDebug()<< a.baseName()<<' '<<a.created()<<' '<<a.lastModified();
        //continue;// ****************
        if (a.isFile()) {} else continue;
        count++;
        QString base = "http://ww4.sinaimg.cn/large/";
        QString end = ".jpg";



        //qDebug()<<a.filePath();
        QFile x(a.filePath());

        QString s = base + a.baseName() + end;
        out<<s<<endl;

        continue;
        if (a.suffix() == "flv"  )  {
            x.rename("mayu" + a.created().toString("yyyy-MM-dd-HH-mm_") + a.lastModified().toString("HH-mm") + ".flv");
        }
        else if (a.suffix() == "jpg" ||  a.suffix() == "png" ) {
            x.rename(a.created().toString("yyyy-MM-dd-HH-mm_")+ QString::number(count) + "." + a.suffix());
        }
        else if (a.suffix() =="ts") {
            qint64 xx = a.baseName().toLongLong();
            if (xx==0) continue;
            QDateTime dt;
            dt.setMSecsSinceEpoch(xx);
            x.rename("mayu"+dt.toString("yyyy-MM-dd-HH-mm-ss") +".ts");
        }

        //

        //
    }

    return a.exec();

}



