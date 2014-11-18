#ifndef IHTTPDOWNLOADS_H
#define IHTTPDOWNLOADS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QFile>
#include <QNetworkReply>
#include <QTimer>
#include <QProgressBar>

class HttpFileDownloader : public QObject
{
Q_OBJECT
public:
    explicit HttpFileDownloader(QObject *parent = 0, QProgressBar *_progressBar = 0);
    ~HttpFileDownloader();
    bool    getFileFromURL(const QUrl &url, const QString &filePath, int timeout = 20000); /* get file from url which we need to download, and restore to filePath */
    bool    getPageFromURL(const QUrl &url,  int timeout = 20000);
    bool    sycGetPageFromURL(const QUrl &url,  int timeout = 20000);
    bool downloadOK() { return okflag;}
    const QString &getLastErrorMessage(); /* if error occurs, use this to get the error message */
    void setErrorMessage(const QString &msg); /* set _errMsg */
    QNetworkReply * getReply() { return _reply;}
    QByteArray getPage() { return pageContent;}
signals:
    void downloadFinished();
    void downloadAbort();

public slots:
    void replyFinished(QNetworkReply*); /* download finished */
    void replayDownloadProgress(qint64, qint64); /* downloading... */
    void slotError(QNetworkReply::NetworkError); /* handle error */
    void readBuffer();
    void slotReadyRead();/* ready read */
    void handleTimeOut(); /* handle time out */

private:
    qint64 dataLength;
    bool fileFlag;
    int timeout;
    bool okflag;
    QByteArray pageContent;
    qint64 lastProgress;
    QNetworkAccessManager    *_downloadManager;
    QNetworkReply            *_reply;
    QString _errMsg;
    QFile    _file;
    QProgressBar *_progressBar;
    QTimer        *_timeOut;
    QString _filePath;
    QUrl    _url;
};

#endif // IHTTPDOWNLOADS_H
