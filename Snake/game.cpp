#include "game.h"
#include <QTimer>
#include <QMessageBox>

Game::Game()
{

}

bool Game::pause(bool isPause,QTimer *gameTimer,int level)
{
    //暂停和恢复定时器
    if(!isPause){
        isPause = !isPause;
        gameTimer->stop();
    }
    else{
        isPause = !isPause;
        gameTimer->start(level);
    }
    return isPause;
}

bool Game::isGameOver(QList<QPoint> snake)
{
    int x = snake.front().x();
    int y = snake.front().y();
    //出边界
    if(x < 0 || x >= 645/15 || y < 0 || y >= 465/15)
        return true;
    return false;
}
