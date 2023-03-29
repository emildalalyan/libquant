#include "filters.h"

CFUNCTION int effect_meanfilter(slevel_t* samples, size_t length, fheader* header, size_t windowsize, mean_func_t meanfunction)
{
    /* Arguments processing ============= */
    
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1 || windowsize < 1) return FUNC_INVALID_ARG;

    if(windowsize == 1) return FUNC_OK;
    // If window size is 1, then there is no neighboring samples.
    // Mean of one number is the same number.

    uint32_t channels = header->channels;
    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.
    
    if(windowsize > (length/channels)) return FUNC_INVALID_ARG;
    
    size_t windowright = windowsize/2;
    size_t windowleft = (windowsize/2) + (windowsize % 2);
    // They represent size of right and left
    // sides of the window respectively.

    /* ================================== */

    /* Producing the effect ============= */

    slevel_t* result = (slevel_t*)calloc(length, sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;

    size_t loopstart = (windowleft-1)*channels;
    size_t loopend = length-(windowright*channels);

    int lasterrcode = FUNC_OK;
    // Last error code, it's shared between all threads.

    #pragma omp parallel shared(lasterrcode)
    {
        slevel_t* window = (slevel_t*)malloc(windowsize * sizeof(slevel_t));
        if(window == NULL) lasterrcode = FUNC_MEMALLOC_FAILED;

        #pragma omp barrier
        // If one thread failed with memory allocation
        // then we have to not start the loop.
        if(lasterrcode == FUNC_OK)
        {
            #pragma omp for schedule(static)
            for(omp_iter_t i = loopstart; i < loopend; i++)
            {
                omp_iter_t windowstart = i-loopstart;
                for(size_t wi = 0; wi < windowsize; wi++)
                {
                    window[wi] = samples[windowstart + (wi*channels)];
                }
                // Collecting samples into the window.

                result[i] = meanfunction(window, windowsize);
                // Result is the mean of samples from the window.
            }
        }

        if(window != NULL) free(window);
    }
    
    /* ================================== */
    
    /* Moving result and freeing memory = */

    if(lasterrcode == FUNC_OK)
        memcpy(samples, result, length * sizeof(slevel_t));

    free(result);
    
    /* ================================== */

    return lasterrcode;
}