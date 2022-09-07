#include "wav.h"

CFUNCTION int wav_write_file(wav_header* header, FILE* file, slevel_t* samples, size_t length)
{
    if(header == NULL || file == NULL || samples == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    uint64_t channels = header->channels;
    uint64_t samplerate = header->samplerate;
    uint16_t format = header->audioformat;
    uint16_t depth = header->samplesdepth;

    if(channels < 1 || samplerate < 1 || depth < 1) return FUNC_INVALID_ARG;

    if(length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    if(channels > UINT16_MAX || samplerate > UINT32_MAX) return FUNC_UNSUPPORTED;
    // In WAV files number of channels is written as 16-bit unsigned integer,
    // so maximum possible number of channels is 65535.
    // And sample rate is written as 32-bit unsigned integer,
    // so maximum possible sample rate is (2^32)-1 Hz

    rewind(file);
    // It sets file position to 0th byte.

    /* RIFF chunk ====== */

    if(fwrite(WAVE_SIGNRIFF, sizeof(char), WAVE_SIGNLENGTH, file) != WAVE_SIGNLENGTH)
    // It writes RIFF sign in beginning of file.
        return FUNC_IO_ERROR;

    if(fseek(file, sizeof(uint32_t), SEEK_CUR))
    // We'll write file size after writing all data.
        return FUNC_IO_ERROR;
    
    if(fwrite(WAVE_SIGNWAVFORMAT, sizeof(char), WAVE_SIGNLENGTH, file) != WAVE_SIGNLENGTH)
    // It writes WAV format sign.
        return FUNC_IO_ERROR;

    uint32_t riffchunksize = ftell(file);
    // RIFF chunk size matches current file position, because
    // this chunk are first in the file.
    
    /* ================= */

    /* Format subchunk = */

    if(fwrite(WAVE_SIGNFMTCHUNK, sizeof(char), WAVE_SIGNLENGTH, file) != WAVE_SIGNLENGTH)
    // It writes format subchunk sign.
        return FUNC_IO_ERROR;

    if(fseek(file, sizeof(uint32_t), SEEK_CUR))
    // We'll write subchunk size after writing all data.
        return FUNC_IO_ERROR;

    uint16_t format_le = format;
    uint16_t depth_le = depth;
    uint64_t channels_le = channels;
    uint64_t samplerate_le = samplerate;

    uint32_t byterate_le;
    switch(format)
    {
        case WAVE_FORMAT_PCM:
        case WAVE_FORMAT_IEEE_FLOAT:
        {
            if(depth < 8u) return FUNC_INVALID_ARG;
            // If sample depth would be less than 8, we'd divide
            // data size by 0, so it would be incorrect.

            if(depth % 8u) return FUNC_INVALID_ARG;
            // Depth must be multiple of 8.
            
            byterate_le = (channels*samplerate*(depth/8u));
            if(byterate_le < 1) return FUNC_INTERNAL_ERROR;

            break;
        }
        default: return FUNC_UNSUPPORTED;
    }
    // We calculate byte rate of the audio depending
    // on in which format data are going to store.

    uint16_t sampleslength_le = byterate_le/samplerate;

    #if ENDIANNESS == __ORDER_BIG_ENDIAN__
        format_le = swap_16b(format);
        depth_le = swap_16b(depth);
        channels_le = swap_64b(channels);
        samplerate_le = swap_64b(samplerate);
        byterate_le = swap_32b(byterate_le);
        sampleslength_le = swap_16b(sampleslength_le);
    #endif
    // All integers in WAV file are little-endian, so if machine
    // is big-endian, we need to swap bytes in integers.

    if( fwrite(&format_le,        sizeof(uint16_t), 1, file)  != 1
    ||  fwrite(&channels_le,      sizeof(uint16_t), 1, file)  != 1
    ||  fwrite(&samplerate_le,    sizeof(uint32_t), 1, file)  != 1
    ||  fwrite(&byterate_le,      sizeof(uint32_t), 1, file)  != 1
    ||  fwrite(&sampleslength_le, sizeof(uint16_t), 1, file)  != 1
    ||  fwrite(&depth_le,         sizeof(uint16_t), 1, file)  != 1 )
        return FUNC_IO_ERROR;

    uint32_t fmtchunkend = ftell(file);
    // Storages position of format chunk end.

    /* It sets format chunk size. */
    {
        uint32_t fmtchunksize = (fmtchunkend-riffchunksize)-(WAVE_SIGNLENGTH+sizeof(uint32_t));
        // Storages format chunk size (except of its signature and chunk size field)

        if(fseek(file, riffchunksize+WAVE_SIGNLENGTH, SEEK_SET))
            return FUNC_IO_ERROR;

        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
            fmtchunksize = swap_32b(fmtchunksize);
        #endif

        if(fwrite(&fmtchunksize, sizeof(uint32_t), 1, file) != 1)
            return FUNC_IO_ERROR;
    }
    // Brackets limit variables scope.

    if(fseek(file, fmtchunkend, SEEK_SET))
    // Returning back to end of format chunk.
        return FUNC_IO_ERROR;
        
    /* ================= */

    /* Data subchunk === */

    if(fwrite(WAVE_SIGNDATACHUNK, sizeof(char), WAVE_SIGNLENGTH, file) != WAVE_SIGNLENGTH)
    // It writes data subchunk sign.
        return FUNC_IO_ERROR;
    
    if(fseek(file, sizeof(uint32_t), SEEK_CUR))
    // We'll write subchunk size after writing all data.
        return FUNC_IO_ERROR;

    uint32_t headerlength = ftell(file);
    
    // This is the end of header.

    switch(format)
    {
        case WAVE_FORMAT_PCM:
        {
            size_t maxlength = UINT32_MAX - (headerlength-(WAVE_SIGNLENGTH + sizeof(uint32_t)));
            // In RIFF, file size is storing as 32-bit number,
            // file size field defines size of entire file
            // EXCLUDING length of 'RIFF' signature and length of file size field,
            // so maximum file size is ((2^32)-1)+8, and maximum data size is (2^32-1)-('header length'-8)

            if(length > maxlength/(depth/8u))
            {
                length = maxlength/(depth/8u);

                length -= (length % channels);
                // Each channel must have the same number of samples.
                
                // Truncating file to maximum size.
            }

            switch(depth)
            {
                case 16:
                {
                    int16_t* output = (int16_t*)malloc(length * sizeof(int16_t));
                    if(output == NULL) return FUNC_MEMALLOC_FAILED;
                    // If allocation was failed, malloc will return NULL pointer

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < length; i++)
                    {
                        int16_t sample = slttoi16(samples[i]);

                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = iswap_16b(sample);
                        #endif

                        output[i] = sample;
                    }

                    if(fwrite(output, sizeof(int16_t), length, file) != length)
                    {
                        free(output);
                        return FUNC_IO_ERROR;
                    }
                    
                    free(output);    
                    break;
                }
                case 24:
                {
                    int8_t* output = (int8_t*)malloc(length * 3);
                    // We read each 24-bit sample as 3 individual bytes, and then
                    // combine them back to 3-byte integer, because C lacks
                    // 24-bit integer type.

                    if(output == NULL) return FUNC_MEMALLOC_FAILED;
                    // If allocation was failed, malloc will return NULL pointer

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < length; i++)
                    {
                        int32_t sample = slttoi24(samples[i]);
                        
                        size_t bytepos = i*3;
                        output[bytepos] = (sample >> 8) & UINT8_MAX;
                        output[bytepos+1] = (sample >> 16) & UINT8_MAX;
                        output[bytepos+2] = (sample >> 24) & UINT8_MAX;
                        // Samples in WAV files (in case of integer PCM format)
                        // are writing as little-endian integers.
                    }

                    if(fwrite(output, 3, length, file) != length)
                    {
                        free(output);
                        return FUNC_IO_ERROR;
                    }
                    
                    free(output);    
                    break;
                }
                case 32:
                {
                    int32_t* output = (int32_t*)malloc(length * sizeof(int32_t));
                    if(output == NULL) return FUNC_MEMALLOC_FAILED;
                    // If allocation was failed, malloc will return NULL pointer

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < length; i++)
                    {
                        int32_t sample = slttoi32(samples[i]);

                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = iswap_32b(sample);
                        #endif

                        output[i] = sample;
                    }

                    if(fwrite(output, sizeof(int32_t), length, file) != length)
                    {
                        free(output);
                        return FUNC_IO_ERROR;
                    }
                    
                    free(output);    
                    break;
                }
                default: return FUNC_UNSUPPORTED;
            }
            break;
        }
        case WAVE_FORMAT_IEEE_FLOAT:
        {
            size_t maxlength = UINT32_MAX - (headerlength-(WAVE_SIGNLENGTH + sizeof(uint32_t)));
            // In RIFF, file size is storing as 32-bit number,
            // file size field defines size of entire file
            // EXCLUDING length of 'RIFF' signature and length of file size field,
            // so maximum file size is ((2^32)-1)+8.

            if(length > maxlength/(depth/8))
            {
                length = maxlength/(depth/8);

                length -= (length % channels);
                // Each channel must have the same number of samples.
                
                // Truncating file to maximum size.
            }

            switch(depth)
            {
                case 32:
                {
                    float* output = (float*)malloc(length * sizeof(float));
                    if(output == NULL) return FUNC_MEMALLOC_FAILED;
                    // If allocation was failed, malloc will return NULL pointer

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < length; i++)
                    {
                        float sample = slttof32(samples[i]);

                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = fswap_32b(sample);
                        #endif

                        output[i] = sample;
                    }

                    if(fwrite(output, sizeof(float), length, file) != length)
                    {
                        free(output);
                        return FUNC_IO_ERROR;
                    }
                    
                    free(output);    
                    break;
                }
                case 64:
                {
                    double* output = (double*)malloc(length * sizeof(double));
                    if(output == NULL) return FUNC_MEMALLOC_FAILED;
                    // If allocation was failed, malloc will return NULL pointer

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < length; i++)
                    {
                        double sample = slttof64(samples[i]);

                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = fswap_64b(sample);
                        #endif

                        output[i] = sample;
                    }

                    if(fwrite(output, sizeof(double), length, file) != length)
                    {
                        free(output);
                        return FUNC_IO_ERROR;
                    }
                    
                    free(output);    
                    break;
                }
                default: return FUNC_UNSUPPORTED;
            }
            break;
        }
        default: return FUNC_UNSUPPORTED;
    }

    /* ================= */

    /* Writing data size and file size === */

    uint32_t filesize = ftell(file);

    {
        uint32_t datasize = filesize - headerlength;
        // Storages size of data.

        if(fseek(file, (headerlength - sizeof(uint32_t)), SEEK_SET))
            return FUNC_IO_ERROR;

        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
            datasize = swap_32b(datasize);
        #endif

        if(fwrite(&datasize, sizeof(uint32_t), 1, file) != 1)
            return FUNC_IO_ERROR;
    }
    // Brackets limit variables scope.

    {
        uint32_t allchunkssize = filesize - (WAVE_SIGNLENGTH + sizeof(uint32_t));
        // Storages size of entire file except of RIFF signature size and file size.

        if(fseek(file, WAVE_SIGNLENGTH, SEEK_SET))
            return FUNC_IO_ERROR;

        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
            allchunkssize = swap_32b(allchunkssize);
        #endif

        if(fwrite(&allchunkssize, sizeof(uint32_t), 1, file) != 1)
            return FUNC_IO_ERROR;
    }
    // Brackets limit variables scope.
    
    rewind(file);
    // It sets file position back to 0th byte.

    /* =================================== */

    return FUNC_OK;
}

