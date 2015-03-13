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
        lq.push_back(myTr("顶"));
        lq.push_back(myTr("锅"));
        lq.push_back(myTr("溜"));
        lq.push_back(myTr("麻辣烫"));
        lq.push_back(myTr("女盆友"));
        lq.push_back(myTr("大宝剑"));

    }
    return lq;
}

const QMap<QString, int> & ZhanQiUtil::getGiftMap() {
    static QMap<QString, int> mqi;
    static bool init = false;
    if (!init) {
        init = true;
        mqi[myTr("顶")] = 1;
        mqi[myTr("锅")] = 1;
        mqi[myTr("溜")] = 1;
        mqi[myTr("麻辣烫")] = 60;
        mqi[myTr("女盆友")] = 660;
        mqi[myTr("大宝剑")] = 6660;
    }
    return mqi;
}


const  QMap<QString, int >   & ZhanQiUtil::getBlackMap(const QJsonObject & a) {
    static QMap< int, QMap<QString, int > > black1;
    static QMap< QString , QMap<QString, int > > black2;
    static bool init = false;
    if (!init) {
        init = true;

        QMap<QString, int > saber, dongjing, mutong, mmmm , meng, puppy, xiamu;

        mmmm["danmumiss"] = 100;
        meng["danmumiss"] = 100;

        saber["danmumiss"] = 40;
        saber["voicecd"] = 50000;

        xiamu["noordersong"] = 1;

        puppy["danmumiss"] = 50;

        black2[myTr("puppy")] = puppy;

        black2[myTr("saber")] = saber;

        black2[myTr("MMMM")] = mmmm;

        black2[myTr("夏目")] = xiamu;

        //black2[myTr("梦夢")] = meng;


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
