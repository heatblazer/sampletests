#ifndef PETERSON_H
#define PETERSON_H

#include <thread>

namespace Peterson
{
    void lock(int other);

    void unlock(int other);
};



#endif // PETERSON_H
