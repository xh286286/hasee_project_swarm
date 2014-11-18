#include "voicecenter.h"
#include "Util.h"
#include <QJsonDocument>
VoiceCenter::VoiceCenter(QWidget *parent) :
    QWidget(parent)
{
    player = new QMediaPlayer(this);
    continuePlayFlag  = true;
    connect(player, &QMediaPlayer::stateChanged, this, &VoiceCenter::stateChanged);
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

        QString cm = a["content"].toString();
        cm.replace("&quot;","\"");
        cm.replace("&apos;","'");
        cm.replace("&amp;","&");
        cm.replace("+"," ");
        cm.replace("_"," ");
        if (cm.indexOf(myTr("语音")) !=0 && cm.indexOf(myTr("日语")) !=0) return;
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
    if (danmuPool.size()==0) return;
    QJsonObject a = danmuPool.front();
    danmuPool.pop_front();

    QString qh1 = "http://tts.baidu.com/text2audio?lan=zh&pid=103&ie=UTF-8&text=";
    QString qh2 = "http://tts.baidu.com/text2audio?lan=jp&pid=103&ie=UTF-8&text=";
    QString qh,cm;
    cm = a["content"].toString();
    if (cm.indexOf(myTr("语音")) == 0) qh  = qh1;
    if (cm.indexOf(myTr("日语")) == 0) qh  = qh2;

    QString name = a["fromname"].toString();
    int x = name.indexOf('_');
    if (x>=0) {
        name = name.left(x);
    }

    QString word = name + myTr("说,")+ cm.mid(2);
    qDebug()<<"play "<<word;
    QByteArray ba = qh.toUtf8();
    ba.append(word.toUtf8().toPercentEncoding());
    QUrl u ( ba );

    player->setMedia(u);
    player->play();
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
