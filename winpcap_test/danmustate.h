#ifndef DANMUSTATE_H
#define DANMUSTATE_H

#include <QRectF>
#include <QString>
class DanmuState
{
public:
    DanmuState() {}
    DanmuState(QRect r, double alpha, double life = 0, QString name = "");
    QString name;
    QRectF rect;
    double alpha;
    double life, leftLife;

    DanmuState inter(const DanmuState & g) ;

};

#endif // DANMUSTATE_H
