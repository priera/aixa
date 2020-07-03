#include "InterleavedBuffer.h"

#include <sstream>

InterleavedBuffer::InterleavedBuffer(int channels, snd_pcm_uframes_t frame_size, snd_pcm_format_t format) :
        channels(channels),
        currentChannel(0),
        m_frameSize(frame_size) {

    format_bits = snd_pcm_format_width(format);
    bps = format_bits / 8;
    phys_bps = snd_pcm_format_physical_width(format) / 8;
    little_endian = snd_pcm_format_big_endian(format) != 1;
    to_unsigned = snd_pcm_format_unsigned(format) == 1;
    m_dataSize = m_frameSize * channels * bps;

    charFrame = static_cast<char *>(malloc(m_dataSize));
    if (charFrame == nullptr) {
        throw std::bad_alloc();
    }

    auto areas_p = calloc(channels, sizeof(snd_pcm_channel_area_t));
    if (areas_p == nullptr) {
        throw std::bad_alloc();
    }

    areas = static_cast<snd_pcm_channel_area_t*>(areas_p);

    for (unsigned int chn = 0; chn < channels; chn++) {
        areas[chn].addr = charFrame;
        areas[chn].first = chn * snd_pcm_format_physical_width(format);
        areas[chn].step = channels * snd_pcm_format_physical_width(format);
    }

    ptrToChanelSample.resize(channels, nullptr);
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

        //Steps array is actually not necessary, since all channels have the same format (so step is the same for all)
        steps[chn] = areas[chn].step / 8;
    }

    m_samplesFrame = reinterpret_cast<signed short*>(charFrame);
}

InterleavedBuffer::~InterleavedBuffer() {
    free(m_samplesFrame);
    free(areas);
}


void InterleavedBuffer::startNewFrame() {
    for (int chn = 0; chn < channels; chn++) {
        ptrToChanelSample[chn] = (((unsigned char *) areas[chn].addr) + (areas[chn].first / 8));
    }
    currentChannel = 0;
}

void InterleavedBuffer::storeNextSample(short sample) {
    if (to_unsigned)
        sample ^= 1U << (format_bits - 1);

    if (little_endian) {
        for (int i = 0; i < bps; i++)
            *(ptrToChanelSample[currentChannel] + phys_bps - 1 - i) = (sample >> i * 8) & 0xff;
    } else {
        for (int i = 0; i < bps; i++)
            *(ptrToChanelSample[currentChannel] + i) = (sample >> i * 8) & 0xff;
    }

    ptrToChanelSample[currentChannel] += steps[currentChannel];
    currentChannel = (currentChannel + 1) % channels;
}
