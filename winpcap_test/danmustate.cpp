#include "danmustate.h"

DanmuState::DanmuState(QRect r, double alpha, double life, QString name):
    rect(r),alpha(alpha),life(life),leftLife(life),name(name)
{
}

double interLiner(double a, double b, double p) {
    return (b-a) * p + a;
}
double interArc(double a, double b, double p) {
    return (b-a) * p*p + a;
}

DanmuState DanmuState::inter(const DanmuState & g) {
    DanmuState n;
    double p = leftLife / life;
    n.alpha = interArc(alpha, g.alpha, p);

    n.rect.setWidth(interArc(rect.width(), g.rect.width(), p) );
    n.rect.setHeight(interArc(rect.height(), g.rect.height(), p) );

    QPointF c;
    c.rx() =interArc(rect.center().x(), g.rect.center().x(), p);
    c.ry() =interArc(rect.center().y(), g.rect.center().y(), p);

    n.rect.moveCenter(c);

    return n;

}
