#include <QDateTime>


#include <QRect>
#include <cassert>
#include <cmath>
#include <QDebug>

#include "danmu.h"
#include "danmustate.h"
#include "danmuwindow.h"
#include "../share_library/Util.h"

Danmu::Danmu(DanmuWindow * p)
{
    pWin = p;
    chatFlag = false;
    flowerFlag = false;
    presentFlag = false;
    delayFlag = false;
    bornTime = QDateTime::currentMSecsSinceEpoch();
    lastCheckTime = bornTime;
}
void Danmu::kill()
{
    if (animation.size()<=1) return;
    while(animation.size()>1) animation.pop_front();
}

void Danmu::refresh(int count)
{
    inc = count;
    QRect r = pWin->rect();
    r.setHeight( pWin->c_rectHeight);
    r.moveBottom(pWin->height() -  pWin->c_rectHeight * channel);
    assert(animation.size()>0);
    DanmuState * ds = animation.back();
    animation.pop_back();
    if (animation.size()==0) {
        interState =  originState;
        ds->leftLife = ds->life;
    }
    while(animation.size()>1) animation.pop_back();

    double multi = log( level() * double(inc)) *10;
    animation.push_back(new DanmuState(r,1, pWin->c_flowerLastTime * multi ));

    animation.push_back(ds);
    if (chatFlag) {
        displayString = "["+name + "]  :  " + content;
    }
    else if (flowerFlag){
        displayString = "["+name + "]" + myTr(" 送出鲜花总计：") + QString::number(total) + myTr("朵");
    }
    else if (presentFlag) {
        displayString = "["+name + "]" + myTr(" 送出：") + QString::number(inc) + myTr("个") + gift;
    }

}
void Danmu::init()
{
    QRect r = pWin->rect();
    r.setHeight( pWin->c_rectHeight);
    r.moveBottom(pWin->height() -  pWin->c_rectHeight * channel);
    QRect r1 = r;
    if (pWin->c_moveChangeFlag) r1.moveLeft(pWin->width());

    double a1;
    if (pWin->c_alphaChangeFlag) a1 = 0;
    else a1 = 1;

    originState =  DanmuState(r1, a1) ;

    if (delayFlag) {
        animation.push_back(new DanmuState(r1,a1, pWin->c_outTime));
    }
    animation.push_back(new DanmuState(r,1,pWin->c_inTime));
    if (chatFlag) {
        animation.push_back(new DanmuState(r,1,pWin->c_lastTime));
    }
    else {
        double multi = log( level() * double(inc))  *10;
        animation.push_back(new DanmuState(r,1, pWin->c_flowerLastTime * multi ));

    }

    animation.push_back(new DanmuState(r1,a1, pWin->c_outTime));

    if (chatFlag) {
        displayString = "["+name + "]  :  " + content;
    }
    else if (flowerFlag){
        displayString = "["+name + "]" + myTr(" 送出鲜花总计：") + QString::number(total) + myTr("朵");
    }
    else if (presentFlag) {
        displayString = "["+name + "]" + myTr(" 送出：") + QString::number(inc) + myTr("个") + gift;
    }

}

void Danmu::paint(QPainter & painter) {
    qint64 passT = pWin->paintingTime - lastCheckTime;
    lastCheckTime = pWin->paintingTime;

    while (animation.front()->leftLife < passT) {
        originState = *(animation.front());
        passT -= animation.front()->leftLife;
        delete animation.front();
        animation.pop_front();
        if (animation.size()==0) return;
        if (animation.front()->name == "addflower") {
            total++;
            displayString = name + myTr(" 送出鲜花总计：") + QString::number(total) + myTr("朵");
        }
    }
    animation.front()->leftLife -= passT;
    interState = animation.front()->inter(originState);

    QRectF backRect = painter.boundingRect(interState.rect, Qt::AlignVCenter | Qt::AlignLeft, displayString);
     painter.setPen(pWin->c_backRectColor);
    painter.drawRoundedRect(backRect,4,4);

    if (presentFlag) painter.setPen("red");
    else painter.setPen(pWin->c_fontColor);
    painter.drawText(interState.rect, Qt::AlignVCenter | Qt::AlignLeft, displayString);



}


bool Danmu::dead()
{
    if (animation.size() == 0) return true;
    return false;
}


bool Danmu::smaller(Danmu* x)
{
    if (level()<= x->level()) return true;
    return false;
}

int Danmu::level()
{

    if (gift==myTr("顶")) return 2;
    if (gift==myTr("锅")) return 3;
    if (gift==myTr("溜")) return 3;
    if (gift==myTr("麻辣烫")) return 3*60;
    if (gift==myTr("超人内裤")) return 3*660;
    if (gift==myTr("大宝剑")) return 3*20000;
    if (gift==myTr("")) return 1;
    if (gift==myTr("")) return 1;
    return 0;
}
