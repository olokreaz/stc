//
// Created by olokreZ on 03.03.2024.
//

#include <glog/stl_logging.h>
#include <glog/logging.h>
#include <vector>
#include <random>
#include <portaudio.h>
#include <sndfile.h>
#include <args.hxx>


// Коэффициент затухания
#define DAMPING_FACTOR 0.996

// Буфер для алгоритма Карплуса-Стронга
std::vector<double> buffer;

// Генератор случайных чисел
std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(-1.0, 1.0);

void karplus_strong(std::vector<int> frequencies, int sample_rate, double duration) {
	int total_samples = sample_rate * duration;
	buffer.resize(total_samples);

	for (int frequency : frequencies) {
		int N = sample_rate / frequency;
		for (int i = 0; i < N; i++) {
			buffer[i] += distribution(generator);
		}

		for (int i = N; i < total_samples; i++) {
			buffer[i] += DAMPING_FACTOR * 0.5 * (buffer[i - N] + buffer[i - N + 1]);
		}
	}

	// Нормализация буфера
	double max_val = *std::max_element(buffer.begin(), buffer.end());
	for (int i = 0; i < total_samples; i++) {
		buffer[i] /= max_val;
	}
}

enum EMiss : uint8_t {kMissfreq = 1 << 0, kMissSampleRate = 1 << 1 , kMissDuration = 1 << 2};

int main(int argc, char** argv) {

	args::ArgumentParser parser("Generate signal using Karplus-Strong algorithm");

	parser.LongPrefix("//");
	parser.LongSeparator(":");
	parser.ShortPrefix("/");
	parser.Prog(*argv);
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

	args::ValueFlagList<int> frequencies(parser, "frequencies", "Frequencies of the signal", {'f', "frequencies"});
	args::ValueFlag<int> sample_rate(parser, "sample_rate", "Sample rate of the signal", {'s', "sample_rate"});
	args::ValueFlag<double> duration(parser, "duration", "Duration of the signal", {'d', "duration"});
	args::Flag play(parser, "play", "Play the signal", {'p', "play"});
	args::Positional<std::string> output(parser, "output", "Output file");

	try {
		parser.ParseCLI(argc, argv);
	} catch (const args::Help& e) {
		std::cerr << parser;
		return 0;
	} catch (const args::ParseError& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}


	uint8_t bitmask = 0;

	if (frequencies.Get().empty()) {
		bitmask |= EMiss::kMissfreq;
	}

	if (!sample_rate.HasFlag(  )) {
		bitmask |= EMiss::kMissSampleRate;
	}

	if (!duration.HasFlag(  )) {
		bitmask |= EMiss::kMissDuration;
	}

	if (bitmask)
	{
		printf("Error: Required flags ");
		if (bitmask & EMiss::kMissfreq) printf("/f ");
		if (bitmask & EMiss::kMissSampleRate) printf("/s ");
		if (bitmask & EMiss::kMissDuration) printf("/d ");
		printf("are missing");
		flush(std::cout);
		return 0;
	}

	DLOG(INFO) << "Frequencies: " << frequencies.Get().size();
	DLOG(INFO) << "Sample rate: " << sample_rate.Get();
	DLOG(INFO) << "Duration: " << duration.Get();
	DLOG(INFO) << "Output: " << output.Get();

	// Инициализация PortAudio
	Pa_Initialize();

	// Запись в файл
	SF_INFO sfinfo;
	sfinfo.samplerate = args::get(sample_rate);
	sfinfo.channels = 1;
	sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	SNDFILE* outfile = sf_open(!output ? "output.wav" : output ->c_str(  ), SFM_WRITE, &sfinfo);

	// Генерация сигнала
	karplus_strong(args::get( frequencies ), args::get( sample_rate ), args::get( duration ));

	// Запись сигнала в файл
	sf_write_double(outfile, buffer.data(  ), buffer.size());

	// Закрытие файла
	sf_close(outfile);

	// Воспроизведение сигнала
	if (play) {
		PaStream* stream;
		Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, args::get( sample_rate ), 256, 0, 0);
		Pa_StartStream(stream);
		Pa_WriteStream(stream, buffer.data(), buffer.size());
		Pa_StopStream(stream);
		Pa_CloseStream(stream);
	}

	// Завершение работы с PortAudio
	Pa_Terminate();

	return 0;
}