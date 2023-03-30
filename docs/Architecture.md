## [<- Back to the main page](Main.md)

## Library architecture

Library consists of following parts:
- Readers/writers of the files  (**io** directory)
- Synthesizers (**synth** directory)
- Effects (**effects** directory)
- Minimal abstraction layers:
  - **[samples.h](../samples.h)**: This file defines <code>slevel_t</code> - type of the samples, by default it's 32-bit wide (alias for <code>uint32_t</code>), but the code is written in way, that <code>slevel_t</code> can be signed integer type of **any** length.
  - **[build.h](../build.h)**: It provides useful macros, for detection compiler/library/language standard versions, and macros for using non-standard compiler extensions, if they're supported, otherwise these macros will be just stubs.
  - **[c99defs.h](../c99defs.h)**: This file has some definitions and functions from C99 standard. It intended for compilers, that support C++11/C99 only partionally (for example Microsoft Visual C++ 2010).
  - **[fheader.h](../io/fheader.h)**: It defines common file header. All file header structures should be compatible with it.
  - **[functions.h](../functions.h)**: It's just function statuses enumeration.
  - **[openmp.h](../openmp.h)**: Defines iterator type - <code>omp_iter_t</code>, that depends on the OpenMP standard version.
- Auxillary files, providing some useful functions.

### Multiple channels notice
Storage of multiple channels are implemented using **interleaving**,
i.e samples are stored in this way (in case of stereo):
- 1st sample of 1st channel
- 1st sample of 2nd channel
- 2nd sample of 1st channel
- 2nd sample of 2nd channel
- 3rd sample of 1st channel
- etc.
