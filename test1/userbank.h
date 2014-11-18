#ifndef USERBANK_H
#define USERBANK_H

#include <QObject>
#include <QMap>
class UserBank : public QObject
{
    Q_OBJECT
    explicit UserBank(QObject *parent = 0);
public:

    static int evaluateGift(QString u);

    static int leftMoney(QString u) ;
    static int saveMoney(QString u, int m);
    static int withdrawMoney(QString u, int m);

    static bool load();
    static bool save();
signals:

public slots:

private:
    static QMap<QString , int> record;
};

#endif // USERBANK_H
