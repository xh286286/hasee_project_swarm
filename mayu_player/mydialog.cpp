#include <qt_windows.h>
#include <QHBoxLayout>
#include <QLabel>
#include "mydialog.h"

#ifdef Q_CC_MSVC
#pragma comment(lib, "user32.lib")
#endif
//nativeEvent
Dialog::Dialog(QWidget *parent)
    : QDialog(parent), m_label(new QLabel)
{
    QHBoxLayout * box = new QHBoxLayout(this);
    box->addWidget(m_label);
    setLayout(box);

    HWND hWnd = ::FindWindowW(NULL, L"Dbzhang800-Recv");
    if (hWnd != NULL) {
        setWindowTitle("Dbzhang800-Send");
        QString str("Message from dbzhang800-send");
        COPYDATASTRUCT cpd;
        cpd.dwData = 0;
        cpd.cbData = str.length()+1;
        cpd.lpData = str.toLatin1().data();
        ::SendMessageW(hWnd, WM_COPYDATA, NULL, (LPARAM)&cpd);
        m_label->setText("Message has been sent.");
    } else {
        setWindowTitle("Dbzhang800-Recv");
        m_label->setText("Ready...");
    }
}

Dialog::~Dialog()
{
}

bool Dialog::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
    MSG* m = (MSG*) message;
    if (m->message == WM_COPYDATA){
        COPYDATASTRUCT * p = reinterpret_cast<COPYDATASTRUCT*>(m->lParam);
        m_label->setText(static_cast<char*>(p->lpData));
    }
    return false;
}
