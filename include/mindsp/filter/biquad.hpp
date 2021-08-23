/*
Copyright (c) 2021 Nathaniel D'Rozario

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "../util/ring_buffer.hpp"

namespace mindsp {
    namespace filter {
        
        struct biquad_coefficients {

            biquad_coefficients() {

            }

            biquad_coefficients(float a0, float a1, float a2, float b0, float b1, float b2) {
                this->a0 = a0;
                this->a1 = a1;
                this->a2 = a2;
                this->b0 = b0;
                this->b1 = b1;
                this->b2 = b2;
            }

            float a0 = 0.0f;
            float a1 = 0.0f;
            float a2 = 0.0f;
            float b0 = 0.0f;
            float b1 = 0.0f;
            float b2 = 0.0f;

            void normalize() {
                if (a0 != 1.0f) {
                    a1 /= a0;
                    a2 /= a0;
                    b0 /= a0;
                    b1 /= a0;
                    b2 /= a0;
                    a0 = 1.0f;
                }
            }
        };

        // direct form II biquad filter
        // it works but it seems to get unstable easily
        class biquad_filter {
        public:

            biquad_filter() { 
                w.resize(2);
            }
            
            biquad_filter(biquad_coefficients c) {
                coefficients = c;
                w.resize(2);
            }

            virtual ~biquad_filter() { }

            void reset_state() {
                w.resize(2);
            }

            void set_coefficients(biquad_coefficients c) {
                coefficients = c;
                reset_state();
            }

            void apply(float *out, const float *in, std::size_t length) {
            /*
                for (std::size_t i = 0; i < length; i++) {
                    float output = coefficients.b0 * in[i];
                    output += (coefficients.b1 * feedforward.read_tap(1)) + (coefficients.b2 * feedforward.read_tap(2))
                              - (coefficients.a1 * feedback.read_tap(1)) - (coefficients.a2 * feedback.read_tap(2));
                    output /= coefficients.a0;
                    feedforward.push(in[i]);
                    feedback.push(output); 
                }
            */
                for (std::size_t i = 0; i < length; i++) {
                    if (coefficients.a0 != 1.0f) {
                        coefficients.normalize();
                    }
                    float w0 = in[i] - (coefficients.a1 * w.read_tap(1)) - (coefficients.a2 * w.read_tap(2));
                    out[i] = (coefficients.b0 * w0) + (coefficients.b1 * w.read_tap(1)) + (coefficients.b2 * w.read_tap(2));
                    w.push(w0); 
                }
            }

        private:
            biquad_coefficients coefficients;
            util::ring_buffer<float> w;

        };

    };
}