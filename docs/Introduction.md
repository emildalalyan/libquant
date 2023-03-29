## [<- Back to the main page](Main.md)

## Introduction to the libquant

### Licence
libquant is published under permissive [MIT Licence](https://en.wikipedia.org/wiki/MIT_Licence)

### Build guide
- **Step 1**: Download and install **make**
    - Command for [Chocolatey](https://chocolatey.org) (Windows): <code>choco install make</code>
    - Command for **apt** (GNU/Linux): <code>apt install make</code>
- **Step 2**: Download and install **compiler** (e.g. GCC, Clang, Visual C++ 2010 and higher)
    - Command for **apt** (GNU/Linux): <code>apt install gcc</code>
- **Step 3**: Download and **extract source code** in some directory.
- **Step 4**: Open the console, and go to directory from **Step 3**
- **Step 5**: Do <code>make</code> and wait for compilation.
    - If you use compiler which executable is not called <code>gcc</code>, then enter <code>make COMPILER=(compiler executable name)</code> instead of just <code>make</code>.
- **Step 6**: All done! Library will be in "*build*" directory

### Linking
By default, libquant builds as **static library**. Example command for its linking:

- <code>gcc main.c -o main libquant.a</code>

### Workspace
You can use any suitable workspace, but libquant is written using **[Visual Studio Code](https://code.visualstudio.com/)**. Source code already has .vscode directory with configured compilation task.

### Simple example program
This program reads **in.wav**, does echo effect, and then saves it to **out.wav**.
```C
    #include "(LIBQUANT SOURCES PATH)/main.h"

    int main()
    {
    /* Opening files ========= */
        FILE* inputfile = fopen("in.wav", "rb");
        FILE* outputfile = fopen("out.wav", "wb");

    /* Header reading ======== */
        wav_header header = {0}; // Creating header structure and zeroing it.
        printf("Reading header: %s\n",
            (wav_read_header(&header, inputfile) == FUNC_OK) ? "Done!" : "Failed.");

    /* Samples reading ======= */
        slevel_t* samples; // Pointer to samples array.
        size_t length; // It'll storage number of samples.

        printf("Reading samples: %s\n",
            (wav_read_samples(&header, inputfile, &samples, &length) == FUNC_OK) ? "Done!" : "Failed.");

    /* Producing echo effect = */
        printf("Echo effect (delay: 4410, feedback: 0.33, bounces: 10): %s\n",
            (effect_echo(samples, length, (fheader*)&header, 4410, 0.33, 10) == FUNC_OK) ? "Done!" : "Failed.");

    /* Saving the results ==== */
        printf("Saving file: %s\n",
            (wav_write_file(&header, outputfile, samples, length) == FUNC_OK) ? "Done!" : "Failed.");

    /* Freeing memory ======== */
        printf("Freeing the memory: %s\n",
            (samples_free(&samples) == FUNC_OK) ? "Done!" : "Failed.");

        return 0;
    }
```

### Language support
libquant is written in C, so you can call libquant functions from any language, that have C [FFI](https://en.wikipedia.org/wiki/Foreign_function_interface).