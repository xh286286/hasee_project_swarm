#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
class QLabel;

class Dialog : public QDialog
{
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
protected:
    bool  nativeEvent(const QByteArray & eventType, void * message, long * result);
private:
    QLabel * m_label; };
#endif // DIALOG_H
