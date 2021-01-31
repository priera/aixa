#include "FrameSynthesizer.h"

void FrameSynthesizer::synthesize(const SideInformation& sideInfo,
                                  const MainDataContent& content,
                                  std::size_t nChannels) {
    for (unsigned int i = 0; i < NR_GRANULES; i++) {
        for (unsigned int channel = 0; channel < nChannels; channel++) {
            const auto& channelInfo = sideInfo.granules[i][channel];
            const auto& channelContent = content.granules[i][channel];
            requantizeSamples(channelContent);
        }
    }
}

void FrameSynthesizer::requantizeSamples(const GranuleChannelContent& channelContent) {}
