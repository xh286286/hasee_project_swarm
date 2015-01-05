#ifndef MYPLAINTEXTEDIT_H
#define MYPLAINTEXTEDIT_H

#include <QPlainTextEdit>
class MyPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    MyPlainTextEdit();
    ~MyPlainTextEdit();
protected:
    void closeEvent(QCloseEvent *e);
};

#endif // MYPLAINTEXTEDIT_H
