#pragma once

#include <stdint.h>

/* Struct, representing common file header. */
typedef struct fheader
{
   /* Number of audio channels. */
   uint32_t channels;

   /* Sample rate (number of samples per second) of the track. */
   uint32_t samplerate;
   
} fheader;