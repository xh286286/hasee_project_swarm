﻿#ifndef DANMUCONNECTION_H
#define DANMUCONNECTION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QJsonObject>
#include <QList>
#include <QSet>
class HttpFileDownloader;
class DanmuConnection : public QObject
{
    Q_OBJECT
public:
    explicit DanmuConnection(QObject *parent = 0);

    bool busy();
    bool debugFlag;
    QList<QJsonObject> getALLMess() {  auto a = infoPool; infoPool.clear(); return a; }
signals:
    void messReady();
protected slots:
    void getHttpInfo();
    void getSocketInfo();
    void socketError(QAbstractSocket::SocketError se);
    void dealError();
public slots:
    bool postDanmu(QString s);
    void login(QString name, QString pass, QString uid, QString id);

    void keepAlive();
protected:
    void work1();
    void work2();
    void work3();
    void work4();
    void work42();
    void work5();
    void work6();
    void writeSocketData(int type, QJsonObject jo);
    void writeSocketString(int type, QString  );


private:


    QSet<QString > historyMessage;
    QList< QJsonObject> infoPool;

    QTimer connectionTimeout, keepaliveTimer;
    QJsonObject loginJo, roomviewerJo;
    QByteArray cache;
    HttpFileDownloader * hfd;
    QTcpSocket * ts;
    int state;
    QString username, password,uid,id;
};

#endif // DANMUCONNECTION_H
