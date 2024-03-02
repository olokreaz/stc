//
// Created by olokreZ on 02.03.2024.
//

#include <windows.h>
#include <iostream>
#include <cmath>
#include <portaudio.h>

// Константы
const double PI = 3.14159265358979323846;
const double SPEED_OF_SOUND = 340.29; // скорость звука, м/с
const double DISTANCE_BETWEEN_EARS = 0.204; // расстояние между ушами, м

// Функция для вычисления ITD
double calculateITD(double angle, double distanceToSource) {
    double itd = (DISTANCE_BETWEEN_EARS * sin(angle) / SPEED_OF_SOUND) * 1000; // переводим в миллисекунды
    return itd;
}

// Callback функция
int generateTone(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    float* out = (float*)outputBuffer;
    double* angle = (double*)userData;
    double leftEarDelay = calculateITD(*angle, 1.0) / 1000.0; // переводим в секунды
    double rightEarDelay = calculateITD(*angle + PI, 1.0) / 1000.0; // переводим в секунды

    for (unsigned long i = 0; i < framesPerBuffer; i++)
    {
        double time = i / 44100.0; // текущее время
        *out++ = sin(2.0 * PI * 440.0 * (time - leftEarDelay)); // левый канал
        *out++ = sin(2.0 * PI * 440.0 * (time - rightEarDelay)); // правый канал
    }

    *angle += 0.01; // увеличиваем угол
    if (*angle > 2 * PI) {
        *angle -= 2 * PI; // если угол больше 2π, то вычитаем 2π
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
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, &generateTone, &angle);
    if (err != paNoError) return -1;

    // Запуск потока
    err = Pa_StartStream(stream);
    if (err != paNoError) return -1;

    // Ждем 5 секунд
    Pa_Sleep(10000);

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