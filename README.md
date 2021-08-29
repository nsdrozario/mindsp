# MinDSP

Header-only DSP library

## Features

- [x] FIR Filtering
- [x] Ring buffer / Delay line
    - [x] Fractional delay
- [x] Conversion between floating point and decibels
- [x] Biquadratic filter
- [ ] Higher-order IIR filter
- [ ] FIR filter design
- Biquad filter design
    - [x] Low pass filter
    - [x] High pass filter
    - [x] Peak filter
    - [x] Low shelf filter
    - [x] High shelf filter
    - [ ] Notch filter
    - [ ] Band pass filter
- Window functions
    - [x] Hann
    - [x] Hamming
    - [x] Blackman-Harris
- [x] FFT
    - Cooley-Tukey (radix-2)
    - Automatically zero-pads non-power of 2 size inputs

## Examples

### Biquad

Low pass filtering

```c++
    using std::vector;
    using namespace mindsp::filter;
    vector<float> wave (44100); // any populated array with a wave in floating-point form
    biquad_filter bq (low_pass_filter(500, 44100, 1));  // low pass at 500hz at a sample rate of 44.1khz with Q factor of 1
    bq.apply(wave.data(),wave.data(),wave.size());
```