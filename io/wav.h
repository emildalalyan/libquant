#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "fheaders.h"
#include "../samples.h"
#include "../endianness.h"

/* ===== FORMATS ===== */

/* Enumeration of supported formats of WAV file. */
enum wav_formats
{
   /* Plain PCM format (without any compression) with integer type samples. */
   WAVE_FORMAT_PCM = 0x1,

   /* Plain PCM format (without any compression), but with floating-point type samples.
      Samples in this format must be in range of [-1, 1],
      i.e they cannot be less than -1, and greater than 1. */
   WAVE_FORMAT_IEEE_FLOAT = 0x3
};

/* =================== */

/* === SIGNATURES ==== */

/* Signature of the RIFF file */
#define WAVE_SIGNRIFF      "RIFF"

/* Signature of the format chunk */
#define WAVE_SIGNFMTCHUNK  "fmt "

/* Signature of the data chunk */
#define WAVE_SIGNDATACHUNK "data"

/* RIFF file format signature. */
#define WAVE_SIGNWAVFORMAT "WAVE"

/* Additional chunks' signatures ======== */

/* Signature of "fact" chunk (it contains additional information
   about file, compressed files usually include this chunk). */
#define WAVE_SIGNFACTCHUNK "fact"

/* Signature of "PEAK" chunk (it contains information about
   peak level of signal for each channel). */
#define WAVE_SIGNPEAKCHUNK "PEAK"

/* Signature of the ID3 chunk (it contains metadata in ID3 format). */
#define WAVE_SIGNID3CHUNK  "id3 "

/* Signature of the LIST chunk (it contains details about writer software). */
#define WAVE_SIGNLISTCHUNK "LIST"

/* Signature of the IDVX chunk (it contains DIVX metadata). */
#define WAVE_SIGNIDVXCHUNK "IDVX"

/* Signature of the JUNK chunk (it needed for data padding). */
#define WAVE_SIGNJUNKCHUNK "JUNK"

/* ====================================== */

/* Length (in bytes) of each sign. Length of all RIFF signs is the same. */
#define WAVE_SIGNLENGTH 4

/* =================== */

/* Structure containing information about WAV file. */
typedef struct wav_header
{
   /* Compatibility with fheader */

   /* Number of audio channels. */
   uint64_t channels;

   /* Sample rate of the track. */
   uint64_t samplerate;

   /* ========================= */

   /* Signature of the RIFF chunk in a file (5th character is null-terminator). */
   char riffsign[WAVE_SIGNLENGTH+1];

   /* File size without RIFF chunk length. */
   uint32_t filesize;

   /* File type signature. For WAV file it's "WAVE" (5th character is null-terminator). */
   char filetype[WAVE_SIGNLENGTH+1];

   /* Signature of Subchunk1 (format chunk). 5th character is null-terminator. */
   char fmtchunksign[WAVE_SIGNLENGTH+1];

   /* Size of the Subchunk1 (format chunk) */
   uint32_t fmtchunksize;
      
   /* File audio format. See "enum wav_formats". */
   uint16_t audioformat;

   /* File byte rate - (Sample Rate * (BitsPerSample/8) * Channels) */
   uint32_t byterate;

   /* Length of one sample in all channels - ((BitsPerSample/8) * Channels)  */
   uint16_t sampleslength;

   /* Depth of samples (in bits) */
   uint16_t samplesdepth;
    
   /* Subchunk2 signature (data chunk). 5th character is null-terminator.*/
   char datasign[WAVE_SIGNLENGTH+1];

   /* Size of the audio data (measured in bytes).
      It equals (NumSamples * NumChannels * (BitsPerSample/8)) */
   uint32_t datasize;

   /* Absolute offset of the data position. */
   uint32_t dataoffset;

} wav_header;

/* Read WAV header from specified file.
   Function will read header from specified header and
   it will write information into header, by provided pointer. */
CFUNCTION int wav_read_header(wav_header* header, FILE* file);

/* Check signatures of WAV file header.
   Function will check following signatures: "RIFF", "WAVE", "fmt ", "data".
   Function will return FUNC_SIGNATURE_FAILURE if signatures are incorrect,
   otherwise it'll return FUNC_OK. (see "enum func_status") */
CFUNCTION int wav_check_signatures(wav_header* header);

/* Read samples from WAV file.
   It reads samples in format, specified in header.
   Samples will be read using provided pointer to FILE.
   Function will allocate memory for samples array, and it will set address of pointer.
   Function also will set the length of samples array. */
CFUNCTION int wav_read_samples(wav_header* header, FILE* file, slevel_t** samples, size_t* length);

/* Write WAV header and samples into specified file.
   Function will write samples in format, specified in header.
   It will be using ONLY following fields in header:
   "channels", "samplerate", "audioformat", "samplesdepth".
   If size of all samples exceed 4GB, samples array will be truncated, because
   in RIFF format, size is storing as 32-bit integer, i.e maximum size is (2^32) bytes. */
CFUNCTION int wav_write_file(wav_header* header, FILE* file, slevel_t* samples, size_t length);