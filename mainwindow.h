#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_positionBtn_clicked();

    void on_moveBtn_clicked();

    void on_openfileBtn_clicked();

    void on_openfilesBtn_clicked();

    void on_progressBtn_clicked();

    //鼠标右键重写
    void mousePressEvent(QMouseEvent *event) override;

    //窗口关闭事件重写
    void closeEvent(QCloseEvent* ev) override;

    //回车不换行
    bool eventFilter(QObject* watched,QEvent *event) override;
    //bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
