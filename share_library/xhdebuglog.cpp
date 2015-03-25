#include "xhdebuglog.h"



XHDebugLog::XHDebugLog(QObject *parent) : QObject(parent), debugString(), idebug(&debugString)
{
    outputLevel = 1;
}

XHDebugLog::~XHDebugLog()
{

}

QString XHDebugLog::flush()
{
    return "";
}


template <class T>
XHDebugLog &XHDebugLog::operator <<(T t)
{
    idebug << t;

    return this;
}
