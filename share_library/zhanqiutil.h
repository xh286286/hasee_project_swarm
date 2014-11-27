#ifndef ZHANQIUTIL_H
#define ZHANQIUTIL_H

#include <QString>
#include <QJsonObject>
class ZhanQiUtil
{
public:
    static QString encodeString(QString s);
    static QString decodeString(QString s);

    static int levelOfDanmu(const QJsonObject &);
private:
    ZhanQiUtil();
};

#endif // ZHANQIUTIL_H
