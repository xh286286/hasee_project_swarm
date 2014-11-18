#ifndef UTIL_H
#define UTIL_H
#include <QJsonDocument>
#include <QString>

void waitMillisec(int s = 1000);

bool loadJsonFile(QString filename, QJsonDocument & d);
bool saveJsonFile(QString filename, const QJsonDocument & d);


QTextCodec * getCodec();

QString myTr(const char * s);


#endif // UTIL_H