CFUNCTION int wav_check_signatures(wav_header* header)
{
    if( strcmp(header->fmtchunksign, WAVE_SIGNFMTCHUNK)
    ||  strcmp(header->datasign,     WAVE_SIGNDATACHUNK)
    ||  strcmp(header->riffsign,     WAVE_SIGNRIFF)
    ||  strcmp(header->filetype,     WAVE_SIGNWAVFORMAT) )
        return FUNC_SIGNATURE_FAILURE;
    else return FUNC_OK;
}

CFUNCTION int wav_read_header(wav_header* header, FILE* file)
{
    if(file == NULL || header == NULL) return FUNC_INVALID_ARG;

    wav_header tmpheader;
    memset(&tmpheader, 0, sizeof(wav_header));

    rewind(file);
    if( fread(&tmpheader.riffsign, sizeof(char), WAVE_SIGNLENGTH, file) != WAVE_SIGNLENGTH
    ||  fread(&tmpheader.filesize, sizeof(uint32_t), 1, file)           != 1
    ||  fread(&tmpheader.filetype, sizeof(char), WAVE_SIGNLENGTH, file) != WAVE_SIGNLENGTH )
        return FUNC_IO_ERROR;

    char currchunksign[WAVE_SIGNLENGTH+1];
    memset(currchunksign, 0, sizeof(currchunksign));

    while(true)
    {
        if(fread(currchunksign, sizeof(char), WAVE_SIGNLENGTH, file) != WAVE_SIGNLENGTH)
            return FUNC_IO_ERROR;

        if(!strcmp(currchunksign, WAVE_SIGNFMTCHUNK))
        {
            strcpy(tmpheader.fmtchunksign, currchunksign);

            if(fread(&tmpheader.fmtchunksize, sizeof(uint32_t), 1, file) != 1)
                return FUNC_IO_ERROR;

            #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                tmpheader.fmtchunksize = swap_32b(tmpheader.fmtchunksize);
            #endif

            uint32_t position = ftell(file);

            if( fread(&tmpheader.audioformat,   sizeof(uint16_t), 1, file) != 1
            ||  fread(&tmpheader.channels,      sizeof(uint16_t), 1, file) != 1
            ||  fread(&tmpheader.samplerate,    sizeof(uint32_t), 1, file) != 1
            ||  fread(&tmpheader.byterate,      sizeof(uint32_t), 1, file) != 1
            ||  fread(&tmpheader.sampleslength, sizeof(uint16_t), 1, file) != 1
            ||  fread(&tmpheader.samplesdepth,  sizeof(uint16_t), 1, file) != 1 )
                return FUNC_IO_ERROR;

            fseek(file, position + tmpheader.fmtchunksize, SEEK_SET);
            continue;
        }
        else if(!strcmp(currchunksign, WAVE_SIGNDATACHUNK))
        {
            strcpy(tmpheader.datasign, currchunksign);

            if(fread(&tmpheader.datasize, sizeof(uint32_t), 1, file) != 1)
                return FUNC_IO_ERROR;

            tmpheader.dataoffset = ftell(file);

            break;
        }
        else if( !strcmp(currchunksign, WAVE_SIGNFACTCHUNK)
              || !strcmp(currchunksign, WAVE_SIGNID3CHUNK)
              || !strcmp(currchunksign, WAVE_SIGNPEAKCHUNK)
              || !strcmp(currchunksign, WAVE_SIGNLISTCHUNK)
              || !strcmp(currchunksign, WAVE_SIGNIDVXCHUNK)
              || !strcmp(currchunksign, WAVE_SIGNJUNKCHUNK) )
        // We don't use these chunks, we just skip them.
        {
            uint32_t size = 0;
            if(fread(&size, sizeof(uint32_t), 1, file) != 1)
                return FUNC_IO_ERROR;
            // In RIFF containers, each chunk (after its signature)
            // contains its size (in bytes).
            
            #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                size = swap_32b(size);
            #endif

            fseek(file, size, SEEK_CUR);
            // We just skip this chunk.

            continue;
        }
        else return FUNC_UNSUPPORTED;
    }

    if(tmpheader.dataoffset == 0)
    // If data offset wasn't be fetched from header,
    // then "data" section is missing in header,
    // so we can't read this file properly.
        return FUNC_UNSUPPORTED;

    if(wav_check_signatures(&tmpheader) == FUNC_SIGNATURE_FAILURE)
        return FUNC_SIGNATURE_FAILURE;

    #if ENDIANNESS == __ORDER_BIG_ENDIAN__
        tmpheader.audioformat = swap_16b(tmpheader.audioformat);
        tmpheader.channels = swap_64b(tmpheader.channels);
        tmpheader.samplesdepth = swap_16b(tmpheader.samplesdepth);
        tmpheader.sampleslength = swap_16b(tmpheader.sampleslength);
        tmpheader.byterate = swap_32b(tmpheader.byterate);
        tmpheader.datasize = swap_32b(tmpheader.datasize);
        tmpheader.filesize = swap_32b(tmpheader.filesize);
        tmpheader.fmtchunksize = swap_32b(tmpheader.fmtchunksize);
        tmpheader.samplerate = swap_64b(tmpheader.samplerate);
    #endif

    *header = tmpheader;

    return FUNC_OK;
}

