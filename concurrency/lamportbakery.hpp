#ifndef LAMPORTBAKERY_H
#define LAMPORTBAKERY_H

template <unsigned N>
struct LamportBakery {

    bool entering[N] = {false};
    int threadId[N] = {0};

    void lock(int tid)
    {
        entering[tid] = true;
        int max = 0;
        for(int j=0; j < N; j++) {
            if (threadId[j] > max)
                max = threadId[j];
        }
        threadId[tid] = max+1;
        entering[tid] = false;

        for(int j=0; j < N; j++) {
            while (entering[j])
                ;
            while ((threadId[j] != 0) &&
                   ((threadId[j] < threadId[tid]) ||
                    ((threadId[j] == threadId[tid])&&
                     j < tid))) {}
        }
    }

    void unlock(int tid)
    {
        threadId[tid] = 0;
    }

    void dbgfn()
    {
        int worker1 = 1;
        int worker2 = 2;
        int wokrer3 = 3;

        int value = 100;
        while (value > 0)
        {
            lock(worker1);
            value--;
            unlock(worker1);
            lock(worker2);
            value--;
            unlock(worker2);
        }
    }
};

#endif // LAMPORTBAKERY_H
