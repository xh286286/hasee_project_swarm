#ifndef VOICECENTER_H
#define VOICECENTER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QMap>
class VoiceCenter : public QWidget
{
    Q_OBJECT
public:
    explicit VoiceCenter(QWidget *parent = 0);
    ~VoiceCenter();
signals:

public slots:
    void stateChanged(QMediaPlayer::State state);
    void getDanmu(QString);
    void playNextVoice();
private:


    QMediaPlayer * player;
    QList<QJsonObject> danmuPool;
    bool continuePlayFlag;
    QMap<int, int>  voiceCD;
    QMap<QString, QString> langMap;
    int state;
};

#endif // VOICECENTER_H
