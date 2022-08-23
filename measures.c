#include "measures.h"

CFUNCTION double conv_volratiotodb(double ratio)
{
    return 20*log10(ratio);
}

CFUNCTION double conv_dbtovolratio(double decibels)
{
    return pow(10, decibels/20);
}