#include "ChordSignal_UTest.h"

#include <QtTest/QTest>

#include <mainlib/math/SineGenerator.h>
#include <mainlib/math/vectorUtils.h>
#include <mainlib/audio/note/Note.h>
#include <mainlib/audio/note/ChordBuilder.h>

void ChordSignal_UTest::initTestCase() {
    storeAt = std::filesystem::path(qgetenv("storeAt").toStdString());
}

void ChordSignal_UTest::test_chordSignal_data() {
    QTest::addColumn<int>("pitch");
    QTest::addColumn<int>("octave");
    QTest::addColumn<QString>("chordName");

    QTest::newRow("C") << static_cast<int>(Note::Pitch::C) << 4 << QString("C");
    QTest::newRow("A") << static_cast<int>(Note::Pitch::A) << 5 << QString("A");
    QTest::newRow("G") << static_cast<int>(Note::Pitch::G) << 4 << QString("G");
}

void ChordSignal_UTest::test_chordSignal() {
    QFETCH(int, pitch);
    QFETCH(int, octave);
    QFETCH(QString, chordName);

    const std::size_t signalSize = 4410;
    const double samplePeriod = 1.0 / 44100;
    auto sineGenerator = aixa::math::SineGenerator(signalSize, samplePeriod, 0);

    const auto note = Note(static_cast<Note::Pitch>(pitch), octave, Note::Modifier::NONE);
    const Chord chord = ChordBuilder::triad(note);

    auto finalSignal = aixa::math::DoubleVector(signalSize);
    sineGenerator.setScaleFactor(500);

    auto freqsSize = chord.getFrequencies().size();
    for (std::size_t i = 0; i < freqsSize; i++) {
        sineGenerator.setFrequency(chord.getFrequencies()[i]);

        finalSignal += sineGenerator.nextSignal();
    }

    storeVector(chordName.toStdString(), storeAt, finalSignal);
}

QTEST_MAIN(ChordSignal_UTest)
