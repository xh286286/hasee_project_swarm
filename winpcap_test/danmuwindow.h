#ifndef DANMUWINDOW_H
#define DANMUWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QList>
#include <QMap>
#include <QSet>
#include <QColor>

#include "../tcp_socket_test/danmuconnection.h"
class Danmu;

class DanmuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DanmuWindow(QWidget *parent = 0);

    static DanmuWindow * instance;

    static void outputDebugInfor(QString s);

    bool ifDetectingFlag;

    int c_rectHeight;
    int c_backRectHeight;
    int c_fontSize;
    int c_flowerChannel;

    double c_inTime, c_lastTime, c_outTime;
    double c_flowerLastTime;
    bool c_alphaChangeFlag, c_moveChangeFlag;

    QColor c_fontColor, c_backRectColor;

    qint64 paintingTime;



signals:
    void broadcastDanmu(QString s);
public slots:
    void getMessage();
    void getMessage2();
    void addOneDebugInfor(QString s);

    void updateLater();

    void hideandshow();


    void postDanmuMessge(QString s);
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void dealOneMessage(QJsonObject);
    void addOneMessage(QString chatid, QString name, QString content);

    void addOnePresent(int gid, int count, QString uname, QString gname);
    void addOneFlower(int hot, int inc, QString name);
    void messageDecode(QString s);

    void clearDead();

    bool checkDuplicated(QString s);



    int nowChatChannel;
    int nowFlowerChannel;
    int totalChannel;

    QTimer autohideandshowTimer;

    QString test;
    QList<Danmu *> danmuPool;

    QMap< QString , int> flowerRank;

    Qt::WindowFlags flags;

    QList< QString > historyList;
    QSet<QString> historySet;

    QList< DanmuConnection *> danmuConnectionPool;
};

#endif // DANMUWINDOW_H
