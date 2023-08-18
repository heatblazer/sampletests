#include "philosophers.h"
#include "peterson.h"
#include "lamportbakery.hpp"

#include <ctime>


static  time_t start_t, end_t;

static void startt()
{
    time(&start_t);
}

static double endt()
{
    time(&end_t);
    return difftime(end_t, start_t);
}

void philosophers();

void peterson();

void bakery();

int main()
{
//    peterson();
    bakery();
    return 0;
}

void philosophers()
{
    Philosophers philosophers{100, 10};
    philosophers.start();
}

#define LMPWRK 4

static void do_work(int tid)
{

//    static LamportBakery<LMPWRK> lmp;
    static volatile int gCounter = 1000000;
    while (gCounter > 0)
    {
//        lmp.lock(tid);
        gCounter--;
        std::cout << "Couter: " << gCounter << " from " << tid << "\r\n";
//        lmp.unlock(tid);
    }
}

void peterson()
{

    std::thread t1 {&do_work, 0};
    std::thread t2 {&do_work, 1};
    t1.join();
    t2.join();
}

void bakery()
{
#if 1
    startt();
    std::thread t[LMPWRK];
    for(int i=0; i < LMPWRK; i++) {
        t[i] = std::thread{do_work, i};
    }

    for(int i=0; i < LMPWRK; i++) {
        t[i].join();
    }
    double timeelapsed = endt();
    std::cout << "execution time : " << timeelapsed << "\r\n";
#else
    startt();
    do_work(1);
    double timeelapsed = endt();
    std::cout << "execution time : " << timeelapsed << "\r\n";
#endif
}
#undef LMPWRK
