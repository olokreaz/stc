#include <iostream>
#include <vector>
#include <complex>
#include <numeric>

// Функция для вычисления автокорреляции
std::vector<double> computeAutocorrelation(const std::vector<std::complex<int16_t>>& signal) {
    int N = signal.size();
    std::vector<double> autocorrelation(N, 0.0);

    for (int lag = 0; lag < N; ++lag) {
        double sum = 0.0;
        for (int i = 0; i < N - lag; ++i) {
            sum += std::real(signal[i] * std::conj(signal[i + lag]));
        }
        autocorrelation[lag] = sum;
    }

    return autocorrelation;
}

int main() {
    // Пример сигнала
    std::vector<std::complex<int16_t>> signal = {
        {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6},
        {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 11}
    };

    // Вычисление автокорреляции
    std::vector<double> autocorrelation = computeAutocorrelation(signal);

    // Вывод результатов
    for (size_t i = 0; i < autocorrelation.size(); ++i) {
        std::cout << "Lag " << i << ": " << autocorrelation[i] << std::endl;
    }

    return 0;
}
