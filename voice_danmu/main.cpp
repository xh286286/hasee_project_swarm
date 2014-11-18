#include "../share_library/messagecenter.h"
#include "voicecenter.h"
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MessageCenter mc;

    QTimer t;

    if (mc.registerTitle("mayu voice")) {
        QObject::connect(&t, &QTimer::timeout, [&]() {
            if (mc.addOutput("mayu danmu") ) {
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

    return a.exec();
}
