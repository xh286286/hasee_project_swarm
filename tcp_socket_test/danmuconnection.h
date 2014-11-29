#ifndef DANMUCONNECTION_H
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

    void postDanmuWork();
public slots:
    bool postDanmu(QString s);
    void login(QString name, QString pass, QString uid, QString id);

    void blockUser(QString name, int uid, int action = 1);

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

    QList<QString> danmuPool;

    QTimer connectionTimeout, keepaliveTimer, postDanmuTimer;
    QJsonObject loginJo, roomviewerJo;
    QByteArray cache;
    HttpFileDownloader * hfd;
    QTcpSocket * ts;
    int state;
    QString username, password,uid,id;
};

#endif // DANMUCONNECTION_H
