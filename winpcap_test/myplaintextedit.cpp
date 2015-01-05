#include "myplaintextedit.h"
#include <QCloseEvent>
MyPlainTextEdit::MyPlainTextEdit(): QPlainTextEdit()
{

}

MyPlainTextEdit::~MyPlainTextEdit()
{

}

void MyPlainTextEdit::closeEvent(QCloseEvent *e) {
    e->ignore();
    //reject();
    hide();

}
