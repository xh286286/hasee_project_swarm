#include "Util.h"

#include <QTime>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QTextCodec>


void waitMillisec(int s )
{
    QTime t;
    t.start();
    while(t.elapsed()<s)  QCoreApplication::processEvents();
}

bool saveJsonFile(QString filename, const QJsonDocument & d)
{
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    saveFile.write(d.toJson());

    return true;
}

bool loadJsonFile(QString filename, QJsonDocument & d)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    d = QJsonDocument::fromJson(saveData);

    return true;
}


QTextCodec * tcl = nullptr;

QTextCodec * getCodec()
{
    if (tcl == nullptr) {
        QByteArray aa = "记";
        qDebug()<< aa.toPercentEncoding();
        if (aa.toPercentEncoding() == "%E8%AE%B0")
            tcl = QTextCodec::codecForName("utf8");
            else tcl = QTextCodec::codecForName("GB18030");
    }
    return tcl;
}

QString myTr(const char * s)
{
    return getCodec()->toUnicode(s);
}
QByteArray consoleTr(QString s) {
    QByteArray ba;

    static QTextCodec *codec1 = QTextCodec::codecForName("GBK");
    static QTextCodec *codec2 = QTextCodec::codecForName("GB18030");
    static QTextCodec *codec3 = QTextCodec::codecForName("GB2312");
    if (codec1) {
        ba   = codec1->fromUnicode(s);

    }
    else if (codec2) {
        ba   = codec2->fromUnicode(s);

    }
    else if (codec3) {
        ba   = codec2->fromUnicode(s);

    }
    else {
        ba = s.toUtf8();
    }

    return ba;
}
void debugToConsole(QString s) {
    qDebug()<< consoleTr(s);
}
