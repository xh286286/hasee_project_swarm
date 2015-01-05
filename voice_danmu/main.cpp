#include "../share_library/messagecenter.h"
#include "../share_library/Util.h"
#include "voicecenter.h"
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<1;
    MessageCenter mc;

    QTimer t;

    if (mc.registerTitle("mayu voice")) {
        QObject::connect(&t, &QTimer::timeout, [&]() {
            if (mc.addPartner("mayu danmu") ) {
            }
            else {
                qDebug()<<"Danmu assistant is not running!"  ;
            }
            t.start(40000);
        });
        t.start(1000);
    }
    else {
        QMessageBox::critical(0, "error",  "The media player was running!"   );
        return 0;
    }

    VoiceCenter vc;
    vc.show();
    vc.hide();

    QObject::connect(&mc, &MessageCenter::sendMessage,  &vc,  &VoiceCenter::getDanmu);


    QTimer onlineTimer;
    onlineTimer.start(10000);
    QObject::connect(&onlineTimer, &QTimer::timeout, [&]() {
        static bool init = false;
        if (!init) {
            init = true;
            mc.broadcast(myTr("麻由的语音弹幕上线"));
        }
        onlineTimer.stop();
    });

    return a.exec();
}
