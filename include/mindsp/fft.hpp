#pragma once

#include <vector>
#include <complex>
#include <iostream>
#include <stack>

namespace mindsp {
    namespace fft {
        
        /**
         * 
         * Pads the end of an array with zeroes (to work with non-power of 2 sizes)
         * Please make sure memory is allocated for up to array_size elements
         * 
         * @param in Input array
         * @param signal_size Number of samples the input is defined for
         * @param array_size Actual size of the array
         * 
         */
        void zero_pad(float *in, std::size_t signal_size, std::size_t array_size) {
            for (std::size_t i = signal_size; i < array_size; i++) {
                in[i] = 0.0f;
            }
        }

        /**
         * 
         * Returns next suitable size for a radix-2 fft 
         * (powers of 2 are kept the same while anything that isn't a power of 2 returns the next power of 2) 
         * @param size Input size to change to a power of 2
         * 
         */
        std::size_t size_power_of_2(std::size_t size) {
            return static_cast<std::size_t> (std::pow(2, std::ceil(std::log2(static_cast<float>(size)))));
        }

        /**
         * 
         * Used internally by fft()
         * DO NOT USE DIRECTLY
         * @param out output array (frequency domain)
         * @param in real valued input (time domain)
         * @param n size in samples
         * 
         */
        void fft_helper(std::vector<std::complex<float>> &out, const float *in, std::size_t stride, std::size_t n) {
            typedef std::vector<std::complex<float>> complex_vector;
            if (n == 2) {
                
                /*

                let a = in[0]
                let b = in[1]

                n=2 dft matrix = [ 1  1 ]
                                 [ 1 -1 ]

                [ 1  1 ] [ a ]  =  [ a + b ]
                [ 1 -1 ] [ b ]     [ a - b ]

                */
               float norm_factor = 1.0f / std::sqrt(2);

                out[0] = (in[0] + in[stride]) * norm_factor;
                out[1] = (in[0] - in[stride]) * norm_factor;

            } else {
                
                complex_vector even (n/2);
                complex_vector odd (n/2);
                fft_helper(even, in, stride * 2, n/2);
                fft_helper(odd, in+stride, stride * 2, n/2);

                for (std::size_t k = 0; k < (n/2)-1; k++) {
                    // equal to e^(2pi * i * k / n) * odd[k]
                    std::complex<float> common_term = std::exp(std::complex<float>(0,-2.0f * 3.141592654f * k / static_cast<float>(n))) * odd[k];
                    out[k] = even[k] + common_term;
                    out[k+(n/2)] = even[k] - common_term;
                }

            }
        }

        /**
         * 
         * Computes the discrete Fourier transform of the input signal using radix-2 Cooley-Tukey algorithm
         * @param out Output vector of complex<float>
         * @param in Real-valued input array
         * @param size Size of the input array (non powers of two are zero-padded automatically)
         * 
         */
        void fft(std::vector<std::complex<float>> &out, const float *in, std::size_t size) {

            std::vector<float> temp;
            bool use_temp = false;
            
            if (size != size_power_of_2(size)) {
                use_temp = true;
                temp.resize(size_power_of_2(size));
                out.resize(size_power_of_2(size));
                for (std::size_t i = 0; i < size; i++) {
                    temp[i] = in[i];
                }
            }
            
            if (size >= 2) {
                fft_helper(out, (use_temp) ? temp.data() : in, 1, size);
            } else {
                out[0] = in[0];
            }

            for (std::size_t i = 0; i < out.size(); i++) {
                out[i] /= static_cast<float>(size);
            }

        }

        /**
         * 
         * Computes actual frequencies for each FFT bin  
         * @param freqs Reference to vector to store the frequencies in
         * @param size Size of the FFT used (non-powers of 2 will be scaled to the next power of 2 )
         * @param sample_rate Intended sample rate to scale frequencies to 
         * 
         */
        void fft_get_frequencies(std::vector<float> &freqs, std::size_t size, float sample_rate) {

            float float_size = static_cast<float>(size_power_of_2(size));
            float factor = sample_rate / float_size;
            if (freqs.size() != size_power_of_2(size)) {
                freqs.resize(size_power_of_2(size));
            }

            for (std::size_t i = 0; i < freqs.size(); i++) {
                freqs[i] = factor * static_cast<float>(i);
            }

        }

    };
}