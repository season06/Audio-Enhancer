# Audio Enhancer


Audio Enhancer is an online **Parametric Equalizer** which can make audio more prominent.

- GitHub repository: [Audio Enhancer](https://github.com/season06/Audio-Enhancer)


## Problem to Solve

Equalization is the process of adjusting the volume of different frequency bands within an audio signal. Such as eliminating noise sounds or making certain voices more prominent.  

[Fourier Transform](https://en.wikipedia.org/wiki/Fourier_transform) can help us converts a signal from a time domain to a frequency domain. After obtaining the frequency series, adjust the volume of the frequency range according to the parameter.

<img src="./images/fft.jpg" alt="fft"/>

Most equalizers are hardware, this program develops an online equalizer and uses the low-level language C++ to implement FFT and optimize it.

### Algorithm and Mathematics

- [Discrete Fourier Transform](https://en.wikipedia.org/wiki/Discrete_Fourier_transform) (DFT)  
    Signals we heard are continuous and non‑periodic. However, computers can not process infinite data.  DFT samples a continuous signal in the time domain, making the Fourier transform discrete in both the time and frequency domains.

    > <p align="center"><img src="./images/dft_definition.svg" alt="dft_definition"/></p>

- [Fast Fourier Transform](https://en.wikipedia.org/wiki/Fast_Fourier_transform) (FFT)  
    FFT is an algorithm that computes the DFT of a sequence, or its inverse (IDFT).  

    Computing DFT directly from the definition is too slow to be practical. FFT rapidly computes such transformations by factorizing the DFT matrix into a product of sparse factors. As a result, it reduces the complexity of computing the DFT from `O(N^2)` to `O(NlogN)`, which `N` is the data size.  

    - Cooley-Tukey FFT Algorithm  
        Cooley-Tukey Algorithm is the most common FFT algorithm which uses divide and conquer to recursively decompose a DFT of length N = N1N2 into two shorter sequences of length N1 and N2.
        > <p align="center"><img src="./images/cooley_tukey_fft.svg" alt="cooley_tukey_fft"/></p>

## Prospective Users

- Audio maker  
    Creators can use Audio Enhancer to improve audio quality online.
- Users who use Real-time Meetings  
    If the system can read the audio in real time and quickly judge and filter the noise, it can help audience hear more clearly.


## System Architecture
<img src="./images/system_architecture.png" alt="system_architecture"/>

## API Description
`equalize(file, mode='allpass', freq=0, gain=0, Q=0)`

- `file`: An audio file, allow type: `.wav`
- `mode`: Define the shape of the processing curve.
    - `lowpass`
    - `highpass`
    - `lowshelf`
    - `highshelf`
    - `bandpass`
    - `notch`
    - `allpass`
- `freq`: A frequency position need to be adjust. (unit: Hz)
- `gain`: Amplification or attenuation of a certain range of frequencies. (unit: Db)
- `Q`: Define the steepness or width of each filter.

The following table describe the meaning of the parameters in each mode:

| `mode`      | description       | `freq`                             | `gain` | `Q` |
| :---------- | :---------------- | :--------------------------------- | :----- | :------- |
| `lowpass`   | Low-pass filter   | The cut-off frequency              | unused | Drop-off steepness at the cut-off frequency |
| `highpass`  | High-pass filter  | The cut-off frequency              | unused | Drop-off steepness at the cut-off frequency |
| `lowshelf`  | Low-shelf filter  | The upper limit of the frequencies | The amplification or attenuation level | unused |
| `highshelf` | High-shelf filter | The lower limit of the frequencies | The amplification or attenuation level | unused |
| `bandpass`  | Band-pass filter  | Center of the frequency band       | unused | The width of the frequency band |
| `notch`     | Notch filter      | Center of the frequency band       | unused | The width of the frequency band |
| `allpass`   | All-pass filter   | unused | unused | unused |


## Engineering Infrastructure

- Automatic build system: `make`
- Version control: `git`
- Testing framework: `pytest`
- Documentation: Update to GitHub README.md
- Continuous integration: Github Action


## Schedule

Planning phase (6 weeks including 2/21, 2/28, 3/7, 3/14, 3/21, 3/28):

| Week         | Planning |
| :----------- | :------- |
| Week 1 (4/4) | Study Fourier Transform <br> Survey information and libraries |
| Week 2 (4/11) | Design system architecture |
| Week 3 (4/18) | Implement functions |
| Week 4 (4/25) | Implement functions <br> Write unit test|
| Week 5 (5/2) | Write unit test <br> Build Github Action (CI)|
| Week 6 (5/9) | Optimize execution performance |
| Week 7 (5/16) | Overall tests |
| Week 8 (5/23) | Prepare presentation <br> Write document |

## References

[Fast Fourier Transform](https://en.wikipedia.org/wiki/Fast_Fourier_transform)  
[What is the Fourier Transform? A visual introduction.](https://www.youtube.com/watch?v=spUNpyF58BY&ab_channel=3Blue1Brown)  
[WAV](https://zh.wikipedia.org/zh-tw/WAV)  