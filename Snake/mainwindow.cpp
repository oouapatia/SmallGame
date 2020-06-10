#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 开始游戏
    this->setWindowTitle("贪吃蛇");
    this->setFixedSize(320,320);
    QPushButton * startBtn = new QPushButton("开始游戏",this);
    startBtn->setFixedSize(60,60);
    startBtn->move(130,50);
    startBtn->setFlat(true);

    // 创建出游戏界面
    gameScene = new GameWindow;

    // 监听游戏界面的返回信号
    connect(gameScene,&GameWindow::back,[=](){
        gameScene->hide();  // 游戏界面隐藏
        this->show();       // 自身显示
    });

    connect(startBtn,&QPushButton::clicked,[=](){
        this->hide();       // 将自身隐藏
        gameScene->show();  // 显示游戏界面
    });

    // 退出游戏
    QPushButton * exitBtn = new QPushButton("退出游戏",this);
    exitBtn->setFixedSize(60,60);
    exitBtn->move(130,100);
    exitBtn->setFlat(true);
    connect(exitBtn,&QPushButton::clicked,[=](){
        this->close();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    QPixmap pix;
    pix.load(":/res/mainwindow.jpg");
    paint.drawPixmap(0,0,this->width(),this->height(),pix);
}
