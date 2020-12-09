#include "SyntheticStreamFactory.h"

#include <mainlib/audio/note/ChordBuilder.h>

#include "SyntheticStream.h"

std::shared_ptr<Stream> SyntheticStreamFactory::probe() {
    using namespace std::chrono_literals;

    std::vector<SyntheticStream::StreamStep> streamSteps;
    auto now = SyntheticStream::Clock::now();
    auto octaveDuration = 8000ms;
    const int firstOctave = 4;

    for (int baseOctave = firstOctave; baseOctave < 9; baseOctave++) {
        int i = baseOctave - firstOctave;

        // Let's do a regular pop song!!!!
        // I–V–vi–IV chord progression
        const auto C = Note(Note::Pitch::C, baseOctave, Note::Modifier::NONE);
        const auto G = Note(Note::Pitch::G, baseOctave, Note::Modifier::NONE);
        const auto A = Note(Note::Pitch::A, baseOctave + 1, Note::Modifier::NONE);
        const auto F = Note(Note::Pitch::F, baseOctave, Note::Modifier::NONE);

        const Chord chordC = ChordBuilder::triad(C);
        const Chord chordG = ChordBuilder::seventh(G);
        const Chord chordAm = ChordBuilder::triad(A).minor();
        const Chord chordF = ChordBuilder::triad(F);

        auto octaveStart = now + (i * octaveDuration);
        streamSteps.emplace_back<SyntheticStream::StreamStep>({500, chordC, octaveStart});
        streamSteps.emplace_back<SyntheticStream::StreamStep>({600, chordG, octaveStart + 2000ms});
        streamSteps.emplace_back<SyntheticStream::StreamStep>({400, chordAm, octaveStart + 4000ms});
        streamSteps.emplace_back<SyntheticStream::StreamStep>({500, chordF, octaveStart + 6000ms});
    }

    const std::size_t signalSize = 4410;
    const double samplePeriod = 1.0 / 44100;
    return std::make_shared<SyntheticStream>(signalSize, samplePeriod, streamSteps);
}
