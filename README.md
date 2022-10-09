## The Quant Library (libquant)
Open source project aims to make **fast cross-platform non-real-time DSP library**, that supports **multi-threading**.

Current library development period is **Alpha**.

### What can libquant do?
- **Reading and writing** following formats:
    - Microsoft WAV files
      - PCM format (16, 24, 32 bits)
      - IEEE-754 (floating-point) PCM format (32, 64 bits)
- It can apply following **effects** to samples:
  - **Distortion** (clipping)
  - **Amplifying** (and auto-amplifying to maximum)
  - **Reverse**
  - **Quantization** (supports dithering)
  - **Inverting**
  - **Median filter**
  - **Echo** (without equalization)
  - **Delay** (single echo effect)
  - **Muting**
  - **Sound speed-up effect**
  - **Mixing with noise** (pseudo-noise, uses rand())
  - **Channels mixing**
  - **Channels difference effect**
- It can **synthesize** some waveforms:
  - **Sinusoid**
  - **Square**
  - **Noise** (pseudo-noise, uses rand())
  - **Sawtooth**
  - **Silence**
  
### Limitations
- Maximum number of channels is **(2^64)-1**
- Maximum sample rate is **(2^64)-1 Hz**
- Number of samples is storing as *size_t*, its width **depends on platform.**
- Each channel must have the same number of samples.

### Requirements
- **C99** (**C17** is recommended) or **C++11** compliant compiler.
- Target architecture must:
    - Have 8-bit bytes (**CHAR_BIT** must be 8).
    - Floating-point numbers must meet [IEEE-754](https://wikipedia.org/wiki/IEEE-754) standard.
    - It must be either Big-Endian or Little-Endian.
- <u>*for multithreading support*</u> | Compiler with **OpenMP 3.0 support**.

### Build guide
#### Microsoft Windows
- **Step 1**: Download and install **make**
    - Command for [Chocolatey](https://chocolatey.org): <code>choco install make</code>
- **Step 2**: Download and install **compiler** (e.g. MinGW-w64 GCC)
- **Step 3**: Download and **extract source code** in some directory.
- **Step 4**: Open **Command Prompt**, and go to directory from **Step 3**
- **Step 5**: Do <code>make</code> and wait for compilation.
    - If you use compiler which executable is not called <code>gcc</code>, then enter <code>make COMPILER=(compiler executable name)</code> instead of just <code>make</code>.
- **Step 6**: All done! Library will be in "*build*" directory

#### *nix systems (e.g. GNU/Linux)
- **Step 1**: Download and install **make**
    - Command for **apt**: <code>apt install make</code>
- **Step 2**: Download and install **compiler** (e.g. GCC)
    - Command for **apt**: <code>apt install gcc</code>
- **Step 3**: Download and **extract source code** in some directory.
- **Step 4**: Go to directory from **Step 3**
- **Step 5**: Do <code>make</code> and wait for compilation.
    - If you use compiler which executable is not called <code>gcc</code>, then enter <code>make COMPILER=(compiler executable name)</code> instead of just <code>make</code>.
- **Step 6**: All done! Library will be in <u>build</u> directory

### Donate
:credit_card: If someone wants to **support** me or my project, then the link to **DonationAlerts** is [**here**](https://donationalerts.com/r/emildalalyan)
