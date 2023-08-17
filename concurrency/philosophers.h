#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <thread>
#include <atomic>
#include <iostream>
#include <semaphore>
#include <vector>

class Philosophers
{
    struct P
    {
        int food_to_eat;
        int index;
        Philosophers* p_Root;
        P* next;
        P* prev;
        std::binary_semaphore fork_lock;

        P():
            food_to_eat{0},index{0},
            p_Root{nullptr}, next{nullptr}, prev{nullptr}, fork_lock{1} {}

        P(int f, int idx, Philosophers* root):
            food_to_eat{f},index{idx},
            p_Root{root}, next{nullptr}, prev{nullptr}, fork_lock{1}
        {};

        ~P() = default;

        void think();

        void take();

        void release();

        void eat();
    };

    std::vector<std::thread*> m_workers;

    P* m_head;

    P* m_tail;

    int m_phiHeadCount;

    void addPhilosopher(int food, int index);

    void makeCirc(bool ok=false);

    bool allDone();

    static void _start(P *p);

public:
    Philosophers() = delete;

    Philosophers(int count, int food);

    ~Philosophers() ;

    void start();

    void debugfn();

};

#endif // PHILOSOPHERS_H
