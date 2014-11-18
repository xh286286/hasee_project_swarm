#include "CHotKeyEventFilter.hpp"
#include <string>
#include <QDebug>

CHotKeyEventFilter::CHotKeyEventFilter(WId id)
    : mWid(id)
{
    RegisterHotKeys();
}

CHotKeyEventFilter::~CHotKeyEventFilter()
{
    UnregisterHotKeys();
}

bool CHotKeyEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
  //  qDebug() << "nativeEventFilter type is :" << eventType;
    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = static_cast<MSG*>(message);
        switch(msg->message)
        {
            case WM_HOTKEY:
                qDebug() << "Here is on HotKey!";
                qDebug() << "msg->wParam is :" << msg->wParam;
                emit HotKeyTrigger();
                return true;
            default:
                break;
        }
    }
    if(eventType == "windows_dispatcher_MSG")
    {}
    return false;
}

int CHotKeyEventFilter::RegisterHotKeys(void)
{
//    std::string globalStr("Hotkey");//app 0x0000 ~ 0xBFFF, A shared DLL must specify a value in the range 0xC000 through 0xFFFF
//    while(0 != (mHotKeyId = GlobalFindAtom(globalStr.c_str())))//if the hotKeyId is not 0, it means the Atom has been registered
//        globalStr += '+';
   // qDebug() << "mWid is :" << mWid;
    for(mOwnId = 0; mOwnId < 0xC000; mOwnId++)
        if(false != RegisterHotKey(reinterpret_cast<HWND>(mWid), mOwnId,  MOD_CONTROL  , VK_OEM_PLUS))//If succeeds, the return value is nonzero.
        {
            qDebug() << "HotKey registered success!" << " mOwnId is " << mOwnId;
            return 0;
        }
    else
        qDebug() << "HotKey registered failed!";
    return -1;
}

void CHotKeyEventFilter::UnregisterHotKeys(void)
{
    if(false != UnregisterHotKey(reinterpret_cast<HWND>(mWid), mOwnId))//reinterpret_cast
        qDebug() << "unregisterHotKey success!";
    else
        qDebug() << "UnregisterHotKey failed!";
    return ;
}
