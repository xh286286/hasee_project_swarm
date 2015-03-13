#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <string>
class QTextCodec;
class QString;
class Util : public QObject
{
    Q_OBJECT
public:
    explicit Util(QObject *parent = 0);
    static void loadData();
    static void inputData();
    static void writeData();
    static QTextCodec * getCodec();

    static QString username, userpass, loginquestionid, loginanswer, filename, host;
    static QString regpage;
    static QString ifreg;
    static int minTopicThreads;
    static int maxPostMesageSize;
signals:

public slots:

protected:
    static QTextCodec * tcl;

};


QString myTr(const char *);

QString full2half( const QString &);

QByteArray toSisPE(QString s);
QString getMidString(const QString &  target, int index, const QString & start, const QString & left, const QString & right, int * resultIndex = nullptr);
void waitMillisec(int s = 1000);
#endif // UTIL_H
