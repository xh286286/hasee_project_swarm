#ifndef ZHANQIHTTPCONNECTION_H
#define ZHANQIHTTPCONNECTION_H

#include <QObject>
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
class ZhanqiHttpConnection : public QObject
{
    Q_OBJECT
public:
    explicit ZhanqiHttpConnection(QObject *parent = 0);
    ~ZhanqiHttpConnection();

    void setUser(QString user, QString password);
    void login();
    void viewRoom(QString room);
    bool debugFlag;
signals:

public slots:

private:
    HttpFileDownloader * hfd;

    QJsonObject loginJo, roomviewerJo;
    QString user, password;
    QJsonObject userInfo;

};

#endif // ZHANQIHTTPCONNECTION_H
