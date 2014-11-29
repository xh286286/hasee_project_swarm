#ifndef ZHANQIUTIL_H
#define ZHANQIUTIL_H

#include <QString>
#include <QJsonObject>
#include <QList>
#include <QMap>
class ZhanQiUtil
{
public:
    static QString encodeString(QString s);
    static QString decodeString(QString s);

    static int levelOfDanmu(const QJsonObject &);
    static const QList<QString> & getGiftList();
    static const QMap<QString, int > & getGiftMap();
    static const   QMap<QString, int >  & getBlackMap(const QJsonObject &);
private:
    ZhanQiUtil();
};

#endif // ZHANQIUTIL_H
