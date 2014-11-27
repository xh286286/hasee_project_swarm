#include "voicecenter.h"
#include "../share_library/Util.h"
#include <QJsonDocument>
#include <QTime>
VoiceCenter::VoiceCenter(QWidget *parent) :
    QWidget(parent)
{
    player = new QMediaPlayer(this);
    continuePlayFlag  = true;
    connect(player, &QMediaPlayer::stateChanged, this, &VoiceCenter::stateChanged);
    state = 0;
    langMap[myTr("中文")] = "zh";
    langMap[myTr("语音")] = "zh";
    langMap[myTr("英语")] = "en";
    langMap[myTr("粤语")] = "cte";
    langMap[myTr("日语")] = "jp";
    langMap[myTr("韩语")] = "kor";


}

VoiceCenter::~VoiceCenter()
{
    delete player;
}
void VoiceCenter::getDanmu(QString s)
{

    QJsonObject a = QJsonDocument::fromJson(s.toUtf8()).object();

    QString host = myTr("御园麻由mayu");
    QString author = myTr("天蝎10000");
    QString cmdid = a["cmdid"].toString();
    if ( cmdid == "chatmessage" ) {

        QString f = a["fromname"].toString();
        if (f=="") return;

        int level = a["level"].toInt();
        if (level ==0 && f!=host && f!=author) return;

        if (f == host) {
            a["fromname"] = myTr("大哥");
        }
        //  get id
        int id = a["fromuid"].toInt();

        int cd = 10000;
        if (id == 299998) {
            cd *=4 +30000;
            a["fromname"] = myTr("进击");

        }

        // get time
        int time = QTime::currentTime().msecsSinceStartOfDay();
        if (!voiceCD.contains(id)) {
            voiceCD[id] = time;
        }
        else {
            int x = time - voiceCD[id];
            if (x > cd) {
                voiceCD[id] = time;
            }
            else return;

        }



        QString cm = a["content"].toString();
        cm.replace("&quot;","\"");
        cm.replace("&apos;","'");
        cm.replace("&amp;","&");
        cm.replace("+"," ");
        cm.replace("_"," ");

        if ( langMap.contains(cm.left(2))) {

        }
        else {
            return;
        }
        qDebug()<<s;
        qDebug()<<"get voice "<<cm;
        a["content"] = cm;
        danmuPool.push_back(a);
        if (player->state() == QMediaPlayer::StoppedState ) {

            playNextVoice();
        }
    }
}



void VoiceCenter::playNextVoice()
{
    QString www = "http://tts.baidu.com/text2audio";
    QString qh1 = "lan=";
    QString qh2 = "&pid=103&ie=UTF-8&text=";

    if (danmuPool.size()==0) return;
    QJsonObject a = danmuPool.front();
    QString word ;
    QString qh,cm;
    QString name;
    cm = a["content"].toString();
    int x;
    switch (state) {
    case 0:
        name = a["fromname"].toString();
        x = name.indexOf('_');
        if (x>=0) {
            name = name.left(x);
        }
        word = name;
        break;
    case 1:
        word =   myTr("说");
        break;
    case 2:
        word = cm.mid(2);
        break;

    }


    if (state!=2) {
        qh  = qh1+ langMap[myTr("中文")] + qh2;

    }
    else {
        qh = qh1 + langMap[cm.left(2)] + qh2;
    }



    QByteArray ba = qh.toUtf8();
    ba.append(word.toUtf8().toPercentEncoding());


    QString s = ba;

    QUrl u ( www);

    u.setQuery(s, QUrl::DecodedMode);

    player->setMedia(u);
    player->play();

    state++;
    if (state == 3) {
        danmuPool.pop_front();
        state = 0;
    }
}

void VoiceCenter::stateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        //statusBar->clearMessage();

        if (continuePlayFlag) playNextVoice();
        break;
    }
}
