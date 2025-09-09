#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int SAMPLE_RATE = 44100;
const double DURATION = 2.0; // seconds
const int NUM_SAMPLES = static_cast<int>(SAMPLE_RATE * DURATION);
const int AMPLITUDE = 3000;
const double VOICE_FREQ = 120.0; // Hz, typical male speaking voice

// Write WAV header (PCM 16-bit mono)
void writeWavHeader(std::ofstream &file, int sampleRate, int numSamples) {
    int byteRate = sampleRate * 2; // mono, 16-bit
    int dataSize = numSamples * 2;

    file.write("RIFF", 4);
    int chunkSize = 36 + dataSize;
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    int subChunk1Size = 16;
    short audioFormat = 1;
    short numChannels = 1;
    short bitsPerSample = 16;
    short blockAlign = numChannels * bitsPerSample / 8;

    file.write(reinterpret_cast<const char*>(&subChunk1Size), 4);
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
}

int main() {
    std::ofstream file("v_sound.wav", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    writeWavHeader(file, SAMPLE_RATE, NUM_SAMPLES);

    for (int i = 0; i < NUM_SAMPLES; ++i) {
        double t = static_cast<double>(i) / SAMPLE_RATE;

        // Voicing: low frequency sine wave
        double voice = std::sin(2 * M_PI * VOICE_FREQ * t);

        // Friction: white noise (random between -1 and 1)
        double noise = 2.0 * std::rand() / RAND_MAX - 1.0;

        // Mix voice and noise
        double sample = 0.6 * voice + 0.4 * noise;

        // Envelope (fade in/out to avoid pops)
        double envelope = 1.0;
        if (t < 0.1)
            envelope = t / 0.1;
        else if (t > DURATION - 0.1)
            envelope = (DURATION - t) / 0.1;

        short intSample = static_cast<short>(AMPLITUDE * sample * envelope);
        file.write(reinterpret_cast<const char*>(&intSample), sizeof(short));
    }

    file.close();
    std::cout << "Generated 'v_sound.wav' — a synthetic /v/ sound.\n";
    return 0;
}