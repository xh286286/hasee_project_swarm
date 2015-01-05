#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H
#include <qt_windows.h>
#include <QWidget>


class MessageCenter : public QWidget
{
    Q_OBJECT
public:
    explicit MessageCenter(QWidget *parent = 0);
    bool registerTitle(QString s);
signals:
    void sendMessage(QString s);
    void globalHotkey(UINT,UINT);
protected:
    bool  nativeEvent(const QByteArray & eventType, void * message, long * result);
public slots:
    void broadcast(QString s);
    bool addOutput(QString title);
    bool addInput(QString title);
    bool addPartner(QString title);
    bool registerHotkey(UINT,UINT);
    bool unregisterHotkey();
private:

    QList< HWND  > targetPool;
    HWND self;
    QList<ATOM> hotKeyId;
};

#endif // MESSAGECENTER_H
