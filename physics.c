#include "physics.h"

CFUNCTION double physics_sndpropagationtime(double distance, double soundspeed)
{
    if(distance == 0) return 0.0;
    // If distance is 0, then sound wave doesn't even need to
    // move to pass this distance, i.e time equals 0.

    if(distance < 0 || soundspeed <= 0)
    {
        errno = EINVAL;
        return 0;
    }
    else errno = 0;

    return (distance/soundspeed);
    // Sound are propagating evenly (without acceleration),
    // so t=S/c, where c - sound speed, t - time, S - distance.
}