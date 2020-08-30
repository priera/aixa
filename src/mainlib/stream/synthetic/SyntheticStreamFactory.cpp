#include "SyntheticStreamFactory.h"

#include <mainlib/audio/note/ChordBuilder.h>

#include "SyntheticStream.h"

std::shared_ptr<Stream> SyntheticStreamFactory::probe() {
    using namespace std::chrono_literals;

    //Let's do a regular pop song!!!!
    //I–V–vi–IV chord progression
    const auto C = Note(Note::Pitch::C, 4, Note::Modifier::NONE);
    const auto G = Note(Note::Pitch::G, 4, Note::Modifier::NONE);
    const auto A = Note(Note::Pitch::A, 5, Note::Modifier::NONE);
    const auto F = Note(Note::Pitch::F, 4, Note::Modifier::NONE);

    const Chord chordC = ChordBuilder::triad(C);
    const Chord chordG = ChordBuilder::seventh(G);
    const Chord chordAm = ChordBuilder::triad(A).minor();
    const Chord chordF = ChordBuilder::triad(F);

    auto now = SyntheticStream::Clock::now();
    std::vector<SyntheticStream::StreamStep> streamSteps = {
        {
                0, chordC, now
        },
        {
                500, chordC, now + 2000ms
        },
        {
                600, chordG, now + 3000ms
        },
        {
                400, chordAm, now + 4000ms
        },
        {
                500, chordF, now + 6000ms
        },
        { //Repeating two last steps, because current algorithm is wrong
                500, chordC, now + 8000ms
        },
        {
                500, chordC, now + 8000ms
        }
    };

    const std::size_t signalSize = 4410;
    const double samplePeriod = 1.0 / 44100;
    return std::make_shared<SyntheticStream>(signalSize, samplePeriod, streamSteps);
}
