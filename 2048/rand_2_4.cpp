#include "rand_2_4.h"
#include <QTime>

Rand_2_4::Rand_2_4()
{

}

int Rand_2_4::rand(){
    // 生成随机数 2 4
    int rand_number = 2;
    while(true){
        rand_number = qrand()%5;
        if(rand_number == 0 || rand_number == 1 || rand_number == 3)    // 除去0
            continue;
        else
            break;
    }
    return rand_number;
}
