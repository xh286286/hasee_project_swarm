#ifndef WINDOWSIZESETTING_H
#define WINDOWSIZESETTING_H

#include <QWidget>
#include <QSystemTrayIcon>
class DanmuWindow;
class WindowSizeSetting : public QWidget
{
    Q_OBJECT
public:
    explicit WindowSizeSetting(QWidget *parent = 0, QWidget * t = 0);


signals:

public slots:
    void activated(QSystemTrayIcon::ActivationReason reason);
protected:
    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *);
    void closeEvent(QCloseEvent *e);
    void focusOutEvent(QFocusEvent * event);

    void saveSetting();
    QWidget * target;

};

#endif // WINDOWSIZESETTING_H
