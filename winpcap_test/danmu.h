#ifndef DANMU_H
#define DANMU_H

#include <QObject>
#include <QList>
#include <QPainter>

#include "danmustate.h"
class DanmuWindow;
class Danmu : public QObject
{
    Q_OBJECT
public:

    Danmu(DanmuWindow * p);
    void kill();
    void refresh(int count);
    bool dead();
    void init( );
    void paint(QPainter & painter);
    bool smaller(Danmu*);

    int level();

    bool chatFlag;
    bool flowerFlag;
    bool presentFlag;
    bool delayFlag;
    int channel;
    QString chatid;
    int hot;
    int inc, total;
    int totalFlower, incFlower;

    QString name, content, gift;

    QString displayString;

protected:
    DanmuWindow * pWin;
    qint64 bornTime,lastCheckTime;
    QList<DanmuState *> animation;
    DanmuState  originState,  interState;

};

#endif // DANMU_H
