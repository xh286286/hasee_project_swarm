#include "filecenter.h"
#include "util.h"
#include <QFile>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include <QTextCodec>
#include <QDir>
#include <QJsonObject>
QTextStream * FileCenter::log  = nullptr;

FileCenter::FileCenter()
{
}
bool FileCenter::initial()
{

    return true;
}

bool FileCenter::load(QString filename, QJsonDocument & d, SaveFormat  sf)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    d = (sf == Json
        ? QJsonDocument::fromJson(saveData)
        : QJsonDocument::fromBinaryData(saveData));

    if (d.isNull()) {
        qDebug()<<"load error";
    }

    return true;
}

bool FileCenter::save(QString filename, const QString & s)
{
    QTextCodec * tcl = QTextCodec::codecForName("utf8");
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    auto f = tcl->fromUnicode(s);
    file.write(f);
    file.close();
    return true;
}

bool FileCenter::save(QString filename, const QJsonDocument & d, SaveFormat  sf)
{
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    saveFile.write(sf == Json
        ? d.toJson()
        : d.toBinaryData());

    return true;
}


template <class T>
bool FileCenter::saveVar(QString filename, const T &, SaveFormat)
{
    return true;
}

template <class T>
bool FileCenter::loadVar(QString filename, T &, SaveFormat )
{
    return true;
}

void checkFileEncoding(){
    QString name = "testcoding.txt";
    QFile file(name);
//    if (!QDir::current().remove(name) ) {
//        qWarning("Couldn't del file.");
//        //return;
//    }

//    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        qWarning("Couldn't open save file.");
//        return;
//    }
//    auto log =  new QTextStream(&file);
//    *log<<myTr("你是大笨蛋");
//    file.close();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open read file.");
        return;
    }
    auto x = file.readAll();
    qDebug()<<x.toPercentEncoding();
}

void FileCenter::outputLog(QString s)
{
    static QTextCodec * tcl = QTextCodec::codecForName("utf8");
    //static QTextCodec * tcl1 = QTextCodec::codecForName("GB18030");

    static const char * enter = "\r\n";
    static QFile file("log.txt");
    static bool fileopen = false;
    if (!fileopen) {
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            qWarning("Couldn't open save file.");
            return;
        }
        file.write("===============");
        file.write(enter);
        auto s = QDateTime::currentDateTime().toString();
        auto f = tcl->fromUnicode(s);
        file.write(f);
        file.write(enter);
        fileopen=true;
    }

    auto f = tcl->fromUnicode(s);
    file.write(f);
    file.write(enter);
    file.flush();
}
