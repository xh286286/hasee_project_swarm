#include "Widget.hpp"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{

}

void Widget::HotKeyTriggered(void)
{
    if(true == isVisible())
        setVisible(false);
    else
        setVisible(true);
    return ;
}
