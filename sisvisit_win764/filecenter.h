#ifndef FILECENTER_H
#define FILECENTER_H

#include <QJsonDocument>
#include <QString>
#include <QTextStream>
class FileCenter
{
public:
    FileCenter();
    enum SaveFormat {
        Json, Binary
    };
    static bool save(QString filename, const QString & s);
    static bool save(QString filename, const QJsonDocument &, SaveFormat = Json);
    static bool load(QString filename, QJsonDocument &, SaveFormat = Json);
    template <class T>
    static bool saveVar(QString filename, const T &, SaveFormat = Json);
    template <class T>
    static bool loadVar(QString filename, T &, SaveFormat = Json);

    static bool initial();

    static void outputLog(QString s);
protected:
    static QTextStream * log;

};

#endif // FILECENTER_H
