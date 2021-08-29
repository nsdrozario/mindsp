#pragma once

#include <vector>
#include <utility>
#include <complex>
#include <algorithm>

namespace mindsp {
    namespace util {
        template <typename T>
        class matrix {
        public:

            matrix() {

            };

            matrix(std::size_t row, std::size_t col) {
                data.resize(row);
                for (std::size_t i = 0; i < row; i++) {
                    data[i].resize(col);
                }
            }

            matrix(std::pair<std::size_t ,std::size_t> s) {
                data.resize(s.first);
                for (std::size_t i = 0; i < s.first; i++) {
                    data[i].resize(s.second);
                }
            }

            virtual ~matrix() {

            }

            void resize(std::size_t row, std::size_t col) {
                data.resize(row);
                for (std::size_t i = 0; i < row; i++) {
                    data[i].resize(col);
                }
            }

            void resize(std::pair<std::size_t, std::size_t> s) {
                data.resize(s.first);
                for (std::size_t i = 0; i < s.first; i++) {
                    data[i].resize(s.second);
                }
            }


            T get(std::size_t row, std::size_t col) const {
                return data[row][col];
            }

            void set(T val, std::size_t row, std::size_t col) {
                data[row][col] = val;
            }

            std::pair<std::size_t, std::size_t> size() const {
                if (data.size() > 0) {
                    return std::make_pair(data.size(), data[0].size());
                } else {
                    return std::make_pair(0,0);
                }
            }

            matrix<T> operator+(const matrix &b) {
                matrix<T> out (b.size());
                for (std::size_t i = 0; i < data.size(); i++) {
                    for (std::size_t j = 0; j < data[i].size(); j++) {
                        out.data[i][j] = data[i][j] + b.data[i][j];
                    }
                }
                return out;
            }

            matrix<T> operator-(const matrix &b) {
                matrix<T> out (b.size());
                for (std::size_t i = 0; i < data.size(); i++) {
                    for (std::size_t j = 0; j < data[i].size(); j++) {
                        out.data[i][j] = data[i][j] - b.data[i][j];
                    }
                }
                return out;
            }

            friend matrix<T> operator*(const matrix<T> &a, const matrix &b) {
                matrix<T> out (a.size().first, b.size().second);
                for (std::size_t i = 0; i < out.size().first; i++) {
                    for (std::size_t j = 0; j < out.size().second; j++) {
                        out.data[i][j] = T();
                        for (std::size_t k = 0; k < a.size().second; k++) {
                            out.data[i][j] += a.data[i][k] * b.data[k][j];
                        }
                    }
                }
                return out;
            }

            matrix<T> operator*(const T &b) {
                matrix<T> out (b.size());
                for (std::size_t i = 0; i < data.size(); i++) {
                    for (std::size_t j = 0; j < data[i].size(); j++) {
                        out.data[i][j] = data[i][j] * b;
                    }
                }
                return out;
            }

            matrix<T> operator/(const T &b) {
                matrix<T> out (b.size());
                for (std::size_t i = 0; i < data.size(); i++) {
                    for (std::size_t j = 0; j < data[i].size(); j++) {
                        out.data[i][j] = data[i][j] / b;
                    }
                }
                return out;
            }

            void copy_vector_horizontal(std::vector<T> v) {
                data.resize(v.size(), 1);
                for (std::size_t i = 0; i < v.size(); i++) {
                    set(v[i], i, 0);
                }
            }

            void copy_vector_vertical(std::vector<T> v) {
                data.resize(1, v.size());
                for (std::size_t i = 0; i < v.size(); i++) {
                    set(v[i], 0, i);
                }
            }

        private:
            std::vector<std::vector<T>> data;
        };
    }
}