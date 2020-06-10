#ifndef GAME_H
#define GAME_H

#include <QTimer>

class Game
{
public:

    Game();

    bool pause(bool isPause,QTimer *gameTimer,int level);       //暂停和恢复游戏
    bool isGameOver(QList<QPoint> snake);                       //判断是否游戏结束

};

#endif // GAME_H
