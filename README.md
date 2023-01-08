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
  - **Fade effect** (linear or sine)
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

**Remark**: When you use OpenMP 2.0 (or older version), then samples iterator will be **long long**.

### Requirements
- **C99** (**C17** is recommended) or **C++11** compliant compiler.
- Target architecture must:
    - Have <ins>8-bit bytes</ins> (CHAR_BIT must be 8).
    - Meet [<ins>IEEE-754</ins>](https://wikipedia.org/wiki/IEEE-754) standard.
    - Be either <ins>one's complement or two's complement</ins>.
    - Be either <ins>Big-Endian or Little-Endian</ins>.
- <u>*for multithreading support*</u> | Compiler with **OpenMP 2.0 support** (OpenMP **3.0** is recommended).

**Remark**: Some compilers, that support C++11 **partially** also can compile libquant.
### Documentation
See [main page of the documentation](docs/Main.md)

### Donate
:credit_card: If someone wants to **support** me or my project, then the link to **DonationAlerts** is [**here**](https://donationalerts.com/r/emildalalyan)
