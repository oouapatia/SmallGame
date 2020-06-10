#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

    QPushButton *button;    // 后面修改为重新开始

    QLabel * remind_1;
    QLabel * remind_2;

    int size = 0;  // 记录格子的尺寸
    int value;      // 记录某一个格子的值

    int lattice_3[3][3];      // 将格子看成一个二维数组
    int lattice_4[4][4];

    int score = 0;      // 分数

    // 上下左右操作
    void pressUp();
    void pressDown();
    void pressLeft();
    void pressRight();

    bool state;     // 游戏状态，true正常，false停止

    // 随机生成函数 判断结束函数
    void myrand();

    // 结构体，储存剩余空白格子的位置
    struct Position{
        int i;
        int j;
    };

public slots:
    void start();       // 初始化，生成第一个数
    void ensureSize3();    // 3*3尺寸
    void ensureSize4();    // 4*4尺寸

public:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
