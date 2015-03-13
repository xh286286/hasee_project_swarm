#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H
#include <QNetworkReply>
#include <QMutex>
#include <QNetworkReply>
#include <QJsonArray>
#include <QList>
class NetworkUtil: public QObject
{
    Q_OBJECT
public:
    NetworkUtil();

    static void initial();
    static QString sendquest(const QUrl & url, QByteArray * =0, QList<QNetworkReply::RawHeaderPair> * = 0, int timeout = 10000);
    static QString getHashCode(const QUrl &,QString * page=0);
    static bool sendreply(QString tid, QString message, QString * pid = 0, QString * pagenumber = 0, QString subject = "", int timeout = 10000);
    static bool editpost(QString tid, QString pid , QString subject, QString message , int timeout = 10000);
    static bool editMainPost(QString tid, QString pid , QString type, QString subject, QString message , int timeout = 10000);
    static QString mustLoadPage(QString url);
    static bool mustSendreply(QString tid, QString subject, QString message ,   QString * pid = 0,   QString * pagenumber = 0 );
    static bool mustEditPost(QString tid, QString pid , QString subject, QString message );
    static bool mustEditMainPost(QString tid, QString pid , QString type, QString subject, QString message );
    static void loginsis();
    static QJsonArray getPosterListPid(QString tid, int n, QString jobfile);
    static QJsonArray getPosterList(QString tid, int n, QString jobfile);
    static QJsonArray getPosterOfPage(QString html );
    static void clearCookies();
    static QList<QString> getPidOfPage(QString html );

    static bool offInternetDebugMode;
    static int baseTimeout;
    static int maxPostMesageSize;
public slots:
    void replyFinished(QNetworkReply*);

    void visiterror(QNetworkReply::NetworkError code);
    void downloadProgress(qint64 , qint64 ) ;


protected:

    static NetworkUtil * network;
    QString uid;
    //QString loadresult;
    QNetworkReply::NetworkError errorcode;
    QString hashcode;
    QMutex hashcodemutex;
    QNetworkAccessManager * nam;
};

#endif // NETWORKUTIL_H
