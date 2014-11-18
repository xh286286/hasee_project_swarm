#include "httpfiledownloader.h"



#include <QCoreApplication>
#include <QDebug>
#include <cassert>
#include <QMutex>
#include <QMutexLocker>
QMutex replyMutex;

HttpFileDownloader::HttpFileDownloader(QObject *parent, QProgressBar *bar) :
    QObject(parent), _progressBar(bar)
{
    _downloadManager = new QNetworkAccessManager(this);
    _timeOut = new QTimer(this);
    _timeOut->stop();
    okflag = false;
    connect(_downloadManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(_timeOut, SIGNAL(timeout()), this, SLOT(handleTimeOut()));
}
HttpFileDownloader::~HttpFileDownloader()
{
    //_reply->deleteLater();
    _downloadManager->deleteLater();
}
bool    HttpFileDownloader::sycGetPageFromURL(const QUrl &url,  int timeout )
{
    bool flag = getPageFromURL(url,timeout);
    assert(flag);

    QTime t;
    t.start();
    while(true) {
        {
            QMutexLocker l(&replyMutex);
            if (!_reply->isRunning())break;

        }
        QCoreApplication::processEvents();
        if (t.elapsed()>timeout) {
            qDebug()<<"timeout";
            _reply->deleteLater();
            return "timeout";
        }
    }

    //qDebug()<<"rece   time spend " << t.elapsed();

    if (_reply->error() != QNetworkReply::NoError) {
        QString loadresult = "error code " + QString::number( _reply->error());
        qDebug()<<loadresult;
        _reply->deleteLater();
        return false;
    }
    _reply->deleteLater();
    return true;
}
bool    HttpFileDownloader::postFromURL(const QUrl &url, QByteArray ba,  int timeout) {
    _errno = QNetworkReply::NoError;
    pageContent = "";
    fileFlag = false;
    okflag = false;
    this->timeout = timeout;
    _timeOut->stop();
    _timeOut->start(timeout);
    /* confirm the url is valid or not */
    if (!url.isValid())
    {
        setErrorMessage(QString("Error:URL has specify a invalid name."));
        return false;
    }

    if (url.scheme() != "http")
    {
        setErrorMessage(QString("Error:URL must start with 'http:'"));
        return false;
    }

    if (url.path().isEmpty())
    {
        setErrorMessage(QString("Error:URL's path is empty."));
        return false;
    }


    _url = url;

    _reply = _downloadManager->post(QNetworkRequest(url), ba);
    connect(_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(replayDownloadProgress(qint64, qint64)));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
//    connect(_reply, SIGNAL(readyRead()), this, SLOT(readBuffer()));
    return true;
}

bool    HttpFileDownloader::getPageFromURL(const QUrl &url,  int timeout  )
{
    _errno = QNetworkReply::NoError;
    pageContent = "";
    fileFlag = false;
    okflag = false;
    this->timeout = timeout;
    _timeOut->stop();
    _timeOut->start(timeout);
    /* confirm the url is valid or not */
    if (!url.isValid())
    {
        setErrorMessage(QString("Error:URL has specify a invalid name."));
        return false;
    }

    if (url.scheme() != "http")
    {
        setErrorMessage(QString("Error:URL must start with 'http:'"));
        return false;
    }

    if (url.path().isEmpty())
    {
        setErrorMessage(QString("Error:URL's path is empty."));
        return false;
    }


    _url = url;

    _reply = _downloadManager->get(QNetworkRequest(url));
    connect(_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(replayDownloadProgress(qint64, qint64)));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
//    connect(_reply, SIGNAL(readyRead()), this, SLOT(readBuffer()));
    return true;
}

bool HttpFileDownloader::getFileFromURL(const QUrl &url, const QString &filePath, int timeout) /* get file from url which we need to download, and restore to filePath */
{
    _errno = QNetworkReply::NoError;
    dataLength = 0;
    fileFlag = true;
    okflag = false;
    this->timeout = timeout;
    _timeOut->stop();
   // _timeOut->start(timeout);
    /* confirm the url is valid or not */
    if (!url.isValid())
    {
        setErrorMessage(QString("Error:URL has specify a invalid name."));
        return false;
    }

    if (url.scheme() != "http")
    {
        setErrorMessage(QString("Error:URL must start with 'http:'"));
        return false;
    }

    if (url.path().isEmpty())
    {
        setErrorMessage(QString("Error:URL's path is empty."));
        return false;
    }

    if (filePath.isEmpty())
    {
        setErrorMessage(QString("Error:invalid filePath."));
        return false;
    }
    _file.close();
    _file.setFileName(filePath);
    if (!_file.open(QIODevice::WriteOnly))
    {
        setErrorMessage(QString("Error:Cannot open file."));
        return false;
    }
    _url = url;
    _filePath = filePath;

    _reply = _downloadManager->get(QNetworkRequest(url));
    _reply->setReadBufferSize(100000000);
    connect(_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(replayDownloadProgress(qint64, qint64)));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
 //   connect(_reply, SIGNAL(readyRead()), this, SLOT(readBuffer()));
    return true;
}

const QString &HttpFileDownloader::getLastErrorMessage()
{
    return _errMsg;
}
void HttpFileDownloader::setErrorMessage(const QString &msg)
{
   // qDebug()<<msg;
    _errMsg = msg;
}

/* slots */
void HttpFileDownloader::handleTimeOut()
{
    qDebug()<<"time out..";
    _timeOut->stop();

    if (_file.isOpen())
    {
        _file.close();
    }

    emit downloadAbort();
    return;
    if (fileFlag)     getFileFromURL(_url, _filePath, timeout); /* try again */
    else getPageFromURL(_url, timeout);
}
void HttpFileDownloader::replyFinished(QNetworkReply* reply) /* download finished */
{
    QMutexLocker l(&replyMutex);
    //qDebug()<< reply->errorString();
 //   qDebug()<< reply->rawHeaderList();
  //  qDebug()<< reply->rawHeaderPairs();

    if (reply->hasRawHeader("Location")) {
        QUrl u(reply->rawHeader("Location"));
        if (fileFlag)     {
            _file.close();
            getFileFromURL(u, _filePath, timeout); /* try again */
        }
        else getPageFromURL(u, timeout);
        return;
    }

    if (fileFlag) {
        //qDebug()<< dataLength;
        _file.waitForBytesWritten(10000);/* wait 10s for write to file complete, can comment this */
        if (0 == _file.size())
        {
           // qDebug()<<"Nothing be downloaded.";
        }
        else
        {
            /* add our updateUI code here... */
          //  qDebug()<<"finished";
        }

        _file.close();

    }
    else {
        //qDebug()<< pageContent.size();
    }
           _timeOut->stop();



        okflag = true;
        emit downloadFinished();
}
void HttpFileDownloader::replayDownloadProgress(qint64 done, qint64 total) /* downloading... */
{

    if (done != lastProgress) {
        _timeOut->start(timeout);
        lastProgress = done;
    }
    static int lastStep = 0;
    int a = done / (double)total * 100;
    if (a+10<lastStep) lastStep = 0;
    if (a>=lastStep) {
  //      qDebug()<<QString("%1%").arg(a);
        lastStep +=10;
    }
    if ((0 != _progressBar) && (0 != total))
    {
        _progressBar->setMaximum(total);
        _progressBar->setValue(done);
    }
}

void HttpFileDownloader::readBuffer()
{
    QMutexLocker l(&replyMutex);
    if (fileFlag) {
            _file.write(this->_reply->readAll());

    }
    else {
        pageContent += this->_reply->readAll();
    }
}

void HttpFileDownloader::slotReadyRead() /* if this is not been fired for 30s, we trate this timeout, and the timout handle will re download the file */
{
    QMutexLocker l(&replyMutex);
    if (fileFlag) {
          dataLength +=  _file.write(this->_reply->readAll());

    }
    else {
        pageContent += this->_reply->readAll();
    }

    //_timeOut->start(30000);/* wait 30 seconds */
}
void HttpFileDownloader::slotError(QNetworkReply::NetworkError errorCode) /* handle error */
{
    _errno = errorCode;
  //  qDebug()<<"error:"<<errorCode;
    setErrorMessage(QString("Error:NetworkError code:%1").arg(errorCode));
    if (_file.isOpen())
    {
        _file.close();
    }
    return;
    getFileFromURL(_url, _filePath);/* try again */
}
