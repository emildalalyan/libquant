#include "filters.h"

CFUNCTION int effect_medianfilter(slevel_t* samples, size_t length, fheader* header, size_t windowsize)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1 || windowsize < 1) return FUNC_INVALID_ARG;

    if(windowsize == 1) return FUNC_OK;
    // If window size is 1, then there is no neighboring samples.
    // Median of one number is the same number.

    uint64_t channels = header->channels;

    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because number
    // of samples in each channel must be the same.

    if(windowsize > (length / channels)) return FUNC_INVALID_ARG;

    slevel_t* result = (slevel_t*)calloc(length, sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, NULL
    // pointer will be returned by malloc

    size_t windowright = windowsize/2;
    size_t windowleft = (windowsize/2) + ((windowsize % 2) ? 1 : 0);
    // "windowright" and "windowleft" represent size of right and left
    // sides of the window respectively.

    size_t loopstart = (windowleft-1)*channels;
    size_t loopend = length-(windowright*channels);

    int lasterrcode = FUNC_OK;
    // Last error code, it's shared between
    // all threads. By default it's FUNC_OK.

    #pragma omp parallel shared(lasterrcode)
    {
        slevel_t* window = (slevel_t*)malloc(windowsize * sizeof(slevel_t));
        // Window array is using by different threads simultaneously,
        // so we should use different arrays in each thread.

        if(window == NULL) lasterrcode = FUNC_MEMALLOC_FAILED;

        #pragma omp barrier
        // If one thread failed with memory allocation
        // then we have to not start the loop.
        if(lasterrcode == FUNC_OK)
        {
            #pragma omp for schedule(static)
            for(size_t i = loopstart; i < loopend; i++)
            {
                for(size_t wi = 0; wi < windowleft; wi++)
                {
                    window[wi] = samples[i - (wi*channels)];
                }
                // It collects samples from left side
                // of the window (preceding samples and current sample).

                for(size_t wi = 1; wi <= windowright; wi++)
                {
                    window[wi+(windowleft-1)] = samples[i + (wi*channels)];
                }
                // It collects samples from right side
                // of the window (forwarding samples).

                qsort(window, windowsize, sizeof(slevel_t), (int(*)(const void*, const void*))samples_compare);
                // It sorts neighboring samples.

                result[i] = window[windowsize/2];
                // Resulting sample equals median (middle
                // value in sorted list) of neighboring samples.
            }
        }

        if(window != NULL) free(window);
    }

    if(lasterrcode == FUNC_OK)
        memcpy(samples, result, length * sizeof(slevel_t));

    free(result);
    // We're copying the results of median filter
    // into samples array and freeing the memory.

    return lasterrcode;
}