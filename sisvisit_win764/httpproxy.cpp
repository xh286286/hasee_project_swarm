#include "httpproxy.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QNetworkProxy>
HttpProxy::HttpProxy()
{
}

void HttpProxy::loadProxyFile(const QString & filename)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open proxy file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonObject p = QJsonDocument::fromJson(saveData) .object();

    qDebug()<< "set http proxy";
    qDebug()<<p;

    static QNetworkProxy proxy;
//    proxy.setType(QNetworkProxy::HttpProxy);
//    proxy.setHostName(p["hostname"].toString());
//    proxy.setPort(p["port"].toInt());
//    proxy.setUser(p["username"].toString());
//    proxy.setPassword(p["password"].toString());
//    QNetworkProxy::setApplicationProxy(proxy);

//    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName( "54.0.223.1");
    proxy.setPort(8080);
    proxy.setUser("lixufeng.hn");
    proxy.setPassword( "menge651219" );
    QNetworkProxy::setApplicationProxy(proxy);



    qDebug()<< proxy;
}
