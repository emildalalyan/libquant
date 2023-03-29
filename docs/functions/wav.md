## [<- Back to the main page](../Main.md)

## WAV reader/writer functions ([wav.h](../../io/wav.h))

### <code>int wav_read_header(wav_header* header, FILE* file)</code>
- <code>header</code>: Pointer to header structure.
- <code>file</code>: File pointer.

Read WAV header from specified file and write it into structure by provided pointer.

### <code>int wav_check_signatures(wav_header* header)</code>
Check signatures of WAV file header.

Function will check following signatures: <code>"RIFF", "WAVE", "fmt ", "data"</code>.

See <code>[enum func_status](../../functions.h)</code> for function statuses enumeration.

### <code>int wav_read_samples(wav_header* header, FILE* file, slevel_t** samples, size_t* length)</code>
- <code>header</code>: Pointer to header structure.
- <code>file</code>: File pointer, that will be read.
- <code>samples</code>: Pointer to array pointer.
- <code>length</code>: Pointer to integer variable, function will write length of the samples array there. 

Read samples from WAV file.\
It reads samples in format, specified in header.

Function will do following things:
- allocating memory for samples array and set address of specified pointer.
- setting the length of samples array.

See [this example](../Introduction.md#simple-example-program) for better understanding.

### <code>int wav_write_file(wav_header* header, FILE* file, slevel_t* samples, size_t length)</code>
- <code>header</code>: Pointer to header structure.
- <code>file</code>: File pointer, that will be filled.
- <code>samples</code>: Pointer to samples array.
- <code>length</code>: Length of the samples array.

Write WAV header and samples into specified file.

Function will write samples in format, specified in header. \
It will use ONLY following fields in header:
<code>"channels", "samplerate", "audioformat", "samplesdepth"</code>.

*Note: If size of entire file will exceed 4GB, samples array will be truncated.*