#include "util.h"
#include <cassert>
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QInputDialog>
#include <QUrl>
#include <QTextCodec>
#include <QTime>
#include <QCoreApplication>
Util::Util(QObject *parent) :
    QObject(parent)
{
}

QString Util::filename = "save.ini";
QString Util::username="", Util::userpass="", Util::host="";
QString Util::loginquestionid="", Util::loginanswer="";
QString Util::ifreg = "false", Util::regpage = "5685922";
int Util::minTopicThreads = 5;

QTextCodec * Util::tcl = nullptr;

QTextCodec * Util::getCodec()
{
    if (tcl == nullptr) {
        QByteArray aa = "记";
        qDebug()<< aa.toPercentEncoding();
        if (aa.toPercentEncoding() == "%E8%AE%B0")
            tcl = QTextCodec::codecForName("utf8");
            else tcl = QTextCodec::codecForName("GB18030");
    }
    return tcl;
}

QString myTr(const char * s)
{
    return Util::getCodec()->toUnicode(s);
}

QString full2half( const QString & s)
{

    static QMap < QChar, QChar> mcc;
    static bool init = false;
    if (!init) {
        init = true;
        QString s1 = myTr("ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ０１２３４５６７８９（）");
        QString s2 = myTr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789()");
        assert(s1.size() == s2.size());
        for (int i=0; i<s1.size(); i++) {
            mcc[s1[i]] = s2[i];
        }
    }
    QString re = s;
    for (int i=0; i<s.size(); i++) {
        if (mcc.count(s[i])>0) {
            re[i] = mcc[s[i]];
        }
    }
    return re;
}

QByteArray toSisPE(QString s)
{
    static QTextCodec * tcl = QTextCodec::codecForName("GB18030");
    return tcl->fromUnicode(s).toPercentEncoding();
}
void waitMillisec(int s )
{
    QTime t;
    t.start();
    while(t.elapsed()<s)  QCoreApplication::processEvents();
}

QString getMidString(const QString & target, int index, const QString &  start, const QString &  left, const QString &  right, int * resultIndex )
{
    int f;
    if (resultIndex == nullptr)  resultIndex = & f;

    int a1 = target.indexOf(start,index);
    int a2 = target.indexOf(left,a1);
    int a3 = target.indexOf(right,a2+1);
    a2+= left.size();
    if (a1<0 || a2 <0 || a3<0) {
        *resultIndex = -1;
        return "";
    }

    *resultIndex = a2;
    return target.mid(a2,a3-a2);
}

void Util::inputData()
{
    bool ok;
    username = QInputDialog::getText(0, "", myTr("用户名"), QLineEdit::Normal, username , &ok);

    userpass = QInputDialog::getText(0, "", myTr("密码"), QLineEdit::Password, userpass , &ok);

    QStringList items;
    items << myTr("无安全提问") << myTr("母亲的名字") << myTr("爷爷的名字") << myTr("父亲出生的城市") << myTr("您其中一位老师的名字") << myTr("您个人计算机的型号") << myTr("您最喜欢的餐馆名称") << myTr("驾驶执照的最后四位数字");
    QString item = QInputDialog::getItem(0, "", myTr("安全提问") , items, loginquestionid.toLong(), false, &ok);
    for (int i=0; i<items.size(); i++) {
        if (item == items.at(i)) loginquestionid = QString::number(i);
    }
    loginanswer = QInputDialog::getText(0, "", myTr("回答"), QLineEdit::Normal, loginanswer , &ok);


    QStringList hosts;
    hosts << "http://sexinsex.net/"<< "http://174.127.195.200/" << "http://174.127.195.201/"<< "http://67.220.90.10/"<< "http://174.127.195.166/"<< "http://67.220.90.4/";
    int x = 1;
    auto t =  QUrl::fromUserInput(host);
    host =t.url();
    if (host != "") {

        bool same = false;

        for (int i=0; i<hosts.size(); i++) {
            if (host == hosts.at(i)) { x = i; same = true; break; }
        }
        if (!same) {
            hosts<< host;
            x = hosts.size()-1;
        }
    }
    host = QInputDialog::getItem(0, "", myTr("IP地址") , hosts, x, true, &ok);
    regpage = QInputDialog::getText(0, "", myTr("本月登陆页面编号"), QLineEdit::Normal, regpage , &ok);

    writeData();
}

void Util::loadData()
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(0, "Warning", myTr("找不到配置文件,请输入用户信息!"));
        inputData();
        return;

    }
    QXmlStreamReader xml(&file);

    while (!xml.atEnd()) {
        bool ok = xml.readNextStartElement();
        if (!ok) continue;
        if (xml.name() == "login") {
            Util::username = xml.attributes().value("username").toString();
            Util::userpass = xml.attributes().value("userpass").toString();
            Util::loginquestionid = xml.attributes().value("loginquestionid").toString();
            Util::loginanswer = xml.attributes().value("loginanswer").toString();
        }
        if (xml.name() == "host") {
            Util::host = xml.attributes().value("hostname").toString();
        }
        if (xml.name() == "register") {
            Util::ifreg = xml.attributes().value("ifregister").toString();
            Util::regpage = xml.attributes().value("regpage").toString();
        }
    }
    if (xml.hasError() && xml.error() != 4) {
        QMessageBox::warning(0, "Warning", "user info error");
        qDebug()<<"xml error !!!  "<< xml.error();
        return;
    }
}

void Util::writeData()
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(0, "Warning", "Can't save data");
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("save");
        stream.writeStartElement("login");
        stream.writeAttribute("username", Util::username);
        stream.writeAttribute("userpass", Util::userpass);
        stream.writeAttribute("loginquestionid", Util::loginquestionid);
        stream.writeAttribute("loginanswer", Util::loginanswer);
        stream.writeEndElement();

        stream.writeStartElement("host");
        stream.writeAttribute("hostname", Util::host);
        stream.writeEndElement();

        stream.writeStartElement("register");
        stream.writeAttribute("ifregister", Util::ifreg);
        stream.writeAttribute("regpage", Util::regpage);
        stream.writeEndElement();


    stream.writeEndElement();

    stream.writeEndDocument();

    file.close();
}
