#include "zhanqiutil.h"
#include "Util.h"
ZhanQiUtil::ZhanQiUtil()
{
}
QString ZhanQiUtil::encodeString(QString s) {
     s.replace("&", "&amp;");
     s.replace("\"","&quot;" );
    s.replace("'", "&apos;");

    return s;
}

QString ZhanQiUtil::decodeString(QString s) {
    s.replace("&quot;","\"");
    s.replace("&apos;","'");
    s.replace("&amp;","&");

    return s;
}


int  ZhanQiUtil::levelOfDanmu(const QJsonObject & a) {
    static QString host = myTr("御园麻由mayu");
    static QString author = myTr("天蝎10000");

    QString cmdid = a["cmdid"].toString();
    if ( cmdid != "chatmessage" ) return -1;

    QString f = a["fromname"].toString();
    if (f=="") return 0;
    if (f==host || f == author) return 99;
    int p  = a["permission"].toInt();
    if (  p>=10 ) return p;
    int level = a["level"].toInt();

    if (level ==0  ) return 0;
    return 1;

}
