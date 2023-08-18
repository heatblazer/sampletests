#include "peterson.h"
#include <atomic>
namespace Peterson {
static std::atomic<int> flag[2] = {0};
static std::atomic<int> turn = 0;


void lock(int other)
{
    flag[other] = 1;
    turn = 1 - other;

    while(turn == 1 && flag[turn] == 1) ; // block
}

void unlock(int other)
{
    flag[other] = 0;
}

}
