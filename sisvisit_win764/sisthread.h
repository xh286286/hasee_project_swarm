#ifndef SISTHREAD_H
#define SISTHREAD_H

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QMultiMap>
#include <QList>

class AuthorSet;
class KeywordSet;

class SisThread : public QJsonObject
{
//    Q_OBJECT
protected:
    SisThread();
    void init();
public:
//    SisThread();
    SisThread(const QJsonObject &);
    SisThread(const QString & page, int index, const QString & forum );

    bool filter();
    QString postString(QString format = "[date] [title] [refer]");

    static QList<SisThread *> getOnePage(const QString &, QString  forum );
    static QList<SisThread *> getOneForum(const QString &  );

    static QMultiMap<QString, SisThread*> phrase;


    static void read(const QJsonArray &);
    static void write(QJsonArray &);

    static QMap<QString, SisThread * > pool;
public:
    static QList< SisThread * > trashList;

    QString type, title, author;
    QString tid,forum;
    QDate date;
    QList<AuthorSet * > backReferAuthor;
    QList<KeywordSet * > backReferKeyword;
protected:
    static void addOne(const QString &, SisThread* );
};
bool sisThreadCompare(SisThread *, SisThread*);
bool sisThreadCompareByTid(SisThread *, SisThread*);
#endif // SISTHREAD_H
