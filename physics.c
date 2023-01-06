#include "physics.h"

CFUNCTION double physics_getechodelay(double distance, double soundspeed)
{
    if(distance == 0) return 0.0;
    // If distance is 0, then sound wave doesn't even need to
    // move to pass this distance, i.e time equals 0.

    return (soundspeed == 0) ? HUGE_VAL : (distance/soundspeed);
    // Sound are propagating evenly (without acceleration),
    // so t=S/c, where c - sound speed, t - time, S - distance.
    // But if sound speed is 0, then sound wave doesn't even
    // move, so time is infinity.
}