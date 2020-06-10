#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rand_2_4.h"
#include <QPushButton>
#include <QTime>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <QBrush>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(320,480);
    this->setWindowTitle("2048");

    // 3*3按钮
    QPushButton* size_btn_3 = new QPushButton("3X3");
    size_btn_3->move(50,80);
    size_btn_3->setParent(this);
    size_btn_3->setFixedSize(80,30);
    connect(size_btn_3,SIGNAL(clicked()),this,SLOT(ensureSize3()));

    // 4*4按钮
    QPushButton* size_btn_4 = new QPushButton("4X4");
    size_btn_4->setFixedSize(80,30);
    size_btn_4->setParent(this);
    size_btn_4->move(180,80);
    connect(size_btn_4,SIGNAL(clicked()),this,SLOT(ensureSize4()));

    remind_1 = new QLabel("先选择大小，再点击开始游戏",this);
    remind_2 = new QLabel("WASD控制方向",this);

    remind_1->setGeometry(60,150,200,30);
    remind_2->setGeometry(60,170,100,30);

    // 开始按钮
    button = new QPushButton("开始游戏",this);
    button->setGeometry(60,400,200,50);

    connect(button,SIGNAL(clicked()),this,SLOT(start()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化
void MainWindow::start(){
    if(size == 0){
        QMessageBox::warning(this,"提示！","请先选择游戏类型！");
        return;
    }

    remind_1->setText(" ");
    remind_2->setText(" ");

    score = 0;  // 分数清零
    for(int i = 0;i     < size;i++){
        for(int j = 0;j < size;j++){
            if(size == 3)
                lattice_3[i][j] = 0;
            else if(size == 4)
                lattice_4[i][j] =0;
        }
    }

    button->setText("重新开始");

    int rand_i = qrand()%size;  // 生成格子的随机行
    int rand_j = qrand()%size;  // 生成格子的随机列

    // 随机生成2，4
    Rand_2_4 rand;
    int rand_number = rand.rand();
    if(size == 3)
        lattice_3[rand_i][rand_j] = rand_number;
    else if(size == 4)
        lattice_4[rand_i][rand_j] = rand_number;

    state = true;
    update();
}

// 3*3
void MainWindow::ensureSize3()
{
    size = 3;
    button->setText("开始游戏");
}

// 4*4
void MainWindow::ensureSize4()
{
    size =4;
    button->setText("开始游戏");
}

// 格子的绘制
void MainWindow::paintEvent(QPaintEvent *){
    QPainter paint(this);

    // 设置背景图片
    QPixmap pix;
    pix.load(":/res/background.jpg");
    paint.drawPixmap(0,0,this->width(),this->height(),pix);

    paint.setFont(QFont("微软雅黑",20,700,false));
    QString str_score;
    paint.drawText(QPoint(30,50),"分数    "+QString::number(score));

    // 格子实现0 2 4 8 16 32 64 128 256 512 1024 2048
    for(int i=0;i < size;i++){
        for(int j=0;j < size;j++){
            paint.setPen(Qt::transparent);

            if(size == 3)
                value = lattice_3[i][j];
            else if(size == 4)
                value = lattice_4[i][j];

            // 根据值设置格子刷子颜色
            switch (value) {
                case 0:     paint.setBrush(QBrush(QColor(204,202,202,80)));   break;
                case 2:     paint.setBrush(QBrush(QColor(255,252,227,210)));  break;
                case 4:     paint.setBrush(QBrush(QColor(255,241,140,210)));  break;
                case 8:     paint.setBrush(QBrush(QColor(255,243,26,210)));   break;
                case 16:    paint.setBrush(QBrush(QColor(128,255,252,210)));  break;
                case 32:    paint.setBrush(QBrush(QColor(38,255,250,210)));   break;
                case 64:    paint.setBrush(QBrush(QColor(35,254,150,210)));   break;
                case 128:   paint.setBrush(QBrush(QColor(0,200,60,210)));     break;
                case 256:   paint.setBrush(QBrush(QColor(255,51,51,210)));    break;
                case 512:   paint.setBrush(QBrush(QColor(255,2,2,210)));      break;
                case 1024:  paint.setBrush(QBrush(QColor(96,78,254,210)));    break;
                case 2048:  paint.setBrush(QBrush(QColor(48,24,254,210)));    break;
                default:    paint.setBrush(QBrush(QColor(254,1,159,210)));    break;
            }

            int x = 70,y = 150;
            if(size == 4){
                x = 40;
                y = 120;
            }
            paint.drawRect(i*60+x,j*60+y,55,55); // 对格子上色

            if(value != 0){
                paint.setPen(Qt::black);
                paint.setFont(QFont("微软雅黑",10,700,false));
                //在方块中 绘制Text， 并且文本位置位于中部
                paint.drawText(QRectF(i*60+x,j*60+y,55,55),QString::number(value),QTextOption(Qt::AlignCenter));
            }
        }
    }
}

// 按键操作
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(!state)
        return;

    switch(event->key()){
        case Qt::Key_W:  // 按上键
            pressUp();
            break;

        case Qt::Key_S:  // 按下键
            pressDown();
            break;

       case Qt::Key_A:   // 按左键
            pressLeft();
            break;

       case Qt::Key_D:  // 按右键
            pressRight();
            break;

        default:
            return;
    }
    myrand();
    update(); // 强制界面刷新
}

// 上
void MainWindow::pressUp()
{
    // 向上移动
    for(int i = 0;i < size;i++){
        for(int j = 1;j < size;j++){   // 从左上第二行开始向上移动
            // 3*3
            if(size == 3){
                if(lattice_3[i][j] == 0)  // 为0跳过
                    continue;

                for(int k = 0;k < j;k++){
                    if(lattice_3[i][k] == 0){     // 查看上面是否有格子可以移动
                        lattice_3[i][k] = lattice_3[i][j];
                        lattice_3[i][j] = 0;
                        break;
                    }
                }
            }
            // 4*4
            else if(size == 4){
                if(lattice_4[i][j] == 0)  // 为0跳过
                    continue;

                for(int k = 0;k < j;k++){
                    if(lattice_4[i][k] == 0){     // 查看上面是否有格子可以移动
                        lattice_4[i][k] = lattice_4[i][j];
                        lattice_4[i][j] = 0;
                        break;
                    }
                }
            }
        }
    }

    // 向上相加
    for(int i = 0;i < size;i++){
        for(int j = 0;j < size;j++){
            // 3*3
            if(size == 3){ // 上面的格子等于下面的格子
                if(lattice_3[i][j] == lattice_3[i][j+1]){
                    lattice_3[i][j] = lattice_3[i][j]*2;  // 上面的格子加倍
                    lattice_3[i][j+1] = 0;              // 下面的格子置0
                    score += lattice_3[i][j];
                    for(int k = j+2;k < size;k++)    // 将后面的格子往前一个格子移栋
                        lattice_3[i][k-1] = lattice_3[i][k];
                }
            }
            // 4*4
            else if(size == 4){
                if(lattice_4[i][j] == lattice_4[i][j+1]){
                    lattice_4[i][j] = lattice_4[i][j]*2;  // 上面的格子加倍
                    lattice_4[i][j+1] = 0;              // 下面的格子置0
                    score += lattice_4[i][j];
                    for(int k = j+2;k < size;k++)     // 将后面的格子往前一个格子移栋
                        lattice_4[i][k-1] = lattice_4[i][k];
                }
            }
        }
    }
}

// 下
void MainWindow::pressDown()
{
    // 向下移动
    for (int i = 0;i < size;i++) {
        for (int j = (size-2);j >= 0;j--) {   // 从左下倒数第二行开始向下移动
            // 3*3
            if(size == 3){
                if(lattice_3[i][j] == 0)  // 为0跳过
                    continue;

                for (int k = 3;k > j;k--) {
                    if(lattice_3[i][k] == 0){     // 查看下面是否有空格子可移动
                        lattice_3[i][k] = lattice_3[i][j];
                        lattice_3[i][j] = 0;
                        break;
                    }
                }
            }
            // 4*4
            else if(size == 4){
                if(lattice_4[i][j] == 0)  // 为0跳过
                    continue;

                for (int k = 3;k > j;k--) {
                    if(lattice_4[i][k] == 0){     // 查看下面是否有空格子可移动
                        lattice_4[i][k] = lattice_4[i][j];
                        lattice_4[i][j] = 0;
                        break;
                    }
                }
            }
        }
    }

    // 向下相加
    for (int i = 0;i < size;i++) {
        for (int j = (size-1);j > 0;j--) {
            // 3*3
            if(size == 3){
                if(lattice_3[i][j] == lattice_3[i][j-1]){   // 下面的格子等于上面的格子
                    lattice_3[i][j] = 2*lattice_3[i][j];    // 下面的格子加倍
                    lattice_3[i][j-1]=0;                  // 上面的格子置0
                    score += lattice_3[i][j];
                    for (int k = j-2; k >= 0;k--)   // 将后面的格子往下移
                        lattice_3[i][k+1] = lattice_3[i][k];
                }
            }
            // 4*4
            else if(size == 4){
                if(lattice_4[i][j] == lattice_4[i][j-1]){   // 下面的格子等于上面的格子
                    lattice_4[i][j] = 2*lattice_4[i][j];    // 下面的格子加倍
                    lattice_4[i][j-1]=0;                  // 上面的格子置0
                    score += lattice_4[i][j];
                    for (int k = j-2; k >= 0;k--)     // 将后面的格子往下移
                        lattice_4[i][k+1] = lattice_4[i][k];
                }
            }
        }
    }
}

// 左
void MainWindow::pressLeft()
{
    // 向左移动
    for (int j = 0;j < size;j++) {
        for (int i = 1;i < size;i++) { // 从左第一列开始向左移动
            // 3*3
            if(size == 3){
                if(lattice_3[i][j] == 0)
                    continue;

                for (int k = 0;k < i;k++) {
                    if(lattice_3[k][j] == 0){ // 查看左边是否有空格可移入
                        lattice_3[k][j] = lattice_3[i][j];  // 左移
                        lattice_3[i][j] = 0;
                        break;
                    }
                }
            }
            // 4*4
            else if(size == 4){
                if(lattice_4[i][j] == 0)
                    continue;

                for (int k = 0;k < i;k++) {
                    if(lattice_4[k][j] == 0){ // 查看左边是否有空格可移入
                        lattice_4[k][j] = lattice_4[i][j];  // 左移
                        lattice_4[i][j] = 0;
                        break;
                    }
                }
            }
        }
    }
    // 向左相加
    for (int j = 0;j < size;j++) {
        // 3*3
        if(size == 3){
            for (int i = 0;i < (size-1);i++) {
                if(lattice_3[i][j] == lattice_3[i+1][j]){   // 左格子等于右格子
                    lattice_3[i][j] = lattice_3[i][j]*2;    // 左格子加倍
                    lattice_3[i+1][j] = 0;                // 右格子置0
                    score += lattice_3[i][j];
                    for(int k = i+2;k < size;k++)        // 后面格子左移
                        lattice_3[k-1][j] = lattice_3[k][j];
                }
            }
        }
        // 4*4
        else if(size == 4){
            for (int i = 0;i < (size-1);i++) {
                if(lattice_4[i][j] == lattice_4[i+1][j]){   // 左格子等于右格子
                    lattice_4[i][j] = lattice_4[i][j]*2;    // 左格子加倍
                    lattice_4[i+1][j] = 0;                // 右格子置0
                    score += lattice_4[i][j];
                    for(int k = i+2;k < size;k++)       // 后面格子左移
                        lattice_4[k-1][j] = lattice_4[k][j];
                }
            }
        }
    }
}

// 右
void MainWindow::pressRight()
{
    // 向右移动
    for (int j = 0;j < size;j++) {
        for (int i = (size-2);i >= 0;i--) {    // 从右倒数第二列开始
            // 3*3
            if(size == 3){
                if(lattice_3[i][j] == 0)
                    continue;

                for (int k = (size-1);k > i;k--) {
                    if(lattice_3[k][j] == 0){   //查看右面是否有空格可移入
                        lattice_3[k][j] = lattice_3[i][j];  // 右移
                        lattice_3[i][j] = 0;
                        break;
                    }
                }
            }
            // 4*4
            else if(size == 4){
                if(lattice_4[i][j] == 0)
                    continue;

                for (int k = (size-1);k > i;k--) {
                    if(lattice_4[k][j] == 0){   //查看右面是否有空格可移入
                        lattice_4[k][j] = lattice_4[i][j];  // 右移
                        lattice_4[i][j] = 0;
                        break;
                    }
                }
            }
        }
    }
    // 向右相加
    for (int j = 0;j < size;j++) {
        for (int i = (size-1);i >= 0;i--) {    // 从右倒数第一个格子开始
            // 3*3
            if(size == 3){
                if(lattice_3[i][j] == lattice_3[i-1][j]){   // 右格子等于左格子
                    lattice_3[i][j] = lattice_3[i][j]*2;    // 右格子加倍
                    lattice_3[i-1][j] = 0;                // 左格子置0
                    score += lattice_3[i][j];
                    for(int k = i-2;k >= 0;k--)   // 后面的格子右移
                        lattice_3[k+1][j] = lattice_3[k][j];
                }
            }
            // 4*4
            else if(size == 4){
                if(lattice_4[i][j] == lattice_4[i-1][j]){   // 右格子等于左格子
                    lattice_4[i][j] = lattice_4[i][j]*2;    // 右格子加倍
                    lattice_4[i-1][j] = 0;                // 左格子置0
                    score += lattice_4[i][j];
                    for(int k = i-2;k >= 0;k--)    // 后面的格子右移
                        lattice_4[k+1][j] = lattice_4[k][j];
                }
            }
        }
    }
}

// 找出空白格子，判断游戏是否结束
void MainWindow::myrand()
{
    // 找出空白格子
    int i = 0,j = 0;    // 记录空白格子位置
    int number = 0;     // 记录空白格子个数

    // 储存空白格子位置
    struct Position site_3[3*3];
    struct Position site_4[4*4];

    for(i = 0;i < size;i++){
        for(int j = 0;j < size;j++){
            if(size == 3){
                if(lattice_3[i][j] == 0){
                    site_3[number].i = i;
                    site_3[number].j = j;
                    number++;
                }
            }
            else if(size == 4){
                if(lattice_4[i][j] == 0){
                    site_4[number].i = i;
                    site_4[number].j = j;
                    number++;
                }
            }
        }
    }

    //判断游戏是否结束
    if (number == 0) {
        // 上下相邻是否有相等的
        for(i = 0;i < size;i++) {
            for(j = 0;j < (size-1);j++) {
                if(size == 3){
                    if(lattice_3[i][j] == lattice_3[i][j+1])
                        return;
                }
                else if(size == 4){
                    if(lattice_4[i][j] == lattice_4[i][j+1])
                        return;
                }
            }
        }
        // 左右相邻是否有相等的
        for(j = 0;j < size;j++) {
            for(i = 0;i < (size-1);i++) {
                if(size == 3){
                    if(lattice_3[i][j] == lattice_3[i+1][j]){
                        return;
                    }
                }
                else if(size == 4){
                    if(lattice_4[i][j] == lattice_4[i+1][j]){
                        return;
                    }
                }
            }
        }
        QMessageBox::warning(this,"Game Over！","游戏结束！");
        return;
    }

    // 随机获取空白格子
    int site_rand = qrand()%number;

    // 随机生成2，4
    Rand_2_4 ran;
    int rand_number = ran.rand();

    int x,y;
    if(size == 3){
        x = site_3[site_rand].i;
        y = site_3[site_rand].j;
        lattice_3[x][y] = rand_number;
    }
    else if(size == 4){
        x = site_4[site_rand].i;
        y = site_4[site_rand].j;
        lattice_4[x][y] = rand_number;
    }
}

