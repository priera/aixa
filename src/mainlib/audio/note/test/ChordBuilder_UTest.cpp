#include "ChordBuilder_UTest.h"

#include <QtTest/QTest>

#include <mainlib/audio/note/ChordBuilder.h>

bool compareVectorDoubles(const std::vector<double> &lhs, const std::vector<double> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](double l, double r) {
        return std::abs(l - r) < 5e-3;
    });
}

void ChordBuilder_UTest::test_triad_data() {
    QTest::addColumn<int>("pitch");
    QTest::addColumn<int>("octave");
    QTest::addColumn<int>("modifier");
    QTest::addColumn<std::vector<double>>("expected");

    std::vector<double> v1 = {440.0, 554.365, 659.255};
    QTest::newRow("A4") << static_cast<int>(Note::Pitch::A) << 4 << static_cast<int>(Note::Modifier::NONE) << v1;

    std::vector<double> v2 = {880, 1108.73, 1318.51};
    QTest::newRow("A5") << static_cast<int>(Note::Pitch::A) << 5 << static_cast<int>(Note::Modifier::NONE) << v2;

    std::vector<double> v3 = {523.251, 659.255, 783.991};
    QTest::newRow("C4") << static_cast<int>(Note::Pitch::C) << 4 << static_cast<int>(Note::Modifier::NONE) << v3;

    std::vector<double> v4 = {554.365, 698.456, 830.609};
    QTest::newRow("C#4") << static_cast<int>(Note::Pitch::C) << 4 << static_cast<int>(Note::Modifier::SHARP) << v4;

    std::vector<double> v5 = {233.082, 293.665, 349.228};
    QTest::newRow("Bb3") << static_cast<int>(Note::Pitch::B) << 3 << static_cast<int>(Note::Modifier::FLAT) << v5;
}

void ChordBuilder_UTest::test_triad() {
    QFETCH(int, pitch);
    QFETCH(int, octave);
    QFETCH(int, modifier);
    QFETCH(std::vector<double>, expected);

    auto tonic = Note(static_cast<Note::Pitch>(pitch), octave, static_cast<Note::Modifier>(modifier));
    auto chord = ChordBuilder::triad(tonic).build();

    QVERIFY2(compareVectorDoubles(chord.getFrequencies(), expected), "Not matching chord frequencies");
}

void ChordBuilder_UTest::test_seventh_data() {
    QTest::addColumn<int>("pitch");
    QTest::addColumn<int>("octave");
    QTest::addColumn<int>("modifier");
    QTest::addColumn<std::vector<double>>("expected");

    std::vector<double> v1 = {440.0, 554.365, 659.255, 830.609};
    QTest::newRow("A4") << static_cast<int>(Note::Pitch::A) << 4 << static_cast<int>(Note::Modifier::NONE) << v1;

    std::vector<double> v2 = {880, 1108.73, 1318.51, 1661.22};
    QTest::newRow("A5") << static_cast<int>(Note::Pitch::A) << 5 << static_cast<int>(Note::Modifier::NONE) << v2;

    std::vector<double> v3 = {523.251, 659.255, 783.991, 987.767 };
    QTest::newRow("C4") << static_cast<int>(Note::Pitch::C) << 4 << static_cast<int>(Note::Modifier::NONE) << v3;

    std::vector<double> v4 = {554.365, 698.456, 830.609, 1046.5};
    QTest::newRow("C#4") << static_cast<int>(Note::Pitch::C) << 4 << static_cast<int>(Note::Modifier::SHARP) << v4;

    std::vector<double> v5 = {233.082, 293.665, 349.228, 440};
    QTest::newRow("Bb3") << static_cast<int>(Note::Pitch::B) << 3 << static_cast<int>(Note::Modifier::FLAT) << v5;
}

void ChordBuilder_UTest::test_seventh() {
    QFETCH(int, pitch);
    QFETCH(int, octave);
    QFETCH(int, modifier);
    QFETCH(std::vector<double>, expected);

    auto tonic = Note(static_cast<Note::Pitch>(pitch), octave, static_cast<Note::Modifier>(modifier));
    auto chord = ChordBuilder::seventh(tonic).build();

    QVERIFY2(compareVectorDoubles(chord.getFrequencies(), expected), "Not matching chord frequencies");
}

QTEST_MAIN(ChordBuilder_UTest)