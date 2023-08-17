#include "philosophers.h"
#include <cstdlib>
int main(int argc, char** argv)
{
    int p,f;
    if (argc != 3) {
        p=5;
        f=100;
    } else{
        p = atoi(argv[1]);
        f = atoi(argv[2]);
    }
    Philosophers philosophers{p, f};
    philosophers.start();
    return 0;
}
