#ifndef AUTHORSET_H
#define AUTHORSET_H

#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QJsonArray>
#include <QDate>

class SisThread;
class KeywordSet;
class AuthorSet : public QJsonObject
{
protected:
    AuthorSet(const QString &);
    QDate lastPost;
public:

    AuthorSet(const QJsonObject &);

    QString author;
    QString tid, page, pid;

    QList<SisThread*> sisthreads;
    QList<KeywordSet*> keywords;

    void model();
    QString postString(QString format = "[author] | [size] [refer]");
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

    static void addOneThread(const QString & author, SisThread * );
    static void filterAuthor();

    static void checking();

    static QString allPostString(bool authorSetCmp(AuthorSet *, AuthorSet *), QString & subject);
    static QString allPostStringByType(QString & subject);

    static QMap<QString, AuthorSet*> pool;

};

bool authorSetCmp1(AuthorSet *, AuthorSet *);
bool authorSetCmp2(AuthorSet *, AuthorSet *);

#endif // AUTHORSET_H
