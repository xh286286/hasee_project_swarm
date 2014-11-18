#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void HotKeyTriggered(void);

};

#endif // WIDGET_HPP
