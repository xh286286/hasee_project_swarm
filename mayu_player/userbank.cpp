#include "userbank.h"
#include "../share_library/Util.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QDebug>
QMap<QString , int> UserBank::record;

UserBank::UserBank(QObject *parent) :
    QObject(parent)
{

}
int UserBank::evaluateGift(QString u)
{
    if (u == myTr("顶")) return 1;
    if (u == myTr("锅")) return 1;
    if (u == myTr("溜")) return 1;
    if (u == myTr("麻辣烫")) return 60;
    if (u == myTr("女盆友")) return 660;
    if (u == myTr("大宝剑")) return 6660;
    if (u == myTr("")) return 0;
    return 0;
}

int UserBank::leftMoney(QString u)
{
    return record[u];
}
int UserBank::saveMoney(QString u, int m)
{
    qDebug()<<u<<" add money "<<m;
    record[u]+=m;
    return record[u];
}

int UserBank::withdrawMoney(QString u, int m)
{
    qDebug()<<u<<" subtract money "<<m;
    record[u]-=m;
    return record[u];
}
bool UserBank::load()
{
    QJsonArray a;
    QJsonDocument d;
    bool ok = loadJsonFile("giftrecord.json", d);
    a = d.array();

    if (!ok) {
        qDebug()<< " load record file failed!!";
        return false;
    }
    else {
         qDebug()<< " load record file succeed!!";
    }
    for (int i=0; i<a.size(); i++) {
        QJsonObject b = a[i].toObject();
        record[b["name"].toString()] = b["money"].toInt();
    }
    record[myTr("天蝎1000")] = 10000000;
    record[myTr("天蝎10000")] = 10000000;
    return true;
}

bool UserBank::save()
{
    QJsonArray a;
    QList<QString> names;
    QList<int> values;
    names = record.keys();
    values = record.values();
    for (int i=0; i<record.size(); i++) {
        QJsonObject b ;
        b["name"] = names[i];
        b["money"] = values[i];
        a.push_back(b);
    }
    bool ok = saveJsonFile("giftrecord.json", QJsonDocument(a));
    if (!ok) {
        qDebug()<< " save record file failed!!";
        return false;
    }
    else {
         qDebug()<< " save record file succeed!!";
         return true;
    }
}
