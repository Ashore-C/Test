#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QMouseEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::shared_ptr<int> a1(new int(222));
    std::shared_ptr<int> a2(a1);

    std::shared_ptr<int> a3 = a1;
    std::shared_ptr<int> a4(std::move(a1));
    qDebug() <<"a1的引用计数为" <<a1.use_count();
    qDebug() <<"a2的引用计数为" <<a2.use_count();
    qDebug() <<"a3的引用计数为" <<a3.use_count();
    qDebug() <<"a4的引用计数为" <<a4.use_count();


    QTimer * t = new QTimer(this);

    t->setTimerType(Qt::PreciseTimer);

    connect(ui->pushButton,&QPushButton::clicked,this,[=](){

        if(t->isActive()){
            t->stop();  // 关闭定时器
            ui->pushButton->setText("开启");
        }
        else{
            t->start(1000);
            ui->pushButton->setText("关闭");
        }
    });

    connect(t,&QTimer::timeout,this,[=](){

        QTime tm = QTime::currentTime();

        QString s = tm.toString();

        ui->label->setText(s);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_positionBtn_clicked()
{
    QRect x = this->frameGeometry();

    //ui->textEdit->setText(x.topLeft());

    qDebug()<< "左上角" << x.topLeft()
            << "右上角" << x.topRight()
            << "左下角" << x.bottomLeft()
            << "右下角" << x.bottomRight();
}


void MainWindow::on_moveBtn_clicked()
{
    QRect x = this->frameGeometry();
    //move(x.top()  QPoint(10,20));
    move(x.topLeft() + QPoint(10, 20));
}


void MainWindow::on_openfileBtn_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"文件","E:/","*.xlsx");

    ui->textEdit->setText("选择单个文件的绝对位置为" + file);
}


void MainWindow::on_openfilesBtn_clicked()
{
    QStringList  file = QFileDialog::getOpenFileNames(this,"文件","E:/","*.xlsx");
    QString names;
    for (int var = 0; var < file.count(); ++var) {
        names += file.at(var) + " ";

    }
    QMessageBox::information(this, "打开文件(s)", "您选择的文件是: " + names);

    ui->textEdit->setText(names);

}


void MainWindow::on_progressBtn_clicked()
{
    QProgressDialog * progress = new QProgressDialog( "正在拷贝数据...", "取消拷贝", 0, 100, this);
    // 2. 初始化并显示进度条窗口
    progress->setWindowTitle("请稍后");
    progress->setWindowModality(Qt::WindowModal);
    progress->show();


    static int value = 0;
    QTimer *time = new QTimer;
    connect(time,&QTimer::timeout,this,[=](){
        progress->setValue(value);
        value++;

        if(value > progress->maximum()){
            time->stop();
            value = 0;
            delete progress;
            delete time;

        }

    });

    connect(progress,&QProgressDialog::canceled,this,[=](){
        time->stop();
        value = 0;
        delete progress;
        delete time;
    });

    time->start(50);

    ui->textEdit->installEventFilter(this);
}
//在窗口添加右键菜单
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        // 弹出一个菜单, 菜单项是 QAction 类型
        QMenu menu;
        QAction* act = menu.addAction("C++");
        connect(act, &QAction::triggered, this, [=]()
                {
                    QMessageBox::information(this, "title", "您选择的是C++...");
                });
        menu.addAction("Java");
        menu.addAction("Python");
        menu.exec(QCursor::pos()); // 右键菜单被模态显示出来了

    }
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    QMessageBox::Button btn =QMessageBox::question(this, "关闭窗口","确认关闭吗");

    if(btn == QMessageBox::Yes){
        ev->accept();
    }
    else{
        ev->ignore();
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->textEdit && event->type() == QEvent::KeyPress){
        QKeyEvent* keyEv = (QKeyEvent*)event;
        if(keyEv->key() == Qt::Key_Enter ||         // 小键盘确认
            keyEv->key() == Qt::Key_Return)     // 大键盘回车
        {
            qDebug() << "我是回车, 被按下了...";
            return true;
        }

    }
}

