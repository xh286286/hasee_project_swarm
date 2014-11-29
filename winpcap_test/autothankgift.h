#ifndef AUTOTHANKGIFT_H
#define AUTOTHANKGIFT_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QJsonObject>
class AutoThankGift : public QObject
{
    Q_OBJECT
public:
    explicit AutoThankGift(QObject *parent = 0);

signals:
    void postThankMessage(QString s);
public slots:
    void dealPresent(QJsonObject jo);
protected slots:
    void work();
private:
    QMap< QString , QMap< QString , int> >  giftHistory;
    QMap< QString , int > lastGiftTime;
    QMap< QString , int > lastThankTime;

    QTimer eventTimer;

    QList<QString > thankWord;

};

#endif // AUTOTHANKGIFT_H
