#ifndef MYMENU_H
#define MYMENU_H

#include <QMenu>

class MyMenu : public QMenu
{
    Q_OBJECT
public:
    MyMenu();
public slots:
    void about();
    void endProgram();

    void hideAndShow();

    void startDectecting();
    void stopDectecting();


    //void settingWindow();

};

#endif // MYMENU_H
