#ifndef AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORD_H
#define AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORD_H

#include <utility>
#include <vector>

class Chord {
public:
    explicit Chord(std::vector<double> freqs)
        : freqs(std::move(freqs)) { };

    virtual ~Chord() = default;

    const std::vector<double> &getFrequencies() const { return freqs; }

private:
    std::vector<double> freqs;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORD_H
