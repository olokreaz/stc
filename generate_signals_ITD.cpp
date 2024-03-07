//
// Created by olokreZ on 02.03.2024.
//

#include <windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


// Функция для вычисления интерауральной временной разницы (ITD) в зависимости от угла и скорости звука
#include <portaudio.h>

// Константы
constexpr double kPi = glm::pi<double>();
constexpr double kSpeedOfSound = 340.29; // скорость звука, м/с
constexpr double kDistanceBetwwnEars = 0.204; // расстояние между ушами, м

// Функция для вычисления ITD
double calculateITD(double angle) {
    return  (kDistanceBetwwnEars * glm::sin(angle) / kSpeedOfSound) * 1000;
}

// Callback функция
int generateTone(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    float* out = (float*)outputBuffer;
    double* angle = (double*)userData;
    double leftEarDelay = calculateITD(*angle) / 1000.0; // переводим в секунды
    double rightEarDelay = calculateITD(*angle + kPi) / 1000.0; // переводим в секунды

    for (unsigned long i = 0; i < framesPerBuffer; i++)
    {
        double time = i / 44100.0; // текущее время
        *out++ = sin(2.0 * kPi * 440.0 * (time - leftEarDelay)); // левый канал
        *out++ = sin(2.0 * kPi * 440.0 * (time - rightEarDelay)); // правый канал
    }

    *angle += 0.01; // увеличиваем угол
    if (*angle > 2 * kPi) {
        *angle -= 2 * kPi; // если угол больше 2π, то вычитаем 2π
    }

    return paContinue;
}

int main()
{
    PaStream* stream;
    PaError err;

    // Инициализация PortAudio
    err = Pa_Initialize();
    if (err != paNoError) return -1;

    double angle = 0.0; // начальный угол

    // Открытие потока
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, generateTone, &angle);
    if (err != paNoError) return -1;

    // Запуск потока
    err = Pa_StartStream(stream);
    if (err != paNoError) return -1;

    // Ждем 5 секунд
    Pa_Sleep(5000);

    // Останавливаем поток
    err = Pa_StopStream(stream);
    if (err != paNoError) return -1;

    // Закрываем поток
    err = Pa_CloseStream(stream);
    if (err != paNoError) return -1;

    // Деинициализация PortAudio
    Pa_Terminate();

    return 0;
}
