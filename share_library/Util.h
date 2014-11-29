#ifndef UTIL_H
#define UTIL_H
#include <QJsonDocument>
#include <QString>
#include <QByteArray>

void waitMillisec(int s = 1000);

bool loadJsonFile(QString filename, QJsonDocument & d);
bool saveJsonFile(QString filename, const QJsonDocument & d);

const QJsonObject & getGlobalParameter() ;

QString getGlobalParameterString(QString p, QString d);

QTextCodec * getCodec();

QString myTr(const char * s);

QByteArray consoleTr(QString s);
void debugToConsole(QString s);

int getRandomInt();

#endif // UTIL_H
