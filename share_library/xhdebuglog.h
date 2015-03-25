#ifndef XHDEBUGLOG_H
#define XHDEBUGLOG_H

#include <QObject>
#include <QTextStream>
#include <QDebug>
class XHDebugLog : public QObject
{
    Q_OBJECT
public:
    explicit XHDebugLog(QObject *parent = 0);
    ~XHDebugLog();

signals:

public slots:
    QString flush();
public:
    template <class T>
    XHDebugLog & operator << ( T t);

    QString debugString;
    QDebug  idebug;

    int outputLevel;

};

#endif // XHDEBUGLOG_H
