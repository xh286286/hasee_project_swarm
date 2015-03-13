#ifndef HTTPPROXY_H
#define HTTPPROXY_H

#include <QString>
class HttpProxy
{
public:
    enum SaveFormat {
        Json, Binary
    };
    HttpProxy();
    static void loadProxyFile(const QString & filename);
};

#endif // HTTPPROXY_H
