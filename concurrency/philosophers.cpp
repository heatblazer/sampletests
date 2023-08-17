#include "philosophers.h"
#include <chrono>
#include <random>
#include <mutex>

using namespace std::chrono_literals;

static std::mutex g_PrintMtx;

namespace
{
size_t my_rand(size_t min, size_t max)
{
    static std::mt19937 rnd(std::time(nullptr));
    return std::uniform_int_distribution<>(min, max)(rnd);
}
}

void Philosophers::addPhilosopher(int food, int index)
{
    P* newnode = new P{food, index, this};
    if (!m_head) {
        newnode->next = m_tail;
        newnode->prev = m_head;
        m_head = m_tail = newnode;
    } else {
        m_tail->next = newnode;
        newnode->prev = m_tail;
        m_tail = newnode;
    }
}

void Philosophers::makeCirc(bool ok)
{
    if (m_head)
    {
        if (ok) {
            m_head->prev = m_tail;
            m_tail->next = m_head;
        } else {
            m_head->prev = nullptr;
            m_tail->next = nullptr;
        }
    }
}

bool Philosophers::allDone()
{
    bool ok = true;
    P* it = m_head;
    for(int i=0; i < m_phiHeadCount; i++) {
        if (it->food_to_eat > 0)
            ok &= false;
        it = it->next;
    }
    return ok;
}

//proxy fn to main class
void Philosophers::_start(P *p)
{
    //while there is a food in the plate
        //think
        //take the forks
        // eat
        // release the forks
    while (!p->p_Root->allDone())
    {
        p->think();
        p->take();
        p->eat();
        p->release();
    }
}

Philosophers::Philosophers(int count, int food):
    m_workers{0},
    m_head{nullptr}, m_tail{nullptr},
    m_phiHeadCount{count}
{
    for(int i=0; i < m_phiHeadCount; i++) {
        addPhilosopher(food, i+1);
    }
    makeCirc(true);
}

Philosophers::~Philosophers()
{
    makeCirc(false);
    while (m_head)
    {
        P* it = m_head;
        m_head = m_head->next;
        delete it;
        it = nullptr;
    }
    for(int i=0; i < m_phiHeadCount; i++) {
        delete m_workers[i];
        m_workers[i] = nullptr;
    }
}


void Philosophers::start()
{
    P* it = m_head;
    for(int i=0; i < m_phiHeadCount; i++) {
        m_workers[i] = new std::thread{&Philosophers::_start, it};
        it = it->next;
    }        

    for(int i=0; i < m_phiHeadCount; i++) {
        m_workers[i]->join();
    }

    std::cout << "All finished eating..." << std::endl;
    return;
}


//this function is for tests and debugging different stuff
void Philosophers::debugfn()
{
    P* it = m_head;
    while (!allDone())
    {
        std::cout << "still food on table, Idx:  " << it->index <<
            " remaining : " << it->food_to_eat << "\r\n";
        it->food_to_eat--;
        it = it->next;
    }
}

void Philosophers::P::think()
{
    size_t duration = my_rand(400, 800);
    {
        std::lock_guard<std::mutex> lk(g_PrintMtx); // critical section for uninterrupted print
        std::cout <<"Philosopher No: " << index << " is thinking " << duration << "ms\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));

}

void Philosophers::P::take()
{
    prev->fork_lock.release();
    next->fork_lock.release();
    fork_lock.acquire();
    {
        std::lock_guard<std::mutex> lk(g_PrintMtx); // critical section for uninterrupted print
        std::cout <<"Philosopher No: " << index << " is trying to take a forks from : "
                  << this->prev->index << " and " << this->next->index << "\r\n";
    }
}

void Philosophers::P::release()
{
    prev->fork_lock.acquire();
    next->fork_lock.acquire();
    fork_lock.release();
    {
        std::lock_guard<std::mutex> lk(g_PrintMtx); // critical section for uninterrupted print
        std::cout <<"Philosopher No: " << index << " has stopped to eat..." << "\r\n";
    }
}

void Philosophers::P::eat()
{
    food_to_eat--;
    {
        std::lock_guard<std::mutex> lk(g_PrintMtx); // critical section for uninterrupted print
        std::cout <<"Philosopher No: " << index << " food remaining : " << food_to_eat
                   << "\r\n";
    }
}

