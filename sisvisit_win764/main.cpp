#include <map>
#include <string>
#include <initializer_list>
#include <cstdio>
#include <cstdlib>

#include <QApplication>
#include <QTextCodec>
#include <QDateTime>
#include <QThread>
#include <QTime>
#include <QFile>
#include <QDir>
#include <QList>
#include <QNetworkProxy>
#include <QJsonDocument>
#include <QJsonArray>

#include "mainwindow.h"
#include "sisthread.h"
#include "httpproxy.h"
#include "networkutil.h"
#include "filecenter.h"
#include "authorset.h"
#include "util.h"
#include "keywordset.h"

void test();
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
int forkWork(QString,  MainWindow &  );
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug()<<argc;
    for (int i=0; i<argc; i++)     qDebug()<<argv[i];

    QDir::current().mkdir("data");
    QDir::setCurrent("./data");

    HttpProxy::loadProxyFile("proxy.ini");

    NetworkUtil::initial();
    FileCenter::initial();
#ifdef QT_NO_DEBUG
    qDebug()<< "release";
#else
    qDebug()<< "debug";
#endif

    test();



    Util::loadData();
    MainWindow w;
    w.show();
    MainWindow::setMainW(&w);
    int z = 0;

    QString order = argc >1 ? argv[1] : "";

    int r = forkWork(order,w);

    if (r != 0) {
        z = a.exec();
    }

    FileCenter::save("runninglog.txt",w.runninglog);

    return z;
}
int forkWork(QString order, MainWindow & w)
{
    if (order == "robfloor") {
        w.loginsis();
        w.robfloor();
        waitMillisec(3000);
        return 0;
    }
    else if (order == "workall") {
        w.loginsis();
        w.loadthreadfromeinternet();
        w.working();
        w.generateCatalog();
        waitMillisec(3000);
        return 0;
    }
    else if (order == "onesign") {
        w.loginsis();
        w.registersis();
        waitMillisec(3000);
        return 0;
    }
    else if (order == "test") {

        w.loginsis();
        w.test4();
        waitMillisec(3000);
        return 0;
    }
    return 1;
}

void test()
{

    return;
    {
        FileCenter::outputLog(myTr("日志"));
        FileCenter::outputLog("dsfadfasdf");
        qDebug() << full2half(myTr("ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ０１２３４５６７８９（）"));
        qDebug() << full2half(myTr("【ＳＩＳ首发】全家性福之我不是一个人在战斗(三)"));
    qDebug()<<QDate::fromString("2012-9-11" ,"yyyy-M-d");
    qDebug()<<QDate::fromString("2012-11-1" ,"yyyy-M-d");
    qDebug()<<QDate::fromString("2012-1-1" ,"yyyy-M-d");
    qDebug()<<QDate::fromString("2012-11-11" ,"yyyy-M-d");

    qDebug()<< QDateTime::currentDateTime().date().toString("yyyy-MM-dd");
    QFile file("1.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray s = file.readAll();
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QString loadresult;
    loadresult = gbk->toUnicode(s);

    SisThread::getOnePage(loadresult,"322");
    QJsonArray aa;
    SisThread::write(aa);
    FileCenter::save("test.txt", QJsonDocument(aa));
    SisThread::pool.clear();
    SisThread::phrase.clear();
    AuthorSet::pool.clear();
    }
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}
