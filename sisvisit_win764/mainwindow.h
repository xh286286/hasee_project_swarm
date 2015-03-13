#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QMutex>
#include <QNetworkReply>
class QTextEdit;
class QPlainTextEdit;
class QString;
class QNetworkAccessManager;
class QUrl;
class QByteArray;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static void outputInfor(const char * s);
    static void outputInfor(QString s);
    static void showrecehtml(QString s, QList<QNetworkReply::RawHeaderPair> hl);
    static void showPostInfor(QString s);

    static QString runninglog;


    //static MainWindow *getMainW() const;
    static void setMainW(MainWindow *value);

    QString loadTid(QString name);
    void loadThreadPreWork();
public slots:
    void loginsis();
    void registersis();
    void inputdata();
    void generateCatalog();
    void fixPoster();
    void loadthreadfromeinternet();
    void loadthreadfromfileandchecking();
    void working();
    void test4();
    void fanye();
    void robfloor();
    void test6();
    void clearCookie();
    void togglenet();
    void setTimeout();
protected slots:
    void closeEvent(QCloseEvent * event);
private:

    QTextEdit * inforWin;
    QPlainTextEdit * debugWin, * debugWin2, * debugWin3;

    Ui::MainWindow *ui;


    static MainWindow * mainW;
};

#endif // MAINWINDOW_H
