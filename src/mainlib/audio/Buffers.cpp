#include "Buffers.h"

#include <sstream>

Buffers::Buffers(int channels, snd_pcm_sframes_t frame_size, snd_pcm_format_t format) :
    channels(channels),
    frameSize(frame_size) {

    format_bits = snd_pcm_format_width(SND_PCM_FORMAT_S16); // 16
    bps = format_bits / 8;
    phys_bps = snd_pcm_format_physical_width(SND_PCM_FORMAT_S16) / 8;
    big_endian = snd_pcm_format_big_endian(SND_PCM_FORMAT_S16) == 1;
    to_unsigned = snd_pcm_format_unsigned(SND_PCM_FORMAT_S16) == 1;

    auto samplesBuffer = malloc(frameSize * channels * bps);
    if (samplesBuffer == nullptr) {
        throw std::bad_alloc();
    }

    auto areas_p = calloc(channels, sizeof(snd_pcm_channel_area_t));
    if (areas_p == nullptr) {
        throw std::bad_alloc();
    }

    areas = static_cast<snd_pcm_channel_area_t*>(areas_p);

    for (unsigned int chn = 0; chn < channels; chn++) {
        areas[chn].addr = samplesBuffer;
        areas[chn].first = chn * snd_pcm_format_physical_width(format);
        areas[chn].step = channels * snd_pcm_format_physical_width(format);
    }

    ptrToChanelSample.resize(channels, 0);
    steps.resize(channels, 0);

    /* verify and prepare the contents of areas */
    for (int chn = 0; chn < channels; chn++) {
        if ((areas[chn].first % 8) != 0) {
            std::stringstream s;
            s << "areas[" << chn << "].first == " << areas[chn].first << ". Not a multiple of a byte";
            throw std::runtime_error(s.str());
        }

        if ((areas[chn].step % 16) != 0) {
            std::stringstream s;
            s << "areas[" << chn << "].step == " << areas[chn].step << ". Not a multiple of 2 bytes";
            throw std::runtime_error(s.str());
        }

        steps[chn] = areas[chn].step / 8;
    }

    samples = static_cast<signed short*>(samplesBuffer);
}

void Buffers::startNewFrame() {
    for (int chn = 0; chn < channels; chn++) {
        ptrToChanelSample[chn] = (((unsigned char *) areas[chn].addr) + (areas[chn].first / 8));
    }
}

void Buffers::storeNextSample(short sample) {
    if (to_unsigned)
        sample ^= 1U << (format_bits - 1);
    for (int chn = 0; chn < channels; chn++) {
        if (big_endian) {
            for (int i = 0; i < bps; i++)
                *(ptrToChanelSample[chn] + phys_bps - 1 - i) = (sample >> i * 8) & 0xff;
        } else {
            for (int i = 0; i < bps; i++)
                *(ptrToChanelSample[chn] + i) = (sample >> i * 8) & 0xff;
        }

        ptrToChanelSample[chn] += steps[chn];
    }
}