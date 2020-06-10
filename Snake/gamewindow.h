#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPushButton>

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);

    enum Direction{
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    QPushButton * restart;  // 开始游戏/重新开始

    void paintEvent(QPaintEvent *event); // 绘制界面
    void keyPressEvent(QKeyEvent *event); // 键盘监听

    QTimer *gameTimer;    // 游戏计时器
    bool isPause;         // 是否暂停
    QPoint foodPoint;     // 食物的坐标
    QList<QPoint> snake;  // 贪吃蛇
    Direction dir;        // 方向
    int score = 0;           // 分数
    int level = 300;           // 游戏难度

    void pauseResumeGame(); // 暂停和恢复游戏
    void generateFood();    // 随机产生果实

signals:
    void back();

public slots:
        void snakeUpdate(); // 贪吃蛇增长
        void startGame();    // 开始游戏
};

#endif // GAMEWINDOW_H