CFUNCTION int wav_read_samples(wav_header* header, FILE* file, slevel_t** samples, size_t* length)
{
    if(header == NULL || file == NULL || samples == NULL || length == NULL) return FUNC_INVALID_ARG;

    if(wav_check_signatures(header) == FUNC_SIGNATURE_FAILURE)
        return FUNC_SIGNATURE_FAILURE;

    uint64_t channels = header->channels;
    uint16_t format = header->audioformat;
    uint16_t depth = header->samplesdepth;
    uint32_t datasize = header->datasize;
    uint32_t dataoffset = header->dataoffset;

    if(channels < 1 || datasize < 1) return FUNC_INVALID_ARG;

    fseek(file, dataoffset, SEEK_SET);
    // Seeking to position, when data begins.

    size_t numsamples;
    switch(format)
    {
        case WAVE_FORMAT_PCM:
        case WAVE_FORMAT_IEEE_FLOAT:
        {
            if(depth < 8) return FUNC_INVALID_ARG;
            // If sample depth would be less than 8, we'd divide
            // data size by 0, so it would be incorrect.
            
            numsamples = (datasize/(depth/8u));
            if(numsamples < 1) return FUNC_INTERNAL_ERROR;

            numsamples -= (numsamples % channels);
            // If number of samples is not multiple of channels, then
            // length of each channel are different, but this cannot be in WAV.
            // So we just subtracting remainder from numsamples for correct editing of signal.

            break;
        }
        default: return FUNC_UNSUPPORTED;
    }
    // We calculate number of samples depending
    // on in which format data are storing in the file.
    
    slevel_t* samplesarr = (slevel_t*)malloc(numsamples * sizeof(slevel_t));
    if(samplesarr == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    *samples = samplesarr;

    switch(format)
    {
        case WAVE_FORMAT_PCM:
        {
            switch(depth)
            {
                case 16:
                {
                    int16_t* ogsamples = (int16_t*)malloc(numsamples * sizeof(int16_t));
                    if(ogsamples == NULL)
                    // If allocation was failed, malloc will return NULL pointer
                    {
                        samples_free(samples);
                        return FUNC_MEMALLOC_FAILED;
                    }

                    if(fread(ogsamples, sizeof(int16_t), numsamples, file) != numsamples)
                    {
                        free(ogsamples);
                        samples_free(samples);
                        return FUNC_IO_ERROR;
                    }

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < numsamples; i++)
                    {
                        int16_t sample = ogsamples[i];

                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = iswap_16b(sample);
                        #endif
                        
                        samplesarr[i] = i16toslt(sample);
                    }

                    free(ogsamples);
                    break;
                }
                case 24:
                {
                    uint8_t* ogsamples = (uint8_t*)malloc(numsamples * 3);
                    // We read each 24-bit sample as 3 individual bytes, and then
                    // combine them back to 3-byte integer, because C lacks
                    // 24-bit integer type.

                    if(ogsamples == NULL)
                    {
                        samples_free(samples);
                        return FUNC_MEMALLOC_FAILED;
                    }
                    // If allocation was failed, malloc will return NULL pointer

                    if(fread(ogsamples, 3, numsamples, file) != numsamples)
                    {
                        free(ogsamples);
                        samples_free(samples);
                        return FUNC_IO_ERROR;
                    }

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < numsamples; i++)
                    {
                        size_t bytepos = i*3;
                        int32_t sample =  (ogsamples[bytepos+0] << 8)
                                        | (ogsamples[bytepos+1] << 16)
                                        | (ogsamples[bytepos+2] << 24);
                        
                        samplesarr[i] = i24toslt(sample);
                    }

                    free(ogsamples);
                    break;
                }
                case 32:
                {
                    int32_t* ogsamples = (int32_t*)malloc(numsamples * sizeof(int32_t));
                    if(ogsamples == NULL)
                    {
                        samples_free(samples);
                        return FUNC_MEMALLOC_FAILED;
                    }
                    // If allocation was failed, malloc will return NULL pointer
                    
                    if(fread(ogsamples, sizeof(int32_t), numsamples, file) != numsamples)
                    {
                        free(ogsamples);
                        samples_free(samples);
                        return FUNC_IO_ERROR;
                    }

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < numsamples; i++)
                    {
                        int32_t sample = ogsamples[i];

                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = iswap_32b(sample);
                        #endif

                        samplesarr[i] = i32toslt(sample);
                    }

                    free(ogsamples);
                    break;
                }
                default:
                {
                    samples_free(samples);
                    return FUNC_UNSUPPORTED;
                }
            }

            *length = numsamples;
            // We set length only if reading
            // of samples was successful.

            return FUNC_OK;
        }
        case WAVE_FORMAT_IEEE_FLOAT:
        {
            switch(depth)
            {
                case 32:
                {
                    float* ogsamples = (float*)malloc(numsamples * sizeof(float));
                    if(ogsamples == NULL)
                    {
                        samples_free(samples);
                        return FUNC_MEMALLOC_FAILED;
                    }
                    // If allocation was failed, malloc will return NULL pointer

                    if(fread(ogsamples, sizeof(float), numsamples, file) != numsamples)
                    {
                        free(ogsamples);
                        samples_free(samples);
                        return FUNC_IO_ERROR;
                    }

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < numsamples; i++)
                    {
                        float sample = ogsamples[i];
                        
                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = fswap_32b(sample);
                        #endif

                        samplesarr[i] = f32toslt(sample);
                    }

                    free(ogsamples);
                    break;
                }
                case 64:
                {
                    double* ogsamples = (double*)malloc(numsamples * sizeof(double));
                    if(ogsamples == NULL)
                    {
                        samples_free(samples);
                        return FUNC_MEMALLOC_FAILED;
                    }
                    // If allocation was failed, malloc will return NULL pointer

                    if(fread(ogsamples, sizeof(double), numsamples, file) != numsamples)
                    {
                        free(ogsamples);
                        samples_free(samples);
                        return FUNC_IO_ERROR;
                    }

                    #pragma omp parallel for schedule(static)
                    for(size_t i = 0; i < numsamples; i++)
                    {
                        double sample = ogsamples[i];

                        #if ENDIANNESS == __ORDER_BIG_ENDIAN__
                            sample = fswap_64b(sample);
                        #endif

                        samplesarr[i] = f64toslt(sample);
                    }
                    
                    free(ogsamples);
                    break;
                }
                default:
                {
                    samples_free(samples);
                    // We have to free the memory, that has been
                    // allocated for samples, to prevent memory leaks.

                    return FUNC_UNSUPPORTED;
                }
            }

            *length = numsamples;
            // We set length only if reading
            // of samples was successful.

            return FUNC_OK;
        }
        default:
        {
            samples_free(samples);
            // We have to free the memory, that has been
            // allocated for samples, to prevent memory leaks.

            return FUNC_UNSUPPORTED;
        }
    }
}