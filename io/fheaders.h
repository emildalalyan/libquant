#pragma once

#include <stdint.h>

/* Struct, representing common file header. */
typedef struct fheader
{
   /* Number of audio channels. */
   uint64_t channels;

   /* Sample rate (number of samples per second) of the track. */
   uint64_t samplerate;

   // Sample rate and channels are "uint64_t", because sometimes
   // we need to swap bytes (because of endianness difference) in this numbers
   // and we have to know exactly how many bytes occupy that numbers.
} fheader;