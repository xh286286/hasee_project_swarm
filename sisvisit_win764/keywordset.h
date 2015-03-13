#ifndef KEYWORDSET_H
#define KEYWORDSET_H

#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QJsonArray>
#include <QDate>

class AuthorSet;
class SisThread;
class KeywordSet : public QJsonObject
{
protected:
    KeywordSet(const QString &);
    QDate lastPost;
public:

    KeywordSet(const QJsonObject &);

    QString keyword;
    QString tid, page, pid;

    QList<SisThread*> sisthreads;
    QList<AuthorSet*> authors;

    void model();
    QString postString(QString format = "[keyword] | [size] [refer]");
    QDate lastPostTime();

    int ifqualify;
    bool qualify();
    bool getPostLocation();
    bool finalPost(bool & same);

    static int qualifySize();
    static bool distributeAllPostLocation(QString tid, QJsonArray);
    static bool getAllPostLocation();
    static bool finalAllPost();

    void buildRefer();
    void buildDepthRefer();

    static void buildAllRefer();
    static void buildAllDepthRefer();

    static void read(const QJsonArray &);
    static void write(QJsonArray &);
    static bool save(QString file);
    static bool load(QString file);

    static QList< QString > strange;
    static void checkKeyword(const QString & title, const QString & author);
    static void filterKeyword();

    static void checking();
    static QString allPostString(bool authorSetCmp(KeywordSet *, KeywordSet *),QString & subject);
    static QString allPostStringByType(QString & subject);

    static QMap<QString, KeywordSet*> pool;

};

bool keywordSetCmp1(KeywordSet *, KeywordSet *);
bool keywordSetCmp2(KeywordSet *, KeywordSet *);

#endif // TITLESET_H
