#include "gamewindow.h"
#include "game.h"
#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QPushButton>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(770,510);

    // 随机生成初始位置
    snake.clear();
    int x = rand()%(645/15);
    int y = rand()%(465/15);
    snake.push_back(QPoint(x,y));

    score = 0;          // 初始化游戏分数
    generateFood();     // 初始化果实

    // 开始按钮
    restart = new QPushButton("开始游戏",this);
    restart->move(670,485);
    restart->setFixedSize(60,20);
    restart->setFlat(true);     // 将按钮设为透明
    restart->setFocusPolicy(Qt::NoFocus);   // 鼠标不聚焦在按钮上

    connect(restart,SIGNAL(clicked()),this,SLOT(startGame()));

    // 返回按钮
    QPushButton * back = new QPushButton("返回",this);
    back->move(735,485);
    back->setFixedSize(30,20);
    back->setFlat(true);
    back->setFocusPolicy(Qt::NoFocus);

    connect(back,&QPushButton::clicked,[=](){

        gameTimer->stop();

        // 随机生成初始位置
        snake.clear();
        int x = rand()%(645/15);
        int y = rand()%(465/15);
        snake.push_back(QPoint(x,y));

        score = 0;
        generateFood();

        restart->setText("开始游戏");
        restart->show();

        emit this->back();
    });

    // 游戏难度按钮
    QPushButton * level_1 = new QPushButton("难度1",this);
    level_1->move(5,170);
    level_1->setFixedSize(50,20);
    level_1->setFlat(true);
    level_1->setFocusPolicy(Qt::NoFocus);
    connect(level_1,&QPushButton::clicked,[=](){
        level = 500;
    });

    QPushButton * level_2 = new QPushButton("难度2",this);
    level_2->move(5,190);
    level_2->setFixedSize(50,20);
    level_2->setFlat(true);
    level_2->setFocusPolicy(Qt::NoFocus);
    connect(level_2,&QPushButton::clicked,[=](){
        level = 300;
    });

    QPushButton * level_3 = new QPushButton("难度3",this);
    level_3->move(5,210);
    level_3->setFixedSize(50,20);
    level_3->setFlat(true);
    level_3->setFocusPolicy(Qt::NoFocus);
    connect(level_3,&QPushButton::clicked,[=](){
        level = 100;
    });

    QPushButton * level_4 = new QPushButton("究极难度",this);
    level_4->move(5,230);
    level_4->setFixedSize(70,20);
    level_4->setFlat(true);
    level_4->setFocusPolicy(Qt::NoFocus);
    connect(level_4,&QPushButton::clicked,[=](){
        level = 10;
    });
}

void GameWindow::startGame(){
    restart->hide();

    dir = RIGHT;
    isPause = false;

    // 初始化计时器
    gameTimer = new QTimer(this);
    connect(gameTimer,SIGNAL(timeout()),this,SLOT(snakeUpdate()));
    gameTimer->start(level);
}

void GameWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    //绘制游戏场景
    QPixmap pix;
    pix.load(":/res/background.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    painter.setBrush(QBrush(QColor(149,242,255,80)));   // 游戏区域颜色，淡蓝，透明
    painter.setPen(QPen(QColor(79,79,79,80)));         // 游戏边界颜色，淡黄，透明
    painter.drawRect(115,5,645,465);

    if(!snake.isEmpty()){
        // 绘制贪吃蛇
        painter.setBrush(QBrush(QColor(255,245,66,150)));    // 淡黄
        for(int i = 0;i < snake.size();i++)
            painter.drawEllipse(115+snake[i].x()*15,5+snake[i].y()*15,15,15);

        // 绘制食物
        painter.setBrush(QBrush(QColor(80,254,88,200)));    // 淡绿
        painter.drawEllipse(115+foodPoint.x()*15,5+foodPoint.y()*15,15,15);
    }

    //绘制提示及分数
    painter.setBrush(QBrush(QColor(71,176,226,80)));
    painter.drawRect(5,5,100,500);

    painter.setPen(QPen(QColor(71,176,226)));
    painter.setFont(QFont("Arial",14));
    painter.drawText(5,25,"score: "+QString::number(score));
    painter.setFont(QFont("Arial",10));
    painter.drawText(5,60,"提示：");
    painter.drawText(5,75,"上下左右控制");
    painter.drawText(5,90,"空格暂停");
    painter.drawText(5,120,"游戏难度：");
    painter.drawText(5,135,"难度1最慢");
    painter.drawText(5,150,"究极最快");
}

void GameWindow::keyPressEvent(QKeyEvent *event){
    Game game;

    // 换方向时，向上不能向下，向下不能向上，向左不能向右，向右不能向左
    switch(event->key()){
    case Qt::Key_Up:
        dir = UP;
        break;
    case Qt::Key_Down:
        dir = DOWN;
        break;
    case Qt::Key_Left:
        dir = LEFT;
        break;
    case Qt::Key_Right:
        dir = RIGHT;
        break;
    case Qt::Key_Space:
        isPause = game.pause(isPause,gameTimer,level);
        break;
    default:
        break;
    }
}

void GameWindow::generateFood(){
    //随机产生位置
    foodPoint.setX(rand()%(645/15));
    foodPoint.setY(rand()%(465/15));

    //如果与贪吃蛇位置冲突，重新生成
    if(snake.contains(foodPoint))
        generateFood();
}

void GameWindow::snakeUpdate(){
    Game game;
    switch(dir){
        case UP:
            snake.push_front(QPoint(snake.front().x(),snake.front().y()-1));
            break;
        case DOWN:
            snake.push_front(QPoint(snake.front().x(),snake.front().y()+1));
            break;
        case LEFT:
            snake.push_front(QPoint(snake.front().x()-1,snake.front().y()));
            break;
        case RIGHT:
            snake.push_front(QPoint(snake.front().x()+1,snake.front().y()));
            break;
        default:
            break;
    }

    //如果吃到了果实，则尾部不删除，否则删除尾部更新头部
    if(snake.contains(foodPoint)){
        score += 1; //得分
        if(score == (645/15)*(465/15)){
            gameTimer->stop();
            QMessageBox::information(this,"You Win","你赢了！");
        }
        else
            generateFood(); //重新生成果实
    }
    else
        snake.pop_back();

    //游戏是否结束
    if(game.isGameOver(snake)){
        gameTimer->stop();
        if(level == 10)
            QMessageBox::information(this,"游戏结束","都说了最难了，没了吧!");
        if(level == 100)
            QMessageBox::information(this,"游戏结束","你死掉了!");
        if(level == 300)
            QMessageBox::information(this,"游戏结束","小兄弟，看来你还是不行啊!");
        if(level == 500)
            QMessageBox::information(this,"游戏结束","最慢你都没了，回家吧!");

        // 随机生成初始位置
        snake.clear();
        int x = rand()%(645/15);
        int y = rand()%(465/15);
        snake.push_back(QPoint(x,y));

        score = 0;
        generateFood();

        restart->setText("重新开始");
        restart->show();
    }
    else
        update(); //重绘
}

