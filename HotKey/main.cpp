#include "Widget.hpp"
#include <QApplication>
#include "CHotKeyEventFilter.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    CHotKeyEventFilter hk(w.winId());
    a.installNativeEventFilter(&hk);


    QObject::connect(&hk, &CHotKeyEventFilter::HotKeyTrigger, &w, &Widget::HotKeyTriggered);
    w.show();
    return a.exec();
}
