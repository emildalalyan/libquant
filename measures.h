#pragma once

#include "build.h"
#include "math.h"

/* Converts volume ratio to dB(decibels). */
CFUNCTION double conv_volratiotodb(double ratio);

/* Converts dB(decibels) to volume ratio. */
CFUNCTION double conv_dbtovolratio(double decibels);