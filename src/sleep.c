#include "sleep.h"

void
sleep (u64 sec)
{
    u64 ticks_per_second = (400000000 / 32);
    u64 ctr = sec * ticks_per_second;

    while (ctr--) {
    }
}

