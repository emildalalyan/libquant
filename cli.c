#include "main.h"

CFUNCTION int cli_start(slevel_t** samples, size_t* length, fheader* header, FILE* output, FILE* input, bool echoinput)
{
    fprintf(output, "\nThis is the command line. Max symbols is %u.\n", MAX_CMD_SYMBOLS-1);
    // Max count of characters is MAX_CMD_SYMBOLS-1, and not MAX_CMD_SYMBOLS,
    // because last symbol must be 0 (null-terminator).
    // MAX_CMD_SYMBOLS is the number of symbols, that will be gathered from console.
    // See cli.h for this macro.
    
    int status = FUNC_OK;
    // Use it as temporary storage of
    // function returned status.

    while(!feof(input))
    {
        fputs(CMD_NEXT_MARK, output);
        
        if(fflush(output) == EOF)
        // We put all data from buffer into the output file.
        // If error was occurred, fflush() returns EOF.
            return FUNC_IO_ERROR;

        char inputarr[MAX_CMD_SYMBOLS];

        if(fgets(inputarr, MAX_CMD_SYMBOLS, input) == NULL)
        {
            if(feof(input)) strcpy(inputarr, "exit");
            // feof() updates information about end
            // of file, ONLY after failed read operation.

            else return FUNC_IO_ERROR;
        }

        if(echoinput)
        {
            fputs(inputarr, output);

            if(feof(input)) fprintf(output, "\n");
            // Command (if it was read from file, and not from console)
            // may not contain line feed (a.k.a LF) symbol,
            // so we put this symbol in output for correct line feeding.
        }

        if(!strcmp("\n", inputarr)) continue;
        // If input string contains only line feed (a.k.a LF) symbol, then user
        // just typed Enter and nothing more, so we won't do anything.

        char command[MAX_CMD_SYMBOLS] = {0};
        char args[MAX_CMD_SYMBOLS] = {0};

        if(sscanf(inputarr, "%s %[^\t\n]", command, args) < 1)
        // We're picking out command and arguments from input.
        {
            continue;
            // If there's no command and arguments, then user just typed
            // Enter and nothing more, so we won't do anything.
        }

        if(!strcmp(command, "exit") || !strcmp(command, "quit")) break;
        else if(!strcmp(command, "info"))
        {
            fprintf(output, "= File information ===========\n");

            fprintf(output, "- Number of channels: %" PRIu64 "\n", header->channels);
            fprintf(output, "- Sample rate: %" PRIu64 " Hz\n", header->samplerate);
            fprintf(output, "- Number of samples (in %" PRIu64 " channels): %zu\n", header->channels, (*length));

            continue;
        }
        else if(!strcmp(command, "about"))
        {
            fprintf(output, "= About =======================\n"
                            "- quant v%s by %s.\n"
                            "- Build by %s (%s)\n"
                            "- Compiled with %s\n\n",
                            build_getversion(), build_getauthors(),
                            __DATE__, __TIME__,
                            build_getcompilername());

            fprintf(output, "= Hardware ====================\n"
                            "- Computing threads: %i\n"
                            "- CPU architecture: %s (endianness: %s)\n\n",
                            omp_get_max_threads(),
                            build_getarchname(), endianness_getname());

            fprintf(output, "= Defines =====================\n"
                            "- CLOCKS_PER_SEC = %li\n"
                            "- FLT_EVAL_METHOD = %i\n\n",
                            CLOCKS_PER_SEC,
                            FLT_EVAL_METHOD);

            fprintf(output, "= Types length ================\n"
                            "- slevel_t (sample level type): %zu byte(s)\n"
                            "- double_t: %zu byte(s)\n"
                            "- float_t: %zu byte(s)\n"
                            "- size_t: %zu byte(s)\n",
                            sizeof(slevel_t),
                            sizeof(double_t),
                            sizeof(float_t),
                            sizeof(size_t));

            continue;
        }
        else if(!strcmp(command, "help"))
        {
            puts("= List of commands =====================");
            puts("- exit (or quit) - exit the program.\n"
                 "- dump - dump raw file to specified file.\n"
                 "- echo - apply echo effect.\n"
                 "- amplify - apply amplifying.\n"
                 "- quantize - apply quantization.\n"
                 "- info - get information about track.\n"
                 "- about - about the program.\n"
                 "- invert - apply inverting.\n"
                 "- reverse - reverse the audio.\n"
                 "- median - apply median filter.\n"
                 "- fade - apply fade-out/fade-in effect.\n"
                 "- speedup - speeds up audio without pitch changing.");
            
            continue;
        }
        else if(!strcmp(command, "reverse"))
        {
            double counted = 0.0;

            fprintf(output, "Applying reversing... ");

            COUNTTIME(status = effect_reverse((*samples), header, (*length)), counted);

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i\n", status);

            continue;
        }
        else if(!strcmp(command, "invert"))
        {
            double counted = 0.0;

            fprintf(output, "Applying inverting... ");

            COUNTTIME(status = effect_invert((*samples), (*length)), counted);

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i\n", status);

            continue;
        }
        else if(!strcmp(command, "median"))
        {
            size_t windowsize = 0;
         
            if(!strcmp(args, "") || sscanf(args, "%zu", &windowsize) < 1)
            {
                fprintf(output, "Usage: median [window size]\n");
                continue;
            }

            double counted = 0.0;

            fprintf(output, "Applying median filter (window size: %zu)... ", windowsize);

            COUNTTIME(status = effect_medianfilter((*samples), (*length), header, windowsize), counted);

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i\n", status);

            continue;
        }
        else if(!strcmp(command, "distort"))
        {
            double maxlvl, minlvl;
         
            if(!strcmp(args, "") || sscanf(args, "%lf %lf", &maxlvl, &minlvl) < 2)
            {
                fprintf(output, "Usage: distort [max level (0-1)] [min level (-1-0)]\n");
                continue;
            }

            double counted = 0.0;

            fprintf(output, "Applying distortion effect (max: %f, min: %f)... ", maxlvl, minlvl);

            COUNTTIME(status = effect_distort((*samples), (*length), maxlvl*SLEVEL_MAX, minlvl*SLEVEL_MAX), counted);

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i\n", status);

            continue;
        }
        else if(!strcmp(command, "fade"))
        {
            size_t duration = 0; double finalvolume = 0.0; unsigned fadetype;
         
            if(!strcmp(args, "") || sscanf(args, "%u %zu %lf", &fadetype, &duration, &finalvolume) < 3)
            {
                fprintf(output, "Usage: fade [0 - out/1 - in] [duration] [final/initial volume]\n");
                continue;
            }

            double counted = 0.0;

            switch(fadetype)
            {
                case 0: // 0 - is fade out
                {
                    fprintf(output, "Applying fade out effect (duration: %zu samples, final volume: %f)... ", duration, finalvolume);
                    COUNTTIME(status = effect_fadeout((*samples), (*length), header, finalvolume, duration), counted);

                    break;
                }
                case 1: // 1 - is fade in
                {
                    fprintf(output, "Applying fade in effect (duration: %zu samples, initial volume: %f)... ", duration, finalvolume);
                    COUNTTIME(status = effect_fadein((*samples), (*length), header, finalvolume, duration), counted);

                    break;
                }
                default:
                {
                    fprintf(output, "Usage: fade [0 - out/1 - in] [duration] [final/initial volume]\n");

                    continue;
                }
            }

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i\n", status);

            continue;
        }
        else if(!strcmp(command, "speedup"))
        {
            size_t chunksize, remaining;
            if(!strcmp(args, "") || sscanf(args, "%zu %zu", &chunksize, &remaining) < 2)
            {
                fprintf(output, "Usage: speedup [chunk size] [remaining samples]\n");
                continue;
            }

            double counted = 0.0;

            fprintf(output, "Applying speed-up effect... ");

            COUNTTIME(status = effect_speedup(samples, length, header, chunksize, remaining), counted);

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i\n", status);

            continue;
        }
        else if(!strcmp(command, "amplify"))
        {
            if(!strcmp(args, ""))
            {
                fprintf(output, "Usage: amplify [max or coefficient]\n");
                continue;
            }
            else if(!strcmp(args, "max"))
            {
                double counted = 0.0;

                fprintf(output, "Applying amplifying... ");

                COUNTTIME(status = effect_ampmax((*samples), (*length)), counted);

                if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
                else fprintf(output, "An error occurred: code %i\n", status);

                continue;
            }
            else
            {
                double counted = 0.0;

                double coefficient = 0.0;

                if(sscanf(args, "%lf", &coefficient) < 1)
                {
                    fprintf(output, "Usage: amplify [max or coefficient]\n");
                    continue;
                }

                fprintf(output, "Applying amplifying... ");

                COUNTTIME(status = effect_amplify((*samples), (*length), coefficient), counted);

                if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
                else fprintf(output, "An error occurred: code %i\n", status);

                continue;
            }
        }
        else if(!strcmp(command, "quantize"))
        {
            double counted = 0.0; size_t bits = 0; unsigned dither = 0;

            if(!strcmp(args, "") || sscanf(args, "%zu %u", &bits, &dither) < 2 || dither > 1)
            {
                fprintf(output, "Usage: quantize [number of bits] [dithering: 1(on) or 0(off)]\n");
                continue;
            }

            fprintf(output, "Applying quantization (depth: %zu bits, dithering: %s)... ", bits, (dither ? "yes" : "no"));

            COUNTTIME(status = effect_quantize((*samples), (*length), bits, (bool)dither), counted);

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i.\n", status);

            continue;
        }
        else if(!strcmp(command, "dump"))
        {
            if(!strcmp(args, ""))
            {
                fprintf(output, "Usage: dump [file]\n");
                continue;
            }

            fprintf(output, "Writing samples into %s... ", args);

            FILE* fileout = fopen(args, "wb");

            fprintf(output, "Done! (%zu/%zu elements is written)\n", fwrite((*samples), sizeof(slevel_t), (*length), fileout), (*length));

            fclose(fileout);

            fprintf(output, "\n"
                            "= Output signal parameters ========\n"
                            "- Format: %i bit PCM\n"
                            "- Endianness (byte order): %s \n"
                            "- Number of channels: %" PRIu64 "\n"
                            "- Sample rate: %" PRIu64 " Hz\n",
                            SLEVEL_BIT_DEPTH,
                            endianness_getname(),
                            header->channels,
                            header->samplerate);

            continue;
        }
        else if(!strcmp(command, "write"))
        {
            char outname[MAX_CMD_SYMBOLS]; uint16_t format = 0; uint16_t depth = 0;

            if(!strcmp(args, "") || sscanf(args, "%s %hu %hu", outname, &format, &depth) < 3)
            {
                fprintf(output, "Usage: write [filename] [format] [bit depth]\n");
                fprintf(output, "format - supported formats are PCM(1) or Float PCM(3).\n"
                                "In case of PCM(1) format, 16, 24, 32 bit bit depth are supported.\n"
                                "In case of Float PCM(3) format, 32, 64 bit depth are supported.\n");
                continue;
            }

            fprintf(output, "Writing file %s... ", outname);
            FILE* fileout = fopen(outname, "wb");

            if(fileout == NULL)
            {
                fprintf(output, "Cannot open file!\n");
                continue;
            }

            wav_header outheader;
            memset(&outheader, 0, sizeof(wav_header));

            outheader.channels = header->channels;
            outheader.samplerate = header->samplerate;
            outheader.audioformat = format;
            outheader.samplesdepth = depth;

            if((status = wav_write_file(&outheader, fileout, (*samples), (*length))) != FUNC_OK)
            {
                fprintf(output, "Failed with code %i!\n", status);
                fclose(fileout);
                continue;
            }
            else fprintf(output, "Done!\n");

            fclose(fileout);

            continue;
        }
        else if(!strcmp(command, "echo"))
        {
            double counted = 0.0;

            size_t delay = 0; double feedback = 0.0; size_t bounces = 0;
            
            if(!strcmp(args, "") || sscanf(args, "%zu %lf %zu", &delay, &feedback, &bounces) != 3)
            {
                fprintf(output, "Usage: echo [delay] [material feedback] [number of bounces]\n");
                continue;
            }

            fprintf(output, "Applying echo (feedback: ~%.1f %%, delay: %zu samples, bounces: %zu)... ", (feedback * 100), delay, bounces);

            COUNTTIME(status = effect_echo((*samples), (*length), header, delay, feedback, bounces), counted);

            if(status == FUNC_OK) fprintf(output, "Done for %.1f seconds!\n", counted);
            else fprintf(output, "An error occurred: code %i.\n", status);

            continue;
        }
        else
        {
            fprintf(output, "Invalid command \"%s\". Type \"help\" to view list of commands.\n", command);
            continue;
        }
    }

    return FUNC_OK;
}