#include "zhanqihttpconnection.h"
#include "../share_library/httpfiledownloader.h"
#include <QJsonDocument>
ZhanqiHttpConnection::ZhanqiHttpConnection(QObject *parent) : QObject(parent)
{
        hfd = NULL;
        debugFlag = false;
}

ZhanqiHttpConnection::~ZhanqiHttpConnection()
{

}

void ZhanqiHttpConnection::setUser(QString user, QString password)
{
    this->user = user;
    this->password = password;

    if (hfd!=NULL) {
        hfd->disconnect();
        hfd->deleteLater();
        hfd = NULL;
    }

    hfd = new HttpFileDownloader();
    userInfo = QJsonObject();
}

void ZhanqiHttpConnection::login()
{
    QUrl u("http://www.zhanqi.tv/api/auth/user.login");
    QByteArray ba= "account=" ;
    ba.append(user.toUtf8().toPercentEncoding());
    ba.append( "&password=");
    ba.append(password.toUtf8().toPercentEncoding());
    //ba = s.toUtf8().toPercentEncoding();
    if (debugFlag) qDebug()<<ba;
   // QObject::connect(hfd, &HttpFileDownloader::downloadFinished, this, &DanmuConnection::getHttpInfo);
   // QObject::connect(hfd, &HttpFileDownloader::downloadAbort, this, &DanmuConnection::dealError);

    hfd->sycPostFromURL(u,ba, 10000);

    loginJo =  QJsonDocument::fromJson(hfd->getPage()) .object();

    if (password != "" ) {
        if (loginJo["code"].isNull() || loginJo["code"].toInt() !=0)     {

        }
    }
}




