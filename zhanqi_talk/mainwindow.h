#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class DanmuConnection;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void sendMsg();

private:
    DanmuConnection dc;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
