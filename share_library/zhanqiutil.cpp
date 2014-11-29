#include "Util.h"
#include "zhanqiutil.h"

ZhanQiUtil::ZhanQiUtil()
{
}
QString ZhanQiUtil::encodeString(QString s) {
     s.replace("&", "&amp;");
     s.replace("\"","&quot;" );
    s.replace("'", "&apos;");
    s.replace(" ", "");
    return s;
}

QString ZhanQiUtil::decodeString(QString s) {
    s.replace("&quot;","\"");
    s.replace("&apos;","'");
    s.replace("&amp;","&");

    return s;
}

//if (u == myTr("顶")) return 1;
//if (u == myTr("锅")) return 1;
//if (u == myTr("溜")) return 1;
//if (u == myTr("麻辣烫")) return 60;
//if (u == myTr("女盆友")) return 660;
//if (u == myTr("大宝剑")) return 6660;

const QList<QString> & ZhanQiUtil::getGiftList() {
    static QList<QString> lq;
    static bool init = false;
    if (!init) {
        init = true;
        lq.push_back("顶");
        lq.push_back("锅");
        lq.push_back("溜");
        lq.push_back("麻辣烫");
        lq.push_back("女盆友");
        lq.push_back("大宝剑");

    }
    return lq;
}

const QMap<QString, int> & ZhanQiUtil::getGiftMap() {
    static QMap<QString, int> mqi;
    static bool init = false;
    if (!init) {
        init = true;
        mqi["顶"] = 1;
        mqi["锅"] = 1;
        mqi["溜"] = 1;
        mqi["麻辣烫"] = 60;
        mqi["女盆友"] = 660;
        mqi["大宝剑"] = 6660;
    }
    return mqi;
}


const  QMap<QString, int >   & ZhanQiUtil::getBlackMap(const QJsonObject & a) {
    static QMap< int, QMap<QString, int > > black1;
    static QMap< QString , QMap<QString, int > > black2;
    static bool init = false;
    if (!init) {
        init = true;

        QMap<QString, int > saber, dongjing, mutong ;
        saber["danmumiss"] = 20;
        saber["voicecd"] = 30000;
        saber["novoiceid"] = 1;

        dongjing["danmumiss"] = 80;
        dongjing["novoice"] = 1;
        dongjing["noordersong"] = 1;

        mutong["danmumiss"] = 35;



        black2[myTr("saber")] = saber;
        black2[myTr("进击的")] = saber;
        black2[myTr("东京")] = dongjing;
        black2[myTr("木桐")] = mutong;

        black2[myTr("九千")] = dongjing;

        black1[299998] = saber;
    }
    int id =  a["fromuid"].toInt();

    if (black1.contains( id )) {
        return black1[id];
    }
    QString name = a["fromname"].toString();
    for (auto i = black2.begin(); i!=black2.end(); i++) {
        if (name.indexOf(i.key())>=0) {
            return i.value();
        }
    }
    static QMap<QString, int >  empty;
    return empty ;


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
