#include <QString>
#include <QDebug>
#include "messagecenter.h"

MessageCenter::MessageCenter(QWidget *parent) :
    QWidget(parent)
{
}

bool MessageCenter::registerTitle(QString title)
{
    QByteArray a = title.toLatin1();


    WCHAR wszClassName[1256];
    memset(wszClassName,0,sizeof(wszClassName));
    MultiByteToWideChar(CP_ACP,0,a.data() ,strlen(a.data())+1,wszClassName,
    sizeof(wszClassName)/sizeof(wszClassName[0]));

    HWND hWnd = ::FindWindowW(NULL, wszClassName);
    if (hWnd != NULL)  return false;
    self  = hWnd;
    qDebug()<<"message register title "<< title;
    setWindowTitle(title);
    show();
    hide();
    return true;
}

bool  MessageCenter::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
    //qDebug()<<eventType;
    MSG* m = (MSG*) message;
    if (m->message == WM_COPYDATA){
        COPYDATASTRUCT * p = reinterpret_cast<COPYDATASTRUCT*>(m->lParam);
       // qDebug()<<static_cast<char*>(p->lpData);
        QByteArray ba(static_cast<char*>(p->lpData));

        QString s = QString::fromUtf8( QByteArray::fromPercentEncoding(ba));
       // qDebug()<<s;


        if (s.indexOf("[req]")==0) {
            QString title;
            title = s.mid(5);

            QByteArray a = title.toLatin1();

            WCHAR wszClassName[1256];
            memset(wszClassName,0,sizeof(wszClassName));
            MultiByteToWideChar(CP_ACP,0,a.data() ,strlen(a.data())+1,wszClassName,
            sizeof(wszClassName)/sizeof(wszClassName[0]));

            HWND hWnd = ::FindWindowW(NULL, wszClassName);
            if (hWnd == NULL)  return false;
            else {
                bool dup = false;
                for (int i=0; i<targetPool.size(); i++) {
                    if (hWnd == targetPool[i]) {
                        dup = true;
                        break;
                    }
                }
                if (dup) {
                //    qDebug()<< " duplicate connection with WinMessage";
                    return false;
                }
                qDebug()<<" add listener "<<title;
                targetPool.push_back(hWnd);
            }
        }
        else if (s.indexOf("[brc]") ==0) {
            QString s1;
            s1 = s.mid(5);
            emit sendMessage(s1);
        }
    }
    else if (m->message == WM_HOTKEY){
        UINT a,b;
        int x = int(m->lParam);
        int d = (1<<16);
        a = x%d;
        b = x/d;
        qDebug()<<"hotkey "<<a<< " "<<b;
        emit globalHotkey(0,0);
    }
    return false;
}


void MessageCenter::broadcast(QString s) {
   // qDebug()<<"broadcast "<<s;
     QList< HWND  > newList;
    for (int i=0; i< targetPool.size(); i++) {
        HWND hWnd = targetPool [i];

        QString str("[brc]");
        str += s;
        COPYDATASTRUCT cpd;
        cpd.dwData = 0;
        QByteArray ba = str.toUtf8().toPercentEncoding();
        cpd.cbData = ba.length()+1;
        cpd.lpData = ba.data();
        //qDebug()<< ba.data();
        bool ok = ::SendMessageW(hWnd, WM_COPYDATA, NULL, (LPARAM)&cpd);
        if (true) newList.push_back(hWnd);
    }
    targetPool = newList;
}

bool MessageCenter::addOutput(QString title) {
    QByteArray a = title.toLatin1();


    WCHAR wszClassName[1256];
    memset(wszClassName,0,sizeof(wszClassName));
    MultiByteToWideChar(CP_ACP,0,a.data() ,strlen(a.data())+1,wszClassName,
        sizeof(wszClassName)/sizeof(wszClassName[0]));

    HWND hWnd = ::FindWindowW(NULL, wszClassName);

    if (hWnd != NULL) {

        QString str("[req]");
        str+= windowTitle();
        COPYDATASTRUCT cpd;
        cpd.dwData = 0;
        QByteArray ba = str.toUtf8().toPercentEncoding();
        cpd.cbData = ba.length()+1;
        cpd.lpData = ba.data();
       // qDebug()<< ba.data();
        ::SendMessageW(hWnd, WM_COPYDATA, NULL, (LPARAM)&cpd);
        return true;
    } else {
        return false;
    }

}


bool MessageCenter::registerHotkey(UINT a,UINT b)
{
    QByteArray t = windowTitle().toLatin1();


    WCHAR wszClassName[1256];
    memset(wszClassName,0,sizeof(wszClassName));
    MultiByteToWideChar(CP_ACP,0,t.data() ,strlen(t.data())+1,wszClassName,
        sizeof(wszClassName)/sizeof(wszClassName[0]));
    ATOM id =  GlobalAddAtom(wszClassName);
    hotKeyId.push_back(id);
    bool ok = RegisterHotKey(self, id, a , b);
    qDebug()<<"register hotkey "<<ok<<' '<<id<<' '<<a<<' '<<b;
    return ok;
}

bool MessageCenter::unregisterHotkey()
{
    for (int i=0; i<hotKeyId.size(); i++) {
        ATOM id =  hotKeyId[i];
        UnregisterHotKey(self,id);
        GlobalDeleteAtom(id);
    }
    return true;
}
